#include <Wire.h> // I2C 통신을 위한 라이브러리 include

const int MPU_addr=0x68; // MPU6050의 I2C 기본주소
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; // Ac : 가속도 / Gy : 자이로


void setup() {
  // put your setup code here, to run once:
  Wire.begin();   // Wire 라이브러리 초기화
  Wire.beginTransmission(MPU_addr); // MPU_6050 I2C 주소와의 통신 시작
  Wire.write(0x6B);   // PWR_MGMT_1 resgister
  Wire.write(0);      // MPU_6050 시작 모드로 설정
  Wire.endTransmission(true); // MPU 데이터 통신 종료
  Serial.begin(9600);  // 시리얼 통신 속도 설정

}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(MPU_addr);   // 데이터 전송 시작
  Wire.write(0x3B);     // register 0x3B 시작 ( ACCEL_XOUT_H ), 큐에 데이터 기록
  Wire.endTransmission(false);  // 연결 유지
  Wire.requestFrom(MPU_addr,14,true);  // MPU에게 데이터 요청
  // 데이터 한 바이트씩 읽어서 반환하도록 하는 과정
  AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // 시리얼 모니터에 값 출력
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcY);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(333);
}
