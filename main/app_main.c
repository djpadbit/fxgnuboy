#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_partition.h"
#include "rombank.h"
#include "save.h"
#include "hw.h"
#include <string.h>

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
#include "esp_log.h"

#include "8bkc-hal.h"
#include "8bkc-ugui.h"
#include "8bkcgui-widgets.h"
#include "appfs.h"

#include "nvs.h"
#include "nvs_flash.h"

unsigned char *gbbootromdata=NULL;

char statefile[128];

void gnuboyTask(void *pvParameters) {
	char rom[128];
	nvs_handle nvsh;
	//Let other threads start
	vTaskDelay(200/portTICK_PERIOD_MS);
	esp_err_t r=nvs_open("gnuboy", NVS_READWRITE, &nvsh);
	if (r!=ESP_OK) {
		printf("nvs_open err %d\n", r);
	}

	int ret;
	int loadState=1;

	while(1) {
		unsigned int size=sizeof(rom);
		r=nvs_get_str(nvsh, "rom", rom, &size);
		if (r==ESP_OK && appfsExists(rom)) {
			//Figure out name for statefile
			strcpy(statefile, rom);
			char *dot=strrchr(statefile, '.');
			if (dot==NULL) dot=statefile+strlen(statefile);
			strcpy(dot, ".state");
			printf("State file: %s\n", statefile);
			//Run emu
			ret=gnuboymain(rom, loadState);
		} else {
			ret=EMU_RUN_NEWROM;
		}

		if (ret==EMU_RUN_NEWROM) {
			kcugui_init();
			appfs_handle_t f=kcugui_filechooser("*.gb,*.gbc", "SELECT ROM", NULL, NULL);
			const char *rrom;
			appfsEntryInfo(f, &rrom, NULL);
			printf("Selected ROM %s\n", rrom);
			nvs_set_str(nvsh, "rom", rrom);
			kcugui_deinit();
			loadState=1;
		} else if (ret==EMU_RUN_RESET) {
			loadState=0;
		} else if (ret==EMU_RUN_POWERDOWN) {
			break;
		}
	}
	//Save state
	appfs_handle_t fd;
	r=appfsCreateFile(statefile, 1<<16, &fd);
	if (r!=ESP_OK) {
		printf("Couldn't create save state %s: %d\n", statefile, r);
	} else {
		savestate(fd);
	}
	kchal_power_down();
}

void lineTask();

int frames; //used in lcd

void monTask() {
	while(1) {
		vTaskDelay(1000/portTICK_PERIOD_MS);
		printf("Fps: %d\n", frames);
		printf("Free mem: %d\n", xPortGetFreeHeapSize());
		frames=0;
	}
//	vTaskDelete(NULL);
}


#include "cpuregs.h"

void startEmuHook() {
	const esp_partition_t* part;
	if (kchal_get_keys()&KC_BTN_START) return;
	if (appfsExists(statefile)) {
		loadstate(appfsOpen(statefile));
		rombankStateLoaded();
	} else {
		printf("Couldn't find state part!\n");
	}
	vram_dirty();
	pal_dirty();
	sound_dirty();
	mem_updatemap();
	printf("Save state boot rom loaded: %d\n", bootromLoaded);
}

#define BOOTROM_NAME "gbcrom.bin"

void app_main()
{
	spi_flash_mmap_handle_t hbootrom;
	esp_err_t err;
	esp_log_level_set("*", ESP_LOG_INFO);
	esp_log_level_set("appfs", ESP_LOG_DEBUG);
	kchal_init();
	nvs_flash_init();
	
	if (appfsExists(BOOTROM_NAME)) {
		appfs_handle_t fd=appfsOpen(BOOTROM_NAME);
		err=appfsMmap(fd, 0, 2304, (const void**)&gbbootromdata, SPI_FLASH_MMAP_DATA, &hbootrom);
		if (err==ESP_OK) {
			printf("Bootrom loaded.\n");
		} else {
			printf("Couldn't map bootrom appfs file!\n");
		}
	} else {
		printf("No bootrom found!\n");
	}
	printf("Initialized. bootROM@%p\n", gbbootromdata);
	xTaskCreatePinnedToCore(&lineTask, "lineTask", 1024, NULL, 6, NULL, 1);
	xTaskCreatePinnedToCore(&gnuboyTask, "gnuboyTask", 1024*6, NULL, 5, NULL, 0);
	xTaskCreatePinnedToCore(&monTask, "monTask", 1024*2, NULL, 7, NULL, 0);
}

