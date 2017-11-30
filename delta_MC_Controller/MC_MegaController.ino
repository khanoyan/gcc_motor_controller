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

*****************************************************************/
//Includes required to use Roboclaw library
#include "RoboClaw.h"

// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>

// .. and Hardware Serial (on the Arduino MEGA) for RoboClaw control
#include <HardwareSerial.h>
//Roboclaw Address
#define address 0x80

// for TM1638 display unit
#include <TM1638.h>

//#define cmdDelay 150

// Digital output for visual indication of Ludicrous mode
#define BIGLIGHT_PIN 31

// ******************************
// ***** Robot ID *****
// ***** Defines which robot the software is on. 
// ***** 0 for Life of the Party and 1 for Armando 
//*******************************

#define ROBOT_ID 30


// *******************************
// **   Parameters 
// *******************************
#define SPEED_SETTINGS         3   // number of speed settings
#define VEL_SLOW             400   // velocity preset
#define VEL_FAST            1200   // velocity preset
#define VEL_LUDICROUS       8000   // use this carefully!!!!
#define RAMP_RATE_SLOW        40   // the ramp rate for motor speed enveloping
#define RAMP_RATE_FAST       120   // the ramp rate for motor speed enveloping
#define RAMP_RATE_LUDICROUS 1000   // the ramp rate for motor speed enveloping
#define THR_SLOW              20   // goal threshold
#define THR_FAST              60   // goal threshold
#define THR_LUDICROUS        100   // goal threshold

#define DIAG_OFFSET_RATE       2   // ????


// what is this???
#define DIAG_OFFSET_SLOW          (VEL_SLOW/DIAG_OFFSET_RATE)
#define DIAG_OFFSET_FAST          (VEL_FAST/DIAG_OFFSET_RATE)
#define DIAG_OFFSET_LUDICROUS     (VEL_LUDICROUS/DIAG_OFFSET_RATE)

// define the name for the drive modes
#define SLOW      0
#define FAST      1
#define LUDICROUS 2

#define LUDICROUS_COMBO_HOLD_COUNT 60 // number of cycles to hold
                                      // combination to trigger LUD mode
#define LUDICROUS_DRIVE_TIME 120                                       

#define LOOP_PERIOD_MS 20   // 50Hz Loop


// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
// on xbee shield, place jumper on XB_TX_11 and DIGITAL, and
// XB_RX_10 and DIGITAL
SoftwareSerial XBee(11, 10); // RX, TX (this is confusing and wrong, probably!)

//Setup communcaitions with roboclaw. Use pins 10 and 11 with 10ms timeout
RoboClaw roboclawL(&Serial2,10000); // 1
RoboClaw roboclawR(&Serial3,10000); // 2

// setup TM1638 module
// pin 48: data         pin 50: clock     pin 52: strobe
TM1638 tm1638(48, 50, 52);



struct JOYSTICK_CMD {
  bool linkActive;      // is the JS link active?
  bool up, dn, lt, rt;  // Up, Down, Left, Right
  bool r1, r2, l1, l2;  // Right1, Right2, Left1, Left2
  bool b1, b2, b3, b4;  // Button1, Button2, Button3, Button4
  bool st, se;          // Start, Select
  // TODO: add analogs
};

struct DRIVE_PARAMS {
  int vel;
  int ramp;
  int thr;
  int diag;
};

// global variables
// ===================
byte drive_mode = SLOW;    // indicates which driving profile is currently used
bool hillMode = true;      // maintain velocity of 0 (ie: brake if not driving)

DRIVE_PARAMS param[SPEED_SETTINGS];  // LUT for driving parameters

unsigned long last_loop_time = 0;
JOYSTICK_CMD jscmd;                  // current joystick command
unsigned long jscmd_cnt = 0;         // count of commands from joystick

// current and goal speeds for each side
int cur_spd_lt  = 0;                 // curent left motor speed
int cur_spd_rt  = 0;                 // curent right motor speed
int goal_spd_lt = 0;                 // left motor goal
int goal_spd_rt = 0;                 // right motor goal

byte tm1638_keys  = 0;               // push button inputs from TM1638

bool eStop = false;                  // emergency stop flag

unsigned int mcL_batt = 250; //0;
unsigned int mcR_batt = 250; //0;

bool batteryOK = true;               // battery status is OK flag

// delete if not being used....
//bool megaSpeed = 0;
//int velocity = FAST_VELOCITY;


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
  *************************************************************/
void loop() {

  // get current time in millis
  unsigned long cur_time = millis();

  if( (cur_time - last_loop_time) >  LOOP_PERIOD_MS) {
    last_loop_time = cur_time;
//    Serial.print("loop time ");
//    Serial.println(cur_time, DEC);

    // TODO: get MC battery levels. If 0, MC is not responding (ie: error)
    //get_roboclaw_status();
    
    // TODO: get button states from TM1638
    //process_tm1638_keys();
    
    // process JS inputs from XBee
    process_joystick_inputs();

    // TODO: process commands from future serial input
    // TBD....
    
    // If data comes in from serial monitor, send it out to XBee  
    // this is our debug route
//    if (Serial.available()) {
//      Serial.println("sending something...");
//      XBee.write(Serial.read());
//    }
  
    // set new speed based on time and current parameters
    set_new_speed();

    Serial.print("goalL: ");
    Serial.print(goal_spd_lt, DEC);
    Serial.print("   goalR: ");
    Serial.print(goal_spd_rt, DEC);
    Serial.print("    curL: ");
    Serial.print(cur_spd_lt, DEC);
    Serial.print("   curR: ");
    Serial.print(cur_spd_rt, DEC);
//    Serial.print("battL: ");
//    Serial.print(mcL_batt, DEC);
//    Serial.print("   battR: ");
//    Serial.print(mcR_batt, DEC);
    Serial.print("   jscnt: ");
    Serial.println(jscmd_cnt, DEC);
    

    // set visual indicator for Ludicrous mode
    if(drive_mode == LUDICROUS) {
      digitalWrite(BIGLIGHT_PIN, HIGH);
    }
    else {
      digitalWrite(BIGLIGHT_PIN, LOW);
    }    
  
    // drive the newly calculated speed
    drive_motors();

    // update the display status
    refresh_tm1638();
   
  } // big if 50Hz loop
  else {
    delay(LOOP_PERIOD_MS / 10);
  }
  
} // loop()


