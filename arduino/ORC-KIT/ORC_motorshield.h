// ###############################################################
// ##  ORC_motorshield.h:                                       ##
// ##  ----------------------------                             ##
// ##  A very simple driver for Adafruit's Motorshield (or      ##
// ##  clone) boards and standard H-bridges. This code controls ##
// ##  4 DC motor outputs and 2 servos.                         ##
// ##                                                           ##
// ##  More info about the board at:                            ##
// ##    https://learn.adafruit.com/adafruit-motor-shield       ##
// ##                                                           ##
// ## Part of the ORC-Kit project.                              ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/12/19 Elias Zacarias                               ##
// ##     Minor adaptations to create a version compatible with ##
// ##     standalone H-bridges.                                 ##
// ##  ----------------------------                             ##
// ## - 2016/10/09 Elias Zacarias                               ##
// ##     Released on GitHub.                                   ##
// ##  ----------------------------                             ##
// ## - 2016/04/10 Elias Zacarias                               ##
// ##     First released version.                               ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################
#ifndef __ORC_MOTORSHIELD_H
#define __ORC_MOTORSHIELD_H

#include "Arduino.h"
#include "ORC_motorbase.h"
#include "Servo.h"

// "Common" alias
#define ORCKITMotorController MotorShieldDrv

// Board pinout -------------------------------
// Adafruit's Motorshield (v1) driver
// This configuration should not be changed!
#define MTRCTL_M1PWM    11
#define MTRCTL_M2PWM    3
#define MTRCTL_M3PWM    6
#define MTRCTL_M4PWM    5
#define MTRCTL_LATCH    12
#define MTRCTL_CLK      4
#define MTRCTL_ENABLE   7
#define MTRCTL_DATA     8

#define MTRCTL_SERVO1   10
#define MTRCTL_SERVO2   9

// Class Definition ---------------------------
class MotorShieldDrv {
public:
  Servo servo1;
  Servo servo2;  

  void attachServos();
  void init();
  void setMotorSpeed (Motor_Index motor, byte mspeed);
  void setMotorCommand (Motor_Index motor, Motor_Cmd command);
  
};

#endif
