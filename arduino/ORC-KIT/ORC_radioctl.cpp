#include "ORC_radioctl.h"
// ###############################################################
// ##  ORC_radioctl.cpp:                                        ##
// ##  ----------------------------                             ##
// ##  Code for ORC-KIT's Radio control Example.                ##
// ##                                                           ##
// ##  Part of the ORC-Kit project.                             ##
// ##                                                           ##
// ## History:                                                  ##
// ##  ----------------------------                             ##
// ## - 2016/12/15 Elias Zacarias                               ##
// ##     Data sending functions renamed for consistency with   ##
// ##     other similar APIs. setCursor() function added.       ##
// ##     Changed baud data type so it supports higher values.  ##
// ##  ----------------------------                             ##
// ## - 2016/10/09 Elias Zacarias                               ##
// ##     Released on GitHub.                                   ##
// ##  ----------------------------                             ##
// ## - 2016/04/10 Elias Zacarias                               ##
// ##     First released version.                               ##
// ##  ----------------------------                             ##
// ##                                                           ##
// ###############################################################

// ###############################################################
// ##                                                           ##
// ##                  S T A T I C   D A T A                    ##
// ##                                                           ##
// ###############################################################
ORCRadioControlClass ORCRadioControl;



// ###############################################################
// ##                                                           ##
// ##                A U X   F U N C T I O N S                  ##
// ##                                                           ##
// ###############################################################
char hexNibbleToDec(byte hexNibble) {
  if (hexNibble >= 'a' && hexNibble <= 'f') return hexNibble - 'a' + 10;
  if (hexNibble >= 'A' && hexNibble <= 'F') return hexNibble - 'A' + 10;
  if (hexNibble >= '0' && hexNibble <= '9') return hexNibble - '0';
  return 0;
}


// ###############################################################
// ##                                                           ##
// ##                 C L A S S   M E T H O D S                 ##
// ##                                                           ##
// ###############################################################
void ORCRadioControlClass::begin(long baud) {
  Serial.begin (baud);
}

void ORCRadioControlClass::write(const char *str) {
  Serial.print(str);
}

void ORCRadioControlClass::write(const String s) {
  Serial.print(s);
}

void ORCRadioControlClass::write(char c) {
  Serial.print(c);
}

void ORCRadioControlClass::setCursor(byte x, byte y){
  Serial.write(17);
  Serial.write((x % RECV_SCREEN_W) + 'A');
  Serial.write(18);
  Serial.write((y % RECV_SCREEN_H) + 'A');
}


void ORCRadioControlClass::write(int num, int fmt) {
  Serial.print(num, fmt);
}

void ORCRadioControlClass::write(long num, int fmt) {
  Serial.print(num, fmt);
}

void ORCRadioControlClass::write(double num, int fmt) {
  Serial.print(num, fmt);
}

void ORCRadioControlClass::sendData() {
  Serial.println("");
}

bool ORCRadioControlClass::readAndParse() {
  if (Serial.available() > 0) {
    // Shift the receiving buffer to the left
    for (byte i = 0; i < RECV_BUFFER_LEN; i++) recvBuffer[i] = recvBuffer[i + 1];

    // Append the received character to the queue
    recvBuffer[RECV_BUFFER_LEN] = Serial.read();

    // Radio data packet format:
    // 82 120 32 <X> <Y> <B> 13 10  ---> "Rx <X><Y><B>\r\n"
    // X, Y are the corresponding axis value in the 0-15 range (ASCII '0' to 'F')
    // B is a 4-bit mask of controller buttons, so it goes  (ASCII '0' to 'F')
    // Do we have a valid packet?
    if ((recvBuffer[0] == 'R') && (recvBuffer[1] == 'x') && (recvBuffer[2] == ' ') && (recvBuffer[6] == 13) && (recvBuffer[7] == 10)) {
      curX = hexNibbleToDec(recvBuffer[3]) - 7;
      curY = hexNibbleToDec(recvBuffer[4]) - 7;

      // Lower the upper range to 0 - 7  for both axes, instead of 0 - 8
      if (curX > 0) curX--;
      if (curY > 0) curY--;
      bttnMask = hexNibbleToDec(recvBuffer[5]);
      return true;
    }
  }
  return false;
}

char ORCRadioControlClass::getCurrentX() {
  return curX;
}

char ORCRadioControlClass::getCurrentY() {
  return curY;
}

bool ORCRadioControlClass::getButtonPressed(byte button) {
  if (button >= TOTAL_RC_BUTTONS) return false;
  return (bttnMask & (1 << button));
}


