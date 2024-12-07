#include "servoControl.h"
void setupServo() {
  servo_right.attach(rightServo);
  servo_left.attach(leftServo);
  gripper.attach(24,openGrip, closeGrip); //p4.0

}

void loopServo(){ //orginally:loopServo()

}
