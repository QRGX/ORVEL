#include "accl.h"
#include "tmp.h"
#include "ultra.h"

void setup() {
  delay(1000);
  Serial.begin(115200);
  setupAccl();
  setupTemp();
  //setupUlt();
  Serial.write('k');
}
long start, accelTime;

void loop() {
  slowLoop();
}

void writeGyroVals() {
  Serial.print("(");
  Serial.print(ypr[2]); //X
  Serial.print(", ");
  Serial.print(ypr[1]); //Y
  Serial.print(", ");
  Serial.print(ypr[0]); //X
  Serial.println(")");
}

void writeTempVals() {
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print("C, Humid: ");
  Serial.print(humid);
  Serial.print("RH, Press: ");
  Serial.print(pressure);
  Serial.println("Pa.");
}

void slowLoop() { //Measured at 4.67ms
  start = micros();  
  pullData(); //Collection time measured at ~3615 us
  accelTime = micros();
  updateValues(); //Collection time measured at ~4605 us = 4.60 ms
  
  //if((accelTime = accelTime - start) > 500)
  //  Serial.println(accelTime);

  writeTempVals();
  //writeGyroVals();
}

