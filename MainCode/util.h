#include "Arduino.h"
#ifndef util_h
#define util_h

#include<math.h>
//#include "accl.h"
//#include "MPU6050_6Axis_MotionApps20.h"

const float ACCEL_DIV[4] = {16384.0, 8192.0, 4096.0, 2048.0};
int ACCEL_VAL = 1;

float norm(float x, float y, float z) {
  return abs(x) + abs(y) + abs(z);
}


#endif
