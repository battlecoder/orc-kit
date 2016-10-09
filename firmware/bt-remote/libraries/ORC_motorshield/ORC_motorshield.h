// ###############################################################
// ##  ORC_motorshield.h:                                       ##
// ##  ----------------------------                             ##
// ##  A very simple driver for Adafruit's Motorshield (and     ##
// ##  clone) boards. This code exposes the 4 motor outputs (as ##
// ##  DC motors) and the 2 servos (using Arduino's servo lib). ##
// ##                                                           ##
// ##  More info about the board at:                            ##
// ##    https://learn.adafruit.com/adafruit-motor-shield       ##
// ##                                                           ##
// ## Part of the ORC-Kit project.                              ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/04/10 Elias Zacarias                               ##
// ##     First released version.                               ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################
#ifndef __ORC_MOTORSHIELD_H
#define __ORC_MOTORSHIELD_H

#include "Arduino.h"
#include "Servo.h"

// Board pinout -------------------------------
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

// DC Motor outputs ---------------------------
typedef enum {
  MOTOR_1,
  MOTOR_2,
  MOTOR_3,
  MOTOR_4
}Motor_Index;

// Available motor commands -------------------
typedef enum {
  MOTOR_STOP,
  MOTOR_FWD,
  MOTOR_BACK
}Motor_Cmd;

// Class Definition ---------------------------
class MotorShieldDrv {
public:
  Servo servo1;
  Servo servo2;

  MotorShieldDrv();
  void attachServos();
  void setMotorSpeed (Motor_Index motor, byte mspeed);
  void setMotorCommand (Motor_Index motor, Motor_Cmd command);
};

extern MotorShieldDrv MotorShield;
#endif
