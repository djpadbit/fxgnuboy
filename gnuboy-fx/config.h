#ifndef __CONFIG_H__
#define __CONFIG_H__

int config_load();
int config_save();
void config_apply_loaded();
void config_init();
void config_reset();

#endif