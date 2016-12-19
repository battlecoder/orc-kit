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
// ## - 2016/12/19 Elias Zacarias                               ##
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
#ifndef __ORC_RADIOCTL_H
#define __ORC_RADIOCTL_H

#include <Arduino.h>

// Constants ----------------------------------
#define TOTAL_RC_BUTTONS    4
#define RECV_BUFFER_LEN     7

// We will define a "setCursor" operation that
// would indicate the receiver that we want to
// output what follows at a particular position
// on the receiving screen. The dimension of such
// screen are defined here.
#define RECV_SCREEN_W      26
#define RECV_SCREEN_H      5
// Class Definition ---------------------------
class ORCRadioControlClass {
private:
  char curX;
  char curY;
  byte bttnMask;
  byte recvBuffer[RECV_BUFFER_LEN+1];

public:
  void begin(long baud);
  void write(const char *str);
  void write(const String s);
  void write(char c);
  void write(int num, int fmt);
  void write(long num, int fmt);
  void write(double, int fmt);
  void setCursor(byte x, byte y);
  void sendData();
 
  bool readAndParse();
  char getCurrentX();
  char getCurrentY();
  bool getButtonPressed(byte button);
};

extern ORCRadioControlClass ORCRadioControl;
#endif

