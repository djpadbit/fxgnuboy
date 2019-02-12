#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <hw.h>
#include "emu.h"
#include <events.h>
#include <keyboard.h>
#include <display.h>
#include <ctype.h>
#include <gray.h>
#include "fxsys.h"

unsigned long timertime = 0;
//static int cbid;
static timer_t *htimer = NULL;

static void timer_callback()
{
	timertime++;
}

void timer_setup()
{
	//cbid = rtc_cb_add(rtc_freq_256Hz,timer_callback,0);
	uint32_t delay = clock_setting(TIMER_FREQ, clock_Hz);
	htimer = htimer_setup(timer_user, delay, timer_Po_4, 0);
	timer_attach(htimer, timer_callback, NULL);
	timer_start(htimer);
}

void timer_cleanup()
{
	//rtc_cb_end(cbid);
	timer_stop(htimer);
}

void mprint(int x,int y, const char* fmt, ...)
{
	if(x < 1 || x > 21 || y < 1 || y > 8) return;
	char k[50];
	va_list args;
	va_start(args, fmt);
	vsprintf(k,fmt,args);
	va_end(args);
	gtext(x * 6 - 5, y * 8 - 8, k);
}

void print_waitkey(int x,int y, int cls,const char* p)
{
	if (cls) gclear();
	mprint(x,y,p);
	gupdate();
	getkey();
}

void keyb_input(char* buf,size_t len,const char* ask)
{
	int key,ptr;
	ptr = 0;
	key = 0;
	int run = 1;
	int lower = 1;
	int shift = 0;
	int ls = 0;
	int alpha = 0;
	int la = 0;
	memset(buf,0,len);
	while (run) {
		gclear();
		mprint(1,1,ask);
		mprint(1,2,buf);
		gline(ptr*6,8,ptr*6,7+7,color_black);
		if (lower) mprint(1,3,"Lowercase");
		else mprint(1,3,"Uppercase");
		mprint(1,4,"Use OPTN to switch");
		mprint(1,5,alpha == 2 ? "Alpha lock" : alpha == 1 ? "Alpha" : shift ? "Shift" : "");
		gupdate();
		key = getkey_opt(getkey_none,0);
		switch (key) {
			case KEY_LEFT:
				if (ptr > 0) ptr--;
				break;
			case KEY_RIGHT: 
				if (buf[ptr] != 0) ptr++;
				break;
			case KEY_EXE:
				if (ptr != 0) run = 0;
				break;
			case KEY_DEL: 
				if (ptr > 0) {buf[ptr--] = 0;buf[ptr] = 0;}
				break;
			case KEY_OPTN:
				lower = !lower;
				break;
			case KEY_SHIFT:
				shift = !shift;
				break;
			case KEY_ALPHA:
				if (shift) alpha = 2;
				else alpha = !alpha;
				break;
			default:
				if (alpha) key |= MOD_ALPHA;
				if (shift) key |= MOD_SHIFT;
				if (key_char(key) != 0 && ptr < len+1) buf[ptr++] = lower ? tolower(key_char(key)) : key_char(key);
				break;
		}
		if (ls) shift = 0;
		if (la == 1) alpha = 0;
		ls = shift;
		la = alpha;
		
	}
	gclear();
	gupdate();
}

void *sys_timer()
{
	return NULL;
}

int sys_elapsed(void *cl)
{
	return 0;
}

void sys_sleep(int us)
{
	//wdt feed
}

int sys_handle_input() {
	/*int k=kchal_get_keys();
	hw.pad=0;
	if (k&KC_BTN_RIGHT) hw.pad|=PAD_RIGHT;
	if (k&KC_BTN_LEFT) hw.pad|=PAD_LEFT;
	if (k&KC_BTN_UP) hw.pad|=PAD_UP;
	if (k&KC_BTN_DOWN) hw.pad|=PAD_DOWN;
	if (k&KC_BTN_SELECT) hw.pad|=PAD_SELECT;
	if (k&KC_BTN_START) hw.pad|=PAD_START;
	if (k&KC_BTN_A) hw.pad|=PAD_A;
	if (k&KC_BTN_B) hw.pad|=PAD_B;
	if (k&KC_BTN_POWER) {
		int r=gbfemtoShowMenu();
		while (kchal_get_keys() & KC_BTN_POWER) vTaskDelay(10);
		return r;
	}
	return EMU_RUN_CONT;*/
	event_t e;
	e = pollevent();
	while(1)
	{
		if (e.type == event_none)
			break;

		if (e.type == event_key_press) {
			switch (e.key.code) {
				case KEY_SHIFT:
					hw.pad|=PAD_A;
					break;
				case KEY_ALPHA:
					hw.pad|=PAD_B;
					break;
				case KEY_OPTN:
					hw.pad|=PAD_SELECT;
					break;
				case KEY_VARS:
					hw.pad|=PAD_START;
					break;
				case KEY_LEFT:
					hw.pad|=PAD_LEFT;
					break;
				case KEY_RIGHT:
					hw.pad|=PAD_RIGHT;
					break;
				case KEY_DOWN:
					hw.pad|=PAD_DOWN;
					break;
				case KEY_UP:
					hw.pad|=PAD_UP;
					break;
				case KEY_MENU:
					return -1;
				case KEY_EXIT:
					return EMU_RUN_EXIT;
			}
		}

		else if (e.type == event_key_release) {
			switch (e.key.code) {
				case KEY_SHIFT:
					hw.pad^=PAD_A;
					break;
				case KEY_ALPHA:
					hw.pad^=PAD_B;
					break;
				case KEY_OPTN:
					hw.pad^=PAD_SELECT;
					break;
				case KEY_VARS:
					hw.pad^=PAD_START;
					break;
				case KEY_LEFT:
					hw.pad^=PAD_LEFT;
					break;
				case KEY_RIGHT:
					hw.pad^=PAD_RIGHT;
					break;
				case KEY_DOWN:
					hw.pad^=PAD_DOWN;
					break;
				case KEY_UP:
					hw.pad^=PAD_UP;
					break;
			}
		}
		e = pollevent();
	}
	return EMU_RUN_CONT;
}

void sys_checkdir(char *path, int wr)
{
}

void sys_initpath(char *exe)
{
}

void sys_sanitize(char *s)
{
}






