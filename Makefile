LDFLAGS = `fxsdk --libs` -Tsections.ld
#LDFLAGS += -Xlinker -Map=output.map
CC = sh3eb-elf-gcc
CFLAGS = -Wall -Ofast -fstrength-reduce -fthread-jumps -fcse-follow-jumps -fcse-skip-blocks -frerun-cse-after-loop  -fexpensive-optimizations -fforce-addr -fomit-frame-pointer
CFLAGS += `fxsdk --cflags`
INCLUDE = -Ignuboy/include -Ignuboy-fx/include
GNUBOYDIR = gnuboy
FX_GNUBOYDIR = gnuboy-fx
SRCFILES = $(wildcard $(GNUBOYDIR)/*.c)
FX_SRCFILES = $(wildcard $(FX_GNUBOYDIR)/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCFILES))
FX_OBJS = $(patsubst %.c,%.o,$(FX_SRCFILES))

all: fxgnuboy

#debug: CFLAGS += -g
#debug: all
CFLAGS += -g

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $<

fxgnuboy: $(OBJS) $(FX_OBJS)
	$(CC) $(OBJS) $(FX_OBJS) $(LDFLAGS) -o fxgnuboy.elf
	sh3eb-elf-objcopy -R .comment -R .bss -R .magic_sec -O binary fxgnuboy.elf fxgnuboy.bin
	g1a-wrapper fxgnuboy.bin -o fxgnuboy.g1a -i MainIcon.bmp

dump: fxgnuboy
	sh3eb-elf-objdump -d -C -S fxgnuboy.elf > dump

clean:
	rm -f *.bin *.elf *.g1a $(GNUBOYDIR)/*.o $(FX_GNUBOYDIR)/*.o

send:
	p7 send -f fxgnuboy.g1a
