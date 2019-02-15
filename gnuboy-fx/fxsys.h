#ifndef __FXSYS_H__
#define __FXSYS_H__

#include <ctype.h>

#define TIMER_FREQ 1024
//#define TIMER_FREQ_60DIV (TIMER_FREQ/60)

extern unsigned long timertime;

void timer_setup();
void timer_cleanup();
void file_make_path(uint16_t* dst,char* root,char *fold,char *fn);

#endif