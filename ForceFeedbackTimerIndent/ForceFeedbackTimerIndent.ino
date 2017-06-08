/* this sketch is part of the proof of concept for the
 *  ForceFeedback concept. This sketch will wait for a 
 *  set time, and after that time, if the user wants to
 *  adjust the lever to a hotter position, it will have 
 *  a indent to indicate the user is wasting energy /water.
 *  
 *  This sketch communicates with a Processing sketch found here:
 *  
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
int setPoint = 20;
int currentHoldPoint = 0;

/* Global variables for the timer */
bool timerActive = false;
bool timePassed = false;
const long intervalTime = 5000;
unsigned long previousTime = 0;


void setup() {
  // Setting up all pins
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
  }
  // otherwise, if the current position is below the setpoint,
  // and the timer is active, stop the timer
  else if (currentPos <= setPoint && timerActive == true) {
    stopTimer();
  }

  // When the set time has passed, measure the current
  // level the lever is on, and remember that. When the
  // user wants to go past that point, add indents.
  if (timePassed) {
    if (currentHoldPoint == 0) {
      currentHoldPoint = currentPos + 2;
      stopTimer();
    }
    if (currentPos == currentHoldPoint) {
      digitalWrite(enablePin, LOW);
    }
    else {
      digitalWrite(enablePin, HIGH);
    }
  }
  else {
    digitalWrite(enablePin, HIGH);
  }

  Serial.write(currentPos);

}

/* Function for stepping the motor, something we just
   want to happen, and don't want to care about
*/
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

/* functions for handling the timer like
    starting, stopping and checking if
    the time has passed
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
