#include <stdlib.h>
#include <stdio.h>
#include "fb.h"
#include "lcd.h"
#include "fxsys.h"
#include "hw.h"
#include <string.h>
#include <display.h>
#include <rtc.h>
#include <timer.h>
#include <gray.h>


static uint8_t __attribute__((section (".magic_sec"))) buff[160*144];

struct fb fb;


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

	fb.cc[0].r = 5;
	fb.cc[1].r = 5;
	fb.cc[2].r = 6;
	fb.cc[0].l = 5;
	fb.cc[1].l = 2;
	fb.cc[2].l = 0;

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


void vid_begin()
{
//	vram_dirty(); //use this to find out a viable size for patcache
	frames++;
	patcachemiss=0;
	patcachehit=0;
	//esp_task_wdt_feed();
}
// RGB to Y conversion for 8 bit color
// Not really needed ? Maybe for GBC games
/*const uint8_t grayscale[256] = {0, 6, 12, 18, 26, 32, 38, 44, 52, 58, 64, 70, 78, 84, 90, 96, 104, 110, 116, 122, 130, 136, 142, 148, 156, 162, 168, 174, 182, 188, 194,
								200, 7, 13, 19, 26, 33, 39, 46, 52, 59, 65, 72, 78, 85, 92, 98, 104, 111, 118, 124, 130, 138, 144, 150, 156, 164, 170, 176, 182, 190,
								196, 202, 208, 15, 21, 27, 33, 41, 47, 53, 59, 67, 73, 79, 85, 93, 99, 105, 112, 119, 125, 131, 138, 145, 151, 158, 164, 171, 177, 184,
								190, 197, 204, 210, 216, 23, 29, 35, 41, 49, 55, 61, 67, 75, 81, 87, 93, 101, 107, 113, 119, 127, 133, 139, 145, 153, 159, 165, 171, 179,
								185, 191, 197, 205, 211, 217, 224, 30, 37, 43, 49, 57, 63, 69, 75, 83, 89, 95, 101, 109, 115, 121, 127, 135, 141, 147, 153, 161, 167, 173,
								179, 187, 193, 199, 205, 213, 219, 225, 231, 38, 44, 50, 57, 64, 70, 77, 83, 90, 96, 103, 109, 116, 123, 129, 135, 142, 149, 155, 161, 169,
								175, 181, 187, 195, 201, 207, 213, 221, 227, 233, 239, 46, 52, 58, 64, 72, 78, 84, 90, 98, 104, 110, 116, 124, 130, 136, 143, 150, 156, 162,
								169, 176, 182, 189, 195, 202, 208, 215, 221, 228, 235, 241, 247, 54, 60, 66, 72, 80, 86, 92, 98, 106, 112, 118, 124, 132, 138, 144, 150, 158,
								164, 170, 176, 184, 190, 196, 202, 210, 216, 222, 228, 236, 242, 248, 255};


static inline void gbc_render()
{
	for (int x=0;x<160;x++) {
		for (int y=0;y<144;y++) {
			if (y < 4*8 && x < 6*5) continue;
			int val = grayscale[buff[(y*160)+x]];
			if (val >= 192) gpixel(x,y,color_black);
			else if (val >= 128) gpixel(x,y,color_dark);
			else if (val >= 64) gpixel(x,y,color_light);
		}

	}
}*/

static inline void gb_render()
{
	for (int x=0;x<160;x++) {
		for (int y=0;y<144;y++) {
			if (y < 4*8 && x < 6*5) continue;
			int val = (buff[(y*160)+x]>>2)&0x7;
			//7,6,4,2,0
			if (val > 6) gpixel(x,y,color_black);
			else if (val > 4) gpixel(x,y,color_dark);
			else if (val > 2) gpixel(x,y,color_light);
		}
	}
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
	static unsigned long framet1,framet2;
	framet2 = timertime;
	int fps = 1.0/((framet2-framet1)/(float)(TIMER_FREQ));
	gclear();
	mprint(1,1,"%i",fps);
	mprint(1,2,"%i",frames);
	mprint(1,3,"m:%i",patcachemiss);
	mprint(1,4,"h:%i",patcachehit);
	if (!hw.cgb) gb_render();
	//else gbc_render();
	gupdate();
	framet1 = timertime;
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


//Averages four pixels into one
int getAvgPix(uint16_t* bufs, int pitch, int x, int y) {
	int col;
	if (x<0 || x>=160) return 0;
	//16-bit: E79C
	//15-bit: 739C
	col=(bufs[x+(y*(pitch>>1))]&0xE79C)>>2;
	col+=(bufs[(x+1)+(y*(pitch>>1))]&0xE79C)>>2;
	col+=(bufs[x+((y+1)*(pitch>>1))]&0xE79C)>>2;
	col+=(bufs[(x+1)+((y+1)*(pitch>>1))]&0xE79C)>>2;
	return col&0xffff;
}

//Averages four pixels into one, but does subpixel rendering to give a slightly higher
//X resolution at the cost of color fringing.
//Bitmasks:
//RRRR.RGGG.GGGB.BBBB
//1111.0111.1101.1110 = F7DE
//0000.1000.0010.0001 = 0821
//1111.1000.0000.0000 = F800
//0000.0111.1110.0000 = 07E0
//0000.0000.0001.1111 = 001F
//so (RGB565val&0xF7DE)>>1 halves the R, G, B color components.
int getAvgPixSubpixrendering(uint16_t* bufs, int pitch, int x, int y) {
	uint32_t *pixduo=(uint32_t*)bufs;
	if (x<0 || x>=160) return 0;
	//Grab top and bottom two pixels.
	uint32_t c1=pixduo[(x/2)+(y*(pitch>>2))];
	uint32_t c2=pixduo[(x/2)+((y+1)*(pitch>>2))];
	//Average the two.
	uint32_t c=((c1&0xF7DEF7DE)+(c2&0xF7DEF7DE))>>1;
	//The averaging action essentially killed the least significant bit of all colors; if
	//both were one the resulting color should be one more. Compensate for that here.
	c+=(c1&c1)&0x08210821;

	//Take the various components from the pixels and return the composite.
	uint32_t red_comp=c&0xF800;
	uint32_t green_comp=c&0x07E0;
	green_comp+=(c>>16)&0x07E0;
	green_comp=(green_comp/2)&0x7E0;
	uint32_t blue_comp=(c>>16)&0x001F;
	return red_comp+green_comp+blue_comp;
}

//Averages 6 pixels into one (area of w=2, h=3), but does subpixel rendering to give a slightly higher
//X resolution at the cost of color fringing. This is slightly more elaborate as we cannot just use additions,
//shifts and bitmasks.
#define RED(i) (((i)>>11) & 0x1F)
#define GREEN(i) (((i)>>5) & 0x3F)
#define BLUE(i) (((i)>>0) & 0x1F)
int getAvgPixSubpixrenderingThreeLines(uint16_t* bufs, int pitch, int x, int y) {
	int r=0, g=0, b=0;
	for (int line=0; line<3; line++) {
		r+=RED(bufs[x+((y+line)*(pitch>>1))]);
		g+=GREEN(bufs[x+((y+line)*(pitch>>1))]);
		g+=GREEN(bufs[x+1+((y+line)*(pitch>>1))]);
		b+=BLUE(bufs[x+1+((y+line)*(pitch>>1))]);
	}
	r=r/3;
	g=g/6;
	b=b/3;
	return (r<<11)+(g<<5)+(b);
}



int addOverlayPixel(uint16_t p, uint32_t ov) {
	int or, og, ob, a;
	int br, bg, bb;
	int r,g,b;
	br=((p>>11)&0x1f)<<3;
	bg=((p>>5)&0x3f)<<2;
	bb=((p>>0)&0x1f)<<3;

	a=(ov>>24)&0xff;
	//hack: Always show background darker
	a=(a/2)+128;

	ob=(ov>>16)&0xff;
	og=(ov>>8)&0xff;
	or=(ov>>0)&0xff;

	r=(br*(256-a))+(or*a);
	g=(bg*(256-a))+(og*a);
	b=(bb*(256-a))+(ob*a);

	return ((r>>(3+8))<<11)+((g>>(2+8))<<5)+((b>>(3+8))<<0);
}