// ###############################################################
// ##  ORC_radioctl.h:                                          ##
// ##  ----------------------------                             ##
// ##  High-level abstraction of the ORC-Kit control app        ##
// ##  communication protocol.                                  ##
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
#ifndef __ORC_RADIOCTL_H
#define __ORC_RADIOCTL_H

#include <Arduino.h>

// Constants ----------------------------------
#define TOTAL_RC_BUTTONS    4
#define RECV_BUFFER_LEN     7

// Class Definition ---------------------------
class ORCRadioControlClass {
private:
  char curX;
  char curY;
  byte bttnMask;
  byte recvBuffer[RECV_BUFFER_LEN];

public:
  void begin(int baud);
  void send(const char *str);
  void send(const String s);
  void send(char c);
  void send(int num, int fmt);
  void send(long num, int fmt);
  void send(double, int fmt);
  void sendDataEnd();
 
  bool readAndParse();
  char getCurrentX();
  char getCurrentY();
  bool getButtonPressed(byte button);
};

extern ORCRadioControlClass ORCRadioControl;
#endif

