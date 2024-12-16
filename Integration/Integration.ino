#include <Servo.h>
#include "header.h"

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

#define DEBUGGING false
#define DEMO false

//states
#define IDLE_STATE 0
#define DRIVING_STATE 1
#define PICKUP_STATE 51
#define T90_LEFT_STATE 3
#define T90_RIGHT_STATE 4
#define DROPPING_STATE 5
#define T180_STATE 50
#define IDLE_PICKUP_STATE 18

//events
#define BT_START_EVENT 52
#define BT_STOP_EVENT 8
#define LINE_DETECTED_EVENT 9
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
volatile int currentState = IDLE_STATE;
volatile int lastColor;
volatile bool turning = false;
volatile bool picking = false;
volatile bool driveInit = false;
volatile int preState = 99;

/*
 * pastHalf and returning are updated BEFORE entering turning state
 *  Current Target and New Target are updated BEFORE entering dropping/pickup state
 */
int handleStateTransition(int inState, int event) {
  if (event == BT_STOP_EVENT) {
    return IDLE_STATE;
  }
  switch(inState) {
    case IDLE_STATE:
      if (event == BT_START_EVENT) { //starting
        Serial.println("Starting");

        currentTarget = UNSORTED_BIN;
        returning = true;
        pastHalf = true;
        return IDLE_PICKUP_STATE;
      }
      return inState;
      break;  
      
    case IDLE_PICKUP_STATE:
      if ((event == RED_DETECTED_EVENT || event == BLUE_DETECTED_EVENT || event == GREEN_DETECTED_EVENT) && (currentTarget == UNSORTED_BIN && (pastHalf && returning))) { // color detected and expected pickup area
        lastTarget = UNSORTED_BIN;
        if (lastColor == 1) {//red
          digitalWrite(RED_LED, HIGH);
          currentTarget = RED_BIN;
        } else if (lastColor == 2) { //green
          currentTarget = GREEN_BIN;
          digitalWrite(GREEN_LED, HIGH);
        } else if (lastColor == 3) {//blue
          currentTarget = BLUE_BIN;
          digitalWrite(BLUE_LED, HIGH);
        } else {
          break;
        }
        Serial.println("RETURN PICKUP");
        return PICKUP_STATE;
      }
      return inState;
      break;
      
    case DRIVING_STATE:

      if (event == LINE_DETECTED_EVENT) {
        
        //// RED BIN LOGIC
        if ((currentTarget == RED_BIN) && (lastTarget == UNSORTED_BIN)) {  // unsorted going to red
          if (!pastHalf) {      
            pastHalf = true;
            return T90_LEFT_STATE;
          } else {
            return DROPPING_STATE;
          }
        }
        if ((currentTarget == UNSORTED_BIN) && (lastTarget == RED_BIN)) {  // red returning to unsorted
          if (!pastHalf) {      
            pastHalf = true;
            return T90_RIGHT_STATE;
          } else {
            return IDLE_PICKUP_STATE;
          }
        }        

        //// GREEN BIN LOGIC
        if ((currentTarget == GREEN_BIN) && (lastTarget == UNSORTED_BIN)) {  // unsorted going to green
          if (!pastHalf) {      
            pastHalf = true;
            return T90_RIGHT_STATE;
          } else {
            return DROPPING_STATE;
          }
        }
        if ((currentTarget == UNSORTED_BIN) && (lastTarget == GREEN_BIN)) {  // green returning to unsorted
          if (!pastHalf) {      
            pastHalf = true;
            return T90_LEFT_STATE;
          } else {
            return IDLE_PICKUP_STATE;
          }
        }        


        //// BLUE BIN LOGIC
        if ((currentTarget == BLUE_BIN) && (lastTarget == UNSORTED_BIN)) {  // unsorted going to blue
          if (!pastHalf) {      
            pastHalf = true;
            turnOnSlow();
            delay(1000);
            return DRIVING_STATE;
          } else {
            return DROPPING_STATE;
          }
        }
        if ((currentTarget == UNSORTED_BIN) && (lastTarget == BLUE_BIN)) {  // blue returning to unsorted
          if (!pastHalf) {      
            pastHalf = true;
            turnOnSlow();
            delay(1000);
            return DRIVING_STATE;
          } else {
            return IDLE_PICKUP_STATE;
          }
        }            
      }
      break;


      
    case T180_STATE:
      if (event == TURN_FINISHED_EVENT) {
        return DRIVING_STATE;
      }
      break;
    case T90_LEFT_STATE:
      if (event == TURN_FINISHED_EVENT) {
        return DRIVING_STATE;
      }
      break;
    case T90_RIGHT_STATE:
      if (event == TURN_FINISHED_EVENT) {
        return DRIVING_STATE;
      }
      break;
    case PICKUP_STATE:
      if (event == PICKUP_FINISHED_EVENT) {
        pastHalf = false;
        returning = false;
        return T180_STATE;
      }
      break;
    case DROPPING_STATE: // not working
      if (event == DROP_FINISHED_EVENT) {
        lastTarget = currentTarget;
        currentTarget = UNSORTED_BIN;
        pastHalf = false;
        returning = true;
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        return T180_STATE;
      }
      break;
  } //end of switch
} //end of func

void printState(int state, bool vars) {
  switch (state) {
    case IDLE_STATE:
      Serial.println("----STATE----: Idle State");
      break;
    case DRIVING_STATE:
      Serial.println("----STATE----: Driving State");
      break;
    case PICKUP_STATE:
      Serial.println("----STATE----: Pickup State");
      break;
    case T90_LEFT_STATE:
      Serial.println("----STATE----: Turn Left State");
      break;
    case T90_RIGHT_STATE:
      Serial.println("----STATE----: Turn Right State");
      break;
    case DROPPING_STATE:
      Serial.println("----STATE----: Dropping State");
      break;
    case T180_STATE:
      Serial.println("----STATE----: Turn Around State");
      break;    
    case IDLE_PICKUP_STATE:
      Serial.println("----STATE----: Idle Pickup State");
      break;
      }
      if (vars) {
        Serial.print("Past half = ");
        Serial.println(pastHalf);
        Serial.print("Returning = ");
        Serial.println(returning);
        Serial.print("Last target = ");
        Serial.println(lastTarget);
        Serial.print("Current target = ");
        Serial.println(currentTarget);
        Serial.println();
      }
}

void setup() {

  Serial.begin(9600); 
  lineSetup();
  currentState = IDLE_PICKUP_STATE; 
  pastHalf = true;
  returning = true;
  turnOn();
  
  servo_left.attach(6);
  gripper.attach(2); 
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}


int incomingByte = 0;
int lineState = 0;
int lineUpdates;
int mil;
int tcolor;
int counter = 0;
void loop() {
  
  if (DEBUGGING) {
    straight();

    /*
    Serial.println("start");
    currentState = handleStateTransition(currentState, BT_START_EVENT);
    printState(currentState, true);
    
    lastColor  = 1;
    currentState = handleStateTransition(currentState, RED_DETECTED_EVENT);
    Serial.println("red detected");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, PICKUP_FINISHED_EVENT);
    Serial.println("pickup finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("turn finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, RLINE_DETECTED_EVENT);
    Serial.println("Rline detected, should not turn");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, LLINE_DETECTED_EVENT);
    Serial.println("Lline detected, turn left");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("Turn finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, LLINE_DETECTED_EVENT);
    Serial.println("Stop line detected");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, DROP_FINISHED_EVENT);
    Serial.println("Drop done");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, DROP_FINISHED_EVENT);
    Serial.println("Double event redundancy check");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("Turn finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, RLINE_DETECTED_EVENT);
    Serial.println("Turn right line detected");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("Turn Finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, LLINE_DETECTED_EVENT);
    Serial.println("Stop line detected");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, DROP_FINISHED_EVENT);
    Serial.println("Dropped, should be idle");
    printState(currentState, true);
    
    Serial.println("end");
    delay(2000);
    */
  } else if (DEMO) {
//    Serial.print(1);
    
    updateLineFollower();
    if (checkLine()) {
      Serial1.write(lineUpdates);
      stopWheels();
      delay(300);
      turnLeft();
      delay(200);
      turnOnSlow();
      delay(1000);
      stopWheels();
      delay(200);
    }
    

    
  } else { //THE LOOOOOOOOOOOOOOOP
    // printing
    if (counter == 50) {
      printState(currentState, true);
      counter = 0;
    } else {
      counter++;
    }
    if (preState != currentState) {
      preState = currentState;
      printState(currentState, true);
    }


bool btval = getBtOn(); //orignall true

  if (!btval) { //check for bluetooth shutdown
    currentState = handleStateTransition(currentState, BT_STOP_EVENT);
    }
  switch(currentState) {
    case IDLE_STATE:
      if (btval){
        Serial.println("Bluetooth start command, moving to driving");
        currentState = handleStateTransition(currentState, BT_START_EVENT);
      }
    case IDLE_PICKUP_STATE:
      tcolor = getColorSamples();
      lastColor = tcolor;
      switch (tcolor) {
        case 1:
          currentState = handleStateTransition(currentState, RED_DETECTED_EVENT);
          break;
        case 2:
          currentState = handleStateTransition(currentState, GREEN_DETECTED_EVENT);
          break;
        case 3:
          currentState = handleStateTransition(currentState, BLUE_DETECTED_EVENT);
          break;
      }
      break;
    case DRIVING_STATE:
      updateLineFollower();
      if (checkLine()) {
        stopWheels();
        currentState = handleStateTransition(currentState, LINE_DETECTED_EVENT);
      }
      break;
    case PICKUP_STATE:
      //pickup sequence
      pickUpObject();
      delay(500);
      Serial.println("STATUS: PickedUp");
      currentState = handleStateTransition(currentState, PICKUP_FINISHED_EVENT);
      break;
    case DROPPING_STATE:
      //dropping sequence
      dropUpObject();
      
      delay(500);//servo code in here  , delete delay afterwards
      Serial.println("STATUS: Dropped");
      currentState = handleStateTransition(currentState, DROP_FINISHED_EVENT);
      break;
      
    case T90_LEFT_STATE:
      Serial.println("STATUS: TURNING 90 LEFT");
      stopWheels();
      delay(50);
      turnLeft();
      delay(50);
      turnOnSlow();
      delay(1000);
      currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
      break;
    case T90_RIGHT_STATE:
      Serial.println("STATUS: TURNING 90 RIGHT");
      stopWheels();
      delay(50);
      turnRight();
      delay(50);
      turnOnSlow();
      delay(1000);
      currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
      break;
    case T180_STATE:
      Serial.println("STATUS: TURNING 180");
      stopWheels();
      delay(50);
      turnAround();
      delay(50);
      turnOnSlow();
      currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
      break;
    }
  }
}


void pickUpObject() {
    backPickUp();
    servo_left.write(60);
    gripper.writeMicroseconds(500);//ensures right 
    delay(4000);
    gripper.writeMicroseconds(1000);
    for (int pos = 60; pos < 91; pos += 1) {  // goes from 0 degrees to 70 in 1 degree steps
      servo_left.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 15ms for the servo to reach the position
    }
    Serial.println("PICKUP DONE");
}

void dropUpObject() {
  servo_left.write(90);
  delay(1000);
  gripper.writeMicroseconds(1000);
  for(int pos = 90; pos >=60;pos-=1)     // goes from 180 degrees to 0 degrees
  {
    servo_left.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  gripper.writeMicroseconds(500);
  delay(1000);
  servo_left.write(90);
  backPickUp();
} 
