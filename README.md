# ForceFeedback
This repo will contain all the code for the ForceFeedback proof of concept. 

## Arduino application

There are three Arduino sketches in this repo. They are all for giving force feedback to the user, but in three different ways:

* With the usage of fake 'indents' in the lever's movement
* Trying to turn the lever back when the user has used a lot of energy
* A combination of the above, with a timer that gradually makes indents in the lever's movement

## Visual application

A Processing application will show the current temperature set by the lever on the hardware. It will also display a timer, so the user can see how long he/she has been under the shower. This application is primarily for testing purposes, so we see what the system is doing without having a real shower.

## The hardware

The hardware used for this proof of concept is the following:

* Arduino Nano
* Stepper motor
* Stepper driver (Polulu A4988)
* Rotary encoder