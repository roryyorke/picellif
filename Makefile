# Makefile for Microsoft Nmake
#  disable 4076, which is 'assignment within conditional expression'; wants this for a expression-zero-check-macro 
CFLAGS=/W4 /wd4706 /MD

LIBS=shell32.lib # CommandLineToArgvW
LIBS=$(LIBS) user32.lib # *Clipboard

MT=mt.exe -outputresource:"$@";2 -manifest "$@.manifest"

ALL= fileclip.exe fileclipw.exe

all: $(ALL)

.c.exe:
	$(CC) $(CFLAGS) -Fe$@ $** $(LIBS)
	$(MT)

# some linker incantations to get a "windows" application
fileclipw.exe: fileclip.c
	$(CC) $(CFLAGS) -Fe$@ $** $(LIBS) /link /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup
	$(MT)
