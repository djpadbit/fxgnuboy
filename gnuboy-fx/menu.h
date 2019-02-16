#ifndef __MENU_H_
#define __MENU_H_

int menu_pause();
int keyb_input(char* buf,size_t len,const char* ask);
int menu_error(const char* first, const char* second);

#endif