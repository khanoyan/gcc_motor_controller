/* Mega Example Code for Bidirectional Communication through UART
 * 
 * Goal: Recieve data via Thing and send back through UART
 * 
 * Instructions:
 * Pin connection on Mega 14,15 is Serial 3
 * 
 * 
 * 
 */
char rxChar;
#include <SoftwareSerial.h>
const int txPin = 10;
const int rxPin = 9;

SoftwareSerial pins(rxPin,txPin);

void setup() {
  // serial communication at 9600 bps:
  pinMode(rxPin, INPUT);
  pinMode(txPin,OUTPUT);
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop() {

  
  // put your main code here, to run repeatedly:
//  if(pins.available()){
//    if(pins.read()){
//      pins.write('c');
//    }
//  if (pins.available()>0){
//    rxChar = pins.read();
//    Serial.print(rxChar);
//    pins.write('t');
//  }
//  else{
//    rxChar = 'x';
//    Serial.print(rxChar);
//  }
  if (Serial3.available()>0){
    rxChar = Serial3.read();
    Serial.print(rxChar);
  }
  else{
    Serial.print("nothing");
  }
  
  delay(5000);
  
  
}
