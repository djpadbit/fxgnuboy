#include "defs.h"
#include "emu.h"
#include "disp.h"
#include "lcd.h"
#include <keyboard.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int keyb_input(char* buf,size_t len,const char* ask)
{
	int key,ptr;
	ptr = 0;
	key = 0;
	int ret = 1;
	int run = 1;
	int lower = 1;
	int shift = 0;
	int ls = 0;
	int alpha = 0;
	int la = 0;
	memset(buf,0,len);
	while (run) {
		mclear();
		mprint(1,1,ask);
		mprint(1,2,buf);
		mline(ptr*6,8,ptr*6,7+7,color_black);
		if (lower) mprint(1,3,"Lowercase");
		else mprint(1,3,"Uppercase");
		mprint(1,4,"Use OPTN to switch");
		mprint(1,5,alpha == 2 ? "Alpha lock" : alpha == 1 ? "Alpha" : shift ? "Shift" : "");
		mupdate();
		key = getkey_opt(getkey_none,0);
		switch (key) {
			case KEY_LEFT:
				if (ptr > 0) ptr--;
				break;
			case KEY_RIGHT: 
				if (buf[ptr] != 0) ptr++;
				break;
			case KEY_EXIT:
				ret = 0;
				run = 0;
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
	mclear();
	return ret;
}

void menu_error(const char* first, const char* second)
{
	mclear();
	if (!second) {
		mprintp((DWIDTH/2)-(text_length(first)/2), (DHEIGHT/2)-4,first);
	} else {
		mprintp((DWIDTH/2)-(text_length(first)/2), ((DHEIGHT/2)-4)-4,first);
		mprintp((DWIDTH/2)-(text_length(second)/2), ((DHEIGHT/2)-4)+4,second);
	}
	mupdate();
	getkey_opt(getkey_none,0);
}

int menu_chooser(const char** choices, int nbchoices, const char* title, int start)
{
	mclear();
	int sel=start;
	int key;
	while (1) {
		mprintp((DWIDTH/2)-(text_length(title)/2), 0,title);
		mrect(0, 0, 128, 7,color_invert);
		//mline(0, 7, 128, 7, color_black);
		for (int i=0;i<nbchoices;i++) mprint(1,i+2,choices[i]);
		mrect(0, (sel+2) * 8 - 8, 128, ((sel+3) * 8 - 8)-1,color_invert);
		mupdate();
		mclear();
		key = getkey_opt(getkey_none,0);
		switch (key) {
			case KEY_EXIT:
				return -1;
			case KEY_DOWN:
				if (sel < nbchoices-1) sel++;
				break;
			case KEY_UP:
				if (sel > 0) sel--;
				break;
			case KEY_EXE:
				return sel;
				break;
		}
	}
}

void menu_config()
{
	int ret = 0;
	while (1) {
		const char *opts[] = {"<--",lcd_gray_enabled ? "Gray Disable" : "Gray Enable"};
		ret = menu_chooser(opts,2,"Config",ret);
		switch (ret) {
			case 0:
				return;
			case 1:
				lcd_update_gray(!lcd_gray_enabled);
				break;
			default:
				return;
		}
	}
}

int menu_pause()
{
	int ret = 0;
	while (1) {
		const char *opts[] = {"Saves (Not impl.)","Reset","Change ROM","Configuration","Quit"};
		ret = menu_chooser(opts,5,"Menu",ret);
		switch (ret) {
			case 0:
				return EMU_RUN_CONT;
			case 1:
				return EMU_RUN_RESET;
			case 2:
				return EMU_RUN_NEWROM;
			case 3:
				menu_config();
				break;
			case 4:
				return EMU_RUN_EXIT;
			default:
				return EMU_RUN_CONT;
		}
	}
}