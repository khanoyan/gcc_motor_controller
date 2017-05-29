// given the current commands, set the drive speed value

void set_new_speed() {

  int goal_thr = param[drive_mode].thr;
  int goal_ramp = param[drive_mode].ramp;

  if(eStop) { 
    goal_spd_lt = 0;
    goal_spd_rt = 0;
    cur_spd_lt = 0;
    cur_spd_rt = 0;
  }

  else {  // system OK, figure out next speed

    // figure out left side
    if( !( (goal_spd_lt-goal_thr < cur_spd_lt) && 
           (goal_spd_lt+goal_thr > cur_spd_lt) ) ) {  // if NOT within RANGE
      // are we increasing or decreasing?
      if(cur_spd_lt < goal_spd_lt) {
        cur_spd_lt += goal_ramp;
      }
      else {
        cur_spd_lt -= goal_ramp;
      }
    }

    // figure out right side
    if( !( (goal_spd_rt-goal_thr < cur_spd_rt) && 
           (goal_spd_rt+goal_thr > cur_spd_rt) ) ) {  // if NOT within RANGE
      // are we increasing or decreasing?
      if(cur_spd_rt < goal_spd_rt) {
        cur_spd_rt += goal_ramp;
      }
      else {
        cur_spd_rt -= goal_ramp;
      }      
    }

  } // else figure out next speed
  
} // set_new_speed()

