#include <stdlib.h>
#include <stdio.h>
#include "fb.h"
#include "lcd.h"
#include "fxsys.h"
#include "disp.h"
#include "hw.h"
#include <string.h>
#include <display.h>
#include <keyboard.h>
#include <rtc.h>
#include <timer.h>
#include <gray.h>


static uint8_t __attribute__((section (".magic_sec"))) buff[160*144];

struct fb fb;

int lcd_xoff,lcd_yoff;
int lcd_scalex,lcd_scaley;
static int lcd_oscalex,lcd_oscaley;
uint8_t lcd_show_debug_info;
uint8_t lcd_gray_enabled = 0;
uint8_t lcd_fps_regul = 1;
static unsigned char scalearrx[160];
static unsigned char scalearry[144];

void lcd_gen_scale_arr(unsigned char w,unsigned char h)
{
	memset(scalearrx,255,160);
	memset(scalearry,255,144);
	float x_ratio = 160.0/(float)w;
	float y_ratio = 144.0/(float)h;
	for (int i=0;i<h;i++) {
		for (int j=0;j<w;j++) {
			scalearrx[(int)(j*x_ratio)] = j;
			scalearry[(int)(i*y_ratio)] = i;
		}
	}
}

void lcd_update_scaling()
{
	if (lcd_scalex == lcd_oscalex && lcd_scaley == lcd_oscaley) return;
	if (lcd_scalex < 0) lcd_scalex = 0;
	else if (lcd_scalex > 160) lcd_scalex = 160;
	if (lcd_scaley < 0) lcd_scaley = 0;
	else if (lcd_scaley > 144) lcd_scaley = 144;
	if (lcd_scalex == lcd_oscalex && lcd_scaley == lcd_oscaley) return;
	lcd_oscalex = lcd_scalex;
	lcd_oscaley = lcd_scaley;
	lcd_gen_scale_arr(lcd_scalex,lcd_scaley);
}

void lcd_update_gray(uint8_t state)
{
	if (state == lcd_gray_enabled) return;
	if (!state) gray_stop();
	else gray_start();
	lcd_gray_enabled = state;
}

void vid_preinit()
{
	timer_setup();
}

void vid_init()
{
	//doShutdown=false;
	/*frontbuff=malloc(160*144*2);
	backbuff=malloc(160*144*2);
	fb.w = 160;
	fb.h = 144;
	fb.pelsize = 2;
	fb.pitch = 160*2;
	fb.ptr = (unsigned char*)frontbuff;
	fb.enabled = 1;
	fb.dirty = 0;

	fb.indexed = 0;
	fb.cc[0].r = fb.cc[2].r = 3;
	fb.cc[1].r = 2;
	fb.cc[0].l = 11;
	fb.cc[1].l = 5;
	fb.cc[2].l = 0;
	
	gbfemtoMenuInit();
	memset(frontbuff, 0, 160*144*2);
	memset(backbuff, 0, 160*144*2);*/

	/*renderSem=xSemaphoreCreateBinary();
	xTaskCreatePinnedToCore(&videoTask, "videoTask", 1024*2, NULL, 5, NULL, 1);
	ets_printf("Video inited.\n");*/

	fb.w = 160;
	fb.h = 144;
	fb.pelsize = 1;
	fb.pitch = fb.w * fb.pelsize;
	fb.indexed = 0;
	fb.enabled = 1;
	fb.dirty = 0;
	fb.ptr = (unsigned char*)&buff;

	// RRRGGGBB
	// Use only 8 bits to save space and the calculator is monochrome anyways
	fb.cc[0].r = 5;
	fb.cc[1].r = 5;
	fb.cc[2].r = 6;
	fb.cc[0].l = 5;
	fb.cc[1].l = 2;
	fb.cc[2].l = 0;

	lcd_xoff = 29;
	lcd_yoff = 0;
	lcd_scalex = 71;
	lcd_scaley = 64;
	lcd_update_scaling();

	lcd_show_debug_info = 1;
	lcd_update_gray(1);

	/*int mask[3];
	mask[0] = 0xe0;
	mask[1] = 0x1c;
	mask[2] = 0x3;

	for (int i = 0; i < 3; i++)
	{
		int l,c;
		for (l = 0; l < 32 && !((1<<l) & mask[i]); l++);
		for (c = 0; l+c < 32 && ((1<<(l+c)) & mask[i]); c++);
		fb.cc[i].l = l;
		fb.cc[i].r = 8-c;
	}*/
	
}

void vid_close()
{
	/*doShutdown=true;
	xSemaphoreGive(renderSem);
	vTaskDelay(100); //wait till video thread shuts down... pretty dirty
	free(frontbuff);
	free(backbuff);
	vQueueDelete(renderSem);*/

	memset(&buff,0,160*144);
	timer_cleanup();
}

void vid_settitle(char *title)
{
}

void vid_setpal(int i, int r, int g, int b)
{
}

extern int patcachemiss, patcachehit, frames;
extern int rombankmiss, rombankhit;
static int fps;

void vid_begin()
{
//	vram_dirty(); //use this to find out a viable size for patcache
	frames++;
	patcachemiss=0;
	patcachehit=0;
	rombankmiss=0;
	rombankhit=0;
	//esp_task_wdt_feed();
	static unsigned long framet;
	fps = 1.0/((timertime-framet)/((float)(TIMER_FREQ)));
	framet = timertime;
}

// RGB to Y conversion for 8 bit color
// Not really needed ? Maybe for GBC games
const uint8_t grayscale[256] = {255, 249, 243, 237, 229, 223, 217, 211, 203, 197, 191, 185, 177, 171, 165, 159, 151, 145, 139, 133, 125, 119, 113, 107, 99, 93, 87, 81, 73, 67, 61,
								55, 248, 242, 236, 229, 222, 216, 209, 203, 196, 190, 183, 177, 170, 163, 157, 151, 144, 137, 131, 125, 117, 111, 105, 99, 91, 85, 79, 73, 65,
								59, 53, 47, 240, 234, 228, 222, 214, 208, 202, 196, 188, 182, 176, 170, 162, 156, 150, 143, 136, 130, 124, 117, 110, 104, 97, 91, 84, 78, 71,
								65, 58, 51, 45, 39, 232, 226, 220, 214, 206, 200, 194, 188, 180, 174, 168, 162, 154, 148, 142, 136, 128, 122, 116, 110, 102, 96, 90, 84, 76,
								70, 64, 58, 50, 44, 38, 31, 225, 218, 212, 206, 198, 192, 186, 180, 172, 166, 160, 154, 146, 140, 134, 128, 120, 114, 108, 102, 94, 88, 82,
								76, 68, 62, 56, 50, 42, 36, 30, 24, 217, 211, 205, 198, 191, 185, 178, 172, 165, 159, 152, 146, 139, 132, 126, 120, 113, 106, 100, 94, 86,
								80, 74, 68, 60, 54, 48, 42, 34, 28, 22, 16, 209, 203, 197, 191, 183, 177, 171, 165, 157, 151, 145, 139, 131, 125, 119, 112, 105, 99, 93,
								86, 79, 73, 66, 60, 53, 47, 40, 34, 27, 20, 14, 8, 201, 195, 189, 183, 175, 169, 163, 157, 149, 143, 137, 131, 123, 117, 111, 105, 97,
								91, 85, 79, 71, 65, 59, 53, 45, 39, 33, 27, 19, 13, 7, 0};


static inline void gbc_grender()
{
	for (int x=0;x<160;x++) {
		for (int y=0;y<144;y++) {
			int sax = scalearrx[x];
			int say = scalearry[y];
			if (sax == 255 || say == 255) continue;
			int val = grayscale[buff[(y*160)+x]];
			int xo = sax + lcd_xoff;
			int yo = say + lcd_yoff;
			if (val >= 192) gpixel(xo,yo,color_black);
			else if (val >= 128) gpixel(xo,yo,color_dark);
			else if (val >= 64) gpixel(xo,yo,color_light);
		}
	}
}

static inline void gbc_mrender()
{
	for (int x=0;x<160;x++) {
		for (int y=0;y<144;y++) {
			int sax = scalearrx[x];
			int say = scalearry[y];
			if (sax == 255 || say == 255) continue;
			if (grayscale[buff[(y*160)+x]] >= 128) dpixel(sax + lcd_xoff,say + lcd_yoff,color_black);
		}
	}
}

static inline void gbc_render()
{
	if (lcd_gray_enabled) gbc_grender();
	else gbc_mrender();
}

static inline void gb_grender()
{
	for (int x=0;x<160;x++) {
		for (int y=0;y<144;y++) {
			int sax = scalearrx[x];
			int say = scalearry[y];
			if (sax == 255 || say == 255) continue;
			int val = (buff[(y*160)+x]>>2)&0x7;
			int xo = sax + lcd_xoff;
			int yo = say + lcd_yoff;
			//7,6,4,2,0
			if (val > 6) gpixel(xo,yo,color_black);
			else if (val > 4) gpixel(xo,yo,color_dark);
			else if (val > 2) gpixel(xo,yo,color_light);
		}
	}
}

static inline void gb_mrender()
{
	for (int x=0;x<160;x++) {
		for (int y=0;y<144;y++) {
			int sax = scalearrx[x];
			int say = scalearry[y];
			if (sax == 255 || say == 255) continue;
			//7,6,4,2,0
			if (((buff[(y*160)+x]>>2)&0x7) > 4) dpixel(sax + lcd_xoff,say + lcd_yoff,color_black);
		}
	}
}

static inline void gb_render()
{
	if (lcd_gray_enabled) gb_grender();
	else gb_mrender();
}

static inline void debug_render()
{
	mprint(1,1,"%i",fps);
	mprint(1,2,"%i",lcd_xoff);
	mprint(1,3,"%i",lcd_yoff);
	mprint(1,4,"%i",lcd_scalex);
	mprint(1,5,"%i",lcd_scaley);
	mprint(1,6,"%i",patcachemiss);
	mprint(1,7,"%i",patcachehit);
	mprint(1,8,"%i,%i",rombankmiss,rombankhit);
}

void vid_end()
{
	/*overlay=NULL;
	toRender=(uint16_t*)fb.ptr;
	xSemaphoreGive(renderSem);
	if (fb.ptr == (unsigned char*)frontbuff ) {
		fb.ptr = (unsigned char*)backbuff;
	} else {
		fb.ptr = (unsigned char*)frontbuff;
	}*/

	mclear();
	if (lcd_show_debug_info) debug_render();
	if (!hw.cgb) gb_render();
	else gbc_render();
	mupdate();
//	printf("Pcm %d pch %d\n", patcachemiss, patcachehit);
}

void kb_init()
{
}

void kb_close()
{
}

void kb_poll()
{
}

void ev_poll()
{
	kb_poll();
}