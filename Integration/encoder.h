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

Romi_Motor_Power left_m;
Romi_Motor_Power right_m;

// Robot Specifications
const float wheelDiameter = 7.0;       // inches
const uint32_t ticksPerRev = 360;      // ticks per wheel revolution
const float trackWidth = 14.5;         // inches (distance between the two wheels)
// Derived Constants
const float wheelCircumference = PI * wheelDiameter; // inches
const float distancePerTick = wheelCircumference / ticksPerRev; // inches/tick
