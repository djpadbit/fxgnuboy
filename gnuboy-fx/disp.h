#ifndef __DISP_H__
#define __DISP_H__

#include <gint/display.h>

#define mprint(x,y,fmt, ...) dprint((x)*6-5, (y)*8-8, C_BLACK, fmt,  ##__VA_ARGS__)
#define mprintp(x,y,fmt, ...) dprint(x, y, C_BLACK, fmt,  ##__VA_ARGS__)
#define mclear() dclear(C_WHITE)
#define mupdate() dupdate()
#define mline(x0,y0,x1,y1,op) dline(x0,y0,x1,y1,op);
#define mrect(x0,y0,x1,y1,op) drect(x0,y0,x1,y1,op);

int text_length(const char *str);

#endif