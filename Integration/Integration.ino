#include <Servo.h>
#include "header.h"

#include <stdio.h>

unsigned long previousTime = 0;
/*  LiftBot - autonomous line following and object sorting robot
 *   
 *   Integration code developed by Cheyenne Arnold, Vasti Orbach and Ethan Liang
 *   Motor encoding code done by Vasti Orbach
 *   Bluetooth communication code developed by Ethan Liang
 *   
 *   Made for final project in EE149 @ UC Berkeley, Fall 2024
 */

#define DEBUGGING false
#define DEMO false

//states
#define IDLE_STATE 'a'
#define DRIVING_STATE 'b'
#define PICKUP_STATE 'c'
#define T90_LEFT_STATE 'd'
#define T90_RIGHT_STATE 'e'
#define DROPPING_STATE 'f'
#define T180_STATE 'g'
#define IDLE_PICKUP_STATE 'h'

//events
#define BT_START_EVENT 52
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

 int turnBuffer = 0;
#define TURNBUFFMS 1000 //1000

volatile int lineBuff = 0;
#define LINEBUFFMS 3000 //3000

int milT;
volatile bool turnLonger = false;
volatile int currentTarget = UNSORTED_BIN;
volatile int lastTarget;
volatile bool pastHalf = true;
volatile bool returning = true;
volatile int currentState = IDLE_STATE;
volatile bool turning = false;
volatile bool driveInit = false;
int preState = 99;


/*
 * pastHalf and returning are updated BEFORE entering turning state
 *  Current Target and New Target are updated BEFORE entering dropping/pickup state
 */
char handleStateTransition(char inState, int event) {
  if (event == BT_STOP_EVENT) {
    Serial.println("btstop");
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
      switch(event) {
        case RED_DETECTED_EVENT:
          currentTarget = RED_BIN;
          break;
        case GREEN_DETECTED_EVENT:
          currentTarget = GREEN_BIN;
          break;
        case BLUE_DETECTED_EVENT:
          currentTarget = BLUE_BIN;
          break;
        
      }
      lastTarget = UNSORTED_BIN;
      returning = 0;
      pastHalf = 0;
      return PICKUP_STATE;
      
    case DRIVING_STATE:
      //Serial.println(milT);

      if (event == LLINE_DETECTED_EVENT && (millis() > (turnBuffer + TURNBUFFMS))) {
        Serial.println("DROPLINETEST");
        if (pastHalf) {
          if (returning) {
            return IDLE_PICKUP_STATE;
          } else {
            returning = true;
            lastTarget = currentTarget;
            currentTarget = UNSORTED_BIN;
            return DROPPING_STATE;
          }
        } else {
          if ((currentTarget == RED_BIN && !returning) || (lastTarget == GREEN_BIN && returning)) {
            pastHalf = true;
            Serial.println("DEBUGGIN");
            return T90_LEFT_STATE;
          } else if (currentTarget == BLUE_BIN) {
            pastHalf = true;
            delay(500);
            return DRIVING_STATE;
          }
          return DRIVING_STATE;
        }
      }
        /*
        Serial.println("infunc");// Left line detected while driving
        if ((currentTarget == UNSORTED_BIN && lastTarget == GREEN_BIN)&& returning && pastHalf) {
          if (pastHalf) {
            Serial.println("buggy");
            return IDLE_PICKUP_STATE; // Left side detected pickup line
          } else {
            pastHalf = true;

            
            Serial.println("Going Green to Unsorted");

            
            return T90_LEFT_STATE; // Left turn going from green to unsorted
          }
        } else if (((currentTarget == RED_BIN || currentTarget == GREEN_BIN) && lastTarget == UNSORTED_BIN) && !returning){
          if (pastHalf) {
            return DROPPING_STATE; // Left side detected droppoff line
          } else {
            pastHalf = true;
            
            
            Serial.println("Going Unsorted to Red");
            
            
            return T90_LEFT_STATE; // Left turn going from unsorted to red;
          }
        } else if ((currentTarget == UNSORTED_BIN) && (returning && pastHalf)) { // left detected pickup line
          Serial.println("DEBUG!");
          return IDLE_PICKUP_STATE;
        } else if ((currentTarget == BLUE_BIN) && !returning) {
            if (!pastHalf) {
              pastHalf = true;

              return inState;
            } else {
              return DROPPING_STATE;
            }
          } else if ((currentTarget == UNSORTED_BIN && lastTarget == BLUE_BIN) ) {
            if (!pastHalf) {
              pastHalf = true;

              return inState;
            } else {
              Serial.println("debug 3");
              return IDLE_PICKUP_STATE;
            }
          }else {
          return inState;
          }
          */
      if (event == RLINE_DETECTED_EVENT  && (millis() > turnBuffer + TURNBUFFMS)) { //Right line detected while driving
        Serial.println("bug2");

        if (pastHalf) {
          if (returning) {
            return IDLE_PICKUP_STATE;
          } else {
            returning = true;
            lastTarget = currentTarget;
            currentTarget = UNSORTED_BIN;
            return DROPPING_STATE;
          }
        } else {
          if ((currentTarget == GREEN_BIN && !returning) || (lastTarget == RED_BIN && returning)) {
            pastHalf = true;
            Serial.println("buggin");
            return T90_RIGHT_STATE;
          } else if (currentTarget == BLUE_BIN) {
            pastHalf = true;
            delay(500);
            return DRIVING_STATE;
          }
          return DRIVING_STATE;
        }
      }
        /*
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
        }else if ((currentTarget == UNSORTED_BIN) && (returning && pastHalf)){
            return IDLE_PICKUP_STATE;
          } else if ((currentTarget == BLUE_BIN) && !returning) {
            if (!pastHalf) {
              pastHalf = true;
              delay(1500);
              return inState;
            } else {
              return DROPPING_STATE;
            }
          } else if ((currentTarget == UNSORTED_BIN && lastTarget == BLUE_BIN) ) {
            if (!pastHalf) {
              pastHalf = true;
              delay(1500);
              return inState;
            } else {
              return IDLE_PICKUP_STATE;
            }
          }else {
          return(inState);
          }
      }
      */
      break;
      
    case T180_STATE:
      if (event == TURN_FINISHED_EVENT) {
        turnBuffer = millis();
        return DRIVING_STATE;
      }
    case T90_LEFT_STATE:
      if (event == TURN_FINISHED_EVENT) {
        turnBuffer = millis();
        return DRIVING_STATE;
      }
    case T90_RIGHT_STATE:
      if (event == TURN_FINISHED_EVENT) {
        turnBuffer = millis();
        return DRIVING_STATE;
      }
    case PICKUP_STATE:
      if (event == PICKUP_FINISHED_EVENT) {
        pastHalf = false;
        returning = false;
        turnLonger = false;
        Serial.println("RETURN T180");
        return T180_STATE;
      }
    case DROPPING_STATE: // not working
      if (event == DROP_FINISHED_EVENT) {
        if (currentTarget != UNSORTED_BIN){
          lastTarget = currentTarget;
          currentTarget = UNSORTED_BIN;
        }
        pastHalf = false;
        returning = true;
        turnLonger = true;
        Serial.println("RETURNT180");
        return T180_STATE;
      }
      break;
  } //end of switch
} //end of func

void printState(int state, bool vars) {
  switch (state) {
    case IDLE_STATE:
<<<<<<< HEAD
      Serial.println("Idle State");
=======
      Serial.println("----STATE----: Idle State");
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
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
<<<<<<< HEAD
    milT = millis();
=======
    straight();

    /*
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
    Serial.println("start");

    currentState = IDLE_PICKUP_STATE;
    pastHalf = true;
    returning = true;
    printState(currentState, true);
    

    currentState = handleStateTransition(currentState, GREEN_DETECTED_EVENT);
    Serial.println("green detected");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, PICKUP_FINISHED_EVENT);
    Serial.println("pickup finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("turn finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, RLINE_DETECTED_EVENT);
    Serial.println("Rline detected, should turn");
    printState(currentState, true);
    

    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("midturndone");
    printState(currentState, true);

    
    
    currentState = handleStateTransition(currentState, LLINE_DETECTED_EVENT);
    Serial.println("dropline");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, DROP_FINISHED_EVENT);
    Serial.println("drop finished");
    printState(currentState, true);
    
    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("Turnaround finished");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, LLINE_DETECTED_EVENT);
    Serial.println("mid detection");
    printState(currentState, true);

    currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
    Serial.println("mid turn done");
    printState(currentState, true); 

    
    currentState = handleStateTransition(currentState, LLINE_DETECTED_EVENT);
    Serial.println("Idle pickup");
    printState(currentState, true);

    Serial.println("end");
    delay(2000);
 
  } else if (DEMO) {
//    Serial.print(1);4
    delay(20);
   getLineUpdates();

    
  } else { //THE LOOOOOOOOOOOOOOOP
    // printing
    if (counter == 10) {
      //printState(currentState, true);
      counter = 0;
    } else {
      counter++;
    }
    if (preState != currentState) {
      preState = currentState;
      printState(currentState, true);
    }
    if (pastHalf) {
      analogWrite(RED_LED, 255);
    } else {
      analogWrite(RED_LED, 0);
    }
    if (millis() > lineBuff + LINEBUFFMS) {
      analogWrite(GREEN_LED, 255);
    } else {
      analogWrite(GREEN_LED, 0);
    }


 bool btval = getBtOn(); //orignall truew
  if (!btval) { //check for bluetooth shutdown
    currentState = handleStateTransition(currentState, BT_STOP_EVENT);
    }
  switch(currentState) {
    case IDLE_STATE:
    stopWheels();
      if (btval) {
        Serial.println("Bluetooth start command, moving to driving");
        currentState = handleStateTransition(currentState, BT_START_EVENT);
      }
    case IDLE_PICKUP_STATE:
    stopWheels();
      tcolor = getColorSamples();
      switch (tcolor) {
        case 1:
        Serial.println("red box detected");
          currentState = handleStateTransition(currentState, RED_DETECTED_EVENT);
          break;
        case 2:
          currentState = handleStateTransition(currentState, GREEN_DETECTED_EVENT);
        Serial.println("green box detected");
          break;
        case 3:
          currentState = handleStateTransition(currentState, BLUE_DETECTED_EVENT);
        Serial.println("blue box detected");
          break;
      }
      break;
    case DRIVING_STATE:
       servo_left.write(90);
      //stopWheels();
      //delay(100);
      updateLineFollower();
      Serial.println("Following");
      
      //delay(100);
      lineUpdates = getLineUpdates();
      Serial.println(lineUpdates);
      if (lineBuff + LINEBUFFMS < millis()) {
      if (lineUpdates == 1) {
        if ((!(currentTarget == BLUE_BIN || lastTarget == BLUE_BIN)) && pastHalf) {
          Serial.println("DEBUG!");
          stopWheels();
          }
          lineBuff = millis();
        currentState = handleStateTransition(currentState, LLINE_DETECTED_EVENT);
      } else if (lineUpdates == 2) {
        if ((!(currentTarget == BLUE_BIN || lastTarget == BLUE_BIN)) && pastHalf) {
          Serial.println("DEBUG!");
          stopWheels();
          }
          lineBuff = millis();
        currentState = handleStateTransition(currentState, RLINE_DETECTED_EVENT);
      }
      }
      break;
    case PICKUP_STATE:
      //pickup sequence
      stopWheels();
      pickUpObject();
      delay(500);
      Serial.println("STATUS: PickedUp");
      currentState = handleStateTransition(currentState, PICKUP_FINISHED_EVENT);
      break;
    case DROPPING_STATE:
      stopWheels();
      Serial.println("DroppingSequence");
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
      delay(1200);
      currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
      break;
    case T90_RIGHT_STATE:
      Serial.println("STATUS: TURNING 90 RIGHT");
      stopWheels();
      delay(50);
      turnRight();
      delay(50);
      turnOnSlow();
      delay(1200);
      Serial.println("postdelay");
      currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
      break;
    case T180_STATE:
      Serial.println("STATUS: TURNING 180");
      stopWheels();
      delay(50);
      turnAround(turnLonger);
      delay(50);
      turnOnSlow();
      delay(500);
      currentState = handleStateTransition(currentState, TURN_FINISHED_EVENT);
      break;
    }
  }
}


void pickUpObject() {
    backPickUp();
    for (int pos = 90; pos >= 60; pos -= 1) {  // goes from 0 degrees to 70 in 1 degree steps
      servo_left.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 15ms for the servo to reach the position
    };
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
  //servo_left.write(90);
  delay(1000);
<<<<<<< HEAD
  gripper.writeMicroseconds(900);
  /*for(int pos = 90; pos >=60;pos-=1)     // goes from 180 degrees to 0 degrees
=======
  gripper.writeMicroseconds(1000);
  for(int pos = 90; pos >=60;pos-=1)     // goes from 180 degrees to 0 degrees
>>>>>>> 4586be88b5763aeca173989ab5075e5a75ac50f7
  {
    servo_left.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  */
  delay(500);
  gripper.writeMicroseconds(500);
  delay(1000);
  servo_left.write(90);
  backPickUp();
} 
