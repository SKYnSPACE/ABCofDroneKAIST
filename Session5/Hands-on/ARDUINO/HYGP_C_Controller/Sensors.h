#ifndef SENSORS_H
#define SENSORS_H

#include "DataStructure.h"

#include "Macros.h"

#define GYRO_MODEL MPU6050

extern struct _Sensor Sensor;
extern struct _Flags Flags;
extern struct PCONCAT_(,GYRO_MODEL) GYRO_MODEL;

extern char str[80];

void MPU6050_Init();
unsigned char MPU6050_IDCheck();
void MPU6050_ReadData();
void MPU6050_CalibGyro();
void MPU6050_TransferData();
void MPU6050_LPF();

struct _MPU6050
{
  int address; // Arduino I2C HAL takes int, not unsigned char.
  int whoAmIREG;
  int whoAmIValue;

  float accScale0;
  float accScale1;
  float accScale2;
  float gyroScale0;
  float gyroScale1;
  float gyroScale2;
  
  short accXInput, accYInput, accZInput;
  float accX, accY, accZ;
  
  short pInput, qInput, rInput; // -32768~32767
  float pAccum, qAccum, rAccum; // [DEG/s]
  int calibDataCount;
  float pBias, qBias, rBias; // [DEG/s]

  short temperatureInput;
};

#endif
