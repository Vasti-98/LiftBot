#include "encoder.h"

void setupEncoder(){
  // Initialize serial for debugging
  /**to find certain pins look at RSLK_PIN*/
  setupEncoder(ENCODER_ELA_PIN,ENCODER_ELB_PIN, ENCODER_ERA_PIN, ENCODER_ERB_PIN);
  //initialize the motors
  left_m.begin(31,29,40);
  right_m.begin(11, 30,39);
  left_m.enableMotor();
  right_m.enableMotor();
  resetLeftEncoderCnt(); 
  resetRightEncoderCnt();
}

void lineFollowHelper(int lineFollowState){
  switch (lineFollowState){
    case LF_LEFT:
      left_m.setSpeed(30);
      right_m.setSpeed(10);
    case LF_MID:
      left_m.setSpeed(10);
      right_m.setSpeed(10);
    case LF_RIGHT:
      left_m.setSpeed(10);
      right_m.setSpeed(15);      
  }
}
void updateLineFollower() {
    readLineSensor(sensorVal);
    readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor);
    uint32_t linePos = getLinePosition(sensorCalVal,lineColor);
  if(linePos > 3000 && linePos < 3250) {
      left_m.setSpeed(10);
      right_m.setSpeed(15);
  } else if(linePos > 3750 && linePos < 4000) {
      left_m.setSpeed(15);
      right_m.setSpeed(10);
  } else if (linePos < 3000){
      left_m.setSpeed(10);
      right_m.setSpeed(20);
  }else if (linePos > 4000){
      left_m.setSpeed(20);
      right_m.setSpeed(10);    
  }
  else {
      left_m.setSpeed(10);
      right_m.setSpeed(10);  
  }
}

void turnOn(){
    // Print message for debugging
  left_m.enableMotor();
  right_m.enableMotor();
  left_m.directionForward();
  right_m.directionForward();


}
void turnOff(){
  left_m.disableMotor();
  right_m.disableMotor();
}
void turnOnSlow(){
    // Print message for debugging
  left_m.setSpeed(10); 
  right_m.setSpeed(10); 
}
void stopWheels() {
  left_m.setSpeed(0);
  right_m.setSpeed(0);
}

void turnAround(){
  left_m.directionBackward();
  right_m.directionForward();
  left_m.setSpeed(15); 
  right_m.setSpeed(15);
  delay(3400);//prev 2060
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
  left_m.directionForward();
}

void backPickUp(){ //after sensing the color, it backs up a bit to pick up the ball
  resetLeftEncoderCnt();
  resetRightEncoderCnt();
  backFeedback(getEncoderRightCnt(),getEncoderLeftCnt(),20);
  delay(500);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);  
}

void turnLeft(){
  left_m.directionBackward();
  right_m.directionForward();
  left_m.setSpeed(15); 
  right_m.setSpeed(15);
  delay(1800);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
  left_m.directionForward();
}

void turnRight(){
  right_m.directionBackward();
  left_m.directionForward();
  left_m.setSpeed(15); 
  right_m.setSpeed(15);
  delay(1800);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
  right_m.directionForward();
}

void straightFeedback(uint32_t tickR, uint32_t tickL){
  left_m.enableMotor();
  right_m.enableMotor();
  left_m.directionForward();
  right_m.directionForward();
  
  int32_t diff = ((int32_t) tickL - (int32_t)tickR)/2;  //positive 
  const float Kp = 0.04; 
  float correct = Kp * diff;
  float baseSpeed = 150; 
  const float deadband = 2.0;     // Deadband threshold to ignore minor errors

  float leftSpeed = baseSpeed - correct;
  float rightSpeed = baseSpeed + correct;

  //no correction is applied to prevent constant minior adjustments 
  if (abs(diff) < deadband) {
    leftSpeed = baseSpeed;
    rightSpeed = baseSpeed;
  } 
  // Ensure motor speeds are within valid range
  leftSpeed = constrain(leftSpeed, 0, 255);   // Assuming motor speed range is 0-255
  rightSpeed = constrain(rightSpeed, 0, 255);

  //debugg
  Serial.print("Diff: ");
  Serial.print(diff);
  Serial.print(" | Correction: ");
  Serial.print(correct);
  Serial.print(" | Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.println(rightSpeed);
  //setting the speeds
  left_m.setSpeed(leftSpeed);
  right_m.setSpeed(rightSpeed);
}

void backFeedback(uint32_t tickR, uint32_t tickL, int speed){
    left_m.enableMotor();
    right_m.enableMotor();
    left_m.directionBackward();
    right_m.directionBackward();
 
  float diff = ((float)tickL - (float)tickR) / 2.0;  //positive 

  const float Kp = 0.06; 
  float correct = Kp * diff;
  float baseSpeed = (float) speed; 
  const float deadband = 2.0;     // Deadband threshold to ignore minor errors

  float leftSpeed = baseSpeed - correct;
  float rightSpeed = baseSpeed + correct;

  //no correction is applied to prevent constant minior adjustments 
  if (fabs(diff) < deadband) {
    leftSpeed = baseSpeed;
    rightSpeed = baseSpeed;
  } 
  // Ensure motor speeds are within valid range
  leftSpeed = constrain(leftSpeed, 0, 255);   // Assuming motor speed range is 0-255
  rightSpeed = constrain(rightSpeed, 0, 255);
  
  //setting the speeds
  left_m.setSpeed((int)leftSpeed);
  right_m.setSpeed((int)rightSpeed);

}


void loopEncoder(){ //orginally: loopEncoder
  
}
