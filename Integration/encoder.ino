#include "encoder.h"

void setupEncoder(int motorSpeed) {
  // Initialize serial for debugging
  /**to find certain pins look at RSLK_PIN*/
  setupEncoder(ENCODER_ELA_PIN,ENCODER_ELB_PIN, ENCODER_ERA_PIN, ENCODER_ERB_PIN);
  //initialize the motors
  left_m.begin(31,29,40);
  right_m.begin(11, 30,39);

  // Print message for debugging
  left_m.enableMotor();
  right_m.enableMotor();
  // Set motor direction to forward
  /*
  left_m.directionForward();
  right_m.directionForward()*/;
  
  left_m.setSpeed(motorSpeed); 
  right_m.setSpeed(motorSpeed); 
  
  resetLeftEncoderCnt(); 
  resetRightEncoderCnt();
}

void loopEncoder(){

}
