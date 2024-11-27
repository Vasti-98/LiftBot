#include "colorSensor.h"
#include "servoControl.h"

// Timing variables
unsigned long previousTime = 0;
const unsigned long samplingInterval = 100; // Milliseconds for color sensor sampling

void setup() {
  Serial.begin(9600);

  // Initialize subsystems
  initColorSensor();
  initServoControl();
}

void loop() {
  // Task: Read the color sensor at intervals
  if (millis() - previousTime >= samplingInterval) {
    previousTime = millis();
    int color = readColorSensor(); // Get the identified color
    handleColorDetection(color);  // Handle servo actions based on the detected color
  }

  // Task: Update servo positions independently if needed
  updateServos();
}
