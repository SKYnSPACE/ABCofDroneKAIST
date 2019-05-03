#include <Wire.h>

//MPU 기본 데이터를 받기위한 변수 선언
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup() {
  initMPU6050();
  Serial.begin(9600);
}

void loop() {
  readAccelGyro();

  static int cnt;
  cnt++
  ic(cnt%2==0)
    SendDataToProcessing();
}

//MPU 초기화 루틴
void initMPU6050()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

// 가속소 자이로 센서의 raw data를 읽는 루틴
void readAccelGyro()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read() << 8 | Wire.read();
  AcY=Wire.read() << 8 | Wire.read();
  AcZ=Wire.read() << 8 | Wire.read();
  Tmp=Wire.read() << 8 | Wire.read();
  GyX=Wire.read() << 8 | Wire.read();
  GyY=Wire.read() << 8 | Wire.read();
  GyZ=Wire.read() << 8 | Wire.read();
}

// 변수명 선언
float dt;
float accel_angle_x, accel_angle_y, accel_angle_z;
float gyro_angle_x, gyro_angle_y, gyro_angle_z;
float filtered_angle_x, filtered_angle_y, filtered_angle_z;
float baseAcX, baseAcY, baseAcZ;
float baseGyX, baseGyY, baseGyZ;

// Processing으로 데이터를 보내는 루틴
void SendDataToProcessing()
{
  Serial.print(F("DEL:"));
  Serial.print(dt,DEC);
  Serial.print(F("#ACC:"));
  Serial.print(accel_angle_x,2);
  Serial.print(F(","));
  Serial.print(accel_angle_y,2);
  Serial.print(F(","));
  Serial.print(accel_angle_z,2);
  Serial.print(F("#GYR:"));
  Serial.print(gyro_angle_x,2);
  Serial.print(F(","));
  Serial.print(gyro_angle_y,2);
  Serial.print(F(","));
  Serial.print(gyro_angle_z,2);
  Serial.print(F("#FIL:"));
  Serial.print(filtered_angle_x,2);
  Serial.print(F(","));
  Serial.print(filtered_angle_y,2);
  Serial.print(F(","));
  Serial.print(filtered_angle_z,2);
  Serial.println(F(""));
  delay(5);
}
