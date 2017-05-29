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

// for RoboClaw
#include <HardwareSerial.h>
//Roboclaw Address
#define address 0x80

// for TM1638 display unit
#include <TM1638.h>

//#define cmdDelay 150


#define BIGLIGHT_PIN 31

// *******************************
// **   Parameters 
// *******************************

#define SPEED_SETTINGS       3   // number of speed settings
#define VEL_SLOW           400   // velocity preset
#define VEL_FAST          1200   // velocity preset
#define VEL_LUDICROUS    8000   // use this carefully!!!!
#define RAMP_RATE_SLOW      40   // the ramp rate for motor speed enveloping
#define RAMP_RATE_FAST      120   // the ramp rate for motor speed enveloping
#define RAMP_RATE_LUDICROUS 1000   // the ramp rate for motor speed enveloping
#define THR_SLOW            20   // goal threshold
#define THR_FAST            60   // goal threshold
#define THR_LUDICROUS       100   // goal threshold

#define DIAG_OFFSET_RATE    2

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
};

struct DRIVE_PARAMS {
  int vel;
  int ramp;
  int thr;
  int diag;
};

// global variables
// ===================
//bool megaSpeed = 0;
//int velocity = FAST_VELOCITY;

byte drive_mode = SLOW;
bool hillMode = true;

DRIVE_PARAMS param[SPEED_SETTINGS];


unsigned long last_loop_time = 0;
JOYSTICK_CMD jscmd;
unsigned long jscmd_cnt = 0; // count of commands from joystick

// current and goal speeds for each side
int cur_spd_lt  = 0;
int cur_spd_rt  = 0;
int goal_spd_lt = 0;
int goal_spd_rt = 0;

byte tm1638_keys  = 0;

bool eStop = false;

unsigned int mcL_batt = 250; //0;
unsigned int mcR_batt = 250; //0;

bool batteryOK = true;


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

    // get MC battery levels. If 0, MC is not responding (ie: error)
    //get_roboclaw_status();
    
//    Serial.print("loop time ");
//    Serial.println(cur_time, DEC);

    // get button states from TM1638
    //process_tm1638_keys();
    
    // process JS inputs from XBee
    process_joystick_inputs();

    // process commands from future serial input
    // TBD....
    
    // If data comes in from serial monitor, send it out to XBee  
    // this is our debug route
//    if (Serial.available()) {
//      Serial.println("sending something...");
//      XBee.write(Serial.read());
//    }
  
    // set new speed
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
    

    if(drive_mode == LUDICROUS) {
      digitalWrite(BIGLIGHT_PIN, HIGH);
    }
    else {
      digitalWrite(BIGLIGHT_PIN, LOW);
    }    
  
    // drive new speed
    drive_motors();


    // update the display status
    refresh_tm1638();
   
  } // big if 50Hz loop
  else {
    delay(LOOP_PERIOD_MS / 10);
  }
  
} // loop()


#if 0    
void OLD_loop() {

  // If data comes in from serial monitor, send it out to XBee  
  if (Serial.available()) {
    Serial.println("sending something...");
    XBee.write(Serial.read());
  }


  // check for data from XBee link
  if (XBee.available()) {

    byte input = XBee.read();
    //Serial.println(input, BIN);
    XBee.flush();
    
    switch(input){

      case 0xA0: // toggle megaSpeed!
        megaSpeed = !megaSpeed;
        if(megaSpeed) {
          velocity = FAST_VELOCITY;
        }
        else {
          velocity = SLOW_VELOCITY;
        }
        break;
      
      case 0xc0: // xbeeMsg[1]= "1100 0000":
        Serial.println("Drive Forward");
        roboclawL.SpeedM1(address,velocity);
        roboclawL.SpeedM2(address,velocity);
        roboclawR.SpeedM1(address,velocity);
        roboclawR.SpeedM2(address,velocity);                        
        delay(cmdDelay);
        break;
      
      case 0x40: //  xbeeMsg[0]= "0100 0000":
        Serial.println("Drive Backward");
        roboclawL.SpeedM1(address,-velocity);
        roboclawL.SpeedM2(address,-velocity);
        roboclawR.SpeedM1(address,-velocity);
        roboclawR.SpeedM2(address,-velocity); 
        delay(cmdDelay);
        break;

      case 0x10: // xbeeMsg[1]= "0010 0000":
        Serial.println("Drive Right");
        roboclawL.SpeedM1(address,velocity);
        roboclawL.SpeedM2(address,velocity);
        roboclawR.SpeedM1(address,-velocity);
        roboclawR.SpeedM2(address,-velocity);         
        delay(cmdDelay);
        break;

      case 0x20: // xbeeMsg[1]= "0010 0000":
        Serial.println("Drive Left");
        roboclawL.SpeedM1(address,-velocity);
        roboclawL.SpeedM2(address,-velocity);
        roboclawR.SpeedM1(address,velocity);
        roboclawR.SpeedM2(address,velocity);          
        delay(cmdDelay);
        break;        
        
      default:
        roboclawL.BackwardM1(address,0); //Stop Motor1 
        roboclawL.BackwardM2(address,0); //Stop Motor2 
        roboclawR.BackwardM1(address,0); //Stop Motor1 
        roboclawR.BackwardM2(address,0); //Stop Motor2 
        break;
    } // switch()
    
  } // if (XBee.available())
  
} // void loop()
#endif
