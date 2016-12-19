// ###############################################################
// ##  ORC_softservo.h:                                         ##
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
#ifndef __ORC_SOFTSERVO_H
#define __ORC_SOFTSERVO_H
#include "Arduino.h"


// Class Definition ---------------------------
class ORC_SoftServo {
private:
  boolean _attached;
  byte    _pin;
  int     _angle;
  word    _pulseW;
  word    _minPulseW, _maxPulseW;
  int     _minAngle, _maxAngle;

public:
  ORC_SoftServo();
  void attach(byte p);
  void detach();
  boolean attached();
  void write (byte a);
  void update ();
  byte read();
  void writeMicroseconds (word us);
  void setParameters (int minAngle, word minPulseW, int maxAngle, word maxPulseW);
};

#endif
