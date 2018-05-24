#ifndef servo_h
#define servo_h

#include "Arduino.h"
#include "DueTimer.h"
#include "DigitalRW_Direct.h"

#define SERVO_PIN 9
#define SLEEP 25
int nextPeriod, currAngle = 1500;
long lastMove = 0;
bool high = false;

void syringeInterrupt();

void setupServo() {
  pinMode(SERVO_PIN, OUTPUT);
  Timer0.attachInterrupt(syringeInterrupt);
  Timer0.setPeriod(SLEEP);
  Timer0.start();
  Serial.println("Servo initialized.");
}

void timerCheck() {
  int i;
  DueTimer myTimer = Timer.getAvailable();
  Serial.print("Timer list: ");
  for(i=0;i<9;i++)
    Serial.print(myTimer == DueTimer(i));
  Serial.println(" done timer list");
}

void testServo() {
  if(millis() - lastMove < 1500) return;
  lastMove = millis();
  currAngle = (currAngle == 1000)? 2000 : 1000;
}


void syringeInterrupt() {
  nextPeriod = high? SLEEP : currAngle;
  high = !high;
  digitalWriteDirect(SERVO_PIN, high);
  Timer0.setPeriod(nextPeriod).start();
}

















/*
#include<Servo.h>

#define SERVO_PIN 9
#define WAIT_TIME 1500
Servo syringeServo;
const int servoPos[5] = {1000, 1250, 1500, 1750, 2000};
int currPos = 0;
long moveTime = 0;

void setupServo() {
  syringeServo.attach(SERVO_PIN);
  Serial.println("Servo attached.");
}

void runServoTest() {
  delay(500);
  Serial.println("Moving to 0 degrees");
  syringeServo.writeMicroseconds(1000);
  delay(1000);
  Serial.println("Moving to 90 degrees");
  syringeServo.writeMicroseconds(1500);
  delay(1000);
  Serial.println("Moving to 180 degrees");
  syringeServo.writeMicroseconds(2000);
  delay(1000);
}

void moveServoToNext() {
  if(millis() - moveTime < WAIT_TIME) return;   //Don't move servo if its been less than 1.5 s
  moveTime = millis();
  
  if(++currPos > 4) currPos = 0;
  Serial.print("Moving servo to ");
  Serial.print(servoPos[currPos]);
  Serial.println(" ms");
  syringeServo.writeMicroseconds(servoPos[currPos]);
}
*/

#endif
