# picellif

picellif (an anagram of fileclip) is a tool to send a list of
filenames to the Microsoft Windows clipboard.  It's based on articles
and code from, among other places

  1. http://blogs.msdn.com/b/oldnewthing/archive/2013/05/20/10419965.aspx
  2. https://stackoverflow.com/a/25852460/1008142

picellif.exe is a console programme, while picellifw.exe a GUI (or no-console) programme; otherwise these two are identical.

Use it like this:

  picellif file-current-dir.txt d:\absolute\path\is\ok ..\patterns*work*too.png

picellif.el has two Emacs functions that use this tool, one for Dired mode, and the other for the current buffer's file.

picellif has been tested on Windows 7 64-bit only.  It compiles with Microsoft SDK 7.0 x64 (I didn't test the x86 compiler), and with the version of mingw-w64 that comes with Ubuntu 14.04.  I may have used some features not available on older versions of Windows.

# Wishlist

 - `--version`, `--help`
 - better error messages
 - dialog box messages for picellifw.exe
 - `--no-strict` to allow patterns to have no matches
 - automated tests
