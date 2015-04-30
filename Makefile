# picellif Makefile for Microsoft Nmake
#  disable 4076, which is 'assignment within conditional expression'; wants this for a expression-zero-check-macro 
CFLAGS=/W4 /wd4706 /MD /D_CRT_SECURE_NO_WARNINGS

LIBS=shell32.lib # CommandLineToArgvW
LIBS=$(LIBS) user32.lib # *Clipboard
LIBS=$(LIBS) shlwapi.lib # Path*

MT=mt.exe -outputresource:"$@";2 -manifest "$@.manifest"

ALL= picellif.exe picellifw.exe

all: $(ALL)

.c.exe:
	$(CC) $(CFLAGS) -Fe$@ $** $(LIBS)
	$(MT)

# some linker incantations to get a "windows" application
picellifw.exe: picellif.c
	$(CC) $(CFLAGS) -Fe$@ $** $(LIBS) /link /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup
	$(MT)
