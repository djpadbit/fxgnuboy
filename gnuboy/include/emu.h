#ifndef EMU_H
#define EMU_H

#define EMU_RUN_CONT 0
#define EMU_RUN_NEWROM 1
#define EMU_RUN_ROMFAIL 2
#define EMU_RUN_RESET 3
#define EMU_RUN_POWERDOWN 4
#define EMU_RUN_EXIT 5


int emu_run();
void emu_reset();

int gnuboymain(char *rom, int loadState);


#endif


