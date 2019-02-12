#ifndef __FXSYS_H__
#define __FXSYS_H__

#define TIMER_FREQ 1024
//#define TIMER_FREQ_60DIV (TIMER_FREQ/60)

extern unsigned long timertime;

void timer_setup();
void timer_cleanup();

void mprint(int x,int y, const char* fmt, ...);
void print_waitkey(int x,int y, int cls,const char* p);
void keyb_input(char* buf,size_t len,const char* ask);

#endif