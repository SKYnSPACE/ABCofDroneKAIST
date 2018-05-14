#ifndef PWMOUT_H
#define PWMOUT_H

#include "DataStructure.h"

#define PWM_LOW 1050

extern struct _Motor Motor;
extern struct _Flags Flags;

extern unsigned short keyInput;

extern volatile unsigned long systemTimer;
extern volatile unsigned long timer;
extern volatile unsigned long motorTimer0;
extern volatile unsigned long motorTimer1, motorTimer2, motorTimer3, motorTimer4;

void PWMOutRaiser();
void PWMOutFaller();
void PWMOutInspection();


#endif
