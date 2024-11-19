#include <stdint.h>
#include <Encoder.h>
#include <RSLK_Pins.h> 
#include <Romi_Motor_Power.h>
/** DOWNLOAD Energia -RSLK-LIBRARY" **/

// Define motor control pins
#define LEFT_MOTOR_PWM_PIN 40 // Energia pin 40
#define RIGHT_MOTOR_PWM_PIN 39 // Energia pin 39
#define LEFT_MOTOR_DIR_PIN 29 // Energia pin 29
#define RIGHT_MOTOR_DIR_PIN 30 // Energia pin 30
#define Rsleep 11
#define Lsleep 31

Romi_Motor_Power leftM;
Romi_Motor_Power rightM;
int wheelSpeed = 15; // Default PWM speed for motors

void setup() {
  // Initialize serial for debugging
  /**to find certain pins look at RSLK_PIN*/
  Serial.begin(9600);
  setupEncoder(ENCODER_ELA_PIN,ENCODER_ELB_PIN, ENCODER_ERA_PIN, ENCODER_ERB_PIN);

  //initialize the motors
  leftM.begin(31,29,40);
  rightM.begin(11, 30,39);

  // Print message for debugging
  Serial.println("Starting motor test...");

  leftM.enableMotor();
  rightM.enableMotor();
  // Set motor direction to forward
  leftM.directionForward();
  rightM.directionForward();

  leftM.setSpeed(150); 
  rightM.setSpeed(150); 

  /**

  analogWrite(LEFT_MOTOR_PWM_PIN, wheelSpeed);
  analogWrite(RIGHT_MOTOR_PWM_PIN, wheelSpeed);
  
  digitalWrite(LEFT_MOTOR_DIR_PIN,LOW);  // HIGH for forward, LOW for backward
  digitalWrite(RIGHT_MOTOR_DIR_PIN,LOW);

  // Set motor speed using PWM
  digitalWrite(LEFT_MOTOR_PWM_PIN, HIGH);
  digitalWrite(RIGHT_MOTOR_PWM_PIN,HIGH );

  digitalWrite(Rsleep,HIGH);
  digitalWrite(Lsleep,HIGH); */

  
}

void loop() {
  // Keep running the motors
  Serial.println("Motors running...");
  delay(1000); // Print status every second

  
}
