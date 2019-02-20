#include "hw.h"
//#include "esp_spi_flash.h"
//#include "esp_partition.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "appfs.h"
#include "rombank.h"
#include "file.h"
#include <bfile.h>

#define NO_MAPS 64

/*
Because the GameBoy has some roms that are >=4MiB, we cannot just map a ROM into memory like that. (The
allowable range for that is 4MiB on the ESP32, and the emulators rodata segment also resides there.) That is
why on bankswap we map in a 64K segment of ROM instead. Because mapping is somewhat slow, we keep the last
NO_MAPS maps active so we can do bankswaps to these without actually mapping the flash again.

Also, bank 0 is special: if the GameBoy Color boot rom exists, this bank is overlaid with it until a call
to a register disables it.
*/

/*typedef struct {
	int bank;
	uint8_t *data;
	spi_flash_mmap_handle_t handle;
	uint32_t use;
} MappedPage;*/

//static int ctr=0;
//static MappedPage page[NO_MAPS];
//static appfs_handle_t romFd;
static int romfd;
// max would be 6 with default settings, but you can lower the pix cache and get a bit more (probably)
#define NO_ROMBANKS 4

static uint8_t __attribute__((section (".magic_sec"))) rombanks[NO_ROMBANKS][16*1024];
static uint8_t rombanks_mapped[NO_ROMBANKS];
static unsigned int rombanks_use_ctr[NO_ROMBANKS];
static uint8_t __attribute__((section (".magic_sec"))) rombank0[16*1024];
int rombankmiss, rombankhit;

//static uint8_t *bootrom=NULL;
//static uint8_t *rombank0=NULL; //ToDo: Fix this

//Game Boy ROM banks are 16KiB. We map it into the 64K MMU banks of the ESP32.
uint8_t *getRomBank(int bank) {
	/*int x;
	int espBank=(bank/4);
	int offset=(bank&3)*(16*1024);
	if (bootrom && bank==0) return bootrom;
//	if (bank==0 && rombank0) return rombank0;
	ctr++;
	for (x=0; x<NO_MAPS; x++) {
		if (page[x].bank==espBank && page[x].use) {
			page[x].use=ctr;
			return page[x].data+offset;
		}
	}
	//No bank found. Map new one.
	int oldest=0;
	for (x=0; x<NO_MAPS; x++) {
		if (page[x].use < page[oldest].use) oldest=x;
	}
	if (page[oldest].use) {
		printf("Unloading ESP bank %d\n", page[oldest].bank);
		spi_flash_munmap(page[oldest].handle);
	}
	
	int bankLen=(1<<16);
	int fdSize;
	appfsEntryInfo(romFd, NULL, &fdSize);
	if (espBank*(1<<16)+bankLen > fdSize) bankLen=fdSize-espBank*(1<<16);
	if (bankLen<=0) {
		printf("Bank %d not in file.\n", espBank);
		return NULL;
	}
	printf("Loading %dK seg %d into slot %d, mempos %p\n", bankLen, espBank, oldest, page[oldest].data);
	esp_err_t err=appfsMmap(romFd, (espBank*(1<<16)), bankLen, (const void**)&page[oldest].data, SPI_FLASH_MMAP_DATA, &page[oldest].handle);
	if (err!=ESP_OK) {
		printf("Couldn't map cartrom part!\n");
		return NULL;
	}

	page[oldest].use=ctr;
	page[oldest].bank=espBank;
	printf("Done, mempos %p\n", page[oldest].data);
	return page[oldest].data+offset;*/
	if (bank==0) return (uint8_t*)&rombank0;
	// Find bank mapped
	for (int i=0;i<NO_ROMBANKS;i++) {
		if (bank == rombanks_mapped[i]) {
			rombankhit++;
			rombanks_use_ctr[i]++;
			return (uint8_t*)&rombanks[i];
		}
	}
	rombankmiss++;
	// No rombanks found, search for open spot or get least used bank
	int op = 0;
	for (int i=0;i<NO_ROMBANKS;i++) {
		if (!rombanks_mapped[i]) {
			op = i;
			break; // if we find an open slot, break
		} // else keep track of the least used bank
		if (rombanks_use_ctr[i] < rombanks_use_ctr[op]) op = i;
	}
	if (romfd<0) return (uint8_t*)&rombanks[op]; // Needs a better thing
	BFile_Read(romfd,&rombanks[op],16*1024,16*1024*bank);
	rombanks_mapped[op] = bank;
	rombanks_use_ctr[op] = 1;
	return (uint8_t*)&rombanks[op];
}

int rombankLoad(char *rom) {
	/*romFd=appfsOpen(rom);
	printf("rombankLoad: Loaded rom %s, fd %d\n", rom, romFd);
	rombankLoadBootrom();*/
	uint16_t path[64];
	file_make_path(path,"fls0","",rom);
	romfd = BFile_Open(path,BFile_ReadOnly);
	if (romfd<0) return 0;
	BFile_Read(romfd,&rombank0,16*1024,0);
	return 1;
}

void rombankUnload() {
	/*for (int x=0; x<NO_MAPS; x++) {
		if (page[x].use) {
			spi_flash_munmap(page[x].handle);
		}
		page[x].use=0;
	}
	appfsClose(romFd);
	romBankUnloadBootrom();*/
	if (romfd<0) return;
	for (int i=0;i<NO_ROMBANKS;i++) {
		if (rombanks_mapped[i]) {
			memset(&rombanks[i],0,16*1024);
			rombanks_mapped[i] = 0;
			rombanks_use_ctr[i] = 0;
		}
	}
	BFile_Close(romfd);
	romfd = -1;
}

uint8_t bootromLoaded=0; //for save state

void rombankLoadBootrom() {
	/*if (bootrom) free(bootrom);
	if (hw.gbbootromdata==NULL) return; //no boot rom to load
	uint8_t *oldBank=getRomBank(0);
	bootrom=malloc(2304);
	if (bootrom==NULL) {
		die("Can't allocate 2304 bytes for boot ROM!\n");
	}
	memcpy(bootrom,hw.gbbootromdata, 2304);
	memcpy(bootrom+256, oldBank+256, 256);
	bootromLoaded=1;*/
}

void romBankUnloadBootrom() {
	/*if (bootrom==NULL) return;
	printf("Unloading boot rom.\n");
	free(bootrom);
	bootrom=NULL;
	bootromLoaded=0;*/
}

void rombankStateLoaded() {
	/*if (!bootromLoaded && bootrom) {
		romBankUnloadBootrom();
	} else if (bootromLoaded && !bootrom) {
		rombankLoadBootrom();
	}*/
}
