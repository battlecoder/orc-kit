// ###############################################################
// ##  OEC_hbridgedrv.cpp:                                      ##
// ##  ----------------------------                             ##
// ##  Code for ORC-KIT's HBridge driver.                       ##
// ##                                                           ##
// ## Part of the ORC-Kit project.                              ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/12/19 Elias Zacarias                               ##
// ##     Created as a standalone class.                        ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################
#include "ORC_hbridgedrv.h"

// ###############################################################
// ##                                                           ##
// ##                  S T A T I C   D A T A                    ##
// ##                                                           ##
// ###############################################################
static byte           motorSpeed[4];
static byte           cycleCnt = 0;
static byte           msCnt = 0;
static byte           motorData;
static byte           motorboard_initd = 0;
static byte           totalMotors;
static ORC_SoftServo* servoPtr1 = NULL;
static ORC_SoftServo* servoPtr2 = NULL;
// ###############################################################
// ##                                                           ##
// ##             U T I L I T Y   F U N C T I O N S             ##
// ##                                                           ##
// ###############################################################
byte get_h_bridge_pin_A (Motor_Index motorId){
    if (motorId == MOTOR_1) return MTRCTL_M1A;
    if (motorId == MOTOR_2) return MTRCTL_M2A;
    if (motorId == MOTOR_3) return MTRCTL_M3A;
    if (motorId == MOTOR_4) return MTRCTL_M4A;
    return 255;
}

byte get_h_bridge_pin_B (Motor_Index motorId){
    if (motorId == MOTOR_1) return MTRCTL_M1B;
    if (motorId == MOTOR_2) return MTRCTL_M2B;
    if (motorId == MOTOR_3) return MTRCTL_M3B;
    if (motorId == MOTOR_4) return MTRCTL_M4B;
    return 255;
}

// ###############################################################
// ##                                                           ##
// ##           I N T E R R U P T   H A N D L E R S             ##
// ##                                                           ##
// ###############################################################
// Called almsot exactly each 1ms
SIGNAL(TIMER0_COMPA_vect) {
  msCnt++;
  if (msCnt >= 19) {
    if (servoPtr1 && servoPtr1->attached()) servoPtr1->update();
    if (servoPtr2 && servoPtr2->attached()) servoPtr2->update();
    msCnt = 0;
  }
}

// Timer1 interrupt proc for our H-Bridge PWM
void motor_pwn_proc() {
  bool hiLo;
  byte i;

  for (i=0; i < totalMotors; i++){
    hiLo = (cycleCnt <= motorSpeed[i]);
    digitalWrite (get_h_bridge_pin_A((Motor_Index)i), bitRead(motorData, i<<1) && hiLo);
    digitalWrite (get_h_bridge_pin_B((Motor_Index)i), bitRead(motorData, (i<<1)+1) && hiLo);
  }
  
  cycleCnt++;
}


// ###############################################################
// ##                                                           ##
// ##                 C L A S S   M E T H O D S                 ##
// ##                                                           ##
// ###############################################################
void MotorHBridgeDrv::setMotorSpeed (Motor_Index motor, byte mspeed){
  motorSpeed[motor] = mspeed;
}

void MotorHBridgeDrv::setMotorCommand (Motor_Index motor, Motor_Cmd command){
  boolean aVal = LOW;
  boolean bVal = LOW;

  if (command == MOTOR_FWD) {
    aVal = HIGH;
    bVal = LOW;
  } else if (command == MOTOR_BACK) {
    aVal = LOW;
    bVal = HIGH;
  }

  bitWrite (motorData, motor<<1, aVal);
  bitWrite (motorData, (motor<<1) + 1, bVal);
}

void MotorHBridgeDrv::attachServos(){
  servo1.attach (MTRCTL_SERVO1);
  servo2.attach (MTRCTL_SERVO2);
  servoPtr1 = &servo1;
  servoPtr2 = &servo2;
}

// Init routine
void MotorHBridgeDrv::init(byte tMotors) {
  byte m;
  if (motorboard_initd) return;

  motorboard_initd = 1;
  totalMotors = min (tMotors, 4);

  for (m=0; m < totalMotors; m++){
    pinMode (get_h_bridge_pin_A((Motor_Index)m), OUTPUT);
    pinMode (get_h_bridge_pin_B((Motor_Index)m), OUTPUT);
    
    setMotorSpeed ((Motor_Index)m, 255);
  }

  Timer1.initialize(150);
  Timer1.attachInterrupt(motor_pwn_proc);

  // Configure TIMER0'S COMPA interrupt
  // Since timer0 is used to count milliseconds by Arduino
  // we will have a routine that is called roughly
  // each 1ms.
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

}

void MotorHBridgeDrv::init() {
    // By default init with 4 DC Motors
    init (4);
}

