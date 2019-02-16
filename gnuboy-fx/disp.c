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

//Gint's gline and dline don't behave like they should so in the meantime i'll just use that
//https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void dline_oth(int x0, int y0, int x1, int y1,color_t op)
{
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;
	for(;;){
		dpixel(x0,y0,op);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void gline_oth(int x0, int y0, int x1, int y1,color_t op)
{
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;
	for(;;){
		gpixel(x0,y0,op);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void mline_oth(int x0, int y0, int x1, int y1,color_t op)
{
	if (lcd_gray_enabled) gline_oth(x0,y0,x1,y1,op);
	else dline_oth(x0,y0,x1,y1,op);
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