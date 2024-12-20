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

#define printy false
#define SAMPLES 100

// constants - adjust if getting poor readings
const int Rc = 100;                      // Clear Relative Responsivity 
const int Rr = 85;                       // Red Relative Responsivity 
const int Rg = 82;                       // Green Relative Responsivity 
const int Rb = 84;                       // Blue Relative Responsivity 
int rcount;
int gcount;
int bcount;
int tempcolor;
int getColorSamples() {
  rcount = 0;
  gcount = 0;
  bcount = 0;
  for (int i = 0; i < SAMPLES; i++) {
    tempcolor = getColor();
    switch(tempcolor) {
      case 0:
        break;
      case 1:
        rcount++;
        break;
      case 2:
        gcount++;
        break;
      case 3:
        bcount++;
        break;
    }
  }
    if (rcount * 3 > SAMPLES) {
      return RED;
    }
    if (gcount * 3 > SAMPLES) {
      return GREEN;
    }
    if (bcount * 3 > SAMPLES) {
      return BLUE;
    }
    return 0;
  }


int getColor(){
  
  digitalWrite(S2, HIGH);                // get ready to read clear
  digitalWrite(S3, LOW);
  int cPulse = pulseIn(outPin, LOW);     // read clear
  if (printy) {
  Serial.print("C pulse = ");
  Serial.println(cPulse);            
  }
  int Ac = cPulse * Rc;                  // adjust reading for responsiveness
  
  digitalWrite(S2, LOW);                 // get ready to read red
  digitalWrite(S3, LOW);
  int rPulse = pulseIn(outPin, LOW);
  if (printy) {    
  Serial.print("R pulse = ");
  Serial.println(rPulse);
  }
  int Ar = rPulse * Rr;                  // adjust reading for responsiveness
  int Cr = Ar - Ac;                      // correct for clear reading
  
  digitalWrite(S2, HIGH);                // get ready to read green
  digitalWrite(S3, HIGH);
  int gPulse = pulseIn(outPin, LOW);     // read green
  if (printy) {
  Serial.print("G pulse = ");
  Serial.println(gPulse);
  }
  int Ag = gPulse * Rg;                  // adjust reading for responsiveness                  
  int Cg = Ag - Ac;                      // correct for clear reading
    
  digitalWrite(S2, LOW);                 // get ready to read blue
  digitalWrite(S3, HIGH);
  int bPulse = pulseIn(outPin, LOW);     // read blue
  if (printy) {
  Serial.print("B pulse = ");
  Serial.println(bPulse);
  }
  int Ab = bPulse * Rb;                  // adjust reading for responsiveness
  int Cb = Ab - Ac;                      // correct for clear reading


  int r;
  int g;
  int b;
  int classify = maxDelta(Cr, Cg, Cb);
  if (true) {
    Serial.println("RGB");
    Serial.print(Cr);
    Serial.print(" ");
    Serial.print(Cg);
    Serial.print(" ");
    Serial.print(Cb);
    Serial.print(" ");
  }
  if (classify != 0) {
    if (classify == RED) {
      if (printy){Serial.println("red");}
      return RED;
    } else if (classify == GREEN) {
      if (printy){Serial.println("green");}
      return GREEN;
    } else {
      if (printy){Serial.println("blue");}
      return BLUE;
    }
  } else {
    if (printy) {Serial.println("No color");}
    return 0;
  }}

const int delta = 25000;
  int maxDelta(int rF, int gF, int bF) {

  if ((abs(rF - gF) > delta) || (abs(rF-bF) > delta) || (abs(bF-gF) > delta)) {
if ((rF < gF) && (rF < bF)){

         return RED;
    } else if ((gF < rF) && (gF < bF)) {

        return GREEN;
    } else if ((bF < rF) && (bF < gF)) {

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

void loopColor() {

}
