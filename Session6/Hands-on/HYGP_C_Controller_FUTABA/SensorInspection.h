#ifndef SENSORINSPECTION_H
#define SENSORINSPECTION_H

#include "DataStructure.h"
#include "Sensors.h"

#include "Macros.h"

extern struct _Flags Flags;
extern struct _Sensor Sensor;
extern struct PCONCAT_(,GYRO_MODEL) GYRO_MODEL;

extern char str[80];


bool GyroInspection();

#endif
