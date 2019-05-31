#ifndef MESSAGE_H
#define MESSAGE_H

#include "DataStructure.h"

extern struct _Sensor Sensor;

void MessageWelcome();
void MessageMenu();
void MessageWrongInput();
void MessageExit();

void MessageSerial();
void MessageSerialNormal();
void MessageSerialAbnormal();

void MessageI2C();
void MessageI2CNormal();
void MessageI2CAbnormal();

void MessageRTR();
void MessageRTRNormal();
void MessageRTRAbnormal();

void MessagePWMOutput();
void MessagePWMOutputNormal();
void MessagePWMOutputAbnormal();


#endif

