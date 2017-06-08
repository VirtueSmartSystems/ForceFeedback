/* This sketch is part of the proof of concept for the 
 * ForceFeedback concept, and will show the temperature
 * and position of the lever. It will do this according
 * to the data that is coming from an Arduino that is 
 * connected to a SerialPort.
 */
import processing.serial.*;

/* Libaries and classes */
Serial arduinoPort; 
StopWatchTimer stopWatch;
PFont f;

/* Global variables */
float angle = 0;
int temp = 0;
PImage bg;
int minValue = 0;
int maxValue = 38;
int triggerValue = 20;


void setup() {
  size(800, 800);
  noStroke();
  bg = loadImage("bg.png");

  // get the second serial port on the machine, 
  // in my case this is correct. But you might
  // want to change it for usage on another computer.
  String portName = Serial.list()[2];
  arduinoPort = new Serial(this, portName, 9600);
  stopWatch = new StopWatchTimer();
  f = createFont("Arial", 16, true);
  textFont(f, 36);
}

void draw() {
  // first, draw the background
  background(bg);

  // then, put the textual data on the screen
  text(stopWatch.minute() + " : " + stopWatch.second(), 20, 40);
  text(temp + " C", 700, 40);
  fill(105);

  // print the bar that indicates the temperature, and rotate
  // it according to the angle coming in from the serial
  translate(width/2, height/2);
  rotate(radians(angle));
  rect(-5, -5, 320, 10);
  fill(105);
}

// Event is called every time there is new data on the serialport
void serialEvent(Serial thisPort) {
  int recvd = thisPort.read(); 
  println("Got " + recvd);
  if (recvd >= minValue & recvd <= maxValue) {
    angle = map(float(recvd), minValue, maxValue, -200, 20);
    temp = recvd;
    println("angle: " + angle);
    if (recvd >= triggerValue) {
      //start the timer 
      stopWatch.stop();
    } else if (recvd <= triggerValue) {
      //stop the timer 
      stopWatch.start();
    }
  }
}