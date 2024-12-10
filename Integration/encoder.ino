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

void turnOn(){
    // Print message for debugging
  left_m.enableMotor();
  right_m.enableMotor();
  left_m.directionForward();
  right_m.directionForward();
  left_m.setSpeed(150); 
  right_m.setSpeed(150); 
}
void turnOff(){
  left_m.disableMotor();
  right_m.disableMotor();
}

void turnAround(){
  left_m.directionBackward();
  right_m.directionForward();
  left_m.setSpeed(55); 
  right_m.setSpeed(55);
  delay(680);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
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
  left_m.setSpeed(55); 
  right_m.setSpeed(55);
  delay(340);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
}

void turnRight(){
  left_m.directionForward();
  right_m.directionBackward();
  left_m.setSpeed(55); 
  right_m.setSpeed(55);
  delay(340);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
}
void encoderReset(){//resets the encoder need to do 
}


void loopEncoder(){ //orginally: loopEncoder
  
}
