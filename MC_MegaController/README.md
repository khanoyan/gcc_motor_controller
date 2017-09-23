# MC_MegaController

 1. MC_MegaController.ino
 2. drive_motors.ino
 3. process_joystick_inputs.ino
 4. set_new_speed.ino
 5. setup.ino
 6. tm1638.ino




## MC_MegaController.ino

* Using RoboClaw, SoftwareSerial, HardwareSerial, TM1638 libraries 

Setting the loop to be at 50Hz per 20 milliseconds
* In each cycle:
	Getting input from joystick and sending output to joystick
	Determining and setting new motor speed

##### Defining Driver Parameters and Setting Speed of Motor
* Defining multiple settings speed and ramp rate to have a better control of the rover as it travels over different surface levels.

##### Visual Indicator For 'Ludicrous mode'
We have a BIGLIGHT_PIN that turns on for when rover’s speed is set at its highest, “Ludicrous mode”


## drive_motors.ino

##### Checking battery status:
We are using a rechargeable battery, and in order to maintain its rechargeability, the battery cannot be completely depleted. Therefore, when the battery levels are too low (less than 226 volts), rover and roboclaw completely stops their movement in order to preserve the last bit of energy remaining in battery and avoid depletion.

##### Limiting Rover Motions
* Completely stopping rover motion
    * Battery Check: Battery level is sufficient when battery voltage is >= 226 volts. If voltage < 226 volts, rover completely stops driving
* Rover approaching an inclined surface

