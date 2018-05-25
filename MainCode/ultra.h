#ifndef ultra_h
#define ultra_h

#include "Arduino.h"

#define TRIG 9
#define ECHO 10
#define TIME_OUT 5000

float dist;
float prev[10];
int mk = 0;
long ultVal;

void ping();
void getDist();
float minV();
void incMk();

void setupUlt() {
	  pinMode(TRIG, OUTPUT);
	  pinMode(ECHO, INPUT);
    getDist();
    if(dist != 0)
      Serial.println("Ultrasonic ready");
    else
      Serial.println("Ultrasonic error");
}

void getDist() {
	unsigned long dur;
	ping();
	dur = pulseIn(ECHO, HIGH, TIME_OUT);
  prev[mk] = (double) dur * 0.0343 / 2;
	dist = minV();
	ultVal = millis();
}

void ping() {
	digitalWrite(TRIG, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);
}

float minV() {
  float mm = prev[0];
  for(int i=1;i<10;i++) {
    if(prev[i] < mm)
      mm = prev[i];
  }
  incMk();
  return mm;
}

void incMk() {
  if(++mk > 9)
    mk = 0;
}

#endif
