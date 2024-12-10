#include <stdio.h>
unsigned long previousTime = 0;

#define IDLE_STATE 0;
#define DRIVING_STATE 1;
#define PICKUP_STATE 2;
#define T90_LEFT_STATE 3;
#define T90_RIGHT_STATE 4;
#define DROPPING_STATE 5;
#define T180_STATE 6;

#define BT_START_EVENT 7;
#define BT_STOP_EVENT 8;
#define LLINE_DETECTED_EVENT 9;
#define RLINE_DETECTED_EVENT 10;
#define RED_DETECTED_EVENT 11;
#define GREEN_DETECTED_EVENT 12;
#define BLUE_DETECTED_EVENT 13;

#define UNSORTED_BIN 14;
#define RED_BIN 15;
#define GREEN_BIN 16;
#define BLUE_BIN 17;


volatile int currentTarget = UNSORTED_BIN;
volatile bool returning = true;



int handleStateTransition(int currentState, int event) {
  switch(currentState) {
    case IDLE_STATE:
      if (event == BT_START_EVENT) {
        currentTarget = UNSORTED_BIN;
        return DRIVING_STATE;
      } else if (event == BLUE_DETECTED_EVENT){ //need to identify the color sensor
        backPickUp(); //this is from encoder.ino
        pickUpObject(); //from servocontrol //this needs editing 
        turnAround(); //turns 180
        //need to drive straight to blue bin
      } else if (event == RED_DETECTED_EVENT){
        backPickUp(); 
        pickUpObject(); 
        turnAround(); 
        //need to drive strsight to homebase
        //drive left  turnLeft()
        //go straight 
        //dropUpObject()
      }
      break;
    case DRIVING_STATE:
      if (event == LLINE_DETECTED_EVENT) {
        if ()
      }
     
      
  }
}



void setup() {

}

int readColorSensor() {
  getLastColor(); //what does
}


void loop() {
  

}
