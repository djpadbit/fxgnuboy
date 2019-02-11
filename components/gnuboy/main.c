#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>

#include "sys.h"
#include "emu.h"
#include "loader.h"

#define VERSION "1.0.4"

void die(char *fmt, ...)
{
	/*va_list ap;
	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	printf("Dying.\n");
	asm("break.n 1");*/
}

void mprint();

void startEmuHook();

int gnuboymain(char *rom, int loadState)
{
	int ret;
	vid_preinit();
	vid_init();
	pcm_init();
	sys_sanitize(rom);
	int r=loader_init(rom);
	if (!r) {
		dclear();
		mprint(1,1,"ROM FAILED");
		dupdate();
		getkey();
		//printf("Loader could not load ROM %s!\n", rom);
		ret=EMU_RUN_NEWROM;
		goto err;
	}
	dclear();
	mprint(1,1,"ROM LOADED");
	dupdate();
	getkey();
	emu_reset();
	startEmuHook();
	mprint(1,2,"HOOK DONE");
	dupdate();
	getkey();
	if (!loadState) emu_reset();
	ret=emu_run();
	dclear();
	mprint(1,1,"EMU FINISHED");
	dupdate();
	getkey();
err:
	rom_unload();
	vid_close();
	pcm_close();
	return ret;
}











