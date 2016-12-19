// ###############################################################
// ##  OEC_hbridgedrv.h:                                        ##
// ##  ----------------------------                             ##
// ##  A very simple driver for standard H-bridges that has     ##
// ##  support for up to 4 DC motor outputs and 2 servos.       ##
// ##  providing a common controlling interface that mirrors    ##
// ##  our Motorshield driver.                                  ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/12/19 Elias Zacarias                               ##
// ##     Created as a standalone class.                        ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################
#ifndef __ORC_HBRIDGEDRV_H
#define __ORC_HBRIDGEDRV_H

// "Common" alias
#define ORCKITMotorController MotorHBridgeDrv


// ADDITIONAL LIBRARIES REQUIRED -------------------------------
//
// +-----------------------------------------------------------+
// | The following libraries can also be installed from within |
// | the Arduino programming environment.                      | 
// +-----------------------------------------------------------+
//
// ORC-KIT H-Bridge driver uses a timer-based interrupt for the
// variable speed control. This feature depends on the TimerOne
// library:
//   https://github.com/PaulStoffregen/TimerOne
//
//--------------------------------------------------------------
#include "Arduino.h"
#include "TimerOne.h"
#include "ORC_SoftServo.h"
#include "ORC_motorbase.h"

// Board pinout -------------------------------
// H-DRIVER DEFINITIONS
// Standard H-bridge driver. You can change
// this configuration if you want.
#define MTRCTL_M1A      2
#define MTRCTL_M1B      3
#define MTRCTL_M2A      4
#define MTRCTL_M2B      5
#define MTRCTL_M3A      6
#define MTRCTL_M3B      7
#define MTRCTL_M4A      8
#define MTRCTL_M4B      9
#define MTRCTL_SERVO1   10
#define MTRCTL_SERVO2   11

// Class Definition ---------------------------
class MotorHBridgeDrv {
public:
  ORC_SoftServo servo1;
  ORC_SoftServo servo2;

  void attachServos();
  void init();
  void init(byte tMotors);
  void setMotorSpeed (Motor_Index motor, byte mspeed);
  void setMotorCommand (Motor_Index motor, Motor_Cmd command);  
};
#endif
