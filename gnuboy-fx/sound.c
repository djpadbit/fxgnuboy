#include "defs.h"
#include "pcm.h"
#include <string.h>
#include <stdio.h>
#include "lcd.h"

struct pcm pcm;

//Keep this preferably very short. No idea why, but >64'ish impacts frame rate.
#define SNDBUFLEN 8

//static uint8_t buf[SNDBUFLEN];

void pcm_init()
{
	/*pcm.buf = buf;
	pcm.len = SNDBUFLEN;
	pcm.stereo = 0;
	pcm.pos = 0;

	pcm.hz = 31250;
	kchal_sound_start(pcm.hz, 1024);*/
}

void pcm_close()
{
	/*memset(&pcm, 0, sizeof pcm);
	kchal_sound_stop();*/
}

//Hackish: send 0-samples
void pcm_mute() {
	/*for (int i=0; i<SNDBUFLEN; i++) buf[i]=128;
	kchal_sound_push(buf, SNDBUFLEN);
	pcm.pos = 0;*/
}

int pcm_submit()
{
	return !lcd_fps_regul; //Really dirty hack to change wether or not to regulate fps
	/*kchal_sound_push(buf, SNDBUFLEN);
	pcm.pos = 0;
	return 1;*/
}





