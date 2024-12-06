#include "SoftwareSerial.h"
// Using software serial port, can speak digital port analog into serial port
SoftwareSerial BT(2, 3);  //New object, receive foot 2, send foot 3
char cmd[64] = {0};
int cmdIndex = 0;
bool cmdSent = false;

char cmd2[64] = {0};
int cmd2Index = 0;

void setup() {
  pinMode(2, INPUT);  //rxPin
  pinMode(3, OUTPUT); //txPin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);   //Serial Port Connection with Computer
  Serial.println("BT Module is ready!");
  BT.begin(9600);  //set baud rate
  BT.write("AT"); //Send AT.
}

void loop() {
  
  //PHONE TO BT on Arduino
  //read input
  while (BT.available()) {
    char c = BT.read();
    if (c == ';') {
      cmd[cmdIndex] = '\0';  // Null-terminate the string
      cmdIndex = 0;  // Reset buffer for next message     
      cmdSent = true;
    } else if (cmdIndex < sizeof(cmd) - 1) {
      cmd[cmdIndex++] = c;  // Add character to buffer
    }
  }

  //// COMMANDS ////    // when sending from phone end command with ';' ex: "on;" "off;"
  if (cmdSent) {
    cmdSent = false;
    Serial.write(cmd);  
    if (strncmp(cmd, "on", strlen("on")) == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(": COMMAND SENT FROM PHONE: TURN ON");
    }
    else if (strncmp(cmd, "off", strlen("off")) == 0) {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println(": COMMAND SENT FROM PHONE: TURN OFF");
    }
    else {  // if sent message doesn't fit commands
      Serial.println(": INVALID COMMAND");
    }  
  }

  while (Serial.available()) {
    char c2 = Serial.read();
    if (c2 == '\n') {  // Detect newline (message terminator)
      cmd2[cmd2Index] = '\0';  // Null-terminate the string
      Serial.print(cmd2);
      Serial.println(" : PHONE RECEIVING");
      BT.write(cmd2);  //send data back to PHONE      
      cmd2Index = 0;  // Reset buffer for next message
    } else if (cmd2Index < sizeof(cmd2) - 1) {
      cmd2[cmd2Index++] = c2;  // Add character to buffer
    }
  }

}
