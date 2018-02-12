#include "accl.h"
#include "tmp.h"
#include "ultra.h"

void setup() {
  Serial.begin(115200);
  setupTemp();
  setupAccl();
  setupUlt();
  Serial.write('k');
}

long start;

void loop() {
  slowLoop();
  delayMicroseconds(100);
}

void slowLoop() { //Measured at 4.67ms
  if(!mpuInterrupt && fifoCount < packetSize)
    return;
  
  start = micros();  
  pullData(); //Collection time measured at ~67 us = 0.067 ms
  updateValues(); //Collection time measured at ~4605 us = 4.60 ms
  getDist(); //Collection time measured at <5000 us = 5 ms
  //Serial.write('p');
  //Serial.println(micros() - start);
  Serial.write(dist);
  Serial.write(0xD);
}

