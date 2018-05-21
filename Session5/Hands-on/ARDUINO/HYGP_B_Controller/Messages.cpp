#include <Arduino.h>

#include "Messages.h"

void MessageWelcome()
{
  Serial.println(F(""));
  Serial.println(F("[안내] 2018 항공우주공학과 역량강화 프로젝트에 오신것을 환영합니다!"));
}

void MessageMenu()
{
  Serial.println(F("============================================================"));
  Serial.println(F("[안내] m - 점검기능 메뉴 확인"));
  Serial.println(F("[안내] s - 시리얼 통신 확인"));
  Serial.println(F("[안내] i - 센서 I2C 통신 확인 및 보정"));
  Serial.println(F("[안내] t - 조종기 연결 확인 및 보정"));
  Serial.println(F("[안내] v - 배터리 전압 확인"));
  Serial.println(F("[안내] p - 주의! 프로펠러 제거후 사용! PWM 신호송출 확인"));
  Serial.println(F("[안내] r - 리포트 출력"));
  Serial.println(F("[안내] w - 설정값 저장"));
  Serial.println(F("[안내] q - 종료"));
  Serial.println(F("============================================================"));
}

void MessageSerial()
{
  Serial.println(F("============================================================"));
  Serial.println(F("[안내] 시리얼 통신 기능을 점검합니다"));
}
void MessageSerialNormal()
{
  Serial.println(F("[정상] 시리얼통신 확인 완료."));
  Serial.println(F("============================================================"));
}
void MessageSerialAbnormal()
{
  Serial.println(F("[오류] 시리얼통신 점검 필요."));
  Serial.println(F("============================================================"));
}


void MessageI2C()
{
  Serial.println(F("============================================================"));
  Serial.println(F("[안내] I2C 통신 기능을 점검합니다"));  
}
void MessageI2CNormal()
{
  Serial.println(F("[정상] I2C 통신 확인 완료."));
  Serial.println(F("============================================================"));
}
void MessageI2CAbnormal()
{
  Serial.println(F("[오류] I2C 통신 점검 필요."));
  Serial.println(F("============================================================"));
}


void MessageRTR()
{
  Serial.println(F("============================================================"));
  Serial.println(F("[안내] 송수신기 기능을 점검합니다"));  
}
void MessageRTRNormal()
{
  Serial.println(F("[정상] 송수신기 기능 확인 완료."));
  Serial.println(F("============================================================"));
}
void MessageRTRAbnormal()
{
  Serial.println(F("[오류] 송수신기 기능 점검 필요."));
  Serial.println(F("============================================================"));
}

void MessagePWMOutput()
{
  Serial.println(F("============================================================"));
  Serial.println(F("[안내] PWM 출력 기능을 점검합니다. 모터 번호와 회전방향을 확인하십시오."));
}
void MessagePWMOutputNormal()
{
  Serial.println(F("[정상] PWM 출력 기능 확인 완료."));
  Serial.println(F("============================================================"));
}
void MessagePWMOutputAbnormal()
{
  Serial.println(F("[오류] PWM 출력 기능 점검 필요."));
  Serial.println(F("============================================================"));
}

void MessageWrongInput()
{
  Serial.println(F("[오류] 잘못된 키 입력입니다."));
}
void MessageExit()
{
  Serial.println(F("[안내] 시스템 점검을 종료합니다 :) "));
}


