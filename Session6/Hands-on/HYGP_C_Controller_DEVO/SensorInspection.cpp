#include <Arduino.h>

#include "SensorInspection.h"

bool GyroInspection()
{
  unsigned long timer;
  
  if((TWBR == 12) && Flags.mainFrequencyCheck)
  {
    
//![1]
    Serial.println(F(">>>>>>[1/4] I2C 클럭 속도 설정을 완료하였습니다."));
    Flags.gyroStatus = 1;

//![2]
    delay(2000);    
    Serial.print(F(">>>>>>[안내] "));
    sprintf(str, "%s", STR2(GYRO_MODEL));
    Serial.print(str);
    Serial.println(F(" 센서를 탐색합니다."));
    delay(3000);
    if(FUNCTION_(GYRO_MODEL, IDCheck)() == GYRO_MODEL.whoAmIValue) // 센서에 WAI 요청하여 센서 ID 확인.
    {
      Serial.print(F(">>>>>>[2/4] "));
      Serial.print(str);
      Serial.print(F(" 센서가 주소 0x"));
      Serial.print(GYRO_MODEL.address,HEX);
      Serial.println(F("로 정상 연결되었습니다."));
      Flags.gyroStatus = 1;

//![3]
      delay(2000);
      Serial.println(F(">>>>>>[안내] 자이로 보정을 시작합니다 10초간 움직이지마세요!"));
      delay(4000);
      FUNCTION_(GYRO_MODEL, CalibGyro)();
      if((Flags.sensorReadTimeout == 0) && (Flags.smallSizedBiasData == 0)) //보정중 타임아웃 에러나 자료부족이 발생하였는지 점검
      {
        Serial.println(F(">>>>>>[안내] 측정된 gyro bias는 다음과 같습니다."));
        Serial.print(F(">>>>>>[안내] "));
        Serial.print(GYRO_MODEL.pBias);
        Serial.print(F(", "));
        Serial.print(GYRO_MODEL.qBias);
        Serial.print(F(", "));
        Serial.println(GYRO_MODEL.rBias);
        Serial.println(F(">>>>>>[3/4] 자이로 보정을 완료하였습니다."));
        Flags.gyroStatus = 1;
      }
      else
      {
        Serial.println(F(">>>>>>[오류] 센서값 획득 중 read time-out, 또는 lack of data point가 발생하였습니다!"));
        Flags.gyroStatus = 0;
        goto exception;
      }

//![4] Gyro 보정후 60초간 센서 데이터 출력
      delay(2000);
      Serial.println(F(">>>>>>[안내] 센서 데이터를 1분간 출력합니다. 축 방향을 점검하세요!"));
      delay(4000);

      timer = millis() + 60000;
      while(timer > millis())
      {
        FUNCTION_(GYRO_MODEL, ReadData)();
        FUNCTION_(GYRO_MODEL, TransferData)();
        Serial.print(F(">>>>>>[안내] p = "));
        Serial.print(Sensor.Gyro.p);
        Serial.print("\t");
        Serial.print(F("q = "));
        Serial.print(Sensor.Gyro.q);
        Serial.print("\t");
        Serial.print(F("r = "));
        Serial.print(Sensor.Gyro.r);
        Serial.print("\t");
        Serial.print(F("ax = "));
        Serial.print(Sensor.Acc.x);
        Serial.print("\t");
        Serial.print(F("ay = "));
        Serial.print(Sensor.Acc.y);
        Serial.print("\t");
        Serial.print(F("az = "));
        Serial.println(Sensor.Acc.z);
        delay(20);
      }
      if(Flags.sensorReadTimeout == 0) //보정중 타임아웃 에러, 가 발생하였는지 점검
      {
        Serial.println(F(">>>>>>[4/4] 센서 데이터 출력을 완료하였습니다."));
        Flags.gyroStatus = 1;
      }
      else
      {
        Serial.println(F(">>>>>>[오류] 센서값 획득 중 read time-out이 발생하였습니다!"));
        Flags.gyroStatus = 0;
        goto exception;
      }
    }
    else
    {
      Serial.println(F(">>>>>>[오류] 센서 연결상태, 또는 GYRO_MODEL의 값을 확인하세요."));
      Serial.print(F(">>>>>>[안내] WAI from Sensor = 0x"));
      Serial.println(FUNCTION_(GYRO_MODEL, IDCheck)(),HEX);
      Flags.gyroStatus = 0;
      goto exception;
    }
  }
  else
  {
    Serial.println(F(">>>>>>[오류] I2C 클럭 속도 조정에 실패하였습니다."));
    Serial.print(F(">>>>>>[안내] TWBR="));
    Serial.print(TWBR);
    Serial.println(F(". CPU 및 TWI 주파수를 점검하세요."));
    Flags.gyroStatus = 0;
    goto exception;
  }
  
exception:
;

}
