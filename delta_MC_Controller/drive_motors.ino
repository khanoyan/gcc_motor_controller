// drive with velocity command
void drive_motors() {

  if (ROBOT_ID==0) {

  if(batteryOK && mcL_batt>1 && mcR_batt>1) {  // IF ALL OK... 
    if(cur_spd_lt != 0 || cur_spd_rt != 0) {
      roboclaw1.SpeedM1(address,rover_cur_spd_lt);
      roboclaw1.SpeedM2(address,rover_cur_spd_lt);
      roboclaw2.SpeedM1(address,rover_cur_spd_rt);
      roboclaw2.SpeedM2(address,rover_cur_spd_rt); 
    }

    else if(hillMode) {
      roboclaw1.SpeedM1(address,0);
      roboclaw1.SpeedM2(address,0);
      roboclaw2.SpeedM1(address,0);
      roboclaw2.SpeedM2(address,0);      
      // drive to velocity 0
    }
    else if(!hillMode) {
      roboclaw1.BackwardM1(address,0); //Stop Motor1 
      roboclaw1.BackwardM2(address,0); //Stop Motor2 
      roboclaw2.BackwardM1(address,0); //Stop Motor1 
      roboclaw2.BackwardM2(address,0); //Stop Motor2      
      // don't drive the motors anymore... 
    }
  } // if everything is OK

  else if(!batteryOK) {
    roboclaw1.BackwardM1(address,0); //Stop Motor1 
    roboclaw1.BackwardM2(address,0); //Stop Motor2 
    roboclaw2.BackwardM1(address,0); //Stop Motor1 
    roboclaw2.BackwardM2(address,0); //Stop Motor2      
  }
} // drive_motors()


// get roboclaw's battery levels
// if roboclaw is not responsive, set voltage to 0. this will
// indicate that RC communication is dead
void get_roboclaw_status() {
  bool rc1_alive = false;
  bool rc2_alive = false;  

  mc1_batt = roboclaw1.ReadMainBatteryVoltage(address, &rc1_alive);
  mc2_batt = roboclaw2.ReadMainBatteryVoltage(address, &rc2_alive);  

  // if battery level gets low, completely stop driving
  if(mc1_batt<227 || mc2_batt<226) {
    batteryOK = false;
  }
 
  if(!rc1_alive) {
    mc1_batt = 0;
  }
  if(!rc2_alive) {
    mc2_batt = 0;
  }

} // get_roboclaw_status()

  else if (ROBOT_ID==1) {

if(batteryOK && mcL_batt>1 && mcR_batt>1) {  // IF ALL OK... 
    if(cur_spd_lt != 0 || cur_spd_rt != 0) {
      RoboClaw1.SpeedM1(address,arm_cur_spd_1);
      RoboCLaw1.SpeedM2(address,arm_cur_spd_2);
      RoboClaw2.SpeedM3(address,arm_cur_spd_3);
      RoboClaw2.SpeedM4(address,arm_cur_spd_4); 
      RoboClaw3.SpeedM5(address,arm_cur_spd_5);
      RoboClaw3.SpeedM6(address,arm_cur_spd_6);
    }
  }
