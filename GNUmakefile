# Makefile for GNU-based tools (mingw, etc.)
CC=i686-w64-mingw32-gcc
STRIP=i686-w64-mingw32-strip
CFLAGS=-Wall -Wextra

ALL= fileclip.exe fileclipw.exe

all: $(ALL)

%.exe: %.c
	$(CC) $(CFLAGS) $< -o $@
	$(STRIP) $@

fileclipw.exe: fileclip.c
	$(CC) $(CFLAGS) $< -o $@
	$(STRIP) $@

fileclipw.exe: CFLAGS+=-mwindows

clean:
	rm -f $(ALL)
