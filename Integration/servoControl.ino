#include "servoControl.h"

//600 to 900 to open and hold the 
const int downTilt = 1200;
const int flatTilt = 1650; 
const int upTilt = 19000; 
const int lowHeight = 500;
const int highHeight = 2000; 
const int openGrip = 500; //open 
const int closeGrip = 2400; // fully clsoed

void setupServo(){
  servo_right.attach(8);
  servo_left.attach(7);
  gripper.attach(2); 
}



void pickUpObject() {
  gripper.writeMicroseconds(openGrip); //ensuring gripper is one
  servo_right.writeMicroseconds(600);
  servo_left.writeMicroseconds(downTilt);
  delay(1000);
  gripper.writeMicroseconds(900);
  delay(1000);
  servo_right.writeMicroseconds(highHeight);
}

void dropUpObject() {
  gripper.writeMicroseconds(900); //ensuring gripper is closed, maybe delete 
  servo_right.writeMicroseconds(lowHeight);
  servo_right.writeMicroseconds(flatTilt);
  delay(1000);
  gripper.writeMicroseconds(openGrip);  
}
void loopServo(){
  
}
