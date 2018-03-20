/* Thing Example Code for Bidirection Communication with WiFi and UART
 * 
 * Instructions:
 * Serial communication using RX,TX
 */

#include <ESP8266WiFi.h>

#define MX_CLIENTS 1

const char* ssid = "WilburNet";
const char* password = "robotics";

WiFiServer server(23);
WiFiClient serverClients[MX_CLIENTS];
void setup() {
  beginningSetup();
}

void loop() {
  //connect to clients
  checkConnection();

  //if there are clients available check for avaiable packets
  //if packets are aviable send through UART 
  if(serverClients[0].connected()){
    recievedAndSendCommand();
  }
  
  //check if packets are aviable from telemetry
  //send to client
}

void beginningSetup(){
    // Serial: UART communication to Mega
  // Serial 1: Debug Monitor
  Serial.begin(115200);
  Serial.println("Beginning Serial Connections");
  // Initalize waiting period
  uint8_t i = 0;

  //Begin Connecting
  WiFi.begin(ssid,password);

  //Check Wifi Connection Status
  while(WiFi.status() != WL_CONNECTED && i++<20) delay(1000);
  if (i==21){
    //Could not connect
    while(1) delay(500);
  }

  //start UART and server
  Serial.println("TCP Server Starting at ");
  Serial.print(WiFi.localIP());
  server.begin();
  server.setNoDelay(true);
  
  //turn built in pin off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
}
void checkConnection(){
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MX_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
}

void recievedAndSendCommand(){
  uint8_t i;
  for(i = 0; i < MX_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //get data from the telnet client and push it to the UART
        if(serverClients[i].available()>4){
          uint8_t packet[4];
          serverClients[i].readBytes(packet,4);
          if(packet[0]==183 && packet[3]==237){
            Serial.write(packet,4);
          }
          
        }
        
        
      }
    }
  }
}

void recieveAndSendTelemetry(){
  uint8_t i;
  //check UART for data and send back up
  
  //check UART for data
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for(i = 0; i < MX_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        //serverClients[i].write(sbuf, len);
        serverClients[i].write('c');
        delay(1);
      }
    }
  }
}

