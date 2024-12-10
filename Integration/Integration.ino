#include <stdio.h>

unsigned long previousTime = 0;

#define IDLE_STATE 0
#define DRIVING_STATE 1
#define PICKUP_STATE 2
#define T90_LEFT_STATE 3
#define T90_RIGHT_STATE 4
#define DROPPING_STATE 5
#define T180_STATE 6
#define IDLE_PICKUP_STATE 18

#define BT_START_EVENT 7
#define BT_STOP_EVENT 8
#define LLINE_DETECTED_EVENT 9
#define RLINE_DETECTED_EVENT 10
#define RED_DETECTED_EVENT 11
#define GREEN_DETECTED_EVENT 12
#define BLUE_DETECTED_EVENT 13

#define UNSORTED_BIN 14
#define RED_BIN 15
#define GREEN_BIN 16
#define BLUE_BIN 17


volatile int currentTarget = UNSORTED_BIN;
volatile int lastTarget;
volatile bool pastHalf = true;
volatile bool returning = true;



int handleStateTransition(int currentState, int event) {
  if (event == BT_STOP_EVENT) {
    return IDLE_STATE;
  }
  switch(currentState) {
    case IDLE_STATE:
      if (event == BT_START_EVENT) {
        currentTarget = UNSORTED_BIN;
        return DRIVING_STATE;
      } else if (event == BLUE_DETECTED_EVENT){ //need to identify the color sensor
        //backPickUp(); //this is from encoder.ino
        //pickUpObject(); //from servocontrol //this needs editing 
        //turnAround(); //turns 180
        //need to drive straight to blue bin
      } else if (event == RED_DETECTED_EVENT){
        backPickUp(); 
        //pickUpObject(); 
        //turnAround(); 
        //need to drive strsight to homebase
        //drive left  turnLeft()
        //go straight 
        //dropUpObject()
      }
      break;
    case DRIVING_STATE:
      if (event == LLINE_DETECTED_EVENT) { // Left line detected while driving
        if ((currentTarget == UNSORTED_BIN && lastTarget == GREEN_BIN)&& returning) {
          if (pastHalf) {
            return IDLE_PICKUP_STATE; //Left side detected pickup line
          } else {
            pastHalf = true;
            return T90_LEFT_STATE; // left turn going from green to unsorted
          }

        } else if ((currentTarget == RED_BIN && lastTarget == UNSORTED_BIN) && !returning){
          if (pastHalf) {
            return DROPPING_STATE; //Left side detected droppoff line
          } else {
            return T90_LEFT_STATE; // left turn going from unsorted to red;
          }
        } else {
          Serial.println("Left line detected, no action taken");
          return currentState;
          }
          break;
      }

      
      if (event == RLINE_DETECTED_EVENT) { //Right line detected while driving
        if ((currentTarget == UNSORTED_BIN && lastTarget == RED_BIN)&& returning) {
          if (pastHalf) {
            return IDLE_PICKUP_STATE; //right side detected pickup line
          } else {
            pastHalf = true;
            return T90_RIGHT_STATE; // right turn going from red to unsorted
          }

        } else if ((currentTarget == GREEN_BIN && lastTarget == UNSORTED_BIN) && !returning){
          if (pastHalf) {
            return DROPPING_STATE; // right side detected droppoff line
          } else {
            return T90_RIGHT_STATE; // right turn going from unsorted to red;
          }
        } else {
          Serial.println("Right line detected, no action taken");
          }
          break;
      }

      
      if (event == RED_DETECTED_EVENT && (currentTarget == UNSORTED_BIN && (pastHalf && returning))) {
        lastTarget = UNSORTED_BIN;
        currentTarget = RED_BIN;
        return PICKUP_STATE;
      }
      
}
}



void setup() {
 
 Serial.begin(9600);
}



void loop() {
  Serial.println("running");
  /*
  pickUpObject();
  delay(1000);
  dropUpObject();
  */

}
