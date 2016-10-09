// ###############################################################
// ##  ORC_sonar.cpp:                                           ##
// ##  ----------------------------                             ##
// ##  Code for ORC-KIT's Sonar HC-SR04 (and similar) driver.   ##
// ##                                                           ##
// ## Part of the ORC-Kit project.                              ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/10/09 Elias Zacarias                               ##
// ##     Released on GitHub.                                   ##
// ##  ----------------------------                             ##
// ## - 2016/04/10 Elias Zacarias                               ##
// ##     First released version.                               ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################
#include "ORC_sonar.h"


// ###############################################################
// ##                                                           ##
// ##                 C L A S S   M E T H O D S                 ##
// ##                                                           ##
// ###############################################################
Sonar::Sonar(byte trigger, byte echo){
  triggerPin = trigger;
  echoPin = echo;
  pinMode (triggerPin, OUTPUT);
  pinMode (echoPin, INPUT);
  digitalWrite(echoPin, LOW);
  
  setMaxRangeInCms (3000);
}

unsigned int Sonar::readSonarInCms(){
  return readSonarEchoTime()/58;
}

unsigned int Sonar::readSonarInInches(){
  return readSonarEchoTime()/148;
}

unsigned int Sonar::setMaxRangeInCms(unsigned int cms){
  // Set the timeout to max distance time + 15 (the approx amount of overhead from emitting the
  // burst of sound and the moment we start reading it.
  timeOut = cms*58 + 15;
}

unsigned int Sonar::setMaxRangeInInches(unsigned int in){
    // Same here.
    timeOut = in*148 + 15;
}

unsigned long Sonar::readSonarEchoTime(){
  unsigned long start_t, delta;

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // We could use pulseIn for detecting response time, but doing this
  // manually allows us to have a customizable timeout period, which
  // will mean not wasting much time if the signal never returns.

  // Wait for the echo pin to go HIGH.
  start_t = micros();
  do{
    delta = micros() - start_t;
    if (delta > timeOut) return timeOut;
  }while (!digitalRead(echoPin));

  // Now wait for the pin to go low
  start_t = micros();
  do{
    delta = micros() - start_t;
    if (delta > timeOut) return timeOut;
  }while (digitalRead(echoPin));

  return delta;
}
