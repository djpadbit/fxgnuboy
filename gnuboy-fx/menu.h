#ifndef __MENU_H_
#define __MENU_H_

int menu_pause();
int keyb_input(char* buf,size_t len,char* ask);
int menu_filechooser(char *pathc,char *title,char *choosen,int start);
int menu_error(const char* first, const char* second);

#endif