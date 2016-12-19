#include <ORC_motorshield.h>
#include <ORC_sonar.h>

// Extra hardware pin assignments ------------------------
#define SONAR_TRIGGER_PIN   A0
#define SONAR_ECHO_PIN      A1

// Global variables and objects --------------------------
Sonar                   sonar1(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN);
ORCKITMotorController   MotorController;
// ###############################################################
// ##                                                           ##
// ##   H I G H - L E V E L   M O T O R   F U N C T I O N S     ##
// ##                                                           ##
// ###############################################################
// setMotors ---------------------------------------------
//  Sends a given command to all motors.
//  Available commands (declared in ORC_motorbase.h)
//  are:
//    MOTOR_STOP
//    MOTOR_FWD
//    MOTOR_BACK
// -------------------------------------------------------
void setMotors (Motor_Cmd command) {
  MotorController.setMotorCommand(MOTOR_1, command);
  MotorController.setMotorCommand(MOTOR_2, command);
  MotorController.setMotorCommand(MOTOR_3, command);
  MotorController.setMotorCommand(MOTOR_4, command);
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
  // Serial (console) link. We will use this to show information
  // to the computer while the robot runs
  Serial.begin(9600);

  // Must be called during setup
  MotorController.init();

  // "Attach" servos, so we can use them.
  MotorController.attachServos();

  // Move servo1 (where the sonar should be mounted) to 90°,
  // so it faces forward
  MotorController.servo1.write (90);

  // Set the motors to "forward".
  setMotors (MOTOR_FWD);
}

// loop --------------------------------------------------
//  This procedure will be executed after setup() and 
//  will loop as long as the arduino board is powered.
// -------------------------------------------------------
void loop() {
  Serial.print("SONAR: ");
  Serial.print(sonar1.readSonarInCms());
  Serial.println(" cms");
  // Wait 1 second before doing this again
  delay(1000);
}

