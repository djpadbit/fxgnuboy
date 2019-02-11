#include "emu.h"
#include "defs.h"
#include "regs.h"
#include "mem.h"
#include "hw.h"
#include "rtc.h"
#include "lcd.h"
#include "inflate.h"
#include "save.h"
#include "sound.h"
#include "sys.h"
#include "rombank.h"
#include "save.h"
#include "hw.h"
#include <string.h>
#include <display.h>
#include <keyboard.h>

int frames;

int main()
{
	/*hw.gbbootromdata=NULL;
	if (appfsExists(BOOTROM_NAME)) {
		appfs_handle_t fd=appfsOpen(BOOTROM_NAME);
		err=appfsMmap(fd, 0, 2304, (const void**)&hw.gbbootromdata, SPI_FLASH_MMAP_DATA, &hbootrom);
		if (err==ESP_OK) {
			printf("Initialized. bootROM@%p\n", hw.gbbootromdata);
		} else {
			printf("Couldn't map bootrom appfs file!\n");
		}
	} else {
		printf("No bootrom found!\n");
	}
	xTaskCreatePinnedToCore(&lineTask, "lineTask", 1024, NULL, 6, NULL, 1);
	xTaskCreatePinnedToCore(&gnuboyTask, "gnuboyTask", 1024*6, NULL, 5, NULL, 0);
	xTaskCreatePinnedToCore(&monTask, "monTask", 1024*2, NULL, 7, NULL, 0);*/
	uint8_t *magic = (uint8_t*)0x88040000;
	memset(magic,0,256*1024);
	gnuboymain("sml.gb",0);
	return 0;
}

void startEmuHook() {
	/*const esp_partition_t* part;
	if (kchal_get_keys()&KC_BTN_START) return;
	if (appfsExists(statefile)) {
		loadstate(appfsOpen(statefile));
		rombankStateLoaded();
	} else {
		printf("Couldn't find state part!\n");
	}*/
	vram_dirty();
	pal_dirty();
	sound_dirty();
	mem_updatemap();
	//printf("Save state boot rom loaded: %d\n", bootromLoaded);
}