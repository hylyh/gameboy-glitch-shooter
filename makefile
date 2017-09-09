CC=/opt/gbdk/bin/lcc
OUTFILE=out.gb

EMU=wine /Users/hayley/Documents/gbshit/bgb/bgb.exe

FILES=main.c

.PHONY: run clean

all: build run

build:
	$(CC) -o $(OUTFILE) $(FILES)

run:
	$(EMU) $(OUTFILE)

clean:
	$(RM) $(TARGET).obj $(TARGET).gb $(TARGET).asm.built
