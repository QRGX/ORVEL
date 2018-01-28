#include "accl.h"

void setup() {
  Serial.begin(115200);
  setupAccl();
  Serial.write('k');
  delay(2000);
}

long start;

void loop() {
  slowLoop();
}

void slowLoop() {
  if(!mpuInterrupt && fifoCount < packetSize)
    return;
  
  start = micros();  
  pullData(); //Collection time measured at ~65 microseconds
  Serial.write('p');
  Serial.write(micros() - start);
}

