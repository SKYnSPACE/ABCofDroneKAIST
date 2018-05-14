#include <Arduino.h>

#include "ReceiverTransmitter.h"

void ReceiverInspection()
{
  unsigned long timer;

  Sensor.Receiver.channel1Input = 0;
  Sensor.Receiver.channel2Input = 0;
  Sensor.Receiver.channel3Input = 0;
  Sensor.Receiver.channel4Input = 0;
  
  delay(2000);
  Serial.println(F(">>>>>>[안내] 수신기 입력을 20초간 송출합니다. 내용을 확인하세요!"));
  delay(4000);

  timer = millis() + 20000;
  while(timer > millis())
  {
    sprintf(str, ">>>>>>[안내] CH1: %d, CH2: %d, CH3: %d, CH4: %d", 
            Sensor.Receiver.channel1Input, Sensor.Receiver.channel2Input,
            Sensor.Receiver.channel3Input, Sensor.Receiver.channel4Input);
    Serial.println(str);
  }

  if ((Sensor.Receiver.channel1Input != 0) && (Sensor.Receiver.channel2Input != 0) &&
      (Sensor.Receiver.channel3Input != 0) && (Sensor.Receiver.channel4Input != 0))
  {
    Serial.println(F(">>>>>>[1/4] 수신기 입력 데이터 출력을 완료하였습니다."));
    Flags.receiverStatus = 1;
  }
  else
  {
    Serial.println(F(">>>>>>[오류] 수신기의 입력 상태를 확인하세요."));
    Flags.receiverStatus = 0;
  }
}

void TransmitterStickRange()
{
  delay(2000);
  Serial.println(F(">>>>>>[안내] 송신기 보정을 시작합니다. 스틱을 모두 [가운데]로 옮겨주세요."));
  delay(6000);
  Sensor.Receiver.channel1Center = Sensor.Receiver.channel1Input;
  Sensor.Receiver.channel2Center = Sensor.Receiver.channel2Input;
  Sensor.Receiver.channel3Center = Sensor.Receiver.channel3Input;
  Sensor.Receiver.channel4Center = Sensor.Receiver.channel4Input;
  Serial.println(F(">>>>>>[2/4] 스틱 중앙값 기록을 완료하였습니다."));

  delay(2000);
  Serial.println(F(">>>>>>[안내] 스틱을 모두 [좌측하단]으로 끝까지 옮겨주세요."));
  delay(5000);
  Sensor.Receiver.channel1Min = Sensor.Receiver.channel1Input;
  Sensor.Receiver.channel2Min = Sensor.Receiver.channel2Input;
  Sensor.Receiver.channel3Min = Sensor.Receiver.channel3Input;
  Sensor.Receiver.channel4Min = Sensor.Receiver.channel4Input;
  Serial.println(F(">>>>>>[3/4] 스틱 최소(좌측/하단)위치 기록을 완료하였습니다."));

  delay(2000);
  Serial.println(F(">>>>>>[안내] 스틱을 모두 [우측상단]으로 끝까지 옮겨주세요."));
  delay(5000);
  Sensor.Receiver.channel1Max = Sensor.Receiver.channel1Input;
  Sensor.Receiver.channel2Max = Sensor.Receiver.channel2Input;
  Sensor.Receiver.channel3Max = Sensor.Receiver.channel3Input;
  Sensor.Receiver.channel4Max = Sensor.Receiver.channel4Input;
  Serial.println(F(">>>>>>[4/4] 스틱 최대(우측/상단)위치 기록을 완료하였습니다."));

  delay(2000);
  Serial.println(F(">>>>>>[안내] 최종 결과를 확인하세요."));
  sprintf(str, ">>>>>>[안내] CH1: %d -- %d -- %d",
          Sensor.Receiver.channel1Min, Sensor.Receiver.channel1Center, Sensor.Receiver.channel1Max);
  Serial.println(str);
  sprintf(str, ">>>>>>[안내] CH2: %d -- %d -- %d",
          Sensor.Receiver.channel2Min, Sensor.Receiver.channel2Center, Sensor.Receiver.channel2Max);
  Serial.println(str);
  sprintf(str, ">>>>>>[안내] CH3: %d -- %d -- %d",
          Sensor.Receiver.channel3Min, Sensor.Receiver.channel3Center, Sensor.Receiver.channel3Max);
  Serial.println(str);
  sprintf(str, ">>>>>>[안내] CH4: %d -- %d -- %d",
          Sensor.Receiver.channel4Min, Sensor.Receiver.channel4Center, Sensor.Receiver.channel4Max);
  Serial.println(str);
}

void ChannelInspection()
{
  //TBD
}

