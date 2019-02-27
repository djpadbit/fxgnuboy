#include "emu.h"
#include "defs.h"
#include "regs.h"
#include "mem.h"
#include "hw.h"
#include "rtc.h"
#include <lcd.h>
#include "inflate.h"
#include "save.h"
#include <sound.h>
#include "sys.h"
#include "rombank.h"
#include "save.h"
#include "hw.h"
#include "menu.h"
#include "disp.h"
#include "loader.h"
#include "config.h"
#include <string.h>
#include <display.h>
#include <keyboard.h>
#include <gray.h>
#include <stdlib.h>

unsigned int frames;
uint8_t config_loaded = 0;

void die(char *fmt, ...)
{
	char tmp[50];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(tmp,fmt,ap);
	va_end(ap);
	while (menu_error(tmp,"Press exit to quit") != KEY_EXIT);
	exit(1);
}

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
	//Clear the magic buffer area, just in case
	memset((uint8_t*)0x88040000,0,256*1024);

	char romn[40];
	int ret = EMU_RUN_NEWROM;
	int chpos = 0;

	while (ret == EMU_RUN_NEWROM || ret == EMU_RUN_ROMFAIL) {
		if (ret == EMU_RUN_ROMFAIL) menu_error("Failed to load rom",NULL);
		chpos = menu_filechooser("*.gb;*.gbc","Choose ROM",(char*)&romn,chpos);
		if (chpos == -1) return 0;
		else if (chpos == -2) die("No roms found");
		ret = gnuboymain((char*)&romn,0);
		config_update_loaded();
		loader_unload();
	}
	config_save();

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
	if (!config_loaded) {
		config_init(); // get default config
		config_loaded = config_load(); // load saved config
	} else if (config_loaded==1) {
		config_apply_loaded(); // We need to reapply the config each time we load a new rom
	}
	vram_dirty();
	pal_dirty();
	sound_dirty();
	mem_updatemap();
	//printf("Save state boot rom loaded: %d\n", bootromLoaded);
}