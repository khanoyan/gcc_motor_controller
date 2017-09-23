Glendale Community College Robotics: MC_GAMEPAD
=====

# GamePad Software

Wireless GamePad V2.0 contains an XBee that can communicate with the XBee Zigbee. 

## Setup

The gamepad can be connected through USB Programmable Interface where Arduino IDE can upload code.

## Software Documentation

Note: Code contains prewritten code from manufacturer that is not used. Also contains unimplemented code for vibration and collision detection.

### Connection Information
The GamePad sends commands at a frequency of 20 HZ. 
* Connection to USB is set at 57600 Baud.
* XBee module is set at 9600 Baud.

Pin 3: Select  
Pin 4: Start  
Pin 5: Up  
Pin 6: Left  
Pin 7: Down  
Pin 8: Right  
Pin 9: Button 1  
Pin 10: Button 4  
Pin 11: Button 2  
Pin 12: Button 3  
Pin 13: Right Bumper 1  
Pin 14: Right Bumper 2  
Pin 15: Left Bumper 1  
Pin 16: Left Bumper 2  

### Command Details

Two bytes of data are sent via serial communication. Each byte takes half the control data. For the sake of compressing the information, the button data is saved as an int but bitwise operators are used to compress that data to a single byte.  
Digital read of the button information will save as an Int, either a 1 or a 0 (representing pressed or not pressed). The bitwise operators will shift the data over to the correct spot on the byte.

xBeeMsg[0]:  

|Byte Index: |7             |6     |5     |4     |3     |2     |1     |0     |
|---| --- | --- | --- | --- | --- | --- | --- | --- |
|Corresponding Control: |Byte Identifer (0 for first Byte)|Down |Left |Right |Right Bumper 1|Right Bumper 2 |Left Bumper 1 |Left Bumper 2 |

#### Line 141-157: Bitwise Operations  

If the Down button is pressed its value is represented as a 1. The bitwise left shift operator << moved the bit over 6 times. The down value is now represented as 0b01000000. This value is added to the message bit and now the controller information is compressed to a single byte.  

If the xBeeMsg is equal to 01001011, the down button, right bumper 1, left bumper 1, and left bumper 2 are all pressed.  

###### Documentation Written by [Charlson So](github.com/so0p)