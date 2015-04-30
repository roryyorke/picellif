# Makefile for GNU-based tools (mingw, etc.)
CC=i686-w64-mingw32-gcc
STRIP=i686-w64-mingw32-strip
CFLAGS=-Wall -Wextra
LDLIBS=-lshlwapi

ALL= picellif.exe picellifw.exe

all: $(ALL)

%.exe: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)
	$(STRIP) $@

picellifw.exe: CFLAGS+=-mwindows

picellifw.exe: picellif.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)
	$(STRIP) $@

clean:
	rm -f $(ALL)
