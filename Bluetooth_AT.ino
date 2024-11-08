/*
 Name:    Bluetooth.ino
 Created: 2016/7/30 13:26:47
 Author:  Fing
HC-06  arduino
VCC---> 5V
GND--->GND
TXD--->D2
RXD--->D3
*/
#include "SoftwareSerial.h"
// Using software serial port, can speak digital port analog into serial port
SoftwareSerial BT(2, 3);  //New object, receive foot 2, send foot 3
char val;  //Store received variables
String cmd = "";
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);   //Serial Port Connection with Computer
  Serial.println("BT Module is ready!");
  BT.begin(9600);  //set baud rate
  BT.write("AT"); //Send AT.
  cmd = "";
}
 
void loop() {
  //Bluetooth to Serial, write all commands here to Serial
  if (Serial.available()) {
    val = Serial.read();
    cmd += val;
  } else if (cmd != "") {
    BT.write(cmd.c_str());
    Serial.write(cmd.c_str());
    if (cmd =="1"){
     digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (cmd =="0"){
     digitalWrite(LED_BUILTIN, LOW); 
    }       
    Serial.println(": SERIAL SENT");
    cmd = "";
  }
 
  //Serial to Bluetooth, send commands to BT
  if (BT.available()) {
    Serial.println("BT SENT");
    val = BT.read();
    Serial.write(val);
    BT.write(val);    
  }
}
