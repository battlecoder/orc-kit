// ###############################################################
// ##  ORC_softservo.cpp:                                       ##
// ##  ----------------------------                             ##
// ##  A more versatile software implementation of a Servo      ##
// ##  controller.                                              ##
// ##  Part of the ORC-Kit project.                             ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/12/15 Elias Zacarias                               ##
// ##     Created.                                              ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################
#include "ORC_SoftServo.h"
#include "Arduino.h"

// ###############################################################
// ##                                                           ##
// ##                 C L A S S   M E T H O D S                 ##
// ##                                                           ##
// ###############################################################
ORC_SoftServo::ORC_SoftServo(){
  _angle     = 0;
  _pin       = 255;
  _attached  = false;

  // Default values
  _minPulseW = 1000;
  _maxPulseW = 2000;
  _minAngle  = 0;
  _maxAngle  = 180;
}

void ORC_SoftServo::attach(byte p){
  if (_attached) detach();

  _attached = true;
  _angle    = 90;
  _pin      = p;
  pinMode (_pin, OUTPUT);
}

void ORC_SoftServo::detach(){
  if (!_attached) return;

  _attached = false;
  pinMode (_pin, INPUT);
}

boolean ORC_SoftServo::attached(){
  return _attached;
}

void ORC_SoftServo::write (byte a){
  if (!_attached) return;

  _angle  = constrain (a, _minAngle, _maxAngle);
  _pulseW = map (_angle, _minAngle, _maxAngle, _minPulseW, _maxPulseW);
}

void ORC_SoftServo::update (){
  if (!_attached) return;
  digitalWrite (_pin, HIGH);
  delayMicroseconds (_pulseW);
  digitalWrite (_pin, LOW);
}

byte ORC_SoftServo::read(){
  return _angle;
}

void ORC_SoftServo::writeMicroseconds (word us){
  _pulseW = constrain(us, _minPulseW, _maxPulseW);
  _angle = map(_pulseW, _minPulseW, _maxPulseW, _minAngle, _maxAngle);
}

void ORC_SoftServo::setParameters(int minAngle, word minPulseW, int maxAngle, word maxPulseW){
  _minPulseW = minPulseW;
  _maxPulseW = maxPulseW;
  _minAngle  = minAngle;
  _maxAngle  = maxAngle; 
}