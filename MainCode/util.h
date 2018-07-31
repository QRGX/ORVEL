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
#define OVERRIDE     'G'
#define SAFETY_STOP  'K'
#define SERVO_MOVE   'm'
#define SERVO_UP     'u'
#define SERVO_DOWN   'd'
#define DEVICE_POWER  3
#define WAIT_TIME    800

bool devicesOn = false;
void restartDevices();

void setupDevices() {
  pinMode(DEVICE_POWER, OUTPUT);
  restartDevices();
}

void restartDevices() {
  DEBUG_PRNTLN("Restarting external devices");
  digitalWrite(DEVICE_POWER, LOW);
  delay(WAIT_TIME);
  digitalWrite(DEVICE_POWER, HIGH);
  delay(WAIT_TIME);
}

#endif
