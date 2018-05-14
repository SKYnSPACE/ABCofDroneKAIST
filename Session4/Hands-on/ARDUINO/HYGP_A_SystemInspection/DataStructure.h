#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

struct _Acc
{
  float x, y, z;
};

struct _Gyro
{
  float p, q, r;
};

struct _Receiver
{
  int channel1Input, channel2Input, channel3Input, channel4Input;
  int channel1Center, channel2Center, channel3Center, channel4Center;
  int channel1Min, channel2Min, channel3Min, channel4Min;
  int channel1Max, channel2Max, channel3Max, channel4Max;
  
  float channel1, channel2, channel3, channel4; // Normalized value (-1~1)
};

struct _Battery
{
  int mainBatteryVoltageInput; //[A0 Port]
  int scaler; //[R1:R2 = 2:1 인 경우 3으로 초기화]
  float batteryVlotage;
};

struct _Sensor
{

  _Acc Acc;
  _Gyro Gyro;
  _Receiver Receiver;
  _Battery Battery;
  
  float temperature;

  
};

struct _Controller
{
  float pRateGain[3]; //[0]: p gain, [1]: i gain, [2]: d gain
  float qRateGain[3];
  float rRateGain[3];
  
  float pReference;
  float qReference;
  float rReference;

  float pError;
  float qError;
  float rError;

  float pPreviousError;
  float qPreviousError;
  float rPreviousError;

  float pAccumulatedError;
  float qAccumulatedError;
  float rAccumulatedError;
};

struct _Motor
{
  int outputPWM1;
  int outputPWM2;
  int outputPWM3;
  int outputPWM4;
};

struct _Flags
{
  bool mainFrequencyCheck;
  
  //PIN Digital value?
  bool pin8;
  bool pin9;
  bool pin10;
  bool pin11;

  // Status during inspections [0]: Abnormal, [1]: Normal or End of inspection
  bool serialStatus;
  bool gyroStatus;
  bool receiverStatus;
  bool transmitterStatus;
  bool termsAndConditionsPWM;
  bool motorStatus;

  // Critical situations [0]: Normal, [1]: Abnormal
  bool sensorReadTimeout;
  bool smallSizedBiasData;
  
  bool exitCommand;
};

#endif
