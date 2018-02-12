// *******************************
// ** Libraries
// *******************************
#include "RoboClaw.h"                                               //Includes required to use Roboclaw library
#include <SoftwareSerial.h>                                         // We'll use SoftwareSerial to communicate with the XBee:
#include <HardwareSerial.h>                                         // .. and Hardware Serial (on the Arduino MEGA) for RoboClaw control
#include <TM1638.h>                                                 // for TM1638 display unit

// *******************************
// ** Settings
// *******************************
#define address 0x80                                                //Roboclaw Address
//#define cmdDelay 150                                              // ???
#define LUDICROUS_COMBO_HOLD_COUNT 60                               // number of cycles to hold combination to trigger LUD mode
#define LUDICROUS_DRIVE_TIME 120                                    // number of cycles to hold combination to trigger LUD mode                                 
#define LOOP_PERIOD_MS 20                                           // 50Hz Loop
#define BIGLIGHT_PIN 31                                             // Digital output for visual indication of Ludicrous mode


SoftwareSerial XBee(11, 10);                                         // XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
                                                                     // XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
                                                                     // on xbee shield, place jumper on XB_TX_11 and DIGITAL, and
                                                                     // XB_RX_10 and DIGITAL
                                                                     // RX, TX (this is confusing and wrong, probably!)
RoboClaw roboclawL(&Serial2,10000);                                  // Setup communcaitions with roboclaw. Use pins 10 and 11 with 10ms timeout
RoboClaw roboclawR(&Serial3,10000);                                  // Setup communcaitions with roboclaw. Use pins 10 and 11 with 10ms timeout
TM1638 tm1638(48, 50, 52);                                          // setup TM1638 module || pin 48: data         pin 50: clock     pin 52: strobe


unsigned long last_loop_time = 0;                                    // last loop time
byte tm1638_keys  = 0;                                               // push button inputs from TM1638


// *******************************
// **   Speed and Drive Parameters 
// *******************************
struct DRIVE_PARAMS {
  int vel;
  int ramp;
  int thr;
  int diag;
};

#define SPEED_SETTINGS         3                                    // number of speed settings
#define VEL_SLOW             400                                    // velocity preset
#define VEL_FAST            1200                                    // velocity preset
#define VEL_LUDICROUS       8000                                    // use this carefully!!!!
#define RAMP_RATE_SLOW        40                                    // the ramp rate for motor speed enveloping
#define RAMP_RATE_FAST       120                                    // the ramp rate for motor speed enveloping
#define RAMP_RATE_LUDICROUS 1000                                    // the ramp rate for motor speed enveloping
#define THR_SLOW              20                                    // goal threshold
#define THR_FAST              60                                    // goal threshold
#define THR_LUDICROUS        100                                    // goal threshold
#define DIAG_OFFSET_RATE       2                                    // ????
#define DIAG_OFFSET_SLOW          (VEL_SLOW/DIAG_OFFSET_RATE)       // ???
#define DIAG_OFFSET_FAST          (VEL_FAST/DIAG_OFFSET_RATE)       // ???
#define DIAG_OFFSET_LUDICROUS     (VEL_LUDICROUS/DIAG_OFFSET_RATE)  // ???
#define SLOW      0                                                 // define the name for the drive modes
#define FAST      1                                                 // define the name for the drive modes
#define LUDICROUS 2                                                 // define the name for the drive modes


byte drive_mode = SLOW;                                             // indicates which driving profile is currently used
bool hillMode = true;                                               // maintain velocity of 0 (ie: brake if not driving)
                                                                    // current and goal speeds for each side
int cur_spd_lt  = 0;                                                // curent left motor speed
int cur_spd_rt  = 0;                                                // curent right motor speed
int goal_spd_lt = 0;                                                // left motor goal
int goal_spd_rt = 0;                                                // right motor goal
DRIVE_PARAMS param[SPEED_SETTINGS];                                 // LUT for driving parameters


// *******************************
// **   Joystick Variables 
// *******************************
struct JOYSTICK_CMD {
  bool linkActive;                                                   // is the JS link active?
  bool up, dn, lt, rt;                                               // Up, Down, Left, Right
  bool r1, r2, l1, l2;                                               // Right1, Right2, Left1, Left2
  bool b1, b2, b3, b4;                                               // Button1, Button2, Button3, Button4
  bool st, se;                                                       // Start, Select
  // TODO: add analogs
};

JOYSTICK_CMD jscmd;                  // current joystick command
unsigned long jscmd_cnt = 0;         // count of commands from joystick



bool eStop = false;                  // emergency stop flag

unsigned int mcL_batt = 250; //0;
unsigned int mcR_batt = 250; //0;

bool batteryOK = true;               // battery status is OK flag


void setup() {
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(57600);

  //Open roboclaw serial ports
  roboclawL.begin(38400);
  roboclawR.begin(38400);

  jscmd.linkActive = true; // override this value until we check for it
  jscmd.up = false;
  jscmd.dn = false;
  jscmd.lt = false;
  jscmd.rt = false;
  jscmd.b1 = false;
  jscmd.b2 = false;
  jscmd.b3 = false;
  jscmd.b4 = false;
  jscmd.r1 = false;
  jscmd.r2 = false;
  jscmd.l1 = false;
  jscmd.l2 = false;
  jscmd.st = false;
  jscmd.se = false;

  // populate drive param 
  param[SLOW].vel       = VEL_SLOW;
  param[SLOW].ramp      = RAMP_RATE_SLOW;
  param[SLOW].thr       = THR_SLOW;
  param[SLOW].diag      = DIAG_OFFSET_SLOW;
  param[FAST].vel       = VEL_FAST;
  param[FAST].ramp      = RAMP_RATE_FAST;
  param[FAST].thr       = THR_FAST;
  param[FAST].diag      = DIAG_OFFSET_FAST;  
  param[LUDICROUS].vel  = VEL_LUDICROUS;
  param[LUDICROUS].ramp = RAMP_RATE_LUDICROUS;
  param[LUDICROUS].thr  = THR_LUDICROUS;
  param[LUDICROUS].diag = DIAG_OFFSET_LUDICROUS;  

  // connect to TM1638 module
  // display a hexadecimal number and set the left 4 dots
  //tm1638.setDisplayToHexNumber(0x6CC, 0xF0);
  char s[8];
  sprintf(s, " HELLO ");
  tm1638.setDisplayToString(s);
  tm1638.setLEDs(0x0);
  delay(500);

  pinMode(BIGLIGHT_PIN, OUTPUT);
  // flush XBee serial stream before we get into loop()
  XBee.flush();
      
} // end setup()




// delete if not being used....
//bool megaSpeed = 0;
//int velocity = FAST_VELOCITY;
