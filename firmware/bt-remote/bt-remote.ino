#include <Servo.h>
#include "libraries/ORC_sonar/ORC_sonar.h"
#include "libraries/ORC_radioctl/ORC_radioctl.h"
#include "libraries/ORC_motorshield/ORC_motorshield.h"

// Extra hardware pin assignments ------------------------
#define SONAR_TRIGGER_PIN   A0
#define SONAR_ECHO_PIN      A1

// Other constants ---------------------------------------
#define MIN_SPEED          96  // Values smaller than this barely move the motors with my batteries. You can try with other values though.
#define MAX_SPEED          255  // This is the max speed supported by the framework. Don't change.

// Global variables and objects --------------------------
Sonar       sonar1(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN);
byte        currentLeftSpeed;
byte        currentRightSpeed;
Motor_Cmd   currentLeftCmd;
Motor_Cmd   currentRightCmd;
// ###############################################################
// ##                                                           ##
// ##   H I G H - L E V E L   S O N A R   F U N C T I O N S     ##
// ##                                                           ##
// ###############################################################
// moveSonarToAngle --------------------------------------
//  Moves the sonar servo to a given angle.
//  Make sure the sonar servo is connected to servo_1 in
//  the Shield.
// -------------------------------------------------------
void moveSonarToAngle (byte angle) {
  MotorShield.servo1.write (angle);
}

// readSonarInCms ----------------------------------------
//  obtains the current sonar reading in cms.
// -------------------------------------------------------
int readSonarInCms () {
  return sonar1.readSonarInCms();
}

// readSonarInInches -------------------------------------
//  obtains the current sonar reading in inches.
// -------------------------------------------------------
int readSonarInInches () {
  return sonar1.readSonarInInches();
}

// ###############################################################
// ##                                                           ##
// ##   H I G H - L E V E L   M O T O R   F U N C T I O N S     ##
// ##                                                           ##
// ###############################################################
// setRightSideSpeed -------------------------------------
//  Changes the speed of the RIGHT-side motors.
// -------------------------------------------------------
void setRightSideSpeed (byte speed) {
  if (currentRightSpeed == speed) return;
  MotorShield.setMotorSpeed(MOTOR_2, speed);
  MotorShield.setMotorSpeed(MOTOR_3, speed);
  currentRightSpeed = speed;
}

// setLeftSideSpeed --------------------------------------
//  Changes the speed of the LEFT-side motors.
// -------------------------------------------------------
void setLeftSideSpeed (byte speed) {
  if (currentLeftSpeed == speed) return;
  MotorShield.setMotorSpeed(MOTOR_1, speed);
  MotorShield.setMotorSpeed(MOTOR_4, speed);
  currentLeftSpeed = speed;
}

// setRightSideMovement ----------------------------------
//  Sends a given command to the RIGHT-side motors.
//  Available commands (declared in ORC_motorshield.h)
//  are:
//    MOTOR_STOP
//    MOTOR_FWD
//    MOTOR_BACK
// -------------------------------------------------------
void setRightSideMovement (Motor_Cmd command) {
  if (currentRightCmd == command) return;
  MotorShield.setMotorCommand(MOTOR_2, command);
  MotorShield.setMotorCommand(MOTOR_3, command);
  currentRightCmd = command;
}

// setLeftSideMovement -----------------------------------
//  Sends a given command to the LEFT-side motors.
//  Available commands (declared in ORC_motorshield.h)
//  are:
//    MOTOR_STOP
//    MOTOR_FWD
//    MOTOR_BACK
// -------------------------------------------------------
void setLeftSideMovement (Motor_Cmd command) {
  if (currentLeftCmd == command) return;
  MotorShield.setMotorCommand(MOTOR_1, command);
  MotorShield.setMotorCommand(MOTOR_4, command);
  currentLeftCmd = command;
}

// spinRight ---------------------------------------------
// Spins clockwise at a given speed.
// -------------------------------------------------------
void spinRight (byte spinSpeed) {
  setLeftSideMovement (MOTOR_FWD);
  setRightSideMovement (MOTOR_BACK);
  setRightSideSpeed (spinSpeed);
  setLeftSideSpeed (spinSpeed);
}

// spinLeft ----------------------------------------------
// Spins counter-clockwise at a given speed.
// -------------------------------------------------------
void spinLeft (byte spinSpeed) {
  setLeftSideMovement (MOTOR_BACK);
  setRightSideMovement (MOTOR_FWD);
  setRightSideSpeed (spinSpeed);
  setLeftSideSpeed (spinSpeed);
}

// motorsOff ---------------------------------------------
// Stops both motors
// -------------------------------------------------------
void motorsOff () {
  setLeftSideMovement (MOTOR_STOP);
  setRightSideMovement (MOTOR_STOP);
}

// reverse -----------------------------------------------
// Drives in reverse. Both motors can be set to spin at
// different speeds.
// -------------------------------------------------------
void reverse (byte leftSpeed, byte rightSpeed) {
  setLeftSideMovement (MOTOR_BACK);
  setRightSideMovement (MOTOR_BACK);
  setLeftSideSpeed (leftSpeed);
  setRightSideSpeed (rightSpeed);
}


// forward -----------------------------------------------
// Drives forward. Both motors can be set to spin at
// different speeds.
// -------------------------------------------------------
void forward (byte leftSpeed, byte rightSpeed) {
  setLeftSideMovement (MOTOR_FWD);
  setRightSideMovement (MOTOR_FWD);
  setLeftSideSpeed (leftSpeed);
  setRightSideSpeed (rightSpeed);
}

// ###############################################################
// ##                                                           ##
// ##         A R D U I N O   E N T R Y   P O I N T S           ##
// ##                                                           ##
// ###############################################################
// setup -------------------------------------------------
//  When the Arduino "boots", this function will be called
//  once, so this is the perfect place to configure stuff before
//  we do anything. After this procedure ends, Arduino
//  will start running our loop() function until the board
//  is turned off (or resets).
// -------------------------------------------------------
void setup() {
  // Serial radio link
  ORCRadioControl.begin(9600);
  
  // "Attach" servos, so we can use them.
  MotorShield.attachServos();

  // Default sonar head position:
  moveSonarToAngle (90);
}

// loop --------------------------------------------------
//  This procedure be executed for as long as the Arduino
//  board is powered.
// -------------------------------------------------------
void loop(){
  // readAndParse() returns true when a new data packet has been processed and we can query the
  // new values for X, Y and buttons. We will use this event to update what our motors are doing.
  if (ORCRadioControl.readAndParse()){
    int xAxis    = ORCRadioControl.getCurrentX();
    int throttle  = ORCRadioControl.getCurrentY();
    byte speed, speed2;

    // When both sticks are centered we want the robot to stop ~~~~~~~~~~~~~~~~~~~~~~~~~
    if (throttle == 0 && xAxis == 0){
      motorsOff();
    }else {
      // If only the throttle stick is centered we want to spin in place  ~~~~~~~~~~~~~~
      if (throttle == 0){
        // Speed will be obtained directly from the horizontal stick.
        speed = map(abs(xAxis), 0, 7, MIN_SPEED, MAX_SPEED);
        if (xAxis < 0){
          spinLeft (speed);
        }else {
          spinRight (speed);
        }

      }else {
        // This section will deal with the general motion case, which normally involves moving forwards or backwards
        // and steering either right or left (to some extent) at the same time.
        // Thus, we will need to compute the speed for the "dominant" (i.e: faster) and "non-dominant" (i.e: slower) side of
        // the robot as follows:
        // Dominant side will get its speed directly from the throtle axis:
        speed = map (abs(throttle), 0, 7, MIN_SPEED, MAX_SPEED);
        // Non-dominant side will move at a fraction of the dominant side speed, proportional to the xAxis value.
        // Note that for xAxis = 0 (centered X stick) both speeds will be equal, so there's no need to treat xAxis = 0
        // as an special case.
        speed2 = map (abs(xAxis), 0, 7, speed, 0); 

        // If the throttle stick is a negative number then we want to go reverse ~~~~~~~~~
        if (throttle < 0) {
          // Which side is dominant depends on whether we are pushing the X stick left or right
          if (xAxis < 0) {
            reverse (speed, speed2);
          }else {
            reverse (speed2, speed);
          }

        // Otherwise we want to go forward ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }else {
          // Let's check what the sonar says before moving forward. If there's an obstacle near
          // we actually want to stop.
          //if (readSonarInCms() < 25){
          if (false){
            motorsOff();

          // Which side is dominant depends on whether we are pushing the X stick left or right
          }else if (xAxis < 0) {
            forward (speed2, speed);
          }else {
            forward (speed, speed2);
          }
        }
      }
    }
    ORCRadioControl.send(ORCRadioControl.getCurrentX(), DEC);
    ORCRadioControl.send(' ');
    ORCRadioControl.send(ORCRadioControl.getCurrentY(), DEC);
    ORCRadioControl.send(' ');
    ORCRadioControl.sendDataEnd();
  }
}

