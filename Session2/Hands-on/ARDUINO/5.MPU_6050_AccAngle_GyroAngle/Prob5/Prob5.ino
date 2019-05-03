#include <Wire.h>

//MPU 기본 데이터를 받기위한 변수 선언
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup() {
  initMPU6050();
  Serial.begin(9600);
  calibAccelGyro();
  initDT();
}

void loop() {
  readAccelGyro();
  calcDT();
  calcAccelYPR();
  calcGyroYPR();

  static int cnt;
  cnt++;
  if(cnt%2==0)
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

// 센서들의 기본값들의 평균을 구하는 루틴
void calibAccelGyro()
{
  float sumAcX = 0, sumAcY = 0, sumAcZ = 0;
  float sumGyX = 0, sumGyY = 0, sumGyZ = 0;

  // 가속도 자이로 센서를 읽어드림
  readAccelGyro();

  //읽은 값을 토대로 평균값을 구하기
  // 우선 초기 10개의 데이터를 읽어 그 합을 구함
  for(int i=0; i<10; i++)
  {
    //readAccelGyro()로 데이터를 읽어 sumAcX, sumGyX 등에
    //초기 base 가속도, 자이로 값을 합하기
    // Hint A += a > A = A + a를 사용
    // 읽은 가속도 값 : AcX, AcY, AcZ
    // 읽은 자이로 값 : GyX, GyY, GyZ
    
    readAccelGyro();
    sumAcX += AcX; sumAcY += AcY; sumAcZ += AcZ;
    sumGyX += GyX; sumGyY += GyY; sumGyZ += GyZ;

    delay(100);
  }

  // 가속도계, 자이로 값의 합의 평균을 구하는 과정
  // baseAcX, baseAcY, baseAcZ, baseGyX, baseGyY, baseGyZ
  // 읽은 데이터는 총 10개
  // Hint 나누기 연산자는 "/"
  baseAcX = sumAcX / 10;
  baseAcY = sumAcY / 10;
  baseAcZ = sumAcZ / 10;

  baseGyX = sumGyX / 10;
  baseGyY = sumGyY / 10;
  baseGyZ = sumGyZ / 10;
}

// 시간 측정용 변수 선언
unsigned long t_now;
unsigned long t_prev;

//시간 초기화 루틴
void initDT()
{
  t_prev = millis();
}

// loop를 한 번 돌 때마다의 시간을 측정하는 루틴
void calcDT()
{
  // millis 함수를 사용
  // t_now와 t_prev를 사용하여 dt 구하기
  // 시간 단위는 ms이므로 sec 단위로 바꾸는 과정 필요 
  t_now=millis();
  dt=(t_now-t_prev)/1000.0;
  t_prev=t_now;
}

//가속도계 값을 이용한 자세를 구하는 루틴 
void calcAccelYPR()
{
  // 각 축에 대한 가속도 값 저장 변수 선언
  float accel_x, accel_y, accel_z;
  float accel_xz, accel_yz;
  //Radian to Degree 상수 선언
  const float R2D = 180 / 3.14159;

  // 각 축의 가속도값 보정
  accel_x=AcX-baseAcX;
  accel_y=AcY-baseAcY;
  accel_z=AcZ-(16384-baseAcZ);

  // accel_angle_x,y,z를 구하는 과정 ( Degree 단위 )
  // sqrt, pow, atan 함수를 사용하여 계산
  // sqrt(2) = 루트 2
  // pow(3,2) = 3^2
  // atan(1/1) = 45*pi/180 rad
  accel_yz = sqrt(pow(accel_y, 2)+pow(accel_z,2));
  accel_angle_y = atan(-accel_x / accel_yz)*R2D;

  accel_xz = sqrt(pow(accel_x,2)+pow(accel_z,2));
  accel_angle_x = atan(accel_y / accel_xz)*R2D;

  accel_angle_z = 0;
}

// 보정된 자이로 값을 저장하는 변수 선언
float gyro_x, gyro_y, gyro_z;

//자이로 값을 이용한 자세를 구하는 루틴
void calcGyroYPR()
{
  //자이로 값 보정 변수 선언
  const float GYROXYZ_TO_DEGREE_PER_SEC = 131; // 각속도 바꾸는 상수

  //자이로 값을 보정하는 단계
  gyro_x=(GyX-baseGyX) / GYROXYZ_TO_DEGREE_PER_SEC;
  gyro_y=(GyY-baseGyY) / GYROXYZ_TO_DEGREE_PER_SEC;
  gyro_z=(GyZ-baseGyZ) / GYROXYZ_TO_DEGREE_PER_SEC;

  //보정된 자이로 값을 기반으로 gyro_angle_x,y,z를 구해야함
  ///////////////////채워넣기///////////////////

  /////////////////////////////////////////////
}
