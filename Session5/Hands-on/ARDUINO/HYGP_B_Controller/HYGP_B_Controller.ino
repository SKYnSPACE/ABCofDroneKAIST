////////////////////////////////////////////////////////////////////////////////
// 2018 항공우주공학과 역량강화 프로젝트 코드B.
// 시스템 점검 이후 드론 비행전 업로드하는 코드입니다.

//!경고: 안전수칙//
// 시스템 조립 상태를 최종 점검한 후
// 모터에 장착된 "프로펠러를 모두 제거"하고 업로드 하십시오.

// 사용순서 //
// 1. 변수 초기화에 필요한 정보 제공. 점검코드에서 획득한 설정값을 입력 등.
// 0. 일부 초기화 변수들은 (추후 ROM에서 로딩하는 방식으로 변경)
// 0. TBD
// 0. TBD
// -----------------------------------------------------------------------------
// 0. 코드 업로드
// 0. 배터리 꼽고 초기화작업하는동안 드론 가만히 둘것!
// 0. 시동커맨드

// 로그 //
// 2018.APR.12. 초안작성: 이성헌 skynspace@kaist.ac.kr
//   본 프로그램은 프로젝트 코드 A를 통해 점검이 완료된 드론의 비행을 위한 제어코드를 담습니다.
//   코드 A에 대비하여 데이터구조체 일부에 변경이 있을 수 있습니다.
//
// 2018.AAA.00. 수정1: ㅇㅇㅇ
//   ㅇㅇㅇ 을 수정함

////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <avr/interrupt.h>

#include "Messages.h"
#include "DataStructure.h"
#include "SensorInspection.h"
#include "ReceiverTransmitter.h"
#include "PWMOut.h"
#include "Filters.h"
#include "Controller.h"

#include "Macros.h"

unsigned short keyInput;

struct _Flags Flags;
struct _Sensor Sensor;
struct _Motor Motor;
struct PCONCAT_(, GYRO_MODEL) GYRO_MODEL;
struct _Controller Controller;

const float systemFreq = 200;  // 시스템 동작 주파수 [Hz], const -> 초기화이후 변경 못하게
const int systemPeriod = 5000; // 시스템 동작 주기 (1/systemFreq) [us, microseconds]

char str[80];
volatile unsigned long systemTimer; // 시스템 동작 측정용 시계 PWMOutRaiser 에서 업데이트됨. 다른곳에서 사용금지! PWM 출력 시간 꼬이면 큰일남. volatile -> 최적화금지
volatile unsigned long setupTimer; // 시스템 동작 점검시 사용하는 타이머
volatile unsigned long stanbyTimer;
volatile unsigned long receiverTimer0; // PWM input 시간체크시 사용되는 reference timer
volatile unsigned long receiverTimer1, receiverTimer2, receiverTimer3, receiverTimer4; // 각 채널별 PWM input 시간 체크용
volatile unsigned long motorTimer0; // PWM output 신호 생성시 사용되는 reference timer
volatile unsigned long motorTimer1, motorTimer2, motorTimer3, motorTimer4; // 각 모터별 PWM output 시간 체크용

void ReadADC0();

void setup() {
  //![0] put your setup code here, to run once:
  pinMode(2, OUTPUT); //LED RED
  pinMode(12, OUTPUT); //LED YELLOW
  pinMode(13, OUTPUT); //LED GREEN

  //![1] EEPROM 초기화
// TBD


  //![2] 시리얼 통신 준비 @Baudrate 57600
  //Serial.begin(57600);
// 개발중 디버깅용으로 사용. 비행코드 업로드시 주석처리

  //![3] I2C통신 준비
  //참고: Two Wire Bit Rate Register, TWBR = ((cpu freq./twi freq.) - 16) / 2
#if F_CPU == 16000000L
  Flags.mainFrequencyCheck = 1;
#else
  Flags.mainFrequencyCheck = 0;
#endif
  Wire.begin(); //아두이노를 I2C 마스터로 설정
  TWBR = 12; //twi freq.를 400kHz로 증폭 (기본값 cpu freq. 16000000L, twi freq. 100000L)


  //![4] 수신기 입력핀 설정
  cli();
  PCICR |= (1 << PCIE0);    // B 포트 (PCINT0 - PCINT7, 8-13) 오픈
  PCMSK0 |= (1 << PCINT0);  // 인터럽트 설정 (PCINT0, 8)
  PCMSK0 |= (1 << PCINT1);  // 인터럽트 설정 (PCINT1, 9)
  PCMSK0 |= (1 << PCINT2);  // 인터럽트 설정 (PCINT2, 10)
  PCMSK0 |= (1 << PCINT3);  // 인터럽트 설정 (PCINT3, 11)
  sei();

  //![5] 변속기 출력핀 설정
  //DDRD |= B00111100; (0b00000000은 gcc에서만..ㅠㅠ)
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);


  //![6] 각종 변수 초기화
    Flags.flightMode = 0;
  
    Sensor.Battery.scaler = 0.014648; // [5V / 1024setps * 3], 3 = (R1+R2)/(R1)
    Sensor.Battery.batteryVoltage = 11.0f; //빠른 필터 수렴을 위해
    
    Motor.outputPWM1 = PWM_LOW;
    Motor.outputPWM2 = PWM_LOW;
    Motor.outputPWM3 = PWM_LOW;
    Motor.outputPWM4 = PWM_LOW;
    
    Sensor.Receiver.channel1Center = 1520;
    Sensor.Receiver.channel2Center = 1520;
    Sensor.Receiver.channel3Center = 1520;
    Sensor.Receiver.channel4Center = 1520;
    Sensor.Receiver.channel1Min = 1100;
    Sensor.Receiver.channel2Min = 1940;
    Sensor.Receiver.channel3Min = 1100;
    Sensor.Receiver.channel4Min = 1100;
    Sensor.Receiver.channel1Max = 1940;
    Sensor.Receiver.channel2Max = 1100;
    Sensor.Receiver.channel3Max = 1940;
    Sensor.Receiver.channel4Max = 1940;

    Sensor.Receiver.deadBand = 8;

    Controller.pRateGain[0] = 1.1; //P Roll
    Controller.pRateGain[1] = 0.02; //I
    Controller.pRateGain[2] = 1; //D
    Controller.qRateGain[0] = 1.1; //P Pitch
    Controller.qRateGain[1] = 0.02; //I
    Controller.qRateGain[2] = 1; //D
    Controller.rRateGain[0] = 4.5; //P Yaw
    Controller.rRateGain[1] = 0.05; //I
    Controller.rRateGain[2] = 0; //D
//
  //![7] 메인 실행전 안전점검사항 (저전압체크, 송수신기 연결, 스로틀 스틱) 불량시 무한루프
  
  while((Sensor.Battery.batteryVoltage<11.1) || 
        (Sensor.Receiver.channel1Input <= 0) ||
        (Sensor.Receiver.channel1Input > 2000) ||
        (Sensor.Receiver.channel3Input > Sensor.Receiver.channel3Min))
  {
    ReadADC0(); // 필터 수렴하여 실제 전압 도달까지 루프내에서 반복
    /*
Serial.print(Sensor.Battery.batteryVoltage);
Serial.print(F(" "));
Serial.print(Sensor.Receiver.channel1Input);
Serial.print(F(" "));
Serial.println(Sensor.Receiver.channel3Input);
*/
    delay(100);
    digitalWrite(2,HIGH);
//TBD: 적색 LED 점등하여 불량상황을 알림. digitalWrite(2,HIGH);
  }
//TBD: 적색 LED 소등. digitalWrite(#,LOW);
    digitalWrite(2,LOW);


  
  //![8] 자이로 설정 및 보정
  digitalWrite(12,HIGH);
  FUNCTION_(GYRO_MODEL, Init)();
  delay(5000);
  FUNCTION_(GYRO_MODEL, CalibGyro)();
//TBD: Calib에서 오류 발생시 적색 LED 다시 점등
//TBD: 정상인 경우에는 황색 LED 소등.
  digitalWrite(12,LOW);


  //![9] 1,2,3,4번 모터 순으로 프로펠러 일시 가동.
  digitalWrite(12,HIGH);
  setupTimer = millis() + 3000;
  while(setupTimer > millis())
  {
    PWMOutRaiser();
    PWMOutFaller();
  }
  
  Motor.outputPWM1 = 1150;
  setupTimer = millis() + 500;
  while(setupTimer > millis())
  {
    PWMOutRaiser();
    PWMOutFaller();
  }
  Motor.outputPWM1 = PWM_LOW;
  Motor.outputPWM2 = 1150;
  setupTimer = millis() + 500;
  while(setupTimer > millis())
  {
    PWMOutRaiser();
    PWMOutFaller();
  }
  Motor.outputPWM2 = PWM_LOW;
  Motor.outputPWM3 = 1150;
  setupTimer = millis() + 500;
  while(setupTimer > millis())
  {
    PWMOutRaiser();
    PWMOutFaller();
  }
  Motor.outputPWM3 = PWM_LOW;
  Motor.outputPWM4 = 1150;
  setupTimer = millis() + 500;
  while(setupTimer > millis())
  {
    PWMOutRaiser();
    PWMOutFaller();
  }
  Motor.outputPWM4 = PWM_LOW;
  setupTimer = millis() + 3000;
  while(setupTimer > millis())
  {
    PWMOutRaiser();
    PWMOutFaller();
  }
  digitalWrite(12,LOW);

//TBD: LED는 녹색으로 변경 -> 메인 동작중 황색 (11.4v) -> 적색(10.8v) voltage indicator로 활용 
  digitalWrite(13,HIGH);
  //![#] 설정 완료후 환영 메시지 송출.. -> 메시지는 모두 LED 또는 부저로 대체
  //MessageWelcome();
  //delay(1000);
  //MessageMenu();
  
//UNCOMMENT TO TEST SETUP ONLY
  //while(1);

  //![#] 
  


  FUNCTION_(GYRO_MODEL, ReadData)();
  FUNCTION_(GYRO_MODEL, TransferData)();
  FUNCTION_(GYRO_MODEL, LPF)();
  EulerEstimator();
  Sensor.Estimates.roll = Sensor.Estimates.rollFromAcc; // 초기값은 무조건 가속도 센서로부터 얻는게 정확함
  Sensor.Estimates.pitch = Sensor.Estimates.pitchFromAcc; // 초기값은 무조건 가속도 센서로부터 얻는게 정확함
    
}

void loop() 
{
//  Serial.print(F(">>>>>>[안내] gR = "));
//  Serial.print(Sensor.Estimates.rollFromGyro);
//  Serial.print("\t");
/*
  Serial.print(F("gP = "));
  Serial.println(Sensor.Estimates.pitchFromGyro);
  */
//  Serial.print("\t");
//  Serial.print(F("aR = "));
//  Serial.print(Sensor.Estimates.rollFromAcc);
//  Serial.print("\t");
//  Serial.print(F("aP = "));
//  Serial.print(Sensor.Estimates.pitchFromAcc);
//  Serial.print("\t");
//  Serial.print(F("R = "));
//  Serial.print(Sensor.Estimates.roll);
//  Serial.print(F("P = "));
//  Serial.println(Sensor.Estimates.pitch);

  FUNCTION_(GYRO_MODEL, ReadData)();
  FUNCTION_(GYRO_MODEL, TransferData)();
  FUNCTION_(GYRO_MODEL, LPF)();
  EulerEstimator();
  StickNormalize();

// TBD: Switch-Case 로 대체
  if(Flags.flightMode < 0)
  {
    Flags.flightMode++;
  }
  
  if((Flags.flightMode == 0) &&
     (Sensor.Receiver.channel3 == 1080) && 
     (Sensor.Receiver.channel2 == 1920) &&
     ((Sensor.Receiver.channel1 == 1080)||(Sensor.Receiver.channel1 == 1920)))
  {
    Motor.outputPWM1 = 1150;
    Motor.outputPWM2 = 1150;
    Motor.outputPWM3 = 1150;
    Motor.outputPWM4 = 1150;
    Flags.flightMode = 2;
  }

  if((Flags.flightMode == 1) && (stanbyTimer + 1000 < millis()) &&
     (Sensor.Receiver.channel3 == 1080) && 
     (Sensor.Receiver.channel2 == 1920) &&
     ((Sensor.Receiver.channel1 == 1080)||(Sensor.Receiver.channel1 == 1920)))
  {
    Motor.outputPWM1 = PWM_LOW;
    Motor.outputPWM2 = PWM_LOW;
    Motor.outputPWM3 = PWM_LOW;
    Motor.outputPWM4 = PWM_LOW;
    Flags.flightMode = -systemFreq*2;
  }

  if((Flags.flightMode == 1) && (stanbyTimer + 3000 < millis()))
  {
    Motor.outputPWM1 = PWM_LOW;
    Motor.outputPWM2 = PWM_LOW;
    Motor.outputPWM3 = PWM_LOW;
    Motor.outputPWM4 = PWM_LOW;
    Flags.flightMode = 0;
  }
  
  if((Flags.flightMode == 2) && (Sensor.Receiver.channel3 <= 1088))
  {
    Motor.outputPWM1 = 1150;
    Motor.outputPWM2 = 1150;
    Motor.outputPWM3 = 1150;
    Motor.outputPWM4 = 1150;
    
    Controller.pAccumulatedError = 0;
    Controller.qAccumulatedError = 0;
    Controller.rAccumulatedError = 0;
    Controller.pPreviousError = 0;
    Controller.qPreviousError = 0;
    Controller.rPreviousError = 0;

    stanbyTimer = millis();
    Flags.flightMode = 1;
  }

  if(((Flags.flightMode == 1) || (Flags.flightMode == 2)) && 
     (Sensor.Receiver.channel3 > 1088))
  {
    Flags.flightMode = 3;
  }

  if(Flags.flightMode == 3)
  {
    PIDController();
    Motor.outputPWM1 = Controller.zDotCommand + Controller.pCommand - Controller.qCommand - Controller.rCommand;
    Motor.outputPWM2 = Controller.zDotCommand - Controller.pCommand - Controller.qCommand + Controller.rCommand;
    Motor.outputPWM3 = Controller.zDotCommand - Controller.pCommand + Controller.qCommand - Controller.rCommand;
    Motor.outputPWM4 = Controller.zDotCommand + Controller.pCommand + Controller.qCommand + Controller.rCommand;

    /* [OPTIONAL] BATTERY COMPENSATION */
    //Motor.outputPWM1 += Motor.outputPWM1 * (12.6f - Sensor.Battery.batteryVoltage) / 1.5 / 10; //11.1V 인 경우 현재 출력에서 10% 추가
    //
    //
    /*
    Motor.outputPWM1 += Motor.outputPWM1 * (12.6f - Sensor.Battery.batteryVoltage) / 1.5 / 10;
    Motor.outputPWM2 += Motor.outputPWM2 * (12.6f - Sensor.Battery.batteryVoltage) / 1.5 / 10;
    Motor.outputPWM3 += Motor.outputPWM3 * (12.6f - Sensor.Battery.batteryVoltage) / 1.5 / 10;
    Motor.outputPWM4 += Motor.outputPWM4 * (12.6f - Sensor.Battery.batteryVoltage) / 1.5 / 10;
    */
  }
  
  if((Flags.flightMode == 3) && (Sensor.Receiver.channel3 <= 1088))
  {
    Flags.flightMode = 2;
  }

  if((micros() - systemTimer > systemPeriod))
  {
    digitalWrite(13,LOW);
    digitalWrite(12,LOW);
    digitalWrite(2,HIGH);
  }

  
  //모터 출력
  PWMOutRaiser(); //여기서 남는시간은 태우고 systemTimer 초기화됨! 
  
  // Raiser 와 Faller 사이에 1ms 여유있음. 
  // 뭔가 넣어도 되지만 1ms 넘으면 샵됨.. high 상태가 정상보다 길게 유지될 수 있음
  // 제어 흐름에 영향을 크게 주지 않으면서 시시한 계산을 넣기 좋음.
  ReadADC0();

  PWMOutFaller();

  // 여기부터는 400Hz인 경우 약0.5ms, 200Hz인 경우 약2ms 여유 있음
}

// 인터럽트 코드에는 최소한의 계산만!
ISR (PCINT0_vect) // Port B, PCINT0 - PCINT7: Handle pin change interrupt from 8 to 13.
{
  //![0]
  receiverTimer0 = micros();

  //![1] 8, PB0, 1번 채널과 연결
  if (PINB & (1 << PB0)) // 8번핀 Rising인 경우
  {
    if (Flags.pin8 == 0) // Low상태를 high로 변경
    {
      receiverTimer1 = receiverTimer0;
      Flags.pin8 = 1;
    }
  }
  else // 8번핀 Falling인 경우
  {
    if (Flags.pin8 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle)
    {
      Flags.pin8 = 0;
      Sensor.Receiver.channel1Input = receiverTimer0 - receiverTimer1;
    }
  }

  //![2] 9, PB1, 2번 채널과 연결
  if (PINB & (1 << PB1)) // 9번핀 Rising인 경우
  {
    if (Flags.pin9 == 0) // Low상태를 high로 변경
    {
      receiverTimer2 = receiverTimer0;
      Flags.pin9 = 1;
    }
  }
  else // 9번핀 Falling인 경우
  {
    if (Flags.pin9 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle)
    {
      Flags.pin9 = 0;
      Sensor.Receiver.channel2Input = receiverTimer0 - receiverTimer2;
    }
  }

  //![3] 10, PB2, 3번 채널과 연결
  if (PINB & (1 << PB2)) // 10번핀 Rising인 경우
  {
    if (Flags.pin10 == 0) // Low상태를 high로 변경
    {
      receiverTimer3 = receiverTimer0;
      Flags.pin10 = 1;
    }
  }
  else // 10번핀 Falling인 경우
  {
    if (Flags.pin10 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle)
    {
      Flags.pin10 = 0;
      Sensor.Receiver.channel3Input = receiverTimer0 - receiverTimer3;
    }
  }

  //![4] 11, PB3, 11번 채널과 연결
  if (PINB & (1 << PB3)) // 11번핀 Rising인 경우
  {
    if (Flags.pin11 == 0) // Low상태를 high로 변경
    {
      receiverTimer4 = receiverTimer0;
      Flags.pin11 = 1;
    }
  }
  else // 11번핀 Falling인 경우
  {
    if (Flags.pin11 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle)
    {
      Flags.pin11 = 0;
      Sensor.Receiver.channel4Input = receiverTimer0 - receiverTimer4;
    }
  }
}
ISR(PCINT1_vect) {}   // Port C, PCINT8 - PCINT14
ISR(PCINT2_vect) {}   // Port D, PCINT16 - PCINT23

void ReadADC0()
{
  Sensor.Battery.mainBatteryVoltageInput = analogRead(0);
  
  Sensor.Battery.batteryVoltage = (0.9 * Sensor.Battery.batteryVoltage + 
                                   0.1 * ((float)Sensor.Battery.mainBatteryVoltageInput * Sensor.Battery.scaler + 0.8318));
  //Low pass filter 포함. 0.8318은 1N4007 다이오드로 인한 전압 하강분 보상임.
}
