/*  
 *   First see what kind of input it is:
 *          1) Arm    robotID = 1
 *          2) Rover  robotID = 0
 *   if arm do "this" code else if rover do "this" code
 *   
 *   dangerOverride = 1 means ignore the "danger"
 *                  = 0 do danger code (see getDanger)
 */

 
void set_speed()
{
  int goal_thr = param[vel_mode].thr;
  int goal_ramp = param[vel_mode].ramp;

if(eStop) { 
    goal_spd_lt = 0;
    goal_spd_rt = 0;
    cur_spd_lt = 0;
    cur_spd_rt = 0;
    
    goal_spd_m1 = 0;
    goal_spd_m2 = 0;
    goal_spd_m3 = 0;
    goal_spd_m4 = 0; 
    goal_spd_m5 = 0;
    goal_spd_m6 = 0;

    cur_spd_m1 = 0;
    cur_spd_m2 = 0;
    cur_spd_m3 = 0;
    cur_spd_m4 = 0;
    cur_spd_m5 = 0;
    cur_spd_m6 = 0;
  }

  else {

 //************************Rover Mode*******************************
    
     if( !( (goal_spd_lt-goal_thr < cur_spd_lt) && 
           (goal_spd_lt+goal_thr > cur_spd_lt) ) ) {  

        if(cur_spd_lt < goal_spd_lt) {
           cur_spd_lt += goal_ramp;
           }
        else {
          cur_spd_lt -= goal_ramp;
          }
     }

      if( !( (goal_spd_rt-goal_thr < cur_spd_rt) && 
           (goal_spd_rt+goal_thr > cur_spd_rt) ) ) { 

    if(cur_spd_rt < goal_spd_rt) {
              cur_spd_rt += goal_ramp;
           }
           else {
                cur_spd_rt -= goal_ramp;
           }      
         }
      
   }  


// ********************* Arm Mode **********************
  if (robotID = 1)
  {       
  
  if ( ! ( (goal_spd_m1-goal_thr < cur_spd_m1)  && 
            (goal_spd_m1+goal_thr > cur_spd_m1) )  )       // if speed is not within range 
     {  
    if (cur_spd_m1 < goal_spd_m1)
      {
      cur_spd_m1 += goal_ramp;      // increase speed
       }
     else {  
     cur_spd_m1 -= goal_ramp;     //decrease speed
      }
     }
    if ( ! ( (goal_spd_m2-goal_thr < cur_spd_m2)  && 
        (goal_spd_m2 + goal_thr > cur_spd_m2) ) )       // Motor 2
     {  
        if (cur_spd_m2 < goal_spd_m2)
        {
            cur_spd_m2 += goal_ramp;
        }
    else {  
     cur_spd_m2 -= goal_ramp;
    }
   }
    if ( ! ( (goal_spd_m3-goal_thr < cur_spd_m3)  && 
      (goal_spd_m3 + goal_thr > cur_spd_m3) ) )       // Motor 3
     {  
     if (cur_spd_m3 < goal_spd_m3)
    {
      cur_spd_m3 += goal_ramp;
    }
     else {  
     cur_spd_m3 -= goal_ramp;
      }
    }
  
    if ( ! ( (goal_spd_m4 - goal_thr < cur_spd_m4)  && 
      (goal_spd_m4 + goal_thr > cur_spd_m4) ) )       // Motor 4 
     {  
    if (cur_spd_m4 < goal_spd_m4)
      {
      cur_spd_m4 += goal_ramp;
      }
else {  
     cur_spd_m4 -= goal_ramp;
}
   }

if ( ! ( (goal_spd_m5 - goal_thr < cur_spd_m5)  && 
        (goal_spd_m5 + goal_thr > cur_spd_m5) ) )       // Motor 5 
     {  
    if (cur_spd_m5 < goal_spd_m5)
 {
      cur_spd_m5 += goal_ramp;
 }
else {  
     cur_spd_m5 -= goal_ramp;
}
   }


if ( ! ( (goal_spd_m6 - goal_thr < cur_spd_m6)  && 
        (goal_spd_m6 + goal_thr > cur_spd m6) ) )       // Motor 6 
     {  
    if (cur_spd_m6 < goal_spd_m6)
 {
      cur_spd_m6 += goal_ramp;
 }
else {  
     cur_spd_m6 -= goal_ramp;
    }
   }

}
//************************End of Arm Mode************************ 

  
} //DONE
