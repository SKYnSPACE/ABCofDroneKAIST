#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

struct _Acc
{
  float x, y, z, total;
};

struct _Gyro
{
  float p, q, r;
  float pFiltered, qFiltered, rFiltered;
};

struct _Estimates
{
  float rollFromGyro, pitchFromGyro, yawFromGyro;
  float rollFromAcc, pitchFromAcc, yawFromAcc;
  float roll, pitch, yaw;
};

struct _Receiver
{
  int channel1Input, channel2Input, channel3Input, channel4Input;
  int channel1Center, channel2Center, channel3Center, channel4Center;
  int channel1Min, channel2Min, channel3Min, channel4Min;
  int channel1Max, channel2Max, channel3Max, channel4Max;
  
  int channel1, channel2, channel3, channel4; // Normalized value (1080~1500~1920)

  int deadBand;
};

struct _Battery
{
  int mainBatteryVoltageInput; //[A0 Port]
  float scaler; //[R1:R2 = 2:1 인 경우 3으로 초기화]
  float batteryVoltage;
};

struct _Sensor
{

  _Acc Acc;
  _Gyro Gyro;
  _Estimates Estimates;
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

  float zDotCommand;
  float pCommand;
  float qCommand;
  float rCommand;
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

  int flightMode; //[-1]: Grounded-listening, [0]: WOW, [1]: engine-up-holding, [2]: ready-to-fly , [3]: flying
  
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
