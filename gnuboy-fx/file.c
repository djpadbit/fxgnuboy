#include <stdint.h>
#include <gint/std/stdio.h>
#include <gint/std/string.h>
#include "file.h"

// The 3 functions below are based on EDIT's source code (see FilePath, Font2Char and Char2Font function)
void file_fc_to_char(uint16_t *src, char *dst)
{
	int i;
	for (i=0;src[i]!=0;i++) dst[i] = src[i];
	dst[i] = 0;
}

void file_char_to_fc(char *src, uint16_t *dst)
{
	int i;
	for (i=0;src[i]!=0;i++) dst[i] = src[i];
	dst[i] = 0;
}

void file_make_path(uint16_t *dst,char *root,char *fold,char *fn)
{
	char tp[2+strlen(root)+1+strlen(fold)+1+strlen(fn)+1]; // probably off by 1 or 2 bytes
	if(strlen(fold)==0) sprintf(tp,"\\\\%s\\%s",root,fn); //File path without folder
	else if(strlen(fn)==0) sprintf(tp,"\\\\%s\\%s",root,fn); //File path without file
	else sprintf(tp,"\\\\%s\\%s\\%s",root,fold,fn); //File path with file & folder
	file_char_to_fc((char*)&tp,dst);
}
