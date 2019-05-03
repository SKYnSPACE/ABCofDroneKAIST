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
    ///////////////////채워넣기///////////////////

    /////////////////////////////////////////////
    delay(100);
  }

  // 가속도계, 자이로 값의 합의 평균을 구하는 과정
  // baseAcX, baseAcY, baseAcZ, baseGyX, baseGyY, baseGyZ
  // 읽은 데이터는 총 10개
  // Hint 나누기 연산자는 "/"
  ///////////////////채워넣기///////////////////

  /////////////////////////////////////////////
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
  ///////////////////채워넣기///////////////////

  /////////////////////////////////////////////
}
