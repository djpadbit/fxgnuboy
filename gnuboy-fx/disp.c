#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "defs.h"
#include <keyboard.h>
#include <display.h>
#include <gray.h>
#include "lcd.h"

void mprint(int x,int y, const char* fmt, ...)
{
	if(x < 1 || x > 21 || y < 1 || y > 8) return;
	char k[50];
	va_list args;
	va_start(args, fmt);
	vsprintf(k,fmt,args);
	va_end(args);
	if (lcd_gray_enabled) gtext(x * 6 - 5, y * 8 - 8, k);
	else dtext(x * 6 - 5, y * 8 - 8, k);
}

void mprintp(int x,int y, const char* fmt, ...)
{
	if(x < 0 || x > 128 || y < 0 || y > 64) return;
	char k[50];
	va_list args;
	va_start(args, fmt);
	vsprintf(k,fmt,args);
	va_end(args);
	if (lcd_gray_enabled) gtext(x, y, k);
	else dtext(x, y, k);
}

void mtext(int x, int y, const char *str)
{
	if (lcd_gray_enabled) gtext(x, y, str);
	else dtext(x, y, str);
}

void mclear()
{
	if (lcd_gray_enabled) gclear();
	else dclear();
}

void mupdate()
{
	if (lcd_gray_enabled) gupdate();
	else dupdate();
}

void mline(int x1, int y1, int x2, int y2, color_t _operator)
{
	if (lcd_gray_enabled) gline(x1,y1,x2,y2,_operator);
	else dline(x1,y1,x2,y2,_operator);
}

void mrect(int x1, int y1, int x2, int y2, color_t _operator)
{
	if (lcd_gray_enabled) grect(x1,y1,x2,y2,_operator);
	else drect(x1,y1,x2,y2,_operator);	
}