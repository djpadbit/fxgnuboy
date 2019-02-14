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
#include "lcd.h"
#include "menu.h"

#define NB_FRAMES_HOLD 10

unsigned long timertime = 0;
// 0 -> normal game
// 1 -> offset adjust
// 2 -> scaling adjust
static int input_mode = 0;
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
	// Will be kept between function calls
	static uint8_t keypress[4];
	int ret;
	event_t e;
	e = pollevent();
	while(1)
	{
		if (e.type == event_none)
			break;

		if (e.type == event_key_press) {
			switch (e.key.code) {
				case KEY_SHIFT:
					if (!input_mode) hw.pad|=PAD_A;
					break;
				case KEY_ALPHA:
					if (!input_mode) hw.pad|=PAD_B;
					break;
				case KEY_OPTN:
					if (!input_mode) hw.pad|=PAD_SELECT;
					break;
				case KEY_VARS:
					if (!input_mode) hw.pad|=PAD_START;
					break;
				case KEY_LEFT:
					if (!input_mode) hw.pad|=PAD_LEFT;
					else keypress[0] = 1;
					break;
				case KEY_RIGHT:
					if (!input_mode) hw.pad|=PAD_RIGHT;
					else keypress[1] = 1;
					break;
				case KEY_DOWN:
					if (!input_mode) hw.pad|=PAD_DOWN;
					else keypress[2] = 1;
					break;
				case KEY_UP:
					if (!input_mode) hw.pad|=PAD_UP;
					else keypress[3] = 1;
					break;
				case KEY_F1:
					input_mode = 0;
					break;
				case KEY_F2:
					input_mode = 1;
					break;
				case KEY_F3:
					input_mode = 2;
					break;
				case KEY_F5: // Faster for debug, in futur change in settings 
					lcd_update_gray(!lcd_gray_enabled);
					break;
				case KEY_F6:
					lcd_show_debug_info = !lcd_show_debug_info;
					break;
				case KEY_MENU:
					ret = menu_pause();
					if (ret!=EMU_RUN_CONT) return ret;
					break;
				case KEY_EXIT:
					return EMU_RUN_EXIT;
			}
		}

		else if (e.type == event_key_release) {
			switch (e.key.code) {
				case KEY_SHIFT:
					if (!input_mode) hw.pad^=PAD_A;
					break;
				case KEY_ALPHA:
					if (!input_mode) hw.pad^=PAD_B;
					break;
				case KEY_OPTN:
					if (!input_mode) hw.pad^=PAD_SELECT;
					break;
				case KEY_VARS:
					if (!input_mode) hw.pad^=PAD_START;
					break;
				case KEY_LEFT:
					if (!input_mode) hw.pad^=PAD_LEFT;
					else keypress[0] = 0;
					break;
				case KEY_RIGHT:
					if (!input_mode) hw.pad^=PAD_RIGHT;
					else keypress[1] = 0;
					break;
				case KEY_DOWN:
					if (!input_mode) hw.pad^=PAD_DOWN;
					else keypress[2] = 0;
					break;
				case KEY_UP:
					if (!input_mode) hw.pad^=PAD_UP;
					else keypress[3] = 0;
					break;
			}
		}
		e = pollevent();
	}
	// This is really bad, but it works for now
	if (keypress[0]) {
		if (keypress[0] > NB_FRAMES_HOLD || keypress[0]==1) {
			if (input_mode == 1) lcd_xoff--;
			else if (input_mode == 2) {lcd_scalex--;lcd_update_scaling();}
			if (keypress[0]==1) keypress[0]++;
		} else {
			keypress[0]++;
		}
	}
	if (keypress[1]) {
		if (keypress[1] > NB_FRAMES_HOLD || keypress[1]==1) {
			if (input_mode == 1) lcd_xoff++;
			else if (input_mode == 2) {lcd_scalex++;lcd_update_scaling();}
			if (keypress[1]==1) keypress[1]++;
		} else {
			keypress[1]++;
		}
	}
	if (keypress[2]) {
		if (keypress[2] > NB_FRAMES_HOLD || keypress[2]==1) {
			if (input_mode == 1) lcd_yoff++;
			else if (input_mode == 2) {lcd_scaley++;lcd_update_scaling();}
			if (keypress[2]==1) keypress[2]++;
		} else {
			keypress[2]++;
		}
	}
	if (keypress[3]) {
		if (keypress[3] > NB_FRAMES_HOLD || keypress[3]==1) {
			if (input_mode == 1) lcd_yoff--;
			else if (input_mode == 2) {lcd_scaley--;lcd_update_scaling();}
			if (keypress[3]==1) keypress[3]++;
		} else {
			keypress[3]++;
		}
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






