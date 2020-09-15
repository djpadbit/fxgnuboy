

#ifndef __RTC_H__
#define __RTC_H__


struct rtc
{
	int batt;
	int sel;
	int latch;
	int d, h, m, s, t;
	int stop, carry;
	byte regs[8];
};

extern struct rtc rtc;

#include "defs.h"

void rtc_latch(byte b);
void rtc_write(byte b);
void rtc_tick();

#include <gint/std/stdio.h>
void rtc_save_internal(int f);
void rtc_load_internal(int f);

#endif



