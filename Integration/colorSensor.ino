/* Adopted from Frank Milburn, originally written 7 August 2015
 * 
 * https://github.com/fmilburn3/TCS3200_colorSensor/blob/master/TCS3200_colorSensor.ico
 * 
 * Added on by Cheyenne Arnold for EE149 @ UC Berkeley, Dec 4 2024
 */
 

#include <msp432.h>


// pin declarations
int outPin = 10;
int S2 = 8;
int S3 = 9;
int OEPin = 11;
int S1 = 12;
int S0 = 13;
int RED = 1;
int GREEN = 2;
int BLUE = 3;
volatile int lastColor = 0;

// constants - adjust if getting poor readings
const int Rc = 100;                      // Clear Relative Responsivity 
const int Rr = 99;                       // Red Relative Responsivity 
const int Rg = 55;                       // Green Relative Responsivity 
const int Rb = 70;                       // Blue Relative Responsivity 


int getLastColor(){return lastColor;}

const int delta = 500;
  int maxDelta(int rF, int gF, int bF) {
    if (abs(rF - gF) > delta) {
      if (rF < gF){
          return RED;
        } else {
          return GREEN;
        }
    }
    if (abs(rF - bF) > delta) {
      if (rF < bF) {
        return RED;
    } else {
      return BLUE;
      }
    }
    if (abs(gF - bF) > delta){
      if (gF < bF) {
        return GREEN;
      } else {
        return BLUE;
      }
    }
    return 0;
  }
void setupColor()
{
  Serial.begin(9600);
  pinMode(outPin, INPUT);               
  pinMode(OEPin, OUTPUT);
  digitalWrite(OEPin, LOW);              // Enable frequency scaling
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  digitalWrite(S0, HIGH);                // set frequency scaling of sensor
  digitalWrite(S1, LOW);                 // set frequency scaling of sensor
  pinMode(S2, OUTPUT);            
  pinMode(S3, OUTPUT);            
  pinMode(RED_LED, OUTPUT);              // R on RGB LED of MSP432
  pinMode(GREEN_LED, OUTPUT);            // G on RGB LED of MSP432
  pinMode(BLUE_LED, OUTPUT);             // B on RGB LED of MSP432
}

void loopColor(){ /** ask what this does
  //------------------ Read raw values and correct them ----------------------
  digitalWrite(S2, HIGH);                // get ready to read clear
  digitalWrite(S3, LOW);
  int cPulse = pulseIn(outPin, LOW);     // read clear
  /*
  Serial.print("C pulse = ");
  Serial.println(cPulse);            
  */
  int Ac = cPulse * Rc;                  // adjust reading for responsiveness
  
  digitalWrite(S2, LOW);                 // get ready to read red
  digitalWrite(S3, LOW);
  int rPulse = pulseIn(outPin, LOW);
  /*// read red    
  Serial.print("R pulse = ");
  Serial.println(rPulse);
  */
  int Ar = rPulse * Rr;                  // adjust reading for responsiveness
  int Cr = Ar - Ac;                      // correct for clear reading
  
  digitalWrite(S2, HIGH);                // get ready to read green
  digitalWrite(S3, HIGH);
  int gPulse = pulseIn(outPin, LOW);     // read green
  /*
  Serial.print("G pulse = ");
  Serial.println(gPulse);
  */
  int Ag = gPulse * Rg;                  // adjust reading for responsiveness                  
  int Cg = Ag - Ac;                      // correct for clear reading
    
  digitalWrite(S2, LOW);                 // get ready to read blue
  digitalWrite(S3, HIGH);
  int bPulse = pulseIn(outPin, LOW);     // read blue
  /*
  Serial.print("B pulse = ");
  Serial.println(bPulse);
  */
  int Ab = bPulse * Rb;                  // adjust reading for responsiveness
  int Cb = Ab - Ac;                      // correct for clear reading


  int r;
  int g;
  int b;
  int classify = maxDelta(rPulse, gPulse, bPulse);
  if (classify != 0) {
    if (classify == RED) {
      Serial.println("red");
      lastColor = RED;
    } else if (classify == GREEN) {
      Serial.println("green");
      lastColor = GREEN;
    } else {
      Serial.println("blue");
      lastColor = BLUE;
    }
  } else {
    Serial.println("No color");
    lastColor = 0;
  }

  

  //----------------------------------- Output ------------------------------------
  /*
  Serial.println("");
  Serial.print("r = "); Serial.println(r);
  Serial.print("g = "); Serial.println(g);
  Serial.print("b = "); Serial.println(b); 
  Serial.println(""); 
  analogWrite(RED_LED, r);
  analogWrite(GREEN_LED, g);
  analogWrite(BLUE_LED, b);
  delay(1000);  
  */
}
