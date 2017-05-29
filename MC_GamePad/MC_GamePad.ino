/*
// # 
// # Editor     : Tong Hui from DFRobot, based on Lauren from DFRobot v1.0 code
// # Date       : 18.01.2012

// # Product name: Wireless Gamepad v2.2 for Arduino
// # Product SKU : DFR0182
// # Code Version: 2.1

// # Description:
// # The sketch for using the gamepad and print the button state and the analog values of the gamepad
// # Enable the vibration function
// #   Send 'v' via the Arduino Serial monitor to enable the vibration
// #   Send 's' via the Arduino Serial monitor to stop it
 
*/

#define UPDATE_PERIOD_MS 50 // update at 20Hz

int buttonState[17];
int joystick[4];
int AnalogButton[2];

char xbeeMsg[2]; 

int inputCommand = 0;

#define virbrationMotorPin 2

void setup() {
  // Serial is the output via the USB connector
  Serial.begin(57600);  //Init the Serial baudrate

  // Serial1 is the output of XBee module
  Serial1.begin(9600);  //Init the Serial1 port to enable the xbee wireless communication
  InitIO();             //Initialize the inputs/outputs and the buffers
}

void InitIO() { 
  for(int i = 0; i < 17; i++) {
    pinMode(i, INPUT); 
  }

// why do we have two output pins controlling vibration? 
// one of them has to be the LED ???
  
  pinMode(virbrationMotorPin,OUTPUT);
  digitalWrite(virbrationMotorPin,LOW);  // Stop shacking of the gamepad

  pinMode(17,OUTPUT);
  digitalWrite(17,LOW);  // Stop shacking of the gamepad
}

unsigned long timer = 0;
unsigned long timer2 = 0;
bool vibing = false;

void loop() {
  if(millis() - timer > UPDATE_PERIOD_MS){  // manage the updating freq of all the controlling information
    DataUpdate();  //read the buttons and the joysticks data
    printData();   //print the datas and states
    timer = millis(); 
  }

  if(vibing && (millis() - timer2 > 1000) ){  //
    digitalWrite(virbrationMotorPin,LOW);
    vibing = false;
  }

  digitalWrite(17,buttonState[9]);  // Stop shacking of the gamepad

  if(Serial1.available()){
    char input = Serial1.read();
    
    switch(input){
      case 'v':
        Serial1.println("Vibration");
        inputCommand = input;
        digitalWrite(virbrationMotorPin,HIGH);
        timer2 = millis();
        vibing = true;
        break;
      
      case 's':
        Serial1.println("Stop");
        inputCommand = input;
        digitalWrite(virbrationMotorPin,LOW);
        break;
        
      default:
        break;
    }
  } // switch()

} // loop()


void DataUpdate() {
  for(int i = 3; i < 17; i++) {
    buttonState[i] = digitalRead(i);
  }
  buttonState[0] = analogRead(0);
  buttonState[1] = analogRead(1);
  for(int i = 0; i < 4; i++)  { 
    joystick[i] = analogRead(i);
  }
  for(int i = 4; i < 6; i++) {
    AnalogButton[i-4] = analogRead(i);
  }
}

String Buttons[17] = {
  "J2","J1"," ","S2","S1","UP","LEFT","DOWN","RIGHT","1","4","2","3","RZ1","RZ2","LZ1","LZ2"};
  // Buttons Nmes

void printData(){
//  for(int i = 0; i < 17; i++)  Serial.print(buttonState[i]),Serial.print(" ");
//  for(int i = 0; i < 4; i++)  Serial.print(joystick[i]),Serial.print(" ");
//  for(int i = 0; i < 2; i++)  Serial.print(AnalogButton[i]),Serial.print(" ");
//  Serial.println("");

//  // output msg via XBee
//  for(int i=0;i<17;i++) {
//    Serial1.print(i);
//    Serial1.print(":");
//    Serial1.print(buttonState[i]);
//    Serial1.print(" ");
//  }
//  Serial1.println(" ");

 // send this to local serial ouput of joystick

  xbeeMsg[0] = 0;
  xbeeMsg[1] = 0;

  // pack buttonState[] into xbeeMsg
  // xbeeMsg[0] : { 0 , DN , L , R , R1 , R2 , L1 , L2 }
  // xbeeMsg[1] : { 1 , UP , B1 , B2 , B3 , B4 , ST , SE } 

  // message 0
  xbeeMsg[0]  = !buttonState[16];        // L2
  xbeeMsg[0] += !buttonState[15] << 1;   // L1
  xbeeMsg[0] += !buttonState[14] << 2;   // R2
  xbeeMsg[0] += !buttonState[13] << 3;   // R1
  xbeeMsg[0] += !buttonState[8]  << 4;   // R
  xbeeMsg[0] += !buttonState[6]  << 5;   // L
  xbeeMsg[0] += !buttonState[7]  << 6;   // DN
  
  // message 1
  xbeeMsg[1]  = !buttonState[3];         // SE
  xbeeMsg[1] += !buttonState[4]  << 1;   // ST
  xbeeMsg[1] += !buttonState[10] << 2;   // B4
  xbeeMsg[1] += !buttonState[12] << 3;   // B3
  xbeeMsg[1] += !buttonState[11] << 4;   // B2
  xbeeMsg[1] += !buttonState[9]  << 5;   // B1
  xbeeMsg[1] += !buttonState[5]  << 6;   // UP
  xbeeMsg[1] += 1  << 7;  // identifier for msg 1
  
//  Serial1.print(0xff);
  Serial1.print(xbeeMsg[0]);
  Serial1.print(xbeeMsg[1]);

  
  Serial.print("Button Pressed:");
  for(int i = 0; i < 2; i++)  if(buttonState[i] < 100)  Serial.print(Buttons[i]),Serial.print(",");
  for(int i = 3; i < 17; i++)  if(buttonState[i] == 0)  Serial.print(Buttons[i]),Serial.print(",");
  Serial.println("");
  Serial.print("Analog Sticks:");
  for(int i = 0; i < 4; i++)  Serial.print(joystick[i]),Serial.print(",");
  for(int i = 0; i < 2; i++)  Serial.print(AnalogButton[i]),Serial.print(",");
  Serial.println("");
  Serial.println(inputCommand);

  Serial.println("aa");

  
} // printData()
