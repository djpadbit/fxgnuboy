#ifndef __FXSYS_H__
#define __FXSYS_H__

#include <ctype.h>

#define TIMER_FREQ 2000
#define TIMER_DELAY_US ((1.0/(double)TIMER_FREQ)*1000000.0)

extern unsigned long timertime;

void timer_setup();
void timer_cleanup();

#endif