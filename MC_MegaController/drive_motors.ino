// drive with velocity command
void drive_motors() {

  if(batteryOK && mcL_batt>1 && mcR_batt>1) {  // IF ALL OK... 
    if(cur_spd_lt != 0 || cur_spd_rt != 0) {
      roboclawL.SpeedM1(address,cur_spd_lt);
      roboclawL.SpeedM2(address,cur_spd_lt);
      roboclawR.SpeedM1(address,cur_spd_rt);
      roboclawR.SpeedM2(address,cur_spd_rt); 
    }

    else if(hillMode) {
      roboclawL.SpeedM1(address,0);
      roboclawL.SpeedM2(address,0);
      roboclawR.SpeedM1(address,0);
      roboclawR.SpeedM2(address,0);      
      // drive to velocity 0
    }
    else if(!hillMode) {
      roboclawL.BackwardM1(address,0); //Stop Motor1 
      roboclawL.BackwardM2(address,0); //Stop Motor2 
      roboclawR.BackwardM1(address,0); //Stop Motor1 
      roboclawR.BackwardM2(address,0); //Stop Motor2      
      // don't drive the motors anymore... 
    }
  } // if everything is OK

  else if(!batteryOK) {
    roboclawL.BackwardM1(address,0); //Stop Motor1 
    roboclawL.BackwardM2(address,0); //Stop Motor2 
    roboclawR.BackwardM1(address,0); //Stop Motor1 
    roboclawR.BackwardM2(address,0); //Stop Motor2      
  }
} // drive_motors()


// get roboclaw's battery levels
// if roboclaw is not responsive, set voltage to 0. this will
// indicate that RC communication is dead
void get_roboclaw_status() {
  bool rcL_alive = false;
  bool rcR_alive = false;  

  mcL_batt = roboclawL.ReadMainBatteryVoltage(address, &rcL_alive);
  mcR_batt = roboclawR.ReadMainBatteryVoltage(address, &rcR_alive);  

  // if battery level gets low, completely stop driving
  if(mcL_batt<227 || mcR_batt<226) {
    batteryOK = false;
  }
 
  if(!rcL_alive) {
    mcL_batt = 0;
  }
  if(!rcR_alive) {
    mcR_batt = 0;
  }

} // get_roboclaw_status()


