#ifndef util_h
#define util_h
#include "Arduino.h"

//Uncomment to enable debug print statements
#define DEBUG_MODE 1

#ifdef DEBUG_MODE
  #define DEBUG_PRNT(x)       Serial.print(x)
  #define DEBUG_PRNTLN(x)     Serial.println(x)
  #define DEBUG_PRNT_C(x, y)    Serial.print(x, y)
  #define DEBUG_PRNTLN_C(x, y)  Serial.println(x, y)
#else
  #define DEBUG_PRNT(x)
  #define DEBUG_PRNTLN(x)
  #define DEBUG_PRNT_C(x, y)
  #define DEBUG_PRNTLN_C(x, y)
#endif

#define PACKET_START 's'
#define SERVO_MOVE   'm'

#endif
