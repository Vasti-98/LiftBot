#include "servoControl.h"
void setupServo() {
  servo_right.attach(rightServo);
  servo_left.attach(leftServo);
  gripper.attach(24,openGrip, closeGrip); //p4.0

}

void loopServo(){ //orginally:loopServo()
  /**
  if(getLastColor() == 1) {
    Serial.println("identified");
    gripper.writeMicroseconds(openGrip);
    
  } else if (getLastColor() == 0) {
    Serial.println("NOPE");
    gripper.writeMicroseconds(1200);
    servo_right.writeMicroseconds(highHeight);
    //servo_left.writeMicroseconds(downTilt);
  } */


}
