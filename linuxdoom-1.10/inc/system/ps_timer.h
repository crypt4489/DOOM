#ifndef PS_TIMER_H
#define PS_TIMER_H

#include "ps_global.h"
u64 getTimeMs(TimerStruct *ts);
int TimerZeroInterrupt(s32 cause, void *arg, void *addr);
TimerStruct* TimerZeroEnable();
void TimerZeroDisable(TimerStruct *timer);
float getTicks(TimerStruct *ts);
#endif
