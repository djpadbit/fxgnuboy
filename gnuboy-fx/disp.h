#ifndef __DISP_H__
#define __DISP_H__

#include <display.h>

void mprint(int x,int y, const char* fmt, ...);
void mprintp(int x,int y, const char* fmt, ...);
void mtext(int x, int y, const char *str);
void mclear();
void mupdate();
void dline_oth(int x0, int y0, int x1, int y1,color_t op);
void gline_oth(int x0, int y0, int x1, int y1,color_t op);
void mline_oth(int x0, int y0, int x1, int y1,color_t op);
void mline(int x1, int y1, int x2, int y2, color_t _operator);
void mrect(int x1, int y1, int x2, int y2, color_t _operator);

#endif