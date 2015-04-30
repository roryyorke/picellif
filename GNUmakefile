# picellif Makefile for GNU-based tools (mingw, etc.)

CC=i686-w64-mingw32-gcc
STRIP=i686-w64-mingw32-strip

CFLAGS=-Wall -Wextra
LDLIBS=-lshlwapi

ALL= picellif.exe picellifw.exe

all: $(ALL)

%.exe: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)
	$(STRIP) $@

%w.exe: %.c
	$(CC) $(CFLAGS) -mwindows $< -o $@ $(LDLIBS)
	$(STRIP) $@

clean:
	rm -f $(ALL)
