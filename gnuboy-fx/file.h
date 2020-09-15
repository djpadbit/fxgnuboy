#ifndef __FILE_H__
#define __FILE_H__

#include <stdint.h>

void file_fc_to_char(uint16_t *src, char *dst);
void file_char_to_fc(char *src, uint16_t *dst);
void file_make_path(uint16_t* dst,char* root,char *fold,char *fn);

#endif