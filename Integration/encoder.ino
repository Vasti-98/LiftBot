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
  left_m.setSpeed(150); 
  right_m.setSpeed(150);
  delay(500);
  left_m.disableMotor();
  right_m.disableMotor();
}

void turnLeft(){
  right_m.directionForward();
  left_m.setSpeed(50); 
  right_m.setSpeed(0);
  delay(40);
  left_m.setSpeed(0); 
  right_m.setSpeed(0);
  Serial.println("left");
}

void turnRight(){
  left_m.directionForward();
  right_m.directionBackward();
  left_m.setSpeed(150); 
  right_m.setSpeed(150);
  delay(250);
  left_m.disableMotor();
  right_m.disableMotor();
}
void encoderReset(){//resets the encoder need to do 

  
}


void loopEncoder(){ //orginally: loopEncoder
  delay(3000);
  turnLeft();
  
  
    
}
