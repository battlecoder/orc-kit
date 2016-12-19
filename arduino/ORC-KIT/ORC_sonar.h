// ###############################################################
// ##  ORC_sonar.h:                                             ##
// ##  ----------------------------                             ##
// ##  Basic library for using the HC-SR04 (and similar) sonar  ##
// ##  module. Unlike other libraries, this code allows you to  ##
// ##  set a max scan distance, so calls to readSonarXXXX()     ##
// ##  functions will not take an eternity to return when no    ##
// ##  obstacles are in range.                                  ##
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
#ifndef __ORC_SONAR_H
#define __ORC_SONAR_H

#include "Arduino.h"

// Class Definition ---------------------------
class Sonar{
private:
  byte echoPin;
  byte triggerPin;
  unsigned long timeOut;

public:
  Sonar(byte trigger, byte echo);
  unsigned int readSonarInCms();
  unsigned int readSonarInInches();
  unsigned int setMaxRangeInCms(unsigned int cms);
  unsigned int setMaxRangeInInches(unsigned int in);
  unsigned long readSonarEchoTime();
};

#endif
