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

  
  unsigned long cur_time = millis();                         // get current time in millis

  if( (cur_time - last_loop_time) >  LOOP_PERIOD_MS) {

 // **************   set visual indicator for Ludicrous mode
    if(drive_mode == LUDICROUS) {
      digitalWrite(BIGLIGHT_PIN, HIGH);
    }
    else {
      digitalWrite(BIGLIGHT_PIN, LOW);
    }    


    last_loop_time = cur_time;
     //    Serial.print("loop time ");
    //    Serial.println(cur_time, DEC);

                                                              // TODO: get MC battery levels. If 0, MC is not responding (ie: error)
                                                              //get_roboclaw_status();
    
                                                              // TODO: get button states from TM1638
                                                              //process_tm1638_keys();
    process_joystick_inputs();                                 // process JS inputs from XBee

                                                            // TODO: process commands from future serial input
                                                            // TBD....
                                                            
                                                            // If data comes in from serial monitor, send it out to XBee  
                                                            // this is our debug route
                                                        //    if (Serial.available()) {
                                                        //      Serial.println("sending something...");
                                                        //      XBee.write(Serial.read());
                                                        //    }
  
    getThing()
    set_new_speed();                                      // set new speed based on time and current parameters

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
    


  
    
    drive_motors();                                     // drive the newly calculated speed
    // ping();                                        //
    // getThing(); 
    refresh_tm1638();                                 // update the display status
    

  } // big if 50Hz loop
  else {
    delay(LOOP_PERIOD_MS / 10);
  }
  
} // loop()


