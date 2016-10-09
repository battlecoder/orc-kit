// ###############################################################
// ##  ORC_motorshield.cpp:                                     ##
// ##  ----------------------------                             ##
// ##  Code for ORC-KIT's MotorShield driver.                   ##
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
#include "ORC_motorshield.h"
#include "Arduino.h"

// ###############################################################
// ##                                                           ##
// ##               L O C A L  C O N S T A N T S                ##
// ##                                                           ##
// ###############################################################
// Bit positions in the 74HCT595 shift register
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// ###############################################################
// ##                                                           ##
// ##                  S T A T I C   D A T A                    ##
// ##                                                           ##
// ###############################################################
static byte    motorData;
static byte    motorboard_initd = 0;

MotorShieldDrv MotorShield;


// ###############################################################
// ##                                                           ##
// ##                 C L A S S   M E T H O D S                 ##
// ##                                                           ##
// ###############################################################
void MotorShieldDrv::setMotorSpeed (Motor_Index motor, byte mspeed){
  switch (motor){
    case MOTOR_1:
      analogWrite(MTRCTL_M1PWM, mspeed);
      break;
    case MOTOR_2:
      analogWrite(MTRCTL_M2PWM, mspeed);
      break;
    case MOTOR_3:
      analogWrite(MTRCTL_M3PWM, mspeed);
      break;
    case MOTOR_4:
      analogWrite(MTRCTL_M4PWM, mspeed);
      break;
  }
}

void MotorShieldDrv::setMotorCommand (Motor_Index motor, Motor_Cmd command){
  byte bitPosA;
  byte bitPosB;
  boolean aVal = LOW;
  boolean bVal = LOW;

  if (command == MOTOR_FWD) {
    aVal = HIGH;
    bVal = LOW;
  } else if (command == MOTOR_BACK) {
    aVal = LOW;
    bVal = HIGH;
  }

  bitWrite (PORTD, 7, LOW); //Enable PIN
  switch (motor) {
    case MOTOR_1:
      bitWrite (motorData, MOTOR1_A, aVal);
      bitWrite (motorData, MOTOR1_B, bVal);
      break;
    case MOTOR_2:
      bitWrite (motorData, MOTOR2_A, aVal);
      bitWrite (motorData, MOTOR2_B, bVal);
      break;
    case MOTOR_3:
      bitWrite (motorData, MOTOR3_A, aVal);
      bitWrite (motorData, MOTOR3_B, bVal);
      break;
    case MOTOR_4:
      bitWrite (motorData, MOTOR4_A, aVal);
      bitWrite (motorData, MOTOR4_B, bVal);
      break;
    default:
      return;
  }
 
  digitalWrite (MTRCTL_LATCH, LOW);
  shiftOut(MTRCTL_DATA, MTRCTL_CLK, MSBFIRST, motorData);
  digitalWrite (MTRCTL_LATCH, HIGH);
}

void MotorShieldDrv::attachServos(){
  servo1.attach(MTRCTL_SERVO1);
  servo2.attach(MTRCTL_SERVO2);
}

// Init routine
MotorShieldDrv::MotorShieldDrv() {
  if (motorboard_initd) return;
  motorboard_initd = 1;
  pinMode (MTRCTL_LATCH, OUTPUT);
  pinMode (MTRCTL_ENABLE, OUTPUT);
  pinMode (MTRCTL_DATA, OUTPUT);
  pinMode (MTRCTL_CLK, OUTPUT);
  
  pinMode (MTRCTL_M1PWM, OUTPUT);
  pinMode (MTRCTL_M2PWM, OUTPUT);
  pinMode (MTRCTL_M3PWM, OUTPUT);
  pinMode (MTRCTL_M4PWM, OUTPUT);

  setMotorCommand(MOTOR_1, MOTOR_STOP);
  setMotorCommand(MOTOR_2, MOTOR_STOP);
  setMotorCommand(MOTOR_3, MOTOR_STOP);
  setMotorCommand(MOTOR_4, MOTOR_STOP);

  setMotorSpeed (MOTOR_1, 255);
  setMotorSpeed (MOTOR_2, 255);
  setMotorSpeed (MOTOR_3, 255);
  setMotorSpeed (MOTOR_4, 255);
}

