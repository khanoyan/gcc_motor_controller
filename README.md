## process_joystick_inputs.ino

##### Setting Speeds and Directions For Buttons on Joystick

* Parses binary data from XBee while using AND operator to determine which button on joystick is pressed
    * First digit of byte, either 1 or 0, determines which set of buttons we are inputting because we are using binary (8 commands per byte). Look at line 188 and 189 for specific commands in each byte.


Each button corresponds to a certain speed and direction

Displays joystick inputs



## set_new_speed.ino

Sets the speed of the motor based of which mode it is. The three modes are slow, fast, and ludicrous. The speed of the motors increase based off a ramp rate and max out at a speed threshold.



/*
setup.ino
XBee Baud Rate: 9600
Serial Baud Rate: 28400
RoboClaw Baud Rate: 38400

Setups the speed variables and displays hello to your display.
*/



## tm1638.ino 

Displays the mode of rover; ludicrous mode(high speed), braking, or open,
LED screen displays remaining battery power
