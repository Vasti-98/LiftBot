#include <stdio.h>

unsigned long previousTime = 0;
/*  LiftBot - autonomous line following and object sorting robot
 *   
 *   Integration code developed by Cheyenne Arnold
 *   Motor encoding code done by Vasti Orbach
 *   Bluetooth communication code developed by Ethan Liang
 *   
 *   Made for final project in EE149 @ UC Berkeley, Fall 2024
 */

#define DEBUGGING true

//states
#define IDLE_STATE 0
#define DRIVING_STATE 1
#define PICKUP_STATE 2
#define T90_LEFT_STATE 3
#define T90_RIGHT_STATE 4
#define DROPPING_STATE 5
#define T180_STATE 6
#define IDLE_PICKUP_STATE 18

//events
#define BT_START_EVENT 7
#define BT_STOP_EVENT 8
#define LLINE_DETECTED_EVENT 9
#define RLINE_DETECTED_EVENT 10
#define RED_DETECTED_EVENT 11
#define GREEN_DETECTED_EVENT 12
#define BLUE_DETECTED_EVENT 13
#define TURN_FINISHED_EVENT 22
#define DROP_FINISHED_EVENT 23
#define PICKUP_FINISHED_EVENT 24

//locations
#define UNSORTED_BIN 14
#define RED_BIN 15
#define GREEN_BIN 16
#define BLUE_BIN 17

#define LF_LEFT 19
#define LF_MID 20
#define LF_RIGHT 21
volatile int lineFollowState = LF_MID;


volatile int currentTarget = UNSORTED_BIN;
volatile int lastTarget;
volatile bool pastHalf = true;
volatile bool returning = true;
volatile int state = IDLE_STATE;
volatile int lastColor;
volatile bool turning = false;
volatile bool picking = false;


/*
 * pastHalf and returning are updated BEFORE entering turning state
 *  Current Target and New Target are updated BEFORE entering dropping/pickup state
 */
int handleStateTransition(int currentState, int event) {
  if (event == BT_STOP_EVENT) {
    return IDLE_STATE;
  }
  switch(currentState) {
    case IDLE_STATE:
      if (event == BT_START_EVENT) { //starting
        currentTarget = UNSORTED_BIN;
        returning = true;
        pastHalf = true;
        return DRIVING_STATE;
      }
      break;  
      
    case IDLE_PICKUP_STATE:
      if ((event == RED_DETECTED_EVENT || event == BLUE_DETECTED_EVENT || event == GREEN_DETECTED_EVENT) && (currentTarget == UNSORTED_BIN && (pastHalf && returning))) { // color detected and expected pickup area
        lastTarget = UNSORTED_BIN;
        if (lastColor == 1) {//red
          currentTarget = RED_BIN;
        } else if (lastColor == 2) { //green
          currentTarget = GREEN_BIN;
        } else if (lastColor == 3) {//blue
          currentTarget = BLUE_BIN;
        } else {
          break;
        }
        return PICKUP_STATE;
      }
      break;
      
    case DRIVING_STATE:
      if (event == LLINE_DETECTED_EVENT) { // Left line detected while driving
        if ((currentTarget == UNSORTED_BIN && lastTarget == GREEN_BIN)&& returning) {
          if (pastHalf) {
            return IDLE_PICKUP_STATE; // Left side detected pickup line
          } else {
            pastHalf = true;
            return T90_LEFT_STATE; // Left turn going from green to unsorted
          }
        } else if ((currentTarget == RED_BIN && lastTarget == UNSORTED_BIN) && !returning){
          if (pastHalf) {
            return DROPPING_STATE; // Left side detected droppoff line
          } else {
            return T90_LEFT_STATE; // Left turn going from unsorted to red;
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
            lastTarget = currentTarget;
            currentTarget = UNSORTED_BIN;
            return DROPPING_STATE; // right side detected at droppoff line
          } else {
            pastHalf = true;
            return T90_RIGHT_STATE; // right turn going from unsorted to red;
          }
        } else {
          Serial.println("Right line detected, no action taken");
          }
          break;
      }
      if ((event == RED_DETECTED_EVENT || event == BLUE_DETECTED_EVENT || event == GREEN_DETECTED_EVENT) && (currentTarget == UNSORTED_BIN && (pastHalf && returning))) { // color detected and expected pickup area
        lastTarget = UNSORTED_BIN;
        if (lastColor == 1) {//red
          currentTarget = RED_BIN;
        } else if (lastColor == 2) { //green
          currentTarget = GREEN_BIN;
        } else if (lastColor == 3) {//blue
          currentTarget = BLUE_BIN;
        } else {
          break;
        }
        return PICKUP_STATE;
      }
      
    case T180_STATE:
      if (event == TURN_FINISHED_EVENT) {
        return DRIVING_STATE;
      }
    case T90_LEFT_STATE:
      if (event == TURN_FINISHED_EVENT) {
        return DRIVING_STATE;
      }
    case T90_RIGHT_STATE:
      if (event == TURN_FINISHED_EVENT) {
        return DRIVING_STATE;
      }
    case PICKUP_STATE:
      if (event == PICKUP_FINISHED_EVENT) {
        pastHalf = false;
        returning = false;
        return T180_STATE;
      }
    case DROPPING_STATE:
      if (event == DROP_FINISHED_EVENT) {
        pastHalf = false;
        returning = true;
        return T180_STATE;
      }
      
  } //end of switch
} //end of func



void setup() {
 //setupBluetooth();
 //setupColor();
 //setupEncoder();
 //lineSetup();
 Serial.begin(9600);
}


int incomingByte = 0;
void loop() {
  if (DEBUGGING) {
            if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
        }
  } else {
  bool btval = getBtOn();
  if (!btval) { //check for bluetooth shutdown
    handleStateTransition(state, BT_STOP_EVENT);
    }
  }
  delay(1000);
}
