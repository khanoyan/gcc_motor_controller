// read and parse full packet from XBee interface and 
// populate the jscmd struct
//

void process_joystick_inputs() {

  static bool last_b4 = false;  // last state of B4 button

  static int lud_hold_count = 0;
  static int lud_drive_timer = 0; 
  
  // read from XBee stream. parse and update jscmd struct
  byte bytes_to_read = XBee.available();
  if(bytes_to_read >= 2) {
    
    for(byte i = 0; i< bytes_to_read; i++) {
      parse_xbee_byte();
      jscmd_cnt++;
    }

    // check if R2 is pressed so that we switch to FAST
    // FAST mode is a momentary switch
    if(drive_mode != LUDICROUS) {
      if(jscmd.r2) {  drive_mode = FAST;   }
      else         {  drive_mode = SLOW;   }
    }

    // check if button 4 state has changed...
    // b4 tells us to toggle the hold mode
    bool cur_b4 = jscmd.b4;
    if(!last_b4 && cur_b4) {
      hillMode = !hillMode;
    }
    last_b4 = cur_b4;

    // detect proper key combo to enter LUDICROUS mode
    if(jscmd.b3 && jscmd.l2 && jscmd.r1) {
      lud_hold_count++;
    }
    else {
      lud_hold_count = 0;
    }
    if(lud_hold_count >= LUDICROUS_COMBO_HOLD_COUNT) {
      drive_mode = LUDICROUS;
      param[LUDICROUS].vel = VEL_LUDICROUS;
      lud_drive_timer = LUDICROUS_DRIVE_TIME;
      lud_hold_count = 0;
    }

    // if in LUDICROUS mode, stop after TBD seconds
    if(drive_mode == LUDICROUS) {
      lud_drive_timer--;

      if(lud_drive_timer < 8) {
        param[LUDICROUS].vel -= RAMP_RATE_LUDICROUS;
      }
      
      if(lud_drive_timer == 0) {
        drive_mode = SLOW;
//        cur_spd_lt = param[SLOW].vel;
//        cur_spd_rt = param[SLOW].vel;        
      }
    }
    
    // update the goal speeds
    set_goal_speed();
    show_joystick_inputs();
  }

  // also check if this link is alive... if TBD millis have gone
  // since last msg, notify main program

  // every TBD cycles, send a HB message to joystick
  
} // process_joystick_inputs()



// set our goal speeds based on the joystick inputs
void set_goal_speed() {
  if(jscmd.up && jscmd.lt) {
    goal_spd_lt = param[drive_mode].vel - param[drive_mode].diag;
    goal_spd_rt = param[drive_mode].vel + param[drive_mode].diag;    
  }
  else if(jscmd.up && jscmd.rt) {
    goal_spd_lt = param[drive_mode].vel + param[drive_mode].diag;
    goal_spd_rt = param[drive_mode].vel - param[drive_mode].diag;    
  }
  else if(jscmd.dn && jscmd.lt) {
    goal_spd_lt = -param[drive_mode].vel + param[drive_mode].diag;
    goal_spd_rt = -param[drive_mode].vel - param[drive_mode].diag;    
  }
  else if(jscmd.dn && jscmd.rt) {
    goal_spd_lt = -param[drive_mode].vel - param[drive_mode].diag;
    goal_spd_rt = -param[drive_mode].vel + param[drive_mode].diag;    
  }  
  else if(jscmd.up) {
    goal_spd_lt = param[drive_mode].vel;
    goal_spd_rt = param[drive_mode].vel;
  }
  else if(jscmd.dn) {
    goal_spd_lt = -param[drive_mode].vel;
    goal_spd_rt = -param[drive_mode].vel;
  }
  else if(jscmd.lt) {
    goal_spd_lt = -param[drive_mode].vel;
    goal_spd_rt = param[drive_mode].vel;    
  }
  else if(jscmd.rt) {
    goal_spd_lt = param[drive_mode].vel;
    goal_spd_rt = -param[drive_mode].vel;    
  }
  else {
    goal_spd_lt = 0;
    goal_spd_rt = 0;     
  }
  
 
} // set_goal_speed()

// set our goal speeds based on the joystick inputs
void set_goal_speed_working() {
  if(jscmd.up && jscmd.lt) {
    goal_spd_lt = param[drive_mode].vel;
    goal_spd_rt = param[drive_mode].vel + param[drive_mode].diag;    
  }
  else if(jscmd.up && jscmd.rt) {
    goal_spd_lt = param[drive_mode].vel + param[drive_mode].diag;
    goal_spd_rt = param[drive_mode].vel;    
  }
  else if(jscmd.dn && jscmd.lt) {
    goal_spd_lt = -param[drive_mode].vel;
    goal_spd_rt = -param[drive_mode].vel - param[drive_mode].diag;    
  }
  else if(jscmd.dn && jscmd.rt) {
    goal_spd_lt = -param[drive_mode].vel - param[drive_mode].diag;
    goal_spd_rt = -param[drive_mode].vel;    
  }  
  else if(jscmd.up) {
    goal_spd_lt = param[drive_mode].vel;
    goal_spd_rt = param[drive_mode].vel;
  }
  else if(jscmd.dn) {
    goal_spd_lt = -param[drive_mode].vel;
    goal_spd_rt = -param[drive_mode].vel;
  }
  else if(jscmd.lt) {
    goal_spd_lt = -param[drive_mode].vel;
    goal_spd_rt = param[drive_mode].vel;    
  }
  else if(jscmd.rt) {
    goal_spd_lt = param[drive_mode].vel;
    goal_spd_rt = -param[drive_mode].vel;    
  }
  else {
    goal_spd_lt = 0;
    goal_spd_rt = 0;     
  }
  
 
} // set_goal_speed()


// debug code for showing joystick inputs
void show_joystick_inputs() {
  if(jscmd.up) Serial.print("UP ");
  if(jscmd.dn) Serial.print("DN ");
  if(jscmd.lt) Serial.print("LT ");
  if(jscmd.rt) Serial.print("RT ");
  if(jscmd.r1) Serial.print("R1 ");
  if(jscmd.r2) Serial.print("R2 ");
  if(jscmd.l1) Serial.print("L1 ");
  if(jscmd.l2) Serial.print("L2 ");
  if(jscmd.b1) Serial.print("B1 ");
  if(jscmd.b2) Serial.print("B2 ");
  if(jscmd.b3) Serial.print("B3 ");
  if(jscmd.b4) Serial.print("B4 ");
  if(jscmd.st) Serial.print("ST ");
  if(jscmd.se) Serial.print("SE ");
//  Serial.print(jscmd_cnt, DEC);
  Serial.println();   
} // show_joystick_inputs()


// XBee packet definition from joystick
// =====================================
// pack buttonState[] into xbeeMsg
// xbeeMsg[0] : { 0 , DN , L , R , R1 , R2 , L1 , L2 }
// xbeeMsg[1] : { 1 , UP , B1 , B2 , B3 , B4 , ST , SE } 
//

// read one byte from XBee stream and parse its content
void parse_xbee_byte() {
  byte xb = XBee.read();
  if(xb > 0x7f) {  // we're reading Msg 1
    jscmd.se = (xb & B00000001);
    jscmd.st = (xb & B00000010);
    jscmd.b4 = (xb & B00000100);
    jscmd.b3 = (xb & B00001000);
    jscmd.b2 = (xb & B00010000);
    jscmd.b1 = (xb & B00100000);
    jscmd.up = (xb & B01000000);
  }
  else {           // we're reading Msg 0
    jscmd.l2 = (xb & B00000001);
    jscmd.l1 = (xb & B00000010);
    jscmd.r2 = (xb & B00000100);
    jscmd.r1 = (xb & B00001000);
    jscmd.rt = (xb & B00010000);
    jscmd.lt = (xb & B00100000);
    jscmd.dn = (xb & B01000000);    
  }
} // parse_xbee_byte()

