// ###############################################################
// ##  ORC_motorbase.h:                                         ##
// ##  ----------------------------                             ##
// ##  Base definitions that should help provide a common       ##
// ##  interface for different motor drivers.                   ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/12/19 Elias Zacarias                               ##
// ##     Created.                                              ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################
#ifndef __ORC_MOTORBASE_H
#define __ORC_MOTORBASE_H

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

#endif