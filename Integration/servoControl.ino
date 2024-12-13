#include <Servo.h>
#include "header.h"

//600 to 900 to open and hold the 
const int downTilt = 1200;
const int flatTilt = 1650; 
const int upTilt = 19000; 
const int low_R = 70;
const int low_L = 180;
const int high_L = 180;
const int high_R = 0;
 
const int openGrip = 0; //open 
const int closeGrip = 35; // closed on block
int pos = 0;

void setupServo(){
  //servo_right.attach(7);
  servo_left.attach(6);
  //gripper.attach(2); 
}

void pickUpObject() {
  Serial.println("go");
  servo_left.write(60);
  //gripper.writeMicroseconds(500);//ensures right 
  delay(900);
  //gripper.writeMicroseconds(900);
   for(pos = 60; pos < 81; pos += 1)  // goes from 0 degrees to 70 degrees
  {                                  // in steps of 1 degree
    servo_left.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
   

}

void dropUpObject() {
  //gripper.writeMicroseconds(900);
  for(pos = 80; pos>=60;pos-=1)     // goes from 180 degrees to 0 degrees
  {
    servo_left.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
  //gripper.writeMicroseconds(500);
  
} 

void loopServo(){
  
}
