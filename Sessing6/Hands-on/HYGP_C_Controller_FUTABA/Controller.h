#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "DataStructure.h"
#include "UserDefinedFunctions.h"

extern struct _Sensor Sensor;
extern struct _Controller Controller;

void StickNormalize();
void PIDController();

#endif
