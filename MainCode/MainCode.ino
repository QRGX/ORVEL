#include "accl.h"
#include "tmp.h"
#include "util.h"
#include "servo.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(1000);

  setupAccl();
  setupTemp();
  setAccel(ACCEL_VAL);
  setupServo();
  
  Serial.print("Initial sensitivity: ");
}
long start, accelTime, tempTime;
short loopCount;

void loop() {
  loopCount = -1;
  //while(++loopCount < 15)
  mediumLoop();
  slowLoop();
}

void mediumLoop() {
  //start = micros();
  pullData(); //Collection time measured at ~1790 us = 1.79 ms
  //accelTime = micros() - start;
  //start = micros();
  updateValues(); //Collection time measured at ~1372 us = 1.372 ms
  //tempTime = micros() - start;
}

void slowLoop() { //Measured at 4.67ms
  //writeTempVals();
  //writeCollectionTime();
  //testServo();
  writeGNet();
  //writeGyroVals();
  //writeAccelVals();
}

void writeGyroVals() {
  Serial.print("(");
  Serial.print(ypr[2]); //X
  Serial.print(", ");
  Serial.print(ypr[1]); //Y
  Serial.print(", ");
  Serial.print(ypr[0]); //Z
  Serial.println(")");
}

void writeTempVals() {
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print("C, Humid: ");
  Serial.print(humid);
  Serial.print("RH, Press: ");
  Serial.print(pressure / 1000.0);
  Serial.println("KPa.");
}

void writeNormAccelVals() {
  Serial.print("(");
  Serial.print(aaReal.x); //X
  Serial.print(", ");
  Serial.print(aaReal.y); //Y
  Serial.print(", ");
  Serial.print(aaReal.z); //Z
  Serial.println(")");
}

void writeAccelVals() {
  Serial.print("(");
  Serial.print(aa.x); //X
  Serial.print(", ");
  Serial.print(aa.y); //Y
  Serial.print(", ");
  Serial.print(aa.z); //Z
  Serial.println(")");
}

void writeGForce() {
  float x = aa.x / ACCEL_DIV[ACCEL_VAL], y = aa.y / ACCEL_DIV[ACCEL_VAL], z = aa.z / ACCEL_DIV[ACCEL_VAL];
  Serial.print("(");
  Serial.print(x); //X
  Serial.print(", ");
  Serial.print(y); //Y
  Serial.print(", ");
  Serial.print(z); //Z
  Serial.println(")");
}
void writeGNet() {
  Serial.println(norm(aa.x, aa.y, aa.z) / ACCEL_DIV[ACCEL_VAL] - .17);
}

void writeCollectionTime() {
  Serial.print("Temp time: ");
  Serial.print(tempTime);
  Serial.print("us , Accel time: ");
  Serial.print(accelTime);
  Serial.println(" us");
}

int setAccel(int val) {
  if (val < 0 || val > 3) return 0;
  ACCEL_VAL = val;
  mpu.setFullScaleAccelRange(val);
}

