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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo_right.attach(rightServo);
  servo_left.attach(leftServo);
  gripper.attach(5,openGrip, closeGrip);

}

void loop() {
  Serial.print("Starting");
  //1. pick up the ball: 
  servo_right.writeMicroseconds(lowHeight);
  servo_right.writeMicroseconds(flatTilt);
  delay(1000);
  gripper.writeMicroseconds(openGrip);


  
}
