#include "defs.h"
#include "emu.h"
#include "disp.h"
#include "lcd.h"
#include "config.h"
#include "file.h"
#include "menu.h"
#include <save.h>
#include <keyboard.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <bfile.h>

#define MAX_FILES 16
#define MAX_FILES_NAME_LEN 12
#define MIN(a,b) (((a)<(b))?(a):(b))

// This is a mess, i need to rework it to have proper deleting and stuff
int keyb_input(char* buf,size_t len,char* ask)
{
	int mptr = 0;
	int ptr = 0;
	int key = 0;
	int ret = 1;
	int run = 1;
	int lower = 1;
	int shift = 0;
	int ls = 0;
	int alpha = 0;
	int la = 0;
	int alen = strlen(ask);
	memset(buf,0,len+1);
	while (run) {
		mclear();
		int ptrl = 1;
		int lastp = 0;
		for (int i=0;i<alen;i++) {
			if (ask[i] == '\n') {
				ask[i] = 0;
				mprint(1,ptrl++,ask+lastp);
				lastp = i+1;
				ask[i] = '\n';
			}
		}
		if (strlen(ask+lastp) > 0) mprint(1,ptrl++,ask+lastp);
		mline(ptr*6,(ptrl-1)*8,ptr*6,((ptrl-1)*8)+7,color_black);
		mprint(1,ptrl++,buf);
		if (lower) mprint(1,ptrl++,"Lowercase");
		else mprint(1,ptrl++,"Uppercase");
		mprint(1,ptrl++,"Use OPTN to switch");
		mprint(1,ptrl++,alpha == 2 ? "Alpha lock" : alpha == 1 ? "Alpha" : shift ? "Shift" : "");
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
				if (ptr > 0 && ptr==mptr) {
					buf[ptr] = 0;
					buf[--ptr] = 0;
					mptr--;
				}
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
				if (key_char(key) != 0 && ptr < len+1) {
					if (ptr==mptr) buf[ptr+1] = 0;
					buf[ptr++] = lower ? tolower(key_char(key)) : key_char(key);
				}
				break;
		}
		if (ls) shift = 0;
		if (la == 1) alpha = 0;
		ls = shift;
		la = alpha;
		if (ptr>mptr) mptr = ptr;
	}
	mclear();
	return ret;
}

int menu_error(const char* first, const char* second)
{
	mclear();
	const char *errorstr = "ERROR";
	mprintp((DWIDTH/2)-(text_length(errorstr)/2), (DHEIGHT/2)-(8*3),errorstr);
	if (!second) {
		mprintp((DWIDTH/2)-(text_length(first)/2), (DHEIGHT/2)-4,first);
	} else {
		mprintp((DWIDTH/2)-(text_length(first)/2), ((DHEIGHT/2)-4)-4,first);
		mprintp((DWIDTH/2)-(text_length(second)/2), ((DHEIGHT/2)-4)+4,second);
	}
	mupdate();
	return getkey_opt(getkey_none,0);
}

int menu_confirm(const char* first, const char* second)
{
	mclear();
	const char *errorstr = "Confirm";
	const char *ynstr = "[F1]Yes [F6]No";
	mprintp((DWIDTH/2)-(text_length(errorstr)/2), (DHEIGHT/2)-(8*3),errorstr);
	if (!second) {
		mprintp((DWIDTH/2)-(text_length(first)/2), (DHEIGHT/2)-4,first);
	} else {
		mprintp((DWIDTH/2)-(text_length(first)/2), ((DHEIGHT/2)-4)-4,first);
		mprintp((DWIDTH/2)-(text_length(second)/2), ((DHEIGHT/2)-4)+4,second);
	}
	mprintp((DWIDTH/2)-(text_length(ynstr)/2), (DHEIGHT/2)+(8*2),ynstr);
	mupdate();
	int key;
	while (1) {
		key = getkey_opt(getkey_none,0);
		if (key == KEY_F1) return 1;
		else if (key == KEY_F6) return 0;
	}
}

static void draw_arrow(int x,int y,int sizex,int sizey,int dir)
{
	int asx = dir ? -sizex : sizex;
	int asy = dir ? -sizey : sizey;
	mline_oth(x,y,x,y+asy,color_black);
	mline_oth(x-sizex,y+asx,x,y,color_black);
	mline_oth(x+sizex,y+asx,x,y,color_black);
}

int menu_chooser(const char** choices, int nbchoices, const char* title, int start)
{
	mclear();
	int sel=start;
	int scoff=0;
	int key;
	while (1) {
		mprintp((DWIDTH/2)-(text_length(title)/2), 0,title);
		mrect(0, 0, 128, 7,color_invert);
		//mline(0, 7, 128, 7, color_black);
		for (int i=0;i<MIN(nbchoices,7);i++) mprint(1,i+2,choices[i+scoff]);
		if (scoff) draw_arrow(128-5,9,2,5,0);
		if (scoff+7 < nbchoices) draw_arrow(128-5,64-2,2,5,1);
		mrect(0, ((sel-scoff)+2) * 8 - 8, 128, (((sel-scoff)+3) * 8 - 8)-1,color_invert);
		mupdate();
		mclear();
		key = getkey_opt(getkey_none,0);
		switch (key) {
			case KEY_EXIT:
				return -1;
			case KEY_DOWN:
				if (sel < nbchoices-1) {
					sel++;
					if ((sel-scoff) > 6) scoff++;
				}
				break;
			case KEY_UP:
				if (sel > 0) {
					sel--;
					if ((sel-scoff) < 0) scoff--;
				}
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
		const char *opts[] = {"<--","Reset config",lcd_gray_enabled ? "Gray Disable" : "Gray Enable",lcd_fps_regul ? "FPS Regul. Disable" : "FPS Regul. Enable"};
		ret = menu_chooser(opts,4,"Config",ret);
		switch (ret) {
			case 1:
				config_reset();
				break;
			case 2:
				lcd_update_gray(!lcd_gray_enabled);
				break;
			case 3:
				lcd_fps_regul = !lcd_fps_regul;
				break;
			case 0:
			default:
				return;
		}
	}
}

void startEmuHook();

void menu_saves()
{
	int ret = 0;
	int fret = 0;
	char saven[40];
	char askstr[2][40];
	uint16_t path[64];
	while (1) {
		const char *opts[] = {"<--","Load","Save"};
		ret = menu_chooser(opts,3,"Saves",ret);
		switch (ret) {
			case 1:
				if (menu_filechooser("*.sav","Choose save file",(char*)&saven,0)+1) {
					file_make_path(path,"fls0","",(char*)&saven);
					int fd = BFile_Open(path,BFile_ReadOnly);
					if (fd<0) {
						BFile_Remove(path);
						menu_error("Coudln't open file","File doesn't exist ?");
						break;
					}
					if (!loadstate(fd)) {
						BFile_Close(fd);
						menu_error("Coudln't load file","Older save version ?");
						break;
					}
					BFile_Close(fd);
					startEmuHook(); // To set the dirty flag on all the things so it clears all the caches
				}
				break;
			case 2:
				while (1) {
					fret = menu_filechooser("*.sav","Choose save file",(char*)&saven,fret);
					if (fret<0) break;
					sprintf(askstr[0],"Save '%s'?",saven);
					sprintf(askstr[1],"Requires %ib",statesize());
					if (!menu_confirm(askstr[0],askstr[1])) continue;
					file_make_path(path,"fls0","",(char*)&saven);
					BFile_Remove(path);
					int asize = statesize();
					int size = asize;
					int fd = BFile_Create(path,BFile_File,&size);
					if (size != asize || fd<0) {
						BFile_Remove(path);
						menu_error("Coudln't make file","Not enough space ?");
						break;
					}
					fd = BFile_Open(path,BFile_WriteOnly);
					if (fd<0) {
						BFile_Remove(path);
						menu_error("Coudln't open file","Not enough space ?");
						break;
					}
					if (!savestate(fd)) {
						BFile_Close(fd);
						BFile_Remove(path);
						menu_error("Coudln't save file","Not enough memory ?");
						break;
					}
					BFile_Close(fd);
				}
				break;
			case 0:
			default:
				return;
		}
	}
}

int menu_filechooser(char *pathc,char *title,char *choosen,int start)
{
	uint16_t path[40];
	uint16_t foundfile[40];
	int fhandle;
	bfile_info fileinfo;
	char *files[MAX_FILES+1] = {NULL};
	files[0] = "Manual entry";
	int ret;
	int i=1;
	char *old = pathc;
	char *curr = NULL;
	for (;i<MAX_FILES+1&&old;) {
		curr = (char*)strchr(old,';');
		if (curr) {
			*curr = 0;
		}
		file_make_path(path,"fls0","",old);
		if (curr) {
			old = curr+1;
		} else {
			old = curr;
		}
		for (int f=0;i<MAX_FILES+1;f++) {
			if (f==0) ret = BFile_FindFirst(path,&fhandle,foundfile,&fileinfo);
			else ret = BFile_FindNext(fhandle,foundfile,&fileinfo);
			if (ret < 0) break;
			files[i] = (char*)calloc(1, MAX_FILES_NAME_LEN+1);
			if (files[i] == NULL) return -1;
			file_fc_to_char((uint16_t*)&foundfile,files[i++]);
		}
		BFile_FindClose(fhandle);
	}
	while (1) {
		ret = menu_chooser((const char**)&files,i,title,start);
		if (ret > 0) {
			memcpy(choosen,files[ret],MAX_FILES_NAME_LEN+1);
			break;
		} else if (!ret) {
			int kret = keyb_input(choosen,MAX_FILES_NAME_LEN,"Enter file name");
			if (kret) break;
			else start = 0;
		} else {
			break;
		}
	}
	for (int i=1;i<MAX_FILES+1;i++) {
		if (files[i] != NULL) free(files[i]);
	}
	return ret;
}

int menu_pause()
{
	int ret = 0;
	while (1) {
		const char *opts[] = {"Saves","Reset","Change ROM","Configuration","Quit"};
		ret = menu_chooser(opts,5,"Menu",ret);
		switch (ret) {
			case 0:
				menu_saves();
				break;
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