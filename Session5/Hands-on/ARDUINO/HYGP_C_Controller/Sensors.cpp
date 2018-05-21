#include <Arduino.h>
#include <Wire.h>

#include "Sensors.h"

//! MPU6050 센서관련 설정
void MPU6050_Init()
{
  GYRO_MODEL.address = 0x68; // 6050 자이로센서의 통신주소
  GYRO_MODEL.whoAmIREG = 0x75; // 6050 자이로센서의 Who am I register 위치
  GYRO_MODEL.whoAmIValue = 0x68; // 6050 자이로센서의 Who am I value

  GYRO_MODEL.accScale0 = 16384.0f;
  GYRO_MODEL.accScale1 = 8192.0f;
  GYRO_MODEL.accScale2 = 4096.0f; //v
  GYRO_MODEL.gyroScale0 = 131.0f;
  GYRO_MODEL.gyroScale1 = 65.5f;  //v
  GYRO_MODEL.gyroScale2 = 32.8f;
  
  //센서 작동시작
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x6B); // 레지스터 PWR_MGMT_1에 접근
  Wire.write(0x00); // 값 변경
  Wire.endTransmission();
  
  //자이로 설정 변경
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x1B); // GYRO_CONFIG
  Wire.write(0x08); // Full scale range를 +/- 500도/초 로 변경
  Wire.endTransmission();
  //가속도계 설정 변경
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x1C); // ACCEL_CONFIG
  Wire.write(0x10); // Full scale range를 +/- 8g로 변경 
  Wire.endTransmission();
  
  //내장 Digital LPF 설정
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x1A); // CONFIG
  Wire.write(0x03); // 대역폭: 가속도계-44Hz(4.9ms delay), 자이로-42Hz(4.8ms delay)
  Wire.endTransmission();
  
}

unsigned char MPU6050_IDCheck()
{
  unsigned long timeToWait;
  
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(GYRO_MODEL.whoAmIREG);
  Wire.endTransmission();
  Wire.requestFrom(GYRO_MODEL.address, 1);

  // 최대 100ms 대기
  timeToWait = millis() + 100;
  while(Wire.available() < 1 && timeToWait > millis()){}
  
  return Wire.read();
}

void MPU6050_ReadData()
{
  unsigned long timeToWait;
  
  Flags.sensorReadTimeout=0;
  
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x3B); // 0x3B 부터 읽기 접근
  Wire.endTransmission();
  Wire.requestFrom(GYRO_MODEL.address,14); //14 바이트 추출

  // 최대 10ms 대기
  timeToWait = millis() + 10;
  while(Wire.available() < 14 && timeToWait > millis()){} // (10ms > 대기시간) 이면서 데이터가 모자라면 대기
  if(timeToWait < millis()) // (10ms < 대기시간) 으로 루프가 종료된 경우 비정상판정
  {
    Flags.sensorReadTimeout=1;
  }
  else
  {
    GYRO_MODEL.accXInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.accYInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.accZInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.temperatureInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.pInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.qInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.rInput = Wire.read()<<8|Wire.read();
  }
  
}

//TBD!
void MPU6050_CalibGyro()
{
  unsigned long timer;
  int duration = 10000;

  Flags.smallSizedBiasData = 0;

  GYRO_MODEL.calibDataCount = 0;
  GYRO_MODEL.pAccum = 0;
  GYRO_MODEL.qAccum = 0;
  GYRO_MODEL.rAccum = 0;
  
  timer = millis() + duration;
  while(timer > millis())
  {
    FUNCTION_(GYRO_MODEL, ReadData)();
    GYRO_MODEL.calibDataCount++;
    GYRO_MODEL.pAccum += ((float)GYRO_MODEL.pInput/GYRO_MODEL.gyroScale1);
    GYRO_MODEL.qAccum += ((float)GYRO_MODEL.qInput/GYRO_MODEL.gyroScale1);
    GYRO_MODEL.rAccum += ((float)GYRO_MODEL.rInput/GYRO_MODEL.gyroScale1);
    if((timer - millis()) < duration)
    {
      duration -= 200;
      Serial.print(F(">"));
    }
  }
  Serial.println(F(""));
  
  if (GYRO_MODEL.calibDataCount < 3000)
  {
    Flags.smallSizedBiasData = 1;
  }
  else
  {
    sprintf(str, "pAccum: %d, qAccum: %d, rAccum: %d, dataCount: %d", GYRO_MODEL.pAccum, GYRO_MODEL.qAccum, GYRO_MODEL.rAccum, GYRO_MODEL.calibDataCount);
    Serial.println(str);    
    GYRO_MODEL.pBias = GYRO_MODEL.pAccum/GYRO_MODEL.calibDataCount;
    GYRO_MODEL.qBias = GYRO_MODEL.qAccum/GYRO_MODEL.calibDataCount;
    GYRO_MODEL.rBias = GYRO_MODEL.rAccum/GYRO_MODEL.calibDataCount;
  }
   
}
//TBD! Data transfer to Sensor structure (형변환 및 보정치 반영)
void MPU6050_TransferData()
{
  Sensor.Gyro.p = ((float)GYRO_MODEL.pInput / GYRO_MODEL.gyroScale1) - GYRO_MODEL.pBias;
  Sensor.Gyro.q = ((float)GYRO_MODEL.qInput / GYRO_MODEL.gyroScale1) - GYRO_MODEL.qBias;
  Sensor.Gyro.q = -Sensor.Gyro.q; // Sensor Y axis reversed.
  Sensor.Gyro.r = ((float)GYRO_MODEL.rInput / GYRO_MODEL.gyroScale1) - GYRO_MODEL.rBias;
  Sensor.Gyro.r = -Sensor.Gyro.r; // Sensor Z axis reversed.

  Sensor.Acc.x = ((float)GYRO_MODEL.accXInput) / GYRO_MODEL.accScale2 * 9.81;
  Sensor.Acc.y = ((float)GYRO_MODEL.accYInput) / GYRO_MODEL.accScale2 * 9.81;
  Sensor.Acc.y = -Sensor.Acc.y; // Sensor Y axis reversed.
  Sensor.Acc.z = ((float)GYRO_MODEL.accZInput) / GYRO_MODEL.accScale2 * 9.81;
  Sensor.Acc.z = -Sensor.Acc.z; // Sensor Z axis reversed.

  Sensor.Acc.total = sqrt((Sensor.Acc.x*Sensor.Acc.x)+
                          (Sensor.Acc.y*Sensor.Acc.y)+
                          (Sensor.Acc.z*Sensor.Acc.z));
}

void MPU6050_LPF()
{
  Sensor.Gyro.pFiltered = 0.7* Sensor.Gyro.pFiltered + 0.3 * Sensor.Gyro.p;
  Sensor.Gyro.qFiltered = 0.7* Sensor.Gyro.qFiltered + 0.3 * Sensor.Gyro.q;
  Sensor.Gyro.rFiltered = 0.7* Sensor.Gyro.rFiltered + 0.3 * Sensor.Gyro.r;
}

