/* fileclip - utility to insert a list of files into the clipboard for subsequent pasting (copying) in  Explorer, Outlook, etc. 
 *
 * based on articles and code from, among other places,
 *   1. http://blogs.msdn.com/b/oldnewthing/archive/2013/05/20/10419965.aspx
 *   2. <todo:find stackoverfow answer by, I think, Remy Lebeau, which shows how to do the above w/o OLE>
 *
 * This whole thing is wchar_t based, following the example of 2 above.
 *
 * Usage: fileclip filename1.exe file?pattern.* andsoon.txt yougettheidea*.doc
 *        Use fileclipw for a no-console-creating version 
 */
#include <stdio.h> /* printf, etc. */
#include <stdlib.h> /* exit */
#include <wchar.h> /* wcslen */
#include <string.h> /* memcpy */

#include <windows.h> /* lots of things */
#include <shlobj.h> /* DROPFILES */
#include <shellapi.h> /* CommandLineToArgvW */
#include <shlwapi.h> /* Path* */

/* print message and exit 
 * TODO: handle no-console case better */
static void dieifzero(int expr, char const *strexpr, char const *filename, unsigned lineno);

/* error handling macro; kind of like assert() */
#define DIZ(x)                                  \
  dieifzero(!!(x),#x,__FILE__,__LINE__)

/* a DNTL is a double-null terminated list.  See http://blogs.msdn.com/b/oldnewthing/archive/2009/10/08/9904646.aspx for more 
 *
 * DROPFILES needs a DNTL, and so it seems simplest to use a DNTL as
 * the basic structure for recoding the filelist.  An alternative is,
 * say, a linked list which one converts to a DNTL when setting the
 * DROPFILES instance.
 */

/* append a string to a DNTL.  DNTL will be realloc()ed, or, if NULL, malloc()ed */
static void dntl_append(wchar_t **dntl, wchar_t const *s);

/* expand and absolutify a glob pattern, append results to a DNTL.  
   DNTL will be realloc()ed, or, if NULL, malloc()ed */
static void dntl_append_glob(wchar_t **dntl, wchar_t const *glob);

/* total number of wchars in a DNTL, including terminators */
static size_t dntl_chars(wchar_t const *dntl);

/* copy list to clipboard */
static void dntl_to_clipboard(wchar_t const *dntl);

int 
main()
{
  int argc;
  LPWSTR *argv;
  wchar_t *dntl= NULL;
  int iarg;

  DIZ(argv = CommandLineToArgvW(GetCommandLineW(), &argc));

  if(argc<2)
    {
      fprintf(stderr,"Need at least one argument, a filename pattern.\n");
      exit(2);
    }

  for(iarg=1; iarg<argc; iarg++)
    {
      dntl_append_glob(&dntl,argv[iarg]);
    }

  LocalFree(argv);

  dntl_to_clipboard(dntl);

  return 0;
}

static void
dieifzero(int expr, char const *strexpr, char const *filename, unsigned lineno)
{
  if(!expr)
    {
      fprintf(stderr,"Fatal:%s(%u): '%s' evaluated to 0\n",filename,lineno,strexpr);
      exit(1);
    }
}

static size_t
dntl_chars(wchar_t const *dntl)
{
  size_t i=0;
  int consecnulls=0;

  if(!dntl)
    return 0;

  while(consecnulls<2)
    {
      if(dntl[i])
        consecnulls=0;
      else
        consecnulls++;
      i++;
    }
  return i;
}

static void
dntl_append(wchar_t **dntl, wchar_t const *s)
{
  size_t s_len= wcslen(s);
  size_t dntl_len= dntl_chars(*dntl);
  if(!*dntl)
    {
      DIZ(*dntl=malloc((s_len+2)*sizeof *dntl));
      memcpy(*dntl, s, (s_len+1)*sizeof *s);
      /* add the second nul terminator */
      (*dntl)[s_len+1]=0;
    }
  else
    {
      DIZ(*dntl=realloc(*dntl, (dntl_len+s_len+1) * sizeof *dntl));
      memcpy((*dntl)+dntl_len-1, s, (s_len+1)*sizeof *s);
      (*dntl)[dntl_len+s_len]=0;
    }
}

static void
dntl_append_glob(wchar_t **dntl, wchar_t const *glob)
{
  WIN32_FIND_DATAW findFileData;

#if defined(_M_IX86)
  PVOID fsredir = NULL;
#endif /* defined(_M_IX86) */ 

  wchar_t *filename = PathFindFileNameW(glob);
  wchar_t dirname[MAX_PATH];
  wchar_t absdirname[MAX_PATH];
  
  HANDLE hFind;
  DIZ(filename < glob + MAX_PATH -1);
  wcsncpy(dirname,glob,filename-glob);
  dirname[filename-glob]=0;
  if(!dirname[0]) /* empty string ... */
    wcscpy(dirname,L".\\");
  GetFullPathNameW(dirname, MAX_PATH, absdirname, NULL);

  /* unclear if this should *only* be done w/ 32-bit compiles on 64-bit system */
#if defined(_M_IX86)
  DIZ(Wow64DisableWow64FsRedirection(&fsredir));
#endif /* defined(_M_IX86) */ 
  
  hFind=FindFirstFileW(glob,&findFileData);
  /* TODO: actual error message here; user error, not a bug */
  DIZ(INVALID_HANDLE_VALUE != hFind );
  for(;;)
    {
      wchar_t abspath[MAX_PATH];
      DIZ(PathCombineW(abspath,absdirname,findFileData.cFileName));
      dntl_append(dntl, abspath);
      if(0==FindNextFileW(hFind,&findFileData))
        /* todo: check GetLastError() return */
        break;
    }
  FindClose(hFind);

#if defined(_M_IX86)
  DIZ(Wow64RevertWow64FsRedirection(fsredir));
#endif /* defined(_M_IX86) */ 
}

static void
dntl_to_clipboard(wchar_t const *dntl)
{
  size_t dropsize = sizeof(DROPFILES) + dntl_chars(dntl)*sizeof *dntl;
  HGLOBAL hGlobal;
  DROPFILES *dropfiles;
  HANDLE hData;

  DIZ(hGlobal = GlobalAlloc(GHND, dropsize));
  DIZ(dropfiles = (DROPFILES*) GlobalLock(hGlobal));
  dropfiles->pFiles = sizeof *dropfiles;
  dropfiles->fWide = TRUE;
  memcpy((wchar_t*)(dropfiles+1), dntl, dntl_chars(dntl)*sizeof *dntl);
  DIZ(!GlobalUnlock(hGlobal));
  DIZ(OpenClipboard(NULL));
  DIZ(hData=SetClipboardData(CF_HDROP, hGlobal));
  DIZ(CloseClipboard());
}
