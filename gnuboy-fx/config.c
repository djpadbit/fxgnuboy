#include <stdlib.h>
#include <string.h>
#include <bfile.h>
#include "fxsys.h"
#include "lcd.h"

#define CONFIG_FILE "fxgnuboy.cfg"
#define CONFIG_VERSION 1
// Size should always even otherwise bfile_write's gonna break
#define CONFIG_SIZE (sizeof(struct config)+(sizeof(struct config)%2))

struct config
{
	uint8_t version;
	//display stuff
	int lcd_xoff,lcd_yoff;
	int lcd_scalex,lcd_scaley;
	uint8_t lcd_show_debug_info,lcd_gray_enabled,lcd_fps_regul;
};

static struct config default_conf;

int config_create()
{
	uint16_t path[64];
	file_make_path(path,"fls0","",CONFIG_FILE);
	BFile_Remove(path);
	int size = CONFIG_SIZE;
	BFile_Create(path,BFile_File,&size);
	if (size != CONFIG_SIZE) {
		BFile_Remove(path);
		return 0;
	}
	return 1;
}

struct config *config_read()
{
	uint16_t path[64];
	file_make_path(path,"fls0","",CONFIG_FILE);
	int fd = BFile_Open(path,BFile_ReadOnly);
	if (fd<0) return NULL;
	int size = BFile_GetFileSize(fd);
	if (size != CONFIG_SIZE) return NULL;
	struct config *cfg = malloc(CONFIG_SIZE);
	if (!cfg) return NULL;
	BFile_Read(fd,cfg,size,0);
	BFile_Close(fd);
	return cfg;
}

int config_write(struct config *cfg)
{
	uint16_t path[64];
	file_make_path(path,"fls0","",CONFIG_FILE);
	int fd = BFile_Open(path,BFile_WriteOnly);
	if (fd<0) return 0;
	int size = BFile_GetFileSize(fd);
	if (size < CONFIG_SIZE) {
		BFile_Close(fd);
		return 0;
	}
	BFile_Write(fd,cfg,CONFIG_SIZE);
	BFile_Close(fd);
	return 1;
}

void config_apply(struct config *cfg)
{
	lcd_xoff = cfg->lcd_xoff;
	lcd_yoff = cfg->lcd_yoff;
	lcd_scalex = cfg->lcd_scalex;
	lcd_scaley = cfg->lcd_scaley;
	lcd_update_scaling();
	lcd_show_debug_info = cfg->lcd_show_debug_info;
	lcd_update_gray(cfg->lcd_gray_enabled);
	lcd_fps_regul = cfg->lcd_fps_regul;
}

void config_get(struct config *cfg)
{
	cfg->lcd_xoff = lcd_xoff;
	cfg->lcd_yoff = lcd_yoff;
	cfg->lcd_scalex = lcd_scalex;
	cfg->lcd_scaley = lcd_scaley;
	cfg->lcd_show_debug_info = lcd_show_debug_info;
	cfg->lcd_gray_enabled = lcd_gray_enabled;
	cfg->lcd_fps_regul = lcd_fps_regul;
}

int config_save()
{
	// We are forced to delete and recreate the file otherwise it crashes for some reason
	if (!config_create()) return 0;
	struct config cfg;
	cfg.version = CONFIG_VERSION;
	config_get(&cfg);
	if (!config_write(&cfg)) return 0;
	return 1;
}

int config_load()
{
	struct config *cfg = config_read();
	if (!cfg) {
		if (!config_save()) return -2;
		return -1;
	}
	if (cfg->version != CONFIG_VERSION) {
		free(cfg);
		return -3;
	}
	config_apply(cfg);
	free(cfg);
	return 1;
}

void config_init()
{
	config_get(&default_conf);
}

void config_reset()
{
	config_apply(&default_conf);
}