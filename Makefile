LDFLAGS = -T fx9860g.ld -lgint-fx -lgcc -Tsections.ld
#LDFLAGS += -Xlinker -Map=output.map
TOOLCHAIN = sh-elf
CC = $(TOOLCHAIN)-gcc
OBJCOPY = $(TOOLCHAIN)-objcopy
CFLAGS = -Wall -Ofast -fstrength-reduce -fthread-jumps -fcse-follow-jumps -fcse-skip-blocks -frerun-cse-after-loop  -fexpensive-optimizations -fforce-addr -fomit-frame-pointer
CFLAGS += -mb -ffreestanding -nostdlib -fstrict-volatile-bitfields -D FX9860G -m3 -I
INCLUDE = -Ignuboy/include -Ignuboy-fx/include
GNUBOYDIR = gnuboy
FX_GNUBOYDIR = gnuboy-fx
SRCFILES = $(wildcard $(GNUBOYDIR)/*.c)
FX_SRCFILES = $(wildcard $(FX_GNUBOYDIR)/*.c)
FX_ASMSRCFILES = $(wildcard $(FX_GNUBOYDIR)/*.s)
OBJS = $(patsubst %.c,%.o,$(SRCFILES))
FX_OBJS = $(patsubst %.c,%.o,$(FX_SRCFILES)) $(patsubst %.s,%.o,$(FX_ASMSRCFILES))

all: fxgnuboy.g1a

#debug: CFLAGS += -g
#debug: all
CFLAGS += -g

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $<

%.o: %.s
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $<

fxgnuboy.elf: $(OBJS) $(FX_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(FX_OBJS) $(LDFLAGS) -o fxgnuboy.elf

fxgnuboy.bin: fxgnuboy.elf
	$(OBJCOPY) -R .comment -R .bss -R .gint_bss -R .magic_sec -O binary fxgnuboy.elf fxgnuboy.bin

fxgnuboy.g1a: fxgnuboy.bin
	fxg1a fxgnuboy.bin -o fxgnuboy.g1a -i "MainIcon.png" -n "fxgnuboy" --internal="@GNUBOY"

dump: fxgnuboy.g1a
	$(TOOLCHAIN)-objdump -d -C -S fxgnuboy.elf > dump
	subl dump

clean:
	rm -f *.bin *.elf *.g1a $(GNUBOYDIR)/*.o $(FX_GNUBOYDIR)/*.o

send:
	p7 send -f fxgnuboy.g1a
