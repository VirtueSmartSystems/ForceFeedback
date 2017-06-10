/* This sketch is part of the proof of concept for the 
 *  ForceFeedback concept. This sketch will wait for a 
 *  set time, then move the lever back to a setpoint. 
 *  
 *  This sketch communicates with a Processing sketch found here:
 *  
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
int setPoint = 28;

/* Global variables for the timer */
bool timerActive = false;
bool timePassed = false;
const long intervalTime = 5000;
unsigned long previousTime = 0;


void setup() {
  /* Setting up all pins */
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(dirPin, LOW);

  Serial.begin(9600);
}

void loop() {
  // Updating the position of the encoder, and the timer
  long newPosition = encoder.read();
  updateTimer();

  if (newPosition > oldPosition) {
    oldPosition = newPosition;
    currentPos--;
  }
  else if (newPosition < oldPosition) {
    oldPosition = newPosition;
    currentPos++;
  }

  // If the encoder hits a setpoint, and the timer is not
  // active, start the timer
  if (currentPos > setPoint && timerActive == false) {
    startTimer();
    Serial.write("100");
  }
  // otherwise, if the current position is below the setpoint,
  // and the timer is active, stop the timer
  else if (currentPos <= setPoint && timerActive == true) {
    stopTimer();
    Serial.write("200");
  }

  // when the time has passed, the motor will step
  // back to a setpoint
  if (timePassed) {
    if (currentPos > setPoint) {
      doSteps(1, 1);
    }
    else {
      stopTimer();
      timePassed = false;
    }
  }
  else {
    digitalWrite(enablePin, HIGH);
  }

  // Write the current position of the lever to the
  // serial monitor, so we can use this in the processing sketch.
  Serial.write(currentPos);

}

/* Function for stepping the motor, something we just 
 * want to happen, and don't want to care about 
 */
void doSteps(bool dir, int amount) {
  digitalWrite(enablePin, LOW);
  digitalWrite(dirPin, dir);
  for (int i = 0; i <= amount; i++) {
    digitalWrite(stepPin, HIGH);
    delay(3);
    digitalWrite(stepPin, LOW);
    delay(3);
  }
  digitalWrite(enablePin, HIGH);
}

/* functions for handling the timer like 
 *  starting, stopping and checking if 
 *  the time has passed
 */
void startTimer() {
  timerActive = true;
  previousTime = millis();
}

void stopTimer() {
  timerActive = false;
}

void updateTimer() {
  if (timerActive) {
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= intervalTime) {
      previousTime = currentTime;
      if (!timePassed) {
        timePassed = true;
      }
    }
  }
}
