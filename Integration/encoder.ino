#include "SimpleRSLK.h"
#include "encoder.h"
uint16_t slowSpeed = 10;
uint16_t slightSpeed = 12;
  uint16_t normalSpeed = 14;
  uint16_t fastSpeed = 20;
  volatile int linePosition = 3500;
  volatile int lastLinePosition= 3500;
void setupEncoder(){
  // Initialize serial for debugging
  /**to find certain pins look at RSLK_PIN*/
  setupEncoder(ENCODER_ELA_PIN,ENCODER_ELB_PIN, ENCODER_ERA_PIN, ENCODER_ERB_PIN);
    enableMotor(BOTH_MOTORS);
      setMotorDirection(BOTH_MOTORS,MOTOR_DIR_FORWARD);  /* Enable both motors */

  //initialize the motors
  left_m.begin(31,29,40);
  right_m.begin(11, 30,39);
  left_m.enableMotor();
  right_m.enableMotor();
  resetLeftEncoderCnt(); 
  resetRightEncoderCnt();
}

<<<<<<< HEAD
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
int getMaxIndex() {
      readLineSensor(sensorVal);
    readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor);

    int maxIndex = 0;
    for (int i = 0; i < 8; i++) {
        if (sensorVal[i] > sensorVal[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;

}
void updateLineFollower() {
  
    readLineSensor(sensorVal);
    readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor);
    linePosition = getLinePosition(sensorCalVal,lineColor);

    int lineDelta = linePosition - lastLinePosition;

    int maximo = getMaxIndex();

    if ((linePosition > 3400) && (linePosition < 3450)) {
        setMotorSpeed(LEFT_MOTOR, slowSpeed);
        setMotorSpeed(RIGHT_MOTOR, 11);
        
    } else if ((linePosition > 3550) && (linePosition < 3600)) {
         setMotorSpeed(LEFT_MOTOR, 11);
        setMotorSpeed(RIGHT_MOTOR, slowSpeed);
     } else  if ((linePosition > 3450) && linePosition < 3550) {
        setMotorSpeed(RIGHT_MOTOR, slowSpeed);
        setMotorSpeed(RIGHT_MOTOR, slowSpeed);
      } else {
    switch(maximo) {
      case 0:
        setMotorSpeed(LEFT_MOTOR, slowSpeed);
        setMotorSpeed(RIGHT_MOTOR, fastSpeed);
        break;
      case 1:
        setMotorSpeed(LEFT_MOTOR, slowSpeed);
        setMotorSpeed(RIGHT_MOTOR, normalSpeed);
        break;
      case 2:
        setMotorSpeed(LEFT_MOTOR, slowSpeed);
        setMotorSpeed(RIGHT_MOTOR, normalSpeed);
        break;
      case 3:
        setMotorSpeed(LEFT_MOTOR, slowSpeed);
        setMotorSpeed(RIGHT_MOTOR, slightSpeed);
        break;
      case 4:
        setMotorSpeed(LEFT_MOTOR, slightSpeed);
        setMotorSpeed(RIGHT_MOTOR, slowSpeed);
        break;
      case 5:
        setMotorSpeed(LEFT_MOTOR, normalSpeed);
        setMotorSpeed(RIGHT_MOTOR, slowSpeed);
        break;
      case 6:
        setMotorSpeed(LEFT_MOTOR, normalSpeed);
        setMotorSpeed(RIGHT_MOTOR, slowSpeed);
        break;
      case 7:
        setMotorSpeed(LEFT_MOTOR, fastSpeed);
        setMotorSpeed(RIGHT_MOTOR, slowSpeed);
        break;
    }
    }
    lastLinePosition = linePosition;

    
     /* 
     * OLD STRAT
      if(linePos >= 2500 && linePos < 3000) {
    setMotorSpeed(LEFT_MOTOR,normalSpeed);
    setMotorSpeed(RIGHT_MOTOR,slowSpeed);
  } else if(linePos >= 3500 && linePos < 4000) {
    setMotorSpeed(LEFT_MOTOR,slowSpeed);
    setMotorSpeed(RIGHT_MOTOR,fastSpeed);
  } else if (linePos >= 4000) {
    setMotorSpeed(LEFT_MOTOR,slowSpeed);
    setMotorSpeed(RIGHT_MOTOR,fastSpeed);
  } else if (linePos < 2500) {
    setMotorSpeed(LEFT_MOTOR,fastSpeed);
    setMotorSpeed(RIGHT_MOTOR,slowSpeed);
  } else {
    setMotorSpeed(LEFT_MOTOR,slowSpeed);
    setMotorSpeed(RIGHT_MOTOR,slowSpeed);
  }
  if(linePos > 3200 && linePos < 3400) {
      left_m.setSpeed(10);
      right_m.setSpeed(12);
  } else if(linePos > 3600 && linePos < 3800) {
      left_m.setSpeed(12);
      right_m.setSpeed(10);
  } else if (linePos < 3200){
      left_m.setSpeed(10);
      right_m.setSpeed(15);
  }else if (linePos > 3800){
      left_m.setSpeed(15);
      right_m.setSpeed(10);    
  }
  else {
      left_m.setSpeed(10);
      right_m.setSpeed(10);  
  }
  */
=======

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
        straightFeedback(getEncoderRightCnt(), getEncoderLeftCnt(),14);
    } else if (leftSum > rightSum) {
        // Line is leaning to the left
//        Serial.println("----------Left");
        left_m.setSpeed(10);
        right_m.setSpeed(15);
    } else if (rightSum > leftSum) {
        // Line is leaning to the right
//        Serial.println("----------Right");
        left_m.setSpeed(15);
        right_m.setSpeed(10);
    } else {
        // Default case (uncertain or lost line)
        Serial.println("----------Lost line---------");
        left_m.setSpeed(0);
        right_m.setSpeed(0);
    }
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
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

void turnAround(bool longer){
  left_m.directionBackward();
  right_m.directionForward();
  left_m.setSpeed(15); 
  right_m.setSpeed(15);
<<<<<<< HEAD
  if (longer) {delay(2200);} else {delay(2200);}
=======
  delay(3300);//prev 2060
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
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
<<<<<<< HEAD
  delay(1100);
  left_m.setSpeed(10); 
  right_m.setSpeed(10);
  delay(1000);
=======
  delay(1800);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
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


void straight() {
  straightFeedback(getEncoderRightCnt(), getEncoderLeftCnt(),14);
  return;
}


void loopEncoder(){ //orginally: loopEncoder
  
}
