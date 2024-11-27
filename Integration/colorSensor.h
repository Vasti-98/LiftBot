/*********
  Based on project by Rui Santos
  Complete baseline project details at https://randomnerdtutorials.com  

  Modified by Cheyenne Arnold
*********/

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#define deltaMax 40
#define PERIOD 10
#include <stdio.h>
#include <stdlib.h>

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
int colors[] = {};
String colorNames[] = {"Red", "Green", "Blue"};
int i;
bool delta(int red, int green, int blue) {
  return (abs(red - blue) < deltaMax && abs(blue - green) < deltaMax && abs(red - green) < deltaMax);
}

void setup() {
  // Setting the outputs
 // pinMode(S0, OUTPUT);
 // pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%: Pins shorted to 5V and GND to simplify harnessing
//  digitalWrite(S0,HIGH);
//  digitalWrite(S1,LOW);
  
   // Begins serial communication 
  Serial.begin(9600);
}
void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // redColor = map(redFrequency, 60, 200, 255, 0);
   // Printing the RED (R) value
  
  Serial.print("R = ");
  Serial.print(redFrequency);
  //Serial.print(redColor);
  

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  greenColor = map(greenFrequency, 40, 250, 255, 0);
  // Printing the GREEN (G) value  
  
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  //Serial.print(greenColor);
  
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  blueColor = map(blueFrequency, 90, 180, 255, 0);
  // Printing the BLUE (B) value 
  
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  //Serial.println(blueColor);
  delay(100);
  
  colors[0] = redColor;
  colors[1] = greenColor;
  colors[2] = blueColor;

  if (delta(redFrequency, greenFrequency, blueFrequency)) {
    Serial.println("Nothing Detected");
  } else { 
    //delay in ms, samples 100/period times
    Serial.print("Sampling: ");
    Serial.println(100/PERIOD);
    int countT = 0;
    int countF = 0;
    for (int i = 0; i < 100/PERIOD; i++){
      delay(1000/(3*PERIOD));
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      int red1 = pulseIn(sensorOut, LOW);
      delay(1000/(3*PERIOD));
      digitalWrite(S3,HIGH);
      int blue1 = pulseIn(sensorOut, LOW);
      delay(1000/(3*PERIOD));
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      int green1 = pulseIn(sensorOut, LOW);
      if (delta(red1, green1, blue1)) {
        Serial.print("F");
        countF++;
      } else {
        Serial.print("T");
        countT++;
      }
    }
    Serial.println(" ");
    if (countT > .7*(100/PERIOD)) {
      Serial.println("SamplePassed");
    } else {
      Serial.println("SampleFailed");
    }
  }
/*
  if (blueColor < 0 && redColor < 0 && greenColor < 0) {
    Serial.println("No color found");
  } else {
    for (i = 0; i < 3; i++) {
      if (colors[i] > 0) {
        Serial.print(colorNames[i]);
        Serial.print(" ");
      }
    }
      Serial.println(" ");
    
  }
*/
}
