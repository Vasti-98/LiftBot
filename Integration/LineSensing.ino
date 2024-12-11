/*
 * Based off a line following code example in the TI Community by Franklin Cooper Jr.
 *
 *  Modified by Cheyenne Arnold
 */
#include <SimpleRSLK.h>

#define LF_LEFT 19
#define LF_MID 20
#define LF_RIGHT 21


uint16_t sensorVal[LS_NUM_SENSORS];
uint16_t sensorCalVal[LS_NUM_SENSORS];
uint16_t sensorMaxVal[LS_NUM_SENSORS];
uint16_t sensorMinVal[LS_NUM_SENSORS];
uint8_t lineColor = DARK_LINE;
void setupLine()
{

  setupRSLK();
  /* Left button on Launchpad */
  setupWaitBtn(LP_LEFT_BTN);
  /* Red led in rgb led */
  setupLed(RED_LED);
  clearMinMax(sensorMinVal,sensorMaxVal);
}

void floorCalibration() {
  /* Place Robot On Floor (no line) */
  delay(2000);
  String btnMsg = "Push left button on Launchpad to begin calibration.\n";
  btnMsg += "Make sure the robot is on the floor away from the line.\n";
  /* Wait until button is pressed to start robot */
  waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);

  delay(1000);

  Serial.println("Running calibration on floor");
  simpleCalibrate();
  Serial.println("Reading floor values complete");

  btnMsg = "Push left button on Launchpad to begin sorting\n";
  btnMsg += "GLHF :)\n";
  /* Wait until button is pressed to start robot */
  waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
  delay(1000);

}

void simpleCalibrate() {

  for(int x = 0;x<100;x++){
    readLineSensor(sensorVal);
    setSensorMinMax(sensorVal,sensorMinVal,sensorMaxVal);
  }
}


int getLineState() {
    readLineSensor(sensorVal);
    readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor);
    uint32_t linePos = getLinePosition(sensorCalVal,lineColor);
    
  if(linePos > 0 && linePos < 3000) {
    return LF_RIGHT;
  } else if(linePos > 3500) {
    return LF_LEFT;
  } else {
    return LF_MID;
  }
}
