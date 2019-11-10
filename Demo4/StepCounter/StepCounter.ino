#include <Wire.h>
#include <SPI.h>
#include "SparkFunLSM6DS3.h"
#include "RF24.h"
#include "BTLE.h"

LSM6DS3Core myIMU(I2C_MODE, 0x6B);

RF24 radio(9, 10);
BTLE btle(&radio);

// Data packet for BTLE advertising
struct step_data {
  uint16_t service_uuid;
  uint16_t value;
};


void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Processor came out of reset.\n");

  // Call .beginCore() to configure the IMU
  if (myIMU.beginCore() != 0) {
    Serial.print("Error at beginCore().\n");
  } else {
    Serial.print("\nbeginCore() passed.\n");
  }

  // Error accumulation variable
  uint8_t errorAccumulator = 0;

  uint8_t dataToWrite = 0;  // temporary variable

  // Setup the accelerometer******************************
  dataToWrite = 0; // Start Fresh!
  //  dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_200Hz;
  dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
  dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_26Hz;

  // Now, write the patched together data
  errorAccumulator += myIMU.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);

  // Set the ODR bit
  errorAccumulator += myIMU.readRegister(&dataToWrite, LSM6DS3_ACC_GYRO_CTRL4_C);
  dataToWrite &= ~((uint8_t)LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED);


  // Enable embedded functions -- ALSO clears the pdeo step count
  errorAccumulator += myIMU.writeRegister(LSM6DS3_ACC_GYRO_CTRL10_C, 0x3E);
  // Enable pedometer algorithm
  errorAccumulator += myIMU.writeRegister(LSM6DS3_ACC_GYRO_TAP_CFG1, 0x40);
  // Step Detector interrupt driven to INT1 pin
  errorAccumulator += myIMU.writeRegister( LSM6DS3_ACC_GYRO_INT1_CTRL, 0x10 );

  if (errorAccumulator) {
    Serial.println("Problem configuring the device.");
  } else {
    Serial.println("Device O.K.");
  }
  delay(200);

  // Set up for Bluetooth Low Energy
  while (!Serial) { }
  Serial.println("BTLE Step Count Sender");

  btle.begin("StpCount");  // 8 chars max
}


void loop() {
  uint8_t readDataByte = 0;
  uint16_t stepsTaken = 0;
  // Read the 16bit value by two 8bit operations
  myIMU.readRegister(&readDataByte, LSM6DS3_ACC_GYRO_STEP_COUNTER_H);
  stepsTaken = ((uint16_t)readDataByte) << 8;

  myIMU.readRegister(&readDataByte, LSM6DS3_ACC_GYRO_STEP_COUNTER_L);
  stepsTaken |= readDataByte;

  // Display steps taken
  Serial.print("Steps taken: ");
  Serial.println(stepsTaken);

  // Packet the data
  step_data stepCount;
  stepCount.service_uuid = NRF_DEVICE_INFORMATION_SERVICE_UUID;
  stepCount.value = stepsTaken;

  // BTLE Advertising
  btle.preparePacket();
  if(!btle.addChunk(0x16, sizeof(stepCount), &stepCount)) {
    Serial.println("BTLE advertisement failure");
    Serial.end();
  }
  btle.transmitPacket();
  btle.hopChannel();

  delay(1000);
}
