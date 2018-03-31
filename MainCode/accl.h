//Please note: The base of the code for the MPU6050 is from the work of Jeff Rowberg (https://github.com/jrowberg/i2cdevlib)

#include "Arduino.h"
#ifndef accl_h
#define accl_h

#include "I2Cdev.h"
#include "util.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

float accel[3], avg = 0;
int ind = 0;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
float retAvg() {
  return norm(accel[0], accel[1], accel[2]);
}
float getAvg(float newVal) {
  if(++ind > 2)
    ind = 0;
  if(newVal > avg * 1.5)
    accel[ind] = newVal + (newVal - avg) * .75;
  else
    accel[ind] = newVal;

  return avg = retAvg();
}


void setupAccl() {
	// join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
	
	// initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
	
	// load and configure the DMP
    devStatus = -1;
    while(devStatus != 0) {
      delay(1500);
      Serial.println(F("Initializing DMP..."));
      devStatus = mpu.dmpInitialize();
    }
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(91);
    mpu.setYGyroOffset(12);
    mpu.setZGyroOffset(33);
    
    // Accelerometer offsets
    /*
    mpu.setXAccelOffset(-444);
    mpu.setYAccelOffset(-1620);
    mpu.setZAccelOffset(1700); 
    */
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(1600); 
    
  
	// make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        attachInterrupt(INTERRUPT_PIN, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}
void pullData() {
	// if programming failed, don't try to do anything
  if (!dmpReady) return;
  if(!mpuInterrupt) return;

  //Reset intrrupt, get current status, and queue size
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();

  //Check for queue overflow
  if((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println("FIFO OVERFLOW!");
    return;
  }
  //Check for data ready
  if(!(mpuIntStatus & 0x02)) return;
  
  //Wait for full packet to be availible
  while(fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
  mpu.getFIFOBytes(fifoBuffer, packetSize);
  
  //NOTE FOR LATER: add something to handle if there are multiple packets availible, and look at modifying packet rate
  

	mpu.dmpGetQuaternion(&q, fifoBuffer);
	mpu.dmpGetAccel(&aa, fifoBuffer);
	mpu.dmpGetGravity(&gravity, &q);
	mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
	mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}

#endif
