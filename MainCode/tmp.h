#ifndef tmp_h
#define tmp_h

#include "Arduino.h"
#include "stdint.h"
#include "SparkFunBME280.h"
#include "Wire.h"

//Global sensor object
BME280 mySensor;

//Variables
float tempC, pressure, altM, humid;

void setupTemp() {
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;

  //***Operation settings*****************************//

  //renMode can be:
  //  0, Sleep mode
  //  1 or 2, Forced mode
  //  3, Normal mode
  mySensor.settings.runMode = 3; //Normal mode

  //tStandby can be:
  //  0, 0.5ms
  //  1, 62.5ms
  //  2, 125ms
  //  3, 250ms
  //  4, 500ms
  //  5, 1000ms
  //  6, 10ms
  //  7, 20ms
  mySensor.settings.tStandby = 6;

  //filter can be off or number of FIR coefficients to use:
  //  0, filter off
  //  1, coefficients = 2
  //  2, coefficients = 4
  //  3, coefficients = 8
  //  4, coefficients = 16
  mySensor.settings.filter = 0;

  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.tempOverSample = 0;

  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.pressOverSample = 0;

  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.humidOverSample = 0;

  DEBUG_PRNT("Program Started\n");
  DEBUG_PRNT("Starting BME280... result of .begin(): 0x");

  //Calling .begin() causes the settings to be loaded
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  DEBUG_PRNTLN_C(mySensor.begin(), HEX);

  DEBUG_PRNT("Displaying ID, reset and ctrl regs\n");

  DEBUG_PRNT("ID(0xD0): 0x");
  DEBUG_PRNTLN_C(mySensor.readRegister(BME280_CHIP_ID_REG), HEX);
  DEBUG_PRNT("Reset register(0xE0): 0x");
  DEBUG_PRNTLN_C(mySensor.readRegister(BME280_RST_REG), HEX);
  DEBUG_PRNT("ctrl_meas(0xF4): 0x");
  DEBUG_PRNTLN_C(mySensor.readRegister(BME280_CTRL_MEAS_REG), HEX);
  DEBUG_PRNT("ctrl_hum(0xF2): 0x");
  DEBUG_PRNTLN_C(mySensor.readRegister(BME280_CTRL_HUMIDITY_REG), HEX);

  DEBUG_PRNTLN("Temperature/Humidity setup done.");
}

void updateValues() {
  tempC = mySensor.readTempC();
  pressure = mySensor.readFloatPressure();
  //altM = mySensor.readFloatAltitudeMeters();
  //humid = mySensor.readFloatHumidity();
}

#endif
