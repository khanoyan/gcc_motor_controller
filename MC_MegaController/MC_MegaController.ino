/*****************************************************************
MC_MegaConotroller.ino

NOTE: This code only compiles on Arduino Mega 2560 as it requires
the 4 hardware serial ports.

Set up a software serial port to pass data between an XBee Shield
and the serial monitor.

Hardware Hookup:
  The XBee Shield makes all of the connections you'll need
  between Arduino and XBee. If you have the shield make
  sure the SWITCH IS IN THE "DLINE" POSITION. That will connect
  the XBee's DOUT and DIN pins to Arduino pins 2 and 3.


Feature requests...
- battery level check
- serial two-way com with PC
- add analong control with joystick
- add third MC option? This should be a fork of the code
- e-stop button
- bumper sensors
- timeout check
- keypad lock code
- add speakers/audio shield
- secondary arduino watchdog with kill switch capability
- (ludicrous) runaway problem fix


*****************************************************************/


// **************************************************************
//     LIBRARIES
// **************************************************************

//Includes required to use Roboclaw library
#include "RoboClaw.h"

// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>

// for TM1638 display unit
#include "TM1638.h"

//#define cmdDelay 150

// Digital output for visual indication of Ludicrous mode
#define BIGLIGHT_PIN 31

// *******************************
// **   Parameters 
// *******************************

// MODE SECTION

#define MODE_ARM 1
#define MODE_ROVER 2

// SPEED SECTION

#define ROVER_VEL_SLOW             400   // velocity preset
#define ROVER_VEL_FAST            1200   // velocity preset
#define ROVER_RAMP_RATE_SLOW        30   // the ramp rate for motor speed enveloping
#define ROVER_RAMP_RATE_FAST       120   // the ramp rate for motor speed enveloping
#define ROVER_THR_SLOW              20   // goal threshold
#define ROVER_THR_FAST              60   // goal threshold

#define ARM_SPEED_SETTINGS           1    // number of speed settings
#define ARM_VEL                    100   // velocity preset FOR ARM
#define ARM_RAMP_RATE               10   // the ramp rate for motor speed enveloping
#define ARM_THR                     10   // goal threshold

#define ROVER_SPEED_SETTINGS         2   // number of speed settings

#define DIAG_OFFSET_RATE       2   // ????
#define DIAG_OFFSET_SLOW          (ROVER_VEL_SLOW/DIAG_OFFSET_RATE)
#define DIAG_OFFSET_FAST          (ROVER_VEL_FAST/DIAG_OFFSET_RATE)

// define the name for the drive modes
#define SLOW      0
#define FAST      1
#define ARM       2

#define LOOP_PERIOD_MS 20   // 50Hz Loop

// ***********************
//       XBEE
// ***********************
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
// on xbee shield, place jumper on XB_TX_11 and DIGITAL, and
// XB_RX_10 and DIGITAL
SoftwareSerial XBee(11, 10); // RX, TX (this is confusing and wrong, probably!)
bool xbee_on = false;
int xbee_counter = 0;
long lastTimeJSMessageRecieved;

struct JOYSTICK_CMD {
  bool linkActive;      // is the JS link active?
  bool up, dn, lt, rt;  // Up, Down, Left, Right
  bool r1, r2, l1, l2;  // Right1, Right2, Left1, Left2
  bool b1, b2, b3, b4;  // Button1, Button2, Button3, Button4
  bool st, se;          // Start, Select
  // TODO: add analogs
};

// ***********************
//       Thing
// ***********************
bool thing_on =false;
struct COMMAND_FROM_THING_TO_MC{
  int direction;
  int motorNum;
  int duration;
};
long lastTimeThingMessageRecieved;

// ************************
//        ROBOCLAW
// ************************

//Setup communcaitions with roboclaw. Use pins 10 and 11 with 10ms timeout
//For Rover, claw1 is L, claw2 is right
//For Arm,

// .. and Hardware Serial (on the Arduino MEGA) for RoboClaw control
#include <HardwareSerial.h>
//Roboclaw Address
#define address 0x80

RoboClaw roboclaw1(&Serial2,10000);
RoboClaw roboclaw2(&Serial3,10000); // 2
//RoboClaw roboclaw3(&Serial1, 10000);

// ************************
// setup TM1638 module
// ************************
// pin 48: data         pin 50: clock     pin 52: strobe
TM1638 tm1638(48, 50, 52);

// ************************
//        STRUCTS
// ************************



struct DRIVE_PARAMS {
  int vel;
  int ramp;
  int thr;
  int diag;
};



// ************************
//      GLOBAL VARIABLES
// ************************


//HardCode the Mode of Rover
int mode = MODE_ROVER;

byte drive_mode = SLOW;    // indicates which driving profile is currently used
bool hillMode = true;      // maintain velocity of 0 (ie: brake if not driving)

DRIVE_PARAMS param[ROVER_SPEED_SETTINGS];  // LUT for driving parameters

unsigned long last_loop_time = 0;
JOYSTICK_CMD jscmd;                  // current joystick command
unsigned long jscmd_cnt = 0;         // count of commands from joystick

COMMAND_FROM_THING_TO_MC CMDS_TO_MC;

// current and goal speeds for each side


int rover_cur_spd_lt  = 0;                 // current left motor speed for Rover
int rover_cur_spd_rt  = 0;                 // current right motor speed for Rover
int rover_goal_spd_lt = 0;                 // left motor goal speed for Rover
int rover_goal_spd_rt = 0;                 // right motor goal speed for Rover 
int arm_cur_spd_m1  = 0;                 // current motor1 speed for Arm
int arm_cur_spd_m2  = 0;                 // current motor2 speed for Arm
int arm_cur_spd_m3  = 0;                 // current motor3 speed for Arm
int arm_cur_spd_m4  = 0;                 // current motor4 speed for Arm
int arm_cur_spd_m5  = 0;                 // current motor5 speed for Arm
int arm_cur_spd_m6  = 0;                 // current motor6 speed for Arm
int arm_goal_spd_m1 = 0;                 // motor1 goal speed for Arm
int arm_goal_spd_m2 = 0;                 // motor2 goal speed for Arm
int arm_goal_spd_m3 = 0;                 // motor3 goal speed for Arm
int arm_goal_spd_m4 = 0;                 // motor4 goal speed for Arm
int arm_goal_spd_m5 = 0;                 // motor5 goal speed for Arm
int arm_goal_spd_m6 = 0;                 // motor6 goal speed for Arm

byte tm1638_keys  = 0;               // push button inputs from TM1638

bool eStop = false;                  // emergency stop flag

unsigned int mc1_batt = 250; //0;
unsigned int mc2_batt = 250; //0;
unsigned int mc3_batt = 250; //Checking battery for MC3, specifically needed for the arm

bool batteryOK = true;               // battery status is OK flag

// delete if not being used....
//bool megaSpeed = 0;
//int velocity = FAST_VELOCITY;

// ************************
//    DANGER
// ************************

bool dangerOverride = false;                        // Danger Override
int dangerCounter = 0;
bool dangerFront = false;                           // Rover Danger Variables
bool dangerBack = false;                            // Rover Danger Variables
bool FrontRight = false;
bool FrontLeft = false;
bool BackRight = false;
bool BackLeft = false;

int buttonStateFront_R = 0;
int buttonStateFront_L = 0;
int buttonStateBack_R = 0;
int buttonStateBack_L = 0;

// Arm Danger Variables

bool dangerM1 = false;
bool dangerM2 = false;
bool dangerM3 = false;
bool dangerM4 = false;
bool dangerM5 = false;
bool dangerM6 = false;   

// Arm Motor Current Theshold

int M1_thresh = 1;
int M2_thresh = 2;
int M3_thresh = 3;
int M4_thresh = 4;
int M5_thresh = 5;
int M6_thresh = 6;              // undertermined values; requires testing

// Pin Locations for Rover
int BUTTON_PIN_FRONT_R = 43;  
int BUTTON_PIN_FRONT_L = 41;
int BUTTON_PIN_BACK_R = 47;
int BUTTON_PIN_BACK_L = 45;     

// ************************
//       TELEMETRY
// ************************

uint8_t currentM1;
uint8_t currentM2;
uint8_t currentM3;
uint8_t currentM4;
uint8_t currentM5;
uint8_t currentM6;


/* ************************************************************
 *  Main Loop
   ======================================
   cycle at the rate of LOOP_PERIOD_MS:
     - get TM1638 buttons
     - get joystick inputs
     - read TBD serial interface
     - send debug msg to joystick if available
     - determine new motor speed
     - set new motor speed
     - update TM1638 display
  ************************************************************
*/
  
void loop() {
    
  // get current time in millis
  unsigned long cur_time = millis();

  if( (cur_time - last_loop_time) >  LOOP_PERIOD_MS) {
    last_loop_time = cur_time;
    //    Serial.print("loop time ");
    //    Serial.println(cur_time, DEC);

    //may not need since included in get joystick
    // Check if joystick is on
    

    //Checking before loop
//    Serial.print("Before the motors - ");
//    Serial.print("    curL: ");
//    Serial.print(rover_cur_spd_lt, DEC);
//    Serial.print("   curR: ");
//    Serial.println(rover_cur_spd_rt, DEC);

    // Get JoyStick Commands
    getJoystick();
//    Serial.print("After get joystick - ");
//    Serial.print("    curL: ");
//    Serial.print(rover_cur_spd_lt, DEC);
//    Serial.print("   curR: ");
//    Serial.println(rover_cur_spd_rt, DEC);
    // Delta Motor Controls Added
    // 12/14/2017


    //***********
    // Get Thing
    //***********
    // Processing packets from SparkFun Thing sent from Wilbur
    getThing();

    //***********
    // Get Danger
    //***********
    // Detects Bumper Collisions and Checks Currents
    getDanger();





    // TODO: process commands from future serial input
    // TBD....
    
    // If data comes in from serial monitor, send it out to XBee  
    // this is our debug route
    //    if (Serial.available()) {
    //      Serial.println("sending something...");
    //      XBee.write(Serial.read());
    //    }
  
    // set new speed based on time and current parameters
    setSpeed();

    Serial.print("goalL: ");
    Serial.print(rover_goal_spd_lt, DEC);
    Serial.print("   goalR: ");
    Serial.print(rover_goal_spd_rt, DEC);
    Serial.print("After set speed");
    Serial.print("    curL: ");
    Serial.print(rover_cur_spd_lt, DEC);
    Serial.print("   curR: ");
    Serial.print(rover_cur_spd_rt, DEC);
    //    Serial.print("battL: ");
    //    Serial.print(mcL_batt, DEC);
    //    Serial.print("   battR: ");
    //    Serial.print(mcR_batt, DEC);
//    Serial.print("   jscnt: ");
//    Serial.println(jscmd_cnt, DEC);
  

    // drive the newly calculated speed
    driveMotors();

    // update the display status
    refresh_tm1638();

    // update values of motor controller
    //getStatus();

    // send information up to web client
    //sendTelemetry();

    // check if arduino is alive
    //ping();
  

    // TODO: get MC battery levels. If 0, MC is not responding (ie: error)
    //get_roboclaw_status();
    
    // TODO: get button states from TM1638
    //process_tm1638_keys();
    
  } // big if 50Hz loop
  else {
    delay(LOOP_PERIOD_MS / 10);
  }
  
} // loop()


