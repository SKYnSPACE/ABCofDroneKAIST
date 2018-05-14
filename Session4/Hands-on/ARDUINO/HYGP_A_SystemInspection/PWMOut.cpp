#include <Arduino.h>

#include "PWMOut.h"

void PWMOutRaiser()
{
  while(micros() - systemTimer < 2500){} // 모터 출력 속도인 2.5ms(400Hz) 내에 모든 계산이 완료된 경우 대기.
  systemTimer = micros(); //다음 루프를 위한 시스템 시계 업데이트

  PORTD |= B01111000; //모든 출력 high.
  motorTimer1 = systemTimer + Motor.outputPWM1; // high 상태가 유지되어야 하는 시간을 기록
  motorTimer2 = systemTimer + Motor.outputPWM2; 
  motorTimer3 = systemTimer + Motor.outputPWM3;
  motorTimer4 = systemTimer + Motor.outputPWM4;
  
}

// 모터 동작을 위해서는 PWMOutRise() 함수 호출 이후 PWMOutFall() 함수가 순차적으로 반복 호출되어야 함
// 참고로 모터 출력 high의 지속은 최소 1ms 보장되므로,
// 해당시간안에 처리하고자 하는 작업을 두 함수 사이에 넣는것이 좋음. 
// e.g. KalmanFilter();
// 아님 timer interrupt를 쓰던지..

void PWMOutFaller()
{
  while(PORTD >= B00001000)
  {
    motorTimer0 = micros();
    if(motorTimer1 <= motorTimer0)PORTD &= B11110111; // high 필요시간 종료시 fall.
    if(motorTimer2 <= motorTimer0)PORTD &= B11101111;
    if(motorTimer3 <= motorTimer0)PORTD &= B11011111;
    if(motorTimer4 <= motorTimer0)PORTD &= B10111111;
  }
}

void PWMOutInspection()
{
  unsigned long timer;
  unsigned long timeToWait;
  Flags.termsAndConditionsPWM = 0;
  Flags.motorStatus=0;

  Serial.println(F(">>>>>>[경고] 사용자께서는 프로펠러가 완전히 제거되었음을 확인하였습니까? [y/n]"));

  // 최대 10초 대기
  timeToWait = millis() + 10000;
  while(Serial.available() < 1 && timeToWait > millis()){}

  if(Serial.available() > 0)
  {
    
    keyInput = Serial.read();
    if (keyInput == 'y')
    {
      Serial.println(F(">>>>>>[경고] 사용자께서는 모터 오동작으로 인해 발생하는 문제에 책임이 있음을 인지하고 계십니까? [y/n]"));
      
      // 최대 10초 대기
      timeToWait = millis() + 10000;
      while(Serial.available() < 1 && timeToWait > millis()){}

      keyInput = Serial.read();
      if (keyInput == 'y')
      {
        Serial.println(F(">>>>>>[경고] 모터 동작에 유의하세요!"));
        Flags.termsAndConditionsPWM = 1;
      }
      else
      {
         Serial.println(F(">>>>>>[오류] Timeout.")); 
      }
    }
  }
  else
  {
    Serial.println(F(">>>>>>[오류] Timeout."));
  }
  
  if(Flags.termsAndConditionsPWM == 1) //모두 동의한 경우 모터 테스트 진행
  {
    Motor.outputPWM1 = PWM_LOW;
    Motor.outputPWM2 = PWM_LOW;
    Motor.outputPWM3 = PWM_LOW;
    Motor.outputPWM4 = PWM_LOW;

    timer = millis() + 3000;
    while(timer > millis())
    {
      PWMOutRaiser();
      PWMOutFaller();
    }

    Serial.println(F(">>>>>>[안내] 1번 모터 (우측상단)(반시계)를 5초간 동작합니다."));
    Motor.outputPWM1 = 1200;
    timer = millis() + 5000;
    while(timer > millis())
    {
      PWMOutRaiser();
      PWMOutFaller();
    }
    PWMOutRaiser();
    Motor.outputPWM1 = PWM_LOW;
    PWMOutFaller();

    Serial.println(F(">>>>>>[안내] 2번 모터 (좌측상단)(시계)를 5초간 동작합니다."));
    Motor.outputPWM2 = 1200;
    timer = millis() + 5000;
    while(timer > millis())
    {
      PWMOutRaiser();
      PWMOutFaller();
    }
    PWMOutRaiser();
    Motor.outputPWM2 = PWM_LOW;
    PWMOutFaller();

    Serial.println(F(">>>>>>[안내] 3번 모터 (좌측하단)(반시계)를 5초간 동작합니다."));
    Motor.outputPWM3 = 1200;
    timer = millis() + 5000;
    while(timer > millis())
    {
      PWMOutRaiser();
      PWMOutFaller();
    }
    PWMOutRaiser();
    Motor.outputPWM3 = PWM_LOW;
    PWMOutFaller();

    Serial.println(F(">>>>>>[안내] 4번 모터 (우측하단)(시계)를 5초간 동작합니다."));
    Motor.outputPWM4 = 1200;
    timer = millis() + 5000;
    while(timer > millis())
    {
      PWMOutRaiser();
      PWMOutFaller();
    }
    PWMOutRaiser();
    Motor.outputPWM4 = PWM_LOW;
    PWMOutFaller();
      
    Flags.motorStatus = 1;
  }
  else
  {
    Serial.println(F(">>>>>>[오류] 모터 테스트를 완료하지 못하였습니다."));
  }
}

