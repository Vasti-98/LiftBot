#define LED_BUILTIN BLUE_LED


char cmd[64] = {0};
int cmdIndex = 0;
bool cmdSent = false;

char cmd2[64] = {0};
int cmd2Index = 0;

//
bool btOn = false; //
bool getBtOn() {return btOn;}
void setupBluetooth() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Initialize the hardware UARTs
  Serial.begin(9600);    // Serial Monitor
  Serial1.begin(9600);   // UART for Bluetooth communication (HM-10)

  Serial.println("BT Module is ready!");
  Serial1.write("AT"); // Send AT command to HM-10
}


void loopbt() {
  // Receive data from HM-10 (Bluetooth)
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == ';') {
      cmd[cmdIndex] = '\0';  // Null-terminate the string
      cmdIndex = 0;          // Reset buffer for next message
      cmdSent = true;
    } else if (cmdIndex < sizeof(cmd) - 1) {
      cmd[cmdIndex++] = c;   // Add character to buffer
    }
  }

  // Process received Bluetooth commands
  if (cmdSent) {
    cmdSent = false;
    Serial.write(cmd);
    if (strncmp(cmd, "on", strlen("on")) == 0) {
      btOn = true; //
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(": COMMAND SENT FROM PHONE: TURN ON");
    } else if (strncmp(cmd, "off", strlen("off")) == 0) {
      btOn = false; //
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println(": COMMAND SENT FROM PHONE: TURN OFF");
    } else {
      Serial.println(": INVALID COMMAND");
    }
  }

  // Send data from Serial Monitor to HM-10 (Bluetooth)
  while (Serial.available()) {
    char c2 = Serial.read();
    if (c2 == '\n') {        // Detect newline (message terminator)
      cmd2[cmd2Index] = '\0'; // Null-terminate the string
      Serial.print(cmd2);
      Serial.println(" : PHONE RECEIVING");
      Serial1.write(cmd2);   // Send data back to PHONE
      cmd2Index = 0;         // Reset buffer for next message
    } else if (cmd2Index < sizeof(cmd2) - 1) {
      cmd2[cmd2Index++] = c2; // Add character to buffer
    }
  }
}
