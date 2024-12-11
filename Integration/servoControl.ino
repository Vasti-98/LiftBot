
#include <Servo.h>
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


Servo myservo;
#define RIGHT_SERVO 6
#define LEFT_SERVO 7
#define GRIPPER 2

volatile int temp;
volatile int rpos = low_R;
volatile int lpos = low_L;
volatile int gpos = openGrip;

void setupServo(){
  myservo.attach(LEFT_SERVO);
  myservo.write(low_L);
  
  
  
}

void pickUpObject() {
  //going down
  myservo.attach(GRIPPER); //ensuring gripper is one
  myservo.write(openGrip);
  gpos = openGrip;
  
  myservo.attach(RIGHT_SERVO);
  moveRange(rpos, low_L);
  rpos = low_L;
  Serial.println("arm down");
  myservo.attach(GRIPPER);
  moveRange(gpos, closeGrip);
  gpos = closeGrip;
  Serial.println("grip closed");


  myservo.attach(RIGHT_SERVO);
  moveRange(rpos, high_L);
  rpos = high_L;
  Serial.println("arm up");

}

void dropUpObject() {
  
  myservo.attach(RIGHT_SERVO);
  moveRange(rpos, low_L);
  rpos = low_L;
  Serial.println("arm down");

  
  myservo.attach(GRIPPER);
  moveRange(gpos, openGrip);
  gpos = openGrip;
  Serial.println("grip open");


}

void moveRange(int low, int high) {
  Serial.println("moverangecheck");
    if (low < high) {
      for (pos = low; pos < high; pos++) {
        Serial.println(pos);
        myservo.write(pos);
        delay(20);
      }
    } else {
      for (pos = low; pos >= high; pos--) {
        myservo.write(pos);
        delay(20);
      }
    }

}
void testpick(){
  pickUpObject();
  temp = millis();
  while(millis() > temp + 1000) {
    Serial.println();
  }
  dropUpObject();
   temp = millis();
  while(millis() > temp + 1000) {
    Serial.println();
  }
  
  


//  servo_right.writeMicroseconds(600);
//  delay(1000);
//  servo_right.writeMicroseconds(2000);
//  delay(1000);
//    servo_left.writeMicroseconds(1200);
//  delay(1000);
//  servo_left.writeMicroseconds(1800);
//  delay(1000);


  
}
void loopServo(){
 //ensuring gripper is one

  
}
