#include <Servo.h>

// Define pins
const int leftServo = 6;          
const int rightServo = 7; 
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

void initServoControl() {
  servo_right.attach(rightServo);
  servo_left.attach(leftServo);
  gripper.attach(5,openGrip, closeGrip);

}

void handleColorDetection(int color) {
}

void pickUpObject() {
  gripper.writeMicroseconds(openGrip); //ensuring gripper is one
  servo_right.writeMicroseconds(lowHeight);
  servo_right.writeMicroseconds(flatTilt);
  delay(1000);
  gripper.writeMicroseconds(closeGrip);
  delay(1000);
  servo_right.writeMicroseconds(highHeight);
}

void dropUpObject() {
  gripper.writeMicroseconds(closeGrip); //ensuring gripper is closed, maybe delete 
  servo_right.writeMicroseconds(lowHeight);
  servo_right.writeMicroseconds(flatTilt);
  delay(1000);
  gripper.writeMicroseconds(openGrip);  
}


/*debug
void loop() {
  Serial.print("Starting");
  //1. pick up the ball: 
  servo_right.writeMicroseconds(lowHeight);
  servo_right.writeMicroseconds(flatTilt);
  delay(1000);
  gripper.writeMicroseconds(openGrip);
} */
