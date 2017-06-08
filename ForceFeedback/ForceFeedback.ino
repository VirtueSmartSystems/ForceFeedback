/* This sketch is the first of three in the prototype / proof
   of concept phase of the ForceFeedback concept. This sketch
   will let the user move the lever to any position, but will
   give a indent on a set point. This only happens when moving
   the lever to a higher position. 
   
   This sketch communicates with a Processing sketch found here:
   
*/
#include <Encoder.h>

/* Pins we are going to use */
const int stepPin = 7;
const int dirPin = 8;
const int enablePin = 9;

/* Libraries to use */
Encoder encoder(2, 3);

/* Global variables */
long oldPosition = -999;
long currentPos = 1;
int triggerValue = 20;

void setup() {
  /* Setting up all pins */
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(dirPin, LOW);

  Serial.begin(9600);
}

void loop() {
  // Updating the position of the encoder
  long newPosition = encoder.read();
  
  if (newPosition > oldPosition) {
    oldPosition = newPosition;
    currentPos--;
  }
  else if (newPosition < oldPosition) {
    oldPosition = newPosition;
    currentPos++;
  }

  // If the encoder hits the triggervalue, start up the motor
  // so the shaft will hold momentarily.
  if (currentPos == triggerValue) {
    digitalWrite(enablePin, LOW);
  }
  else {
    digitalWrite(enablePin, HIGH);
  }

  // Write the current position of the lever to the 
  // serial monitor, so we can use this in the processing sketch.
  Serial.write(currentPos);

}

/* Function for stepping the motor, something we just 
   want to happen, and don't want to care about */
void doSteps(bool dir, int amount) {
  digitalWrite(enablePin, LOW);
  digitalWrite(dirPin, dir);
  for (int i = 0; i <= amount; i++) {
    digitalWrite(stepPin, HIGH);
    delay(2);
    digitalWrite(stepPin, LOW);
    delay(2);
  }
  digitalWrite(enablePin, HIGH);
}

