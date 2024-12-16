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


void updateLineFollower() {
    // Read line sensor data
    readLineSensor(sensorVal);
    //readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor); //returns sensorCalVal

    // Calculate sums for decision-making
    int centerSum = sensorVal[3] + sensorVal[4];
    int leftSum = sensorVal[0] + sensorVal[1] + sensorVal[2];
    int rightSum = sensorVal[5] + sensorVal[6] + sensorVal[7];

//    // Debugging: Print the sums for verification
//    Serial.print("CenterSum: ");
//    Serial.print(centerSum);
//    Serial.print("\tLeftSum: ");
//    Serial.print(leftSum);
//    Serial.print("\tRightSum: ");
//    Serial.println(rightSum);

    // Decision logic based on updated thresholds
      resetLeftEncoderCnt();
      resetRightEncoderCnt();
    if (centerSum > 4000 && leftSum < 3000 && rightSum < 3000) {
        // Line is centered
//        Serial.println("----------Centered");
//        left_m.setSpeed(15);
//        right_m.setSpeed(15);
        straightFeedback(getEncoderRightCnt(), getEncoderLeftCnt(),12);
    } else if (leftSum > rightSum) {
        // Line is leaning to the left
//        Serial.println("----------Left");
        left_m.setSpeed(10);
        right_m.setSpeed(13);
    } else if (rightSum > leftSum) {
        // Line is leaning to the right
//        Serial.println("----------Right");
        left_m.setSpeed(13);
        right_m.setSpeed(10);
    } else {
        // Default case (uncertain or lost line)
        Serial.println("----------Lost line---------");
        left_m.setSpeed(0);
        right_m.setSpeed(0);
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
  straightFeedback(getEncoderRightCnt(), getEncoderLeftCnt(),9);
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

  delay(1500);//prev 2060
  readLineSensor(sensorVal);
  while ((sensorVal[3] < 2000) && (sensorVal[5] < 2000)) {
     Serial.println("TURNING");
     readLineSensor(sensorVal);
  }
  
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
  
  delay(1000);
  readLineSensor(sensorVal);
  while (sensorVal[5] < 2000) {
      readLineSensor(sensorVal);
  }
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
  left_m.directionForward();
}

void turnRight(){
  right_m.directionBackward();
  left_m.directionForward();
  left_m.setSpeed(15); 
  right_m.setSpeed(15);
  
  delay(1000);
  readLineSensor(sensorVal);
  while (sensorVal[2] < 2000) {
      readLineSensor(sensorVal);
  } 
  
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
  right_m.directionForward();

}

void straightFeedback(uint32_t tickR, uint32_t tickL,int speed){
  left_m.enableMotor();
  right_m.enableMotor();
  left_m.directionForward();
  right_m.directionForward();
  
  int32_t diff = ((int32_t) tickL - (int32_t)tickR)/2;  //positive 
  const float Kp = 0.04; 
  float correct = Kp * diff;
  float baseSpeed = (float)speed; 
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
//  Serial.print("Diff: ");
//  Serial.print(diff);
//  Serial.print(" | Correction: ");
//  Serial.print(correct);
//  Serial.print(" | Left Speed: ");
//  Serial.print(leftSpeed);
//  Serial.print(" | Right Speed: ");
//  Serial.println(rightSpeed);
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


void straight() {
  straightFeedback(getEncoderRightCnt(), getEncoderLeftCnt(),12);
  return;
}


void loopEncoder(){ //orginally: loopEncoder
  
}
