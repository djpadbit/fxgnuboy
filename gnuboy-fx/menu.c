#include "defs.h"
#include "emu.h"
#include "disp.h"
#include <keyboard.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
	mupdate();
}

int menu_chooser(const char** choices, int nbchoices, const char* title)
{
	mclear();
	int sel=0;
	int key;
	while (1) {
		mprintp((DWIDTH/2)-(text_length(title)/2), 0,title);
		for (int i=0;i<nbchoices;i++) mprint(1,i+2,choices[i]);
		mrect(1, (sel+2) * 8 - 8,22 * 6 - 5, ((sel+3) * 8 - 8)-1,color_invert);
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

int menu_pause()
{
	const char *opts[4] = {"Saves (Not impl.)","Change ROM","Configuration","Quit"};
	int ret = menu_chooser(opts,4,"Menu");
	switch (ret) {
		case 0:
			return EMU_RUN_CONT;
		case 1:
			return EMU_RUN_CONT;
		case 2:
			return EMU_RUN_CONT;
		case 3:
			return EMU_RUN_EXIT;
		default:
			return EMU_RUN_CONT;
	}
}

void menu_config()
{

}