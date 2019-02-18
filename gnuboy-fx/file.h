#ifndef __FILE_H__
#define __FILE_H__

#include <stdint.h>

// From SDK, temporary, will get removed when BFile_Find... are added to gint

// file info types
#define DT_DIRECTORY  0x0  // Directory
#define DT_FILE       0x1  // File
#define DT_ADDIN_APP  0x2  // Add-In application
#define DT_EACT       0x3  // eActivity
#define DT_LANGUAGE   0x4  // Language
#define DT_BITMAP     0x5  // Bitmap
#define DT_MAINMEM    0x6  // Main Memory data
#define DT_TEMP       0x7  // Temporary data
#define DT_DOT        0x8  // .  (Current directory)
#define DT_DOTDOT     0x9  // .. (Parent directory)
#define DT_VOLUME     0xA  // Volume label

typedef struct
{
	unsigned short  id;
	unsigned short  type;
	unsigned long   fsize;
	unsigned long   dsize;
	unsigned int    property;
	unsigned long   address;
} bfile_info;

void file_fc_to_char(uint16_t *src, char *dst);
void file_char_to_fc(char *src, uint16_t *dst);
void file_make_path(uint16_t* dst,char* root,char *fold,char *fn);
int BFile_FindFirst(const uint16_t *path, int *findhandle, uint16_t *foundfile, bfile_info *fileinfo);
int BFile_FindNext(int findhandle, uint16_t *foundfile, bfile_info *fileinfo);
int BFile_FindClose(int findhandle);

#endif