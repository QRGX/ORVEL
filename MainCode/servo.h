//NOTE: This code is based on the Adafruit example code for this module

#ifndef servo_h
#define servo_h

#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"

Adafruit_PWMServoDriver controller;
void mv(int dist);

#define SERVO_MIN 150
#define SERVO_MAX 600

#define PIN 5
#define MIN 1000
#define MAX 2000
#define NEUTRAL 1995
#define INCREMENT 5

long currAngle = 1500, lastTime = 0;
int pos;

void setupServo() {
  DEBUG_PRNTLN("Setting up servo");
  
  controller = Adafruit_PWMServoDriver();
  controller.begin();
  controller.setPWMFreq(60);
  controller.setPWM(0, 0, currAngle);
}

void writeToServo(short servoN, long us) {
  DEBUG_PRNT("Servo to: ");
  DEBUG_PRNTLN(us);
  
  double pulseLength = 1000000; // 1 second, or 1MM 1us per second
  pulseLength /= 60 * 4096;   // 60Hz rate and 12-bit converter
  pulseLength = us / pulseLength;
  //Make sure it doesn't exceed operating bounds
  if(pulseLength < SERVO_MIN) pulseLength = SERVO_MIN;
  else if(pulseLength > SERVO_MAX) pulseLength = SERVO_MAX;
  controller.setPWM(servoN, 0, pulseLength);
}

void testServo() {
  if(millis() - lastTime < 1500) return;
  lastTime = millis();
  currAngle = (currAngle == 2350)? 625 : 2350;
  writeToServo(0, currAngle);
}

void up() {
  mv(INCREMENT);
}
void down() {
  mv(-INCREMENT);
}

void mv(int dist) {
  if(currAngle + dist > MIN && currAngle + dist < MAX)
    currAngle += dist;
  writeToServo(0, currAngle);
  Serial.print("Position: ");
  Serial.println(pos);
}

#endif
