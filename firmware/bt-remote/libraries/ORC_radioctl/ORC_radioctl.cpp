#include "ORC_radioctl.h"

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
void ORCRadioControlClass::begin(int baud) {
  Serial.begin (baud);
}

void ORCRadioControlClass::send(const char *str) {
  Serial.print(str);
}

void ORCRadioControlClass::send(const String s) {
  Serial.print(s);
}

void ORCRadioControlClass::send(char c) {
  Serial.print(c);
}

void ORCRadioControlClass::send(int num, int fmt) {
  Serial.print(num, fmt);
}

void ORCRadioControlClass::send(long num, int fmt) {
  Serial.print(num, fmt);
}

void ORCRadioControlClass::send(double num, int fmt) {
  Serial.print(num, fmt);
}

void ORCRadioControlClass::sendDataEnd() {
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


