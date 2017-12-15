void setup() {
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(57600);


  //Open roboclaw serial ports
  roboclaw1.begin(38400);
  roboclaw2.begin(38400);

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
  param[ARM].vel        = ARM_VEL;
  param[ARM].ramp       = ARM_RAMP_RATE;
  param[ARM].thr        = ARM_THR;
   

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

  int XBEE_COUNT = 0;

  while(XBee.avaiable()<1 && XBEE_COUNT < 6){
    delay(1000);
    count++;
  }
  if(XBee.avaiable()){
    XBEE_ON = true;
  }
  else{
    XBEE_OFF = false;
  }

  //set the counter to 0
  xbee_counter =0;

  //Intializing Danger Variables
  //Globals
  //***********************
  bool dangerFront = false;
  bool dangerBack = false;

  uint16_t currentM1 = 0;
  uint16_t currentM2 = 0;
  uint16_t currentM3 = 0;
  uint16_t currentM4 = 0;
  uint16_t currentM5 = 0;
  uint16_t currentM6 = 0;

  bool dangerOverride = false;

  bool dangerM1 = false;
  bool dangerM2 = false;
  bool dangerM3 = false;
  bool dangerM4 = false;
  bool dangerM5 = false;
  bool dangerM6 = false;   

  bool FrontRight = false;
  bool FrontLeft = false;
  bool BackRight = false;
  bool BackLeft = false;
  //***********************

  int M1_thresh = 1;
  int M2_thresh = 2;
  int M3_thresh = 3;
  int M4_thresh = 4;
  int M5_thresh = 5;
  int M6_thresh = 6;              // undertermined values; requires testing

  int BUTTON_PIN_FRONT_R = 43;
  int BUTTON_PIN_FRONT_L = 41;
  int BUTTON_PIN_BACK_R = 47;
  int BUTTON_PIN_BACK_L = 45;     // arbitrary pin locations

  int buttonStateFront_R = 0;
  int buttonStateFront_L = 0;
  int buttonStateBack_R = 0;
  int buttonStateBack_L = 0;

  //Initalizing Bumper Pins
  pinMode(BUTTON_PIN_FRONT_R, INPUT);
  pinMode(BUTTON_PIN_FRONT_L, INPUT);
  pinMode(BUTTON_PIN_BACK_R, INPUT);
  pinMode(BUTTON_PIN_BACK_L, INPUT);
      
} // setup()
