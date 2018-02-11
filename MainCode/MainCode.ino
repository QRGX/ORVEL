#include "accl.h"
#include "tmp.h"

void setup() {
  Serial.begin(115200);
  setupTemp();
  setupAccl();
  Serial.write('k');
}

long start;

void loop() {
  slowLoop();
}

void slowLoop() { //Measured at 4.67ms
  if(!mpuInterrupt && fifoCount < packetSize)
    return;
  
  start = micros();  
  pullData(); //Collection time measured at ~65 microseconds = 0.065 ms
  updateValues(); //Collection time measured at ~1350 microseconds = 1.35ms
  //Serial.write('p');
  Serial.println(micros() - start);
}

