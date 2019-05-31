#include <Arduino.h>

#include "Filters.h"

void EulerEstimator()
{
  
  Sensor.Estimates.rollFromGyro  += Sensor.Gyro.pFiltered / systemFreq;
  Sensor.Estimates.pitchFromGyro += Sensor.Gyro.qFiltered / systemFreq;
                                     
  Sensor.Estimates.rollFromGyro  -= Sensor.Estimates.pitchFromGyro * sin(Sensor.Gyro.rFiltered / systemFreq * D2R);
  Sensor.Estimates.pitchFromGyro += Sensor.Estimates.rollFromGyro  * sin(Sensor.Gyro.rFiltered / systemFreq * D2R);

  Sensor.Estimates.rollFromAcc  = (Sensor.Acc.total > abs(Sensor.Acc.y)) ? (-asin(Sensor.Acc.y/Sensor.Acc.total)*R2D):
                                                                           (Sensor.Estimates.rollFromAcc);
  Sensor.Estimates.pitchFromAcc = (Sensor.Acc.total > abs(Sensor.Acc.x)) ? (-asin(Sensor.Acc.x/Sensor.Acc.total)*R2D):
                                                                           (Sensor.Estimates.pitchFromAcc);
  
  Sensor.Estimates.roll  = 0.8 * Sensor.Estimates.rollFromGyro  + 0.2 * Sensor.Estimates.rollFromAcc;
  Sensor.Estimates.pitch = 0.8 * Sensor.Estimates.pitchFromGyro + 0.2 * Sensor.Estimates.pitchFromAcc;
  
}

