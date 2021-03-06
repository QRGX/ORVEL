#include "util.h"
#include "accl.h"
#include "tmp.h"
#include "servo.h"

long startTime, accelTime, tempTime, loopTime, loopStart;
char lastByte;
short loopCount;
bool kill_override = false;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  DEBUG_PRNTLN("Starting setup");

  setupDevices();
  setupServo();
  setupAccl();
  setupTemp();
  DEBUG_PRNTLN("Setup complete.");
}

void loop() {
  loopStart = micros();
  loopCount = -1;
  while(++loopCount < 15)
    mediumLoop();
  slowLoop();
  loopTime = micros() - loopStart;
}

void mediumLoop() {
  //startTime = micros();
  pullData(); //Collection time measured at ~1790 us = 1.79 ms
  //accelTime = micros() - start;
  //startTime = micros();
  updateValues(); //Collection time measured at ~1372 us = 1.372 ms
  //tempTime = micros() - start;
}

void slowLoop() { //Measured at 4.67ms
  checkIncoming();
  safetyCheck();
  #ifndef DEBUG_MODE
    writeDataToLaptop();
  #endif
  
  //Test print options
  //testServo();
  //writeGNet();
  //writeGyroVals();
  //writeAccelVals();
  //writeTempVals();
  //writeCollectionTime();
}

void safetyCheck() {
  if(kill_override) return;
  
  float acceleration = norm(aa.x, aa.y, aa.z) / ACCEL_DIV[ACCEL_VAL] - .17;
  float _pressure = pressure / 1000.0;

  //Accel check
  if(acceleration > 2)
    Serial.println("WARNING: acceleration has exceeded 2g");

  //Air pressure check
  if(_pressure < 95) {
    Serial.println("WARNING: pressure below 95 kpa, entering kill loop.");
    killLoop();
  }
}

void killLoop() {
  while(true && !kill_override) {
    Serial.println("Experiment killed. Parameters outside safe range. (G to override)");
    delay(1000);
    if(Serial.available() > 0) {
      char letter = Serial.read();
      
      if(letter == OVERRIDE) {
        Serial.println("Overriding failsafe, breaking safety loop");
        kill_override = true;
        return;
      } else
        Serial.println("Character not recognised, enter G to override");
    }
  }
}

void writeDataToLaptop() {
  Serial.write(norm(aa.x, aa.y, aa.z) / ACCEL_DIV[ACCEL_VAL] - .17);  //Give Acceleration in Gs
  Serial.write(tempC);                                                //Give Temperature in C
  Serial.write(pressure);                                             //Give Pressure in Pa
  Serial.write(loopTime);                                             //Give last loop time in us
}

//NOTE: To kill experiment enter K
void checkIncoming() {
  if(Serial.available() <= 0) return;
  char currByte = Serial.read();
  //if(lastByte == PACKET_START) {
    switch(currByte)  {
      case SERVO_MOVE:
        testServo();
        DEBUG_PRINTLN("Testing Servo");
        break;
      case SERVO_UP:
        up();
        break;
      case SERVO_DOWN:
        down();
        break;
      case SAFETY_STOP:
        killLoop();
        break;
    }
    Serial.print(currByte);
    Serial.println("received, action taken");
  //}
  lastByte = currByte;
}

//------------------------------------------------
//Test print functions
//------------------------------------------------

void writeGyroVals() {
  DEBUG_PRNT("(");
  DEBUG_PRNT(ypr[2]); //X
  DEBUG_PRNT(", ");
  DEBUG_PRNT(ypr[1]); //Y
  DEBUG_PRNT(", ");
  DEBUG_PRNT(ypr[0]); //Z
  DEBUG_PRNTLN(")");
}

void writeTempVals() {
  DEBUG_PRNT("Temp: ");
  DEBUG_PRNT(tempC);
  DEBUG_PRNT("C, Humid: ");
  DEBUG_PRNT(humid);
  DEBUG_PRNT("RH, Press: ");
  DEBUG_PRNT(pressure / 1000.0);
  DEBUG_PRNTLN("KPa.");
}

void writeNormAccelVals() {
  DEBUG_PRNT("(");
  DEBUG_PRNT(aaReal.x); //X
  DEBUG_PRNT(", ");
  DEBUG_PRNT(aaReal.y); //Y
  DEBUG_PRNT(", ");
  DEBUG_PRNT(aaReal.z); //Z
  DEBUG_PRNTLN(")");
}

void writeAccelVals() {
  DEBUG_PRNT("(");
  DEBUG_PRNT(aa.x); //X
  DEBUG_PRNT(", ");
  DEBUG_PRNT(aa.y); //Y
  DEBUG_PRNT(", ");
  DEBUG_PRNT(aa.z); //Z
  DEBUG_PRNTLN(")");
}

void writeGForce() {
  float x = aa.x / ACCEL_DIV[ACCEL_VAL], y = aa.y / ACCEL_DIV[ACCEL_VAL], z = aa.z / ACCEL_DIV[ACCEL_VAL];
  DEBUG_PRNT("(");
  DEBUG_PRNT(x); //X
  DEBUG_PRNT(", ");
  DEBUG_PRNT(y); //Y
  DEBUG_PRNT(", ");
  DEBUG_PRNT(z); //Z
  DEBUG_PRNTLN(")");
}
void writeGNet() {
  DEBUG_PRNTLN(norm(aa.x, aa.y, aa.z) / ACCEL_DIV[ACCEL_VAL] - .17);
}

void writeCollectionTime() {
  DEBUG_PRNT("Temp time: ");
  DEBUG_PRNT(tempTime);
  DEBUG_PRNT("us , Accel time: ");
  DEBUG_PRNT(accelTime);
  DEBUG_PRNTLN(" us");
}
