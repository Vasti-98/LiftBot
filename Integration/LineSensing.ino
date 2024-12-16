/*
 * Based off a line following code example in the TI Community by Franklin Cooper Jr.
 *
 *  Modified by Cheyenne Arnold
 */
#define LF_LEFT 19
#define LF_MID 20
#define LF_RIGHT 21
#include "SimpleRSLK.h"

uint16_t sensorVal[LS_NUM_SENSORS];
uint16_t sensorCalVal[LS_NUM_SENSORS];
uint16_t sensorMaxVal[LS_NUM_SENSORS];
uint16_t sensorMinVal[LS_NUM_SENSORS];
uint8_t lineColor = DARK_LINE;
void lineSetup()
{
  setupRSLK();
  /* Left button on Launchpad */
  /* Red led in rgb led */
  setupLed(RED_LED);
  clearMinMax(sensorMinVal,sensorMaxVal);
  floorCalibration();
}

void floorCalibration() {
  /* Place Robot On Floor (no line) */
  Serial.println("Running calibration on floor");
<<<<<<< HEAD
  simpleCalibrate();
  Serial.println("Reading floor values complete");

}
void printMinMax() {
  Serial.println("min");
  for (int i = 0; i < 8; i++) {
    Serial.print(sensorMinVal[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("max");
  
    for (int i = 0; i < 8; i++) {
    Serial.print(sensorMaxVal[i]);
    Serial.print(" ");
  }
  Serial.println();
}
void simpleCalibrate() {

  for(int x = 0;x<100;x++){
    readLineSensor(sensorVal);
    setSensorMinMax(sensorVal,sensorMinVal,sensorMaxVal);
  }

=======
  delay(5000);
  for(int x = 0; x < 100; x++){
    readLineSensor(sensorVal);
    setSensorMinMax(sensorVal,sensorMinVal,sensorMaxVal);
  }
  Serial.println("Reading floor values complete");
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
}

int getLineUpdates() {
    readLineSensor(sensorVal);
    readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor);
<<<<<<< HEAD
    uint32_t linePos = getLinePosition(sensorCalVal,lineColor);
    
=======
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
    if (false) {
      for (int i = 0; i < 8; i++) {
        Serial.print(sensorVal[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
    if (sensorVal[7] > 2000 && sensorVal[6] > 2000) {
      Serial.println("Right Line");
      return 2; //right most sensor dark
    }
    if (sensorVal[0] > 2000 && sensorVal[1] > 2000) {
      Serial.println("Left Line");
      return 1; //left most sensor dark
    }
    return 0;

}



void loopLine() {
}
