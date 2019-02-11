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

void mprint(int x,int y, const char* fmt, ...)
{
	if(x < 1 || x > 21 || y < 1 || y > 8) return;
	char k[50];
	va_list args;
	va_start(args, fmt);
	vsprintf(k,fmt,args);
	va_end(args);
	dtext(x * 6 - 5, y * 8 - 8, k);
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

		if (e.type == event_key_release) {
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






