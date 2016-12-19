#include <Servo.h>
#include <ORC_sonar.h>
#include <ORC_radioctl.h>
#include <ORC_motorshield.h>

// Extra hardware pin assignments ------------------------
#define SONAR_TRIGGER_PIN   A0
#define SONAR_ECHO_PIN      A1

// Other constants ---------------------------------------
#define MIN_SPEED          96  // Values smaller than this barely move the motors with my batteries. You can try with other values though.
#define MAX_SPEED          255  // This is the max speed supported by the framework. Don't change.

// Global variables and objects --------------------------
Sonar                   sonar1(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN);
byte                    currentLeftSpeed;
byte                    currentRightSpeed;
Motor_Cmd               currentLeftCmd;
Motor_Cmd               currentRightCmd;
ORCKITMotorController   MotorController;
// ###############################################################
// ##                                                           ##
// ##   H I G H - L E V E L   S O N A R   F U N C T I O N S     ##
// ##                                                           ##
// ###############################################################
// moveSonarToAngle --------------------------------------
//  Moves the sonar servo to a given angle.
//  Make sure the sonar servo is connected to the first
//  servo output of the motor controller.
// -------------------------------------------------------
void moveSonarToAngle (byte angle) {
  MotorController.servo1.write (angle);
}

// readSonarInCms ----------------------------------------
//  Obtains the current sonar reading in cms.
// -------------------------------------------------------
int readSonarInCms () {
  return sonar1.readSonarInCms();
}

// readSonarInInches -------------------------------------
//  Obtains the current sonar reading in inches.
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
  MotorController.setMotorSpeed(MOTOR_2, speed);
  MotorController.setMotorSpeed(MOTOR_3, speed);
  currentRightSpeed = speed;
}

// setLeftSideSpeed --------------------------------------
//  Changes the speed of the LEFT-side motors.
// -------------------------------------------------------
void setLeftSideSpeed (byte speed) {
  if (currentLeftSpeed == speed) return;
  MotorController.setMotorSpeed(MOTOR_1, speed);
  MotorController.setMotorSpeed(MOTOR_4, speed);
  currentLeftSpeed = speed;
}

// setRightSideMovement ----------------------------------
//  Sends a given command to the RIGHT-side motors.
//  Available commands (declared in ORC_motorbase.h)
//  are:
//    MOTOR_STOP
//    MOTOR_FWD
//    MOTOR_BACK
// -------------------------------------------------------
void setRightSideMovement (Motor_Cmd command) {
  if (currentRightCmd == command) return;
  MotorController.setMotorCommand(MOTOR_2, command);
  MotorController.setMotorCommand(MOTOR_3, command);
  currentRightCmd = command;
}

// setLeftSideMovement -----------------------------------
//  Sends a given command to the LEFT-side motors.
//  Available commands (declared in ORC_motorbase.h)
//  are:
//    MOTOR_STOP
//    MOTOR_FWD
//    MOTOR_BACK
// -------------------------------------------------------
void setLeftSideMovement (Motor_Cmd command) {
  if (currentLeftCmd == command) return;
  MotorController.setMotorCommand(MOTOR_1, command);
  MotorController.setMotorCommand(MOTOR_4, command);
  currentLeftCmd = command;
}

// spinRight ---------------------------------------------
// Spins the robot clockwise at a given speed.
// -------------------------------------------------------
void spinRight (byte spinSpeed) {
  setLeftSideMovement (MOTOR_FWD);
  setRightSideMovement (MOTOR_BACK);
  setRightSideSpeed (spinSpeed);
  setLeftSideSpeed (spinSpeed);
}

// spinLeft ----------------------------------------------
// Spins the robot counter-clockwise at a given speed.
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
//  When the Arduino "boots", this function is called once
//  so this is the perfect place to configure stuff before
//  we do anything. After this procedure ends, Arduino
//  will start running our loop() function until the board
//  is turned off (or resets).
// -------------------------------------------------------
void setup() {
  // Serial radio link
  ORCRadioControl.begin(9600);

  // Must be called during setup
  MotorController.init();

  // "Attach" servos, so we can use them.
  MotorController.attachServos();

  // Default sonar head position:
  moveSonarToAngle (90);

  // set a max distance for the sonar, so we won't waste precious time
  // waiting for the signal to return.
  sonar1.setMaxRangeInCms (40);
}

unsigned long lastSonarReadingTime = 0;
unsigned int accumulatedSonar = 0;
unsigned char accSonarCount = 0;
int sonarReading = 0;
// loop --------------------------------------------------
//  This procedure will be executed after setup() and 
//  will loop as long as the arduino board is powered.
// -------------------------------------------------------
void loop(){
  unsigned long rightNow = millis();
  int newRead;

  // Read the Sonar each 100ms
  if (lastSonarReadingTime - rightNow >= 100){
    newRead = readSonarInCms();
    // Ignore zero readings. They are normally timeouts.
    if (newRead != 0){
      accumulatedSonar += newRead;
      accSonarCount++;
      sonarReading = accumulatedSonar / accSonarCount;
      // Reset after 16 reads
      if (accSonarCount >= 16){
        accumulatedSonar = sonarReading;
        accSonarCount = 1;
      }
    }
    lastSonarReadingTime = rightNow;
  }
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
          // The robot will stop if it encounters an obstacle. This behavior can be cancelled
          // by pressing the first button on the controller app.
          if (sonarReading < 25 && !(ORCRadioControl.getButtonPressed(0))){
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
    // NOTE:
    // Sending data takes time, so I'd advise you to configure your bluetooth module at a higher
    // speed (57600 bps is a good choice) if you don't want the code below to make your robot less
    // responsive. Check online for the way you can configure the baudrate of your bluetooth module
    // and then update the call to ORCRadioControl.begin() in the setup() method of this sketch.
    ORCRadioControl.setCursor (0, 0);
    ORCRadioControl.write("SONAR: ");
    ORCRadioControl.write(sonarReading, DEC);
    ORCRadioControl.write("cm");
    ORCRadioControl.setCursor (0, 1);
    ORCRadioControl.write("STICK: ");
    ORCRadioControl.write(ORCRadioControl.getCurrentX(), DEC);
    ORCRadioControl.write(", ");
    ORCRadioControl.write(ORCRadioControl.getCurrentY(), DEC);
    ORCRadioControl.setCursor (0, 2);
    ORCRadioControl.write("SPEEDl:");
    ORCRadioControl.write(currentLeftSpeed, DEC);
    ORCRadioControl.setCursor (0, 3);
    ORCRadioControl.write("SPEEDr:");
    ORCRadioControl.write(currentRightSpeed, DEC);
    ORCRadioControl.sendData();
  }
}

