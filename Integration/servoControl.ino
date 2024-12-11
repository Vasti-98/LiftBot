#include <Servo.h>

// Define pins
const int leftServo = 25; //p4.2
const int rightServo = 23;//p4.7 
const int gripperpin = 24;
const int feedbackPin = A0;      // Analog pin to read feedback

//payload is 100g 
Servo servo_right;//0-60 degrees (height of the robot arm)
const int lowHeight = 500;
const int highHeight = 2000; 

Servo servo_left; //0-110 degrees (wrist tilt )
const int downTilt = 1200;
const int flatTilt = 1650; 
const int upTilt = 19000; 

Servo gripper;
const int openGrip = 500; //open 
const int closeGrip = 2400; // fully clsoed

//600 to 900 to open and hold the 

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
