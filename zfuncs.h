/********************************************************************************
   zfuncs.h      include file for zfuncs functions

   Copyright 2007-2020 Michael Cornelison
   source code URL: https://kornelix.net
   contact: kornelix@posteo.de

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version. See https://www.gnu.org/licenses

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

*********************************************************************************/

#include <cstdint>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <sys/utsname.h>
#include <malloc.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <cmath>
#include <cstdarg>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <execinfo.h>
#include <locale.h>
#include <glob.h>
#include <gtk/gtk.h>

#define VERTICAL GTK_ORIENTATION_VERTICAL                                        //  GTK shortcuts
#define HORIZONTAL GTK_ORIENTATION_HORIZONTAL
#define PIXBUF GdkPixbuf
#define GDKCOLOR GdkColorspace

/*
#define  int8   char                                                             //  number types
#define  int16  short
#define  int32  int
#define  int64  long long                                                        //  long long is always 64 bits
#define  uint8   unsigned char
#define  uint16  unsigned short
#define  uint32  unsigned int
#define  uint64  unsigned long long
#define  uchar  unsigned char*/
#define  cchar  const char
#define  VOL volatile

#ifdef __cplusplus
using  int8    = int8_t    ;                  //  number types
using  int16   = int16_t   ;
using  int32   = int32_t   ;
using  int64   = int64_t   ;                  //  long long is always 64 bits
using  uint8   = uint8_t   ;
using  uint16  = uint16_t  ;
using  uint32  = uint32_t  ;
using  uint64  = uint64_t  ;
using  uchar   = uint8     ;
//using  cchar   = const signed char;
#else
typedef int8_t       int8  ;                  //  number types
typedef int16_t      int16 ;
typedef int32_t      int32 ;
typedef int64_t      int64 ;                  //  long long is always 64 bits
typedef uint8_t      uint8 ;
typedef uint16_t     uint16;
typedef uint32_t     uint32;
typedef uint64_t     uint64;
typedef uint8_t      uchar ;
//typedef const signed char cchar ;
#endif

#define  wstrerror(err) strerror(WEXITSTATUS(err))                               //  get text status for child process

#define  STATB  struct stat                                                      //  stat() file status buffer

#define  mutex_t        pthread_mutex_t                                          //  abbreviations
#define  mutex_init     pthread_mutex_init
#define  mutex_lock     pthread_mutex_lock
#define  mutex_trylock  pthread_mutex_trylock
#define  mutex_unlock   pthread_mutex_unlock
#define  mutex_destroy  pthread_mutex_destroy

#define  XFCC 1000                                                               //  max. file pathname cc tolerated
#define  null   NULL
#define  true   1
#define  false  0
#define  NOP

//  system functions ============================================================

void *zmalloc(size_t cc);                                                        //  malloc() wrapper
void zfree(void *pp);                                                            //  free() wrapper
char *zstrdup(cchar *string, int addcc = 0);                                     //  strdup() wrapper with opt. expand
void zmalloc_report();                                                           //  print statistics report
void printz(cchar *format, ...);                                                 //  printf() with immediate fflush()
void zexit(cchar *message, ...);                                                 //  exit a process and kill child processes
void zbacktrace();                                                               //  produce a backtrace to stdout
void zappcrash(cchar *format, ...);                                              //  crash with popup message in text window
void catch_signals();                                                            //  catch signals and do backtrace dump

void beroot(int argc, char *argv[]);                                             //  restart program as root if password OK
int  runroot(cchar *command);                                                    //  run command as root via su or sudo
double get_seconds();                                                            //  seconds since 2000.01.01
void start_timer(double &time0);                                                 //  start a timer
double get_timer(double &time0);                                                 //  get elapsed time in seconds
void ztimer_start();                                                             //  start 1 millisec. timer
void ztimer_stop();                                                              //  stop 1 millisec. timer
int  ztimer_milliseconds();                                                      //  get millisecs. since startup
void start_CPUtimer(double &time0);                                              //  start a process CPU timer
double get_CPUtimer(double &time0);                                              //  get elapsed CPU time, seconds
double CPUtime();                                                                //  get elapsed process CPU time for main()
double CPUtime2();                                                               //   " include all threads
double jobtime();                                                                //   " include all threads + subprocesses
void compact_time(const time_t DT, char *compactDT);                             //  time_t DT to yyyymmddhhmmss
void pretty_datetime(const time_t DT, char *prettyDT);                           //  time_t DT to yyyy-mm-dd hh:mm:ss
int  parseprocfile(cchar *pfile, cchar *pname, size_t *value, ...);              //  get data from /proc file
int  parseprocrec(char *prec, int field, double *value, ...);                    //  get data from /proc file record
int  coretemp();                                                                 //  get CPU core temperature, deg. C
int  disktemp(char *disk);                                                       //  get disk temp, e.g. "/dev/sda"
void zsleep(uint64 nanosec);                                                     //  sleep specified seconds
int  resource_lock(int &resource);                                               //  simple lock/unlock usable with GTK
void resource_unlock(int &resource);                                             //  (never waits, returns lock status)
int  zget_locked(int &param);                                                    //  lock and get multi-thread parameter
void zput_locked(int &param, int value);                                         //  put and unlock multi-thread parameter
int  zadd_locked(int &param, int incr);                                          //  increment multi-thread parameter
void start_detached_thread(void * tfunc(void *), void * arg);                    //  start detached thread function
pthread_t start_Jthread(void * threadfunc(void *), void * arg);                  //  start joinable thread function
int  wait_Jthread(pthread_t tid);                                                //  wait for completion (join thread)
void synch_threads(int NT = 0);                                                  //  synchronize NT threads
void set_cpu_affinity(int cpu);                                                  //  set cpu affinity for calling thread
int  Qshell(int log, int ack, cchar *command, ...);                              //  do shell command and get status
char * command_output(int &contx, cchar *command, ...);                          //  get shell command output
int  command_status(int contx);                                                  //  get exit status of command
int  command_kill(int contx);                                                    //  kill command before completion
int  signalProc(cchar * pname, cchar * signal);                                  //  pause/resume/kill subprocess
char * fgets_trim(char * buff, int maxcc, FILE *, int bf = 0);                   //  fgets + trim trailing \n \r (blanks)
int  samefolder(cchar *file1, cchar *file2);                                     //  returns 1 if files in same folder
int  parsefile(cchar *path, char **dir, char **file, char **ext);                //  parse a filespec
int  renamez(cchar *file1, cchar *file2);                                        //  rename, also across file systems
int  check_create_dir(char *path);                                               //  check if folder exists, ask to create
int  copyFile(cchar *sfile, cchar *dfile);                                       //  copy file to file or file to folder
int  zreaddir(cchar *folder, char **&files);                                     //  return all files in a folder, sorted
char * combine_argvs(int argc, char *argv[], int Nth);                           //  combine argv[ii] elements Nth to last
char * zescape_quotes(cchar *file);                                              //  escape quote marks (") in file name

int pagefaultrate();                                                             //  monitor own process hard fault rate

//  string macros and functions =================================================

#define strmatch(str1,str2) (! strcmp((str1),(str2)))                            //  TRUE if strings equal
#define strmatchN(str1,str2,cc) (! strncmp((str1),(str2),(cc)))                  //  TRUE if strings[cc] equal
#define strmatchcase(str1,str2) (! strcasecmp((str1),(str2)))                    //  TRUE if strings equal, ignoring case
#define strmatchcaseN(str1,str2,cc) (! strncasecmp((str1),(str2),(cc)))          //  TRUE if strings[cc] equal, ignoring case

cchar * strField(cchar *string, cchar *delims, int Nth);                         //  get Nth delimited field in string
cchar * strField(cchar *string, cchar delim, int Nth);                           //  get Nth delimited field in string
int  strParms(int &bf, cchar *inp, char *pname, int maxcc, double &pval);        //  parse string: name1=val1 | name2 ...
int  strHash(cchar *string, int max);                                            //  string --> random int 0 to max-1
int64  strHash64(cchar *string, int64 max);                                      //  string --> random int 0 to max-1
int  strncpy0(char *dest, cchar *source, uint cc);                               //  strncpy, insure null, return 0 if fit
void strnPad(char *dest, cchar *source, int cc);                                 //  strncpy with blank padding to cc
int  strTrim(char *dest, cchar *source);                                         //  remove trailing blanks
int  strTrim(char *string);                                                      //  remove trailing blanks
int  strTrim2(char *dest, cchar *source);                                        //  remove leading and trailing blanks
int  strTrim2(char *string);                                                     //  remove leading and trailing blanks
int  strCompress(char *dest, cchar *source);                                     //  remove all blanks incl. imbedded
int  strCompress(char *string);                                                  //  remove all blanks
int  strncatv(char *dest, int maxcc, cchar *source, ...);                        //  catenate strings (last = null)
int  strmatchV(cchar *string, ...);                                              //  compare to N strings, return 1-N or 0
void strToUpper(char *dest, cchar *source);                                      //  move and conv. string to upper case
void strToUpper(char *string);                                                   //  conv. string to upper case
void strToLower(char *dest, cchar *source);                                      //  move and conv. string to lower case
void strToLower(char *string);                                                   //  conv. string to lower case
int  repl_1str(cchar *strin, char *strout, cchar *ssin, cchar *ssout);           //  copy string and replace 1 substring
int  repl_Nstrs(cchar *strin, char *strout, ...);                                //  copy string and replace N substrings
int  breakup_text(cchar *in, char **&out, cchar *dlm, int cc1, int cc2);         //  break long string into substrings
void strncpyx(char *out, cchar *in, int ccin);                                   //  conv. string to hex format
void StripZeros(char *pNum);                                                     //  1.230000E+12 --> 1.23E+12
int  blank_null(cchar *string);                                                  //  test for blank/null string
int  clean_escapes(char *string);                                                //  replace \x escapes with real characters
int  utf8len(cchar *utf8string);                                                 //  get graphic cc for UTF8 string
int  utf8substring(char *utf8out, cchar *utf8in, int pos, int cc);               //  get graphic substring from UTF8 string
int  utf8_check(cchar *string);                                                  //  check utf8 string for encoding errors
int  utf8_position(cchar *utf8in, int Nth);                                      //  get byte position of Nth graphic char.
int  zsed(cchar *file, ...);                                                     //  replace string1/3... with string2/4...
cchar * zstrstr(cchar *haystack, cchar *needle);                                 //  work like strstr() and strcasestr()
cchar * zstrcasestr(cchar *haystack, cchar *needle);                             //  (but "" does NOT match any string)

//  number conversion ===========================================================

int convSI (cchar *s, int &i, cchar **delm = 0);                                 //  string to int
int convSI (cchar *s, int &i, int low, int hi, cchar **delm = 0);                //  (with low/high limit checking)
int convSD (cchar *s, double &d, cchar **delm = 0);                              //  string to double
int convSD (cchar *s, double &d, double low, double hi, cchar **delm = 0);       //  (with low/high limit checking)
int convSF (cchar *s, float &f, cchar **delm = 0);                               //  string to double
int convSF (cchar *s, float &f, float low, float hi, cchar **delm = 0);          //  (with low/high limit checking)
int convIS (int iin, char *outp, int *cc = 0);                                   //  int to string, returned cc
int convDS (double din, int prec, char *outp, int *cc = 0);                      //  double to string, precision, ret. cc
char * formatKBMB(double fnum, int prec);                                        //  format nnn B, nn.n KB, n.nn MB, etc.

//  wildcard functions ==========================================================

int MatchWild(cchar * wildstr, cchar * str);                                     //  wildcard string match (match = 0)
int MatchWildIgnoreCase(cchar * wildstr, cchar * str);                           //  wildcard string match, ignoring case
cchar * SearchWild(cchar *wpath, int &flag);                                     //  wildcard file search
cchar * SearchWildCase(cchar *wpath, int &flag);                                 //  wildcard file search, ignoring case
int zfind(cchar *pattern, char **&flist, int &NF);                               //  wildcard file search using glob()

//  search and sort functions ===================================================

int bsearch(int seekint, int nn, int list[]);                                    //  binary search sorted list[nn]
int bsearch(cchar *seekrec, cchar *allrecs, int recl, int nrecs);                //  binary search sorted records
int bsearch(cchar *seekrec, cchar **allrecs, int N, int nrecs);                  //  binary search sorted pointers to recs

typedef int HeapSortUcomp(cchar *rec1, cchar *rec2);                             //  return -1/0/+1 if rec1 </=/> rec2
void HeapSort(int vv[], int nn);                                                 //  Heap Sort - integer
void HeapSort(float vv[], int nn);                                               //  Heap Sort - float
void HeapSort(double vv[], int nn);                                              //  Heap Sort - double
void HeapSort(char *vv[], int nn);                                               //  Heap Sort - char, ascending order
void HeapSort(char *vv1[], char *vv2[], int nn);                                 //  Heap Sort - parallel char *, ascending
void HeapSort(char *vv[], int nn, HeapSortUcomp);                                //  Heap Sort - char, user-defined order
void HeapSort(char *recs, int RL, int NR, HeapSortUcomp);                        //  Heap Sort - records, user-defined order

int MemSort(char * RECS, int RL, int NR, int KEYS[][3], int NK);                 //  memory sort, records with multiple keys

int zmember(int testval, int matchval1, ...);                                    //  test if value matches any in a list

//  variable string list functions ==============================================

struct  pvlist {
   int      max;                          //  max. entries
   int      act;                          //  actual entries
   char   **list;                         //  entries
};

pvlist * pvlist_create(int max);                                                 //  create pvlist
void pvlist_free(pvlist *pv);                                                    //  free pvlist
int pvlist_append(pvlist *pv, cchar *entry, int unique = 0);                     //  append new entry (opt. if unique)
int pvlist_prepend(pvlist *pv, cchar *entry, int unique = 0);                    //  prepend new entry (opt. if unique)
int pvlist_find(pvlist *pv, cchar *entry);                                       //  find entry by name
int pvlist_remove(pvlist *pv, cchar *entry);                                     //  remove entry by name
int pvlist_remove(pvlist *pv, int Nth);                                          //  remove entry by number (0...)
int pvlist_count(pvlist *pv);                                                    //  return entry count
int pvlist_replace(pvlist *pv, int Nth, cchar *entry);                           //  replace Nth entry (0...)
char * pvlist_get(pvlist *pv, int Nth);                                          //  return Nth entry (0...)
int pvlist_sort(pvlist *pv);                                                     //  sort list, ascending

//  random number functions =====================================================

uint32 lrandz(uint64* seed);                                                     //  returns 0 to 0x7fffffff
uint32 lrandz();                                                                 //  built-in seed
//  spline curve-fitting functions ==============================================

void spline1(int nn, float *dx, float *dy);                                      //  define a curve using nn data points
float spline2(float x);                                                          //  return y-value for given x-value

//  FIFO queue for text strings, single or dual-thread access ===================

typedef struct  {
   int      qcap;                                                                //  queue capacity
   int      qnewest;                                                             //  newest entry position, circular
   int      qoldest;                                                             //  oldest entry position, circular
   int      qdone;                                                               //  flag, last entry added to queue
   char     **qtext;                                                             //  up to qcap text strings
}  Qtext;

void Qtext_open(Qtext *qtext, int cap);                                          //  initialize Qtext queue, empty
void Qtext_put(Qtext *qtext, cchar *format, ...);                                //  add text string to Qtext queue
char * Qtext_get(Qtext *qtext);                                                  //  remove text string from Qtext queue
void Qtext_close(Qtext *qtext);                                                  //  close Qtext, zfree() leftover strings

//  application initialization and administration ===============================

int appimage_install(cchar *appname);                                            //  make appimage desktop and icon files
void appimage_unstall();                                                         //  uninstall appimage

int zinitapp(cchar *appvers, cchar *homedir = 0);                                //  initialize app (appname-N.N, homedir)

cchar * get_zprefix();                                                           //  get /usr or /usr/local  ...
cchar * get_zhomedir();                                                          //  get /home/user/.appname/
cchar * get_zdatadir();                                                          //  get data folder
cchar * get_zimagedir();                                                         //  get image folder
cchar * get_zdocdir();                                                           //  get document folder

void zsetfont(cchar *newfont);                                                   //  set new app font and size
int  widget_font_metrics(GtkWidget *widget, int &fww, int &fhh);                 //  get widget font char width/height

int  locale_filespec(cchar *ftype, cchar *fname, char *filespec);                //  get a locale dependent file
void showz_logfile(GtkWidget *parent);                                           //  show log file in popup window
void showz_textfile(cchar *type, cchar *file, GtkWidget *parent);                //  show text file [.gz] in popup window
void showz_html(cchar *url);                                                     //  show html via preferred browser
void showz_docfile(GtkWidget *, cchar *docfile, cchar *topic);                   //  show docfile topic and assoc. image

void appruns_update();                                                           //  update run counter in appruns file
void phone_home();                                                               //  send usage statistics
void phone_home_allow(GtkWidget *parent);                                        //  user dialog to allow/block phone_home()

//  translation functions =======================================================

#define E2Xmaxent 2000                                                           //  max. translation strings
#define E2Xmaxcc 4000                                                            //  max. cc per string

void E2Xinit(cchar *lang, int Fdump);                                            //  setup for message translation
cchar * E2X(cchar *english);                                                     //  get translation for English message
cchar * E2X_missing(int &ftf);                                                   //  get missing translations, one per call


/********************************************************************************
   GTK utility functions
*********************************************************************************/

void zmainloop(int skip = 0);                                                    //  do main loop, process menu events
void zmainsleep(float secs);                                                     //  do main loop and sleep designated time


/********************************************************************************/

//  cairo drawing region for GDK window      GTK 3.21 version

#if GTK_CHECK_VERSION(3,22,0)

typedef struct {
   GdkWindow               *win;
   cairo_rectangle_int_t   rect;
   cairo_region_t          *reg;
   GdkDrawingContext       *ctx;
   cairo_t                 *dcr = 0;
}  draw_context_t;

#else

typedef struct {
   cairo_t    *dcr;
}  draw_context_t;

#endif

cairo_t * draw_context_create(GdkWindow *gdkwin, draw_context_t &context);
void draw_context_destroy(draw_context_t &context);


/********************************************************************************/

//  textwidget functions - scrollable text widget for text reports and line editing
//  widget = zdialog_widget(zd,textwidget) where textwidget is a zdialog "text" widget type

void textwidget_clear(GtkWidget *widget);                                              //  clear all text
void textwidget_clear(GtkWidget *widget, int line);                                    //  clear text from line to end
int  textwidget_linecount(GtkWidget *widget);                                          //  get current line count
void textwidget_append(GtkWidget *widget, int bold, cchar *format, ...);               //  append line
void textwidget_append2(GtkWidget *widget, int bold, cchar *format, ...);              //  append line and scroll to end
void textwidget_insert(GtkWidget *widget, int bold, int line, cchar *format, ...);     //  insert line
void textwidget_replace(GtkWidget *widget, int bold, int line, cchar *format, ...);    //  replace line
void textwidget_delete(GtkWidget *widget, int line);                                   //  delete line
int  textwidget_find(GtkWidget *widget, char *matchtext, int line1);                   //  find matching line
void textwidget_insert_pixbuf(GtkWidget *textwidget, int line, GdkPixbuf *pixbuf);     //  insert pixbuf image
void textwidget_scroll(GtkWidget *widget, int line);                                   //  scroll line on screen
void textwidget_scroll_top(GtkWidget *widget, int line);                               //  scroll line to top of window
void textwidget_get_visible_lines(GtkWidget *textwidget, int &top, int &bott);         //  get range of visible lines
void textwidget_dump(GtkWidget *widget, cchar *filename);                              //  dump all text into a file
void textwidget_save(GtkWidget *widget, GtkWindow *parent);                            //  same, with save-as dialog
char * textwidget_line(GtkWidget *widget, int line, int strip);                        //  retrieve line (strip \n)
void textwidget_highlight_line(GtkWidget *widget, int line);                           //  highlight line
char * textwidget_word(GtkWidget *, int line, int posn, cchar *dlims, char &end);      //  retrieve word
void textwidget_highlight_word(GtkWidget *widget, int line, int posn, int cc);         //  highlight word
void textwidget_bold_word(GtkWidget *widget, int line, int posn, int cc);              //  make word bold
void textwidget_underline_word(GtkWidget *widget, int line, int posn, int cc);         //  make word underlined
void textwidget_font_attributes(GtkWidget *widget);                                    //  set font attributes for all text

typedef void textwidget_callbackfunc_t(GtkWidget *, int line, int posn, int KBkey);    //  widget event function to receive
void textwidget_set_eventfunc(GtkWidget *, textwidget_callbackfunc_t func);            //    mouse click and KB events


/********************************************************************************/

//  functions to simplify building menus, tool bars, status bars

#define G_SIGNAL(window,event,func,arg) \
        g_signal_connect(G_OBJECT(window),event,G_CALLBACK(func),(void *) arg)

#define  zdcbmax 100                                                             //  max. combo box drop-down list

typedef void cbFunc(GtkWidget *, cchar *mname);                                  //  menu or button response function

GtkWidget * create_menubar(GtkWidget *vbox);                                     //  create menubar in packing box
GtkWidget * add_menubar_item(GtkWidget *mbar, cchar *mname, cbFunc func = 0);    //  add menu item to menubar
GtkWidget * add_submenu_item(GtkWidget *mitem, cchar *subname,                   //  add submenu item to menu item
                             cbFunc func = 0, cchar *mtip = 0);                  //    with opt. function and popup tip

GtkWidget * create_toolbar(GtkWidget *vbox, int iconsize = 24);                  //  toolbar in packing box (no vert gtk3)
GtkWidget * add_toolbar_button(GtkWidget *tbar, cchar *lab, cchar *tip,          //  add button with label, tool-tip, icon
                                 cchar *icon, cbFunc func);

GtkWidget * create_stbar(GtkWidget *vbox);                                       //  create status bar in packing box
int stbar_message(GtkWidget *stbar, cchar *message);                             //  display message in status bar


/********************************************************************************/

GtkWidget * create_popmenu();                                                    //  create an empty popup menu
GtkWidget * add_popmenu_item(GtkWidget *popmenu, cchar *mname,                   //  add menu item to popup menu
                           cbFunc func, cchar *arg, cchar *mtip = 0);
void popup_menu(GtkWidget *, GtkWidget *popmenu);                                //  pop-up menu at current mouse posn.


/********************************************************************************/

//  user editable graphic menu in popup window
//  menus can be added and arranged using the mouse

typedef void Gmenuz_cbfunc(cchar *menu);                                         //  caller-supplied callback function
void Gmenuz(GtkWidget *parent, cchar *title, cchar *ufile, Gmenuz_cbfunc);       //  show window, handle mouse drag/click


/********************************************************************************/

//  create vertical menu/toolbar in vertical packing box

struct vmenuent {                                                                //  menu data from caller
   cchar       *name;                                                            //  menu name, text
   cchar       *icon;                                                            //  opt. icon file name
   cchar       *desc;                                                            //  description (mouse hover popup)
   cbFunc      *func;                                                            //  callback func (GtkWidget *, cchar *arg)
   cbFunc      *RMfunc;                                                          //  alternate func for right mouse click
   cchar       *arg;                                                             //  callback arg for func
   cchar       *RMarg;                                                           //  callback arg for RMfunc
   PIXBUF      *pixbuf;                                                          //  icon pixbuf or null
   PangoLayout *playout1, *playout2;                                             //  normal and bold menu text
   int         namex, namey;                                                     //  menu name position in layout
   int         iconx, icony;                                                     //  menu icon position
   int         ylo, yhi;                                                         //  menu height limits
   int         iconww, iconhh;                                                   //  icon width and height
};

struct Vmenu {
   GtkWidget   *vbox;                                                            //  parent window (container)
   GtkWidget   *topwin;                                                          //  top-level window of parent
   GtkWidget   *layout;                                                          //  drawing window
   float       fgRGB[3];                                                         //  font color, RGB scaled 0-1
   float       bgRGB[3];                                                         //  background color, RGB scaled 0-1
   int         xmax, ymax;                                                       //  layout size
   int         mcount;                                                           //  menu entry count
   vmenuent    menu[100];
};

Vmenu *Vmenu_new(GtkWidget *vbox, float fgRGB[3], float bgRGB[3]);               //  create new menu in parent vbox
void Vmenu_add(Vmenu *vbm, cchar *name, cchar *icon,                             //  add menu item with response function
               int iconww, int iconhh, cchar *desc,
               cbFunc func, cchar *arg);                                         //  function may be popup_menu()
void Vmenu_add_RMfunc(Vmenu *vbm, int me, cbFunc RMfunc, cchar *arg);            //  add auto function if right mouse click
void Vmenu_block(int flag);                                                      //  block or unblock menu


/********************************************************************************/

//   functions to implement GTK dialogs with less complexity
//   widget types: dialog, hbox, vbox, hsep, vsep, frame, scrwin, label, link,
//                 entry, edit, text, radio, check, button, togbutt, spin,
//                 combo, comboE, hscale, vscale, imagebutt, colorbutt, icon, image

#define zdmaxwidgets 300
#define zdmaxbutts 10
#define zdsentinel 0x97530000
#define zdialog_max 20
#define zdialog_button_shortcuts "Done Cancel Apply Reset"                       //  buttons that may have KB shortcuts

struct zwidget  {
      cchar       *type;                           //  dialog, hbox, vbox, label, entry ...
      cchar       *name;                           //  widget name
      cchar       *pname;                          //  parent (container) name
      char        *data;                           //  widget data, initial / returned
      pvlist      *cblist;                         //  combo box drop-down list
      int         size;                            //  text entry cc or image pixel size
      int         homog;                           //  hbox/vbox: equal spacing flag
      int         expand;                          //  widget is expandable flag
      int         space;                           //  extra padding space (pixels)
      int         wrap;                            //  wrap mode for edit widget
      int         rescale;                         //  widget is rescaled for more resolution
      double      lval, nval, hval;                //  scale range and neutral value
      double      lolim, hilim, step;              //  range and step value for number widget
      GtkWidget   *widget;                         //  GTK widget pointer
};

struct zdialog  {
      int         sentinel1;                       //  validity sentinel1
      int         uniqueID;                        //  unique ID, monotone increasing
      char        *title;                          //  dialog title
      void        *eventCB;                        //  dialog event user callback function
      void        *popup_report_CB;                //  callback function for popup_report
      int         zrunning;                        //  dialog is running (0,1)
      int         zstat;                           //  dialog status (from completion button)
      char        zstat_button[40];                //  completion button label
      int         disabled;                        //  widget signals/events are disabled
      int         saveposn;                        //  save and recall window position each use
      int         saveinputs;                      //  save and recall user inputs each use
      GtkWidget   *dialog;                         //  dialog window or null (box parent)
      GtkWidget   *parent;                         //  parent window or null
      cchar       *ptype;                          //  null or "window" or "box" parent
      cchar       *compbutton[zdmaxbutts];         //  dialog completion button labels
      GtkWidget   *compwidget[zdmaxbutts];         //  dialog completion button widgets
      zwidget     widget[zdmaxwidgets];            //  dialog widgets (EOF = type = 0)
      char        event[40];                       //  active event or widget
      GtkWidget   *lastwidget;                     //  last widget active
      int         sentinel2;                       //  validity sentinel2
};

zdialog *zdialog_new(cchar *title, GtkWidget *parent, ...);                      //  create a zdialog with opt. buttons
void zdialog_set_title(zdialog *zd, cchar *title);                               //  change zdialog title
void zdialog_set_modal(zdialog *zd);                                             //  set zdialog modal
void zdialog_set_decorated(zdialog *zd, int decorated);                          //  set zdialog decorated or not
void zdialog_present(zdialog *zd);                                               //  zdialog visible and on top
void zdialog_can_focus(zdialog *zd, int Fcan);                                   //  zdialog can/not have focus (e.g. report)
void zdialog_set_focus(zdialog *zd, cchar *widget = null);                       //  set focus on window [ widget ]

int zdialog_add_widget(zdialog *zd,                                              //  add widget to zdialog
      cchar *type, cchar *name, cchar *pname,                                    //  required args
      cchar *data = 0, int size = 0, int homog = 0,                              //  optional args
      int expand = 0, int space = 0, int wrap = 0);

int zdialog_add_widget(zdialog *zd,                                              //  add widget to zdialog
      cchar *type, cchar *name, cchar *pname,                                    //  (alternative form)
      cchar *data, cchar *options);                                              //  "size=nn|homog|expand|space=nn|wrap"

int zdialog_valid(zdialog *zd, cchar *title = 0);                                //  return 1/0 if zdialog valid/invalid
GtkWidget * zdialog_widget(zdialog *zd, cchar *name);                            //  GTK widget from zdialog widget name
int zdialog_set_image(zdialog *zd, cchar *name, GdkPixbuf *);                    //  set "image" widget from a GDK pixbuf
int zdialog_add_ttip(zdialog *zd, cchar *wname, cchar *ttip);                    //  add popup tool tip to a widget
int zdialog_resize(zdialog *zd, int width, int height);                          //  set size > widget sizes
int zdialog_put_data(zdialog *zd, cchar *name, cchar *data);                     //  put data in widget (entry, spin ...)
cchar * zdialog_get_data(zdialog *zd, cchar *name);                              //  get widget data
int zdialog_set_limits(zdialog *, cchar *name, double min, double max);          //  set new widget limits (spin, scale)
int zdialog_rescale(zdialog *zd, cchar *wname, float, float, float);             //  rescale widget, lo/neut/hi vals

typedef int zdialog_event(zdialog *zd, cchar *name);                             //  widget event callback function
int zdialog_run(zdialog *zd, zdialog_event = 0, cchar *posn = 0);                //  run dialog, handle events
void zdialog_KB_addshortcut(cchar *key, cchar *menu);                            //  KB shortcut for zdialog compl. button
void KBevent(GdkEventKey *event);                                                //  extern: pass KB events to main app

int zdialog_send_event(zdialog *zd, cchar *event);                               //  send an event to an active dialog
int zdialog_send_response(zdialog *zd, int zstat);                               //  complete a dialog, set status
int zdialog_show(zdialog *zd, int flag);                                         //  show or hide a dialog
int zdialog_destroy(zdialog *zd);                                                //  destroy dialog (caller resp.)
int zdialog_free(zdialog *&zd);                                                  //  free zdialog memory
int zdialog_wait(zdialog *zd);                                                   //  wait for dialog completion
int zdialog_goto(zdialog *zd, cchar *name);                                      //  put cursor at named widget
void zdialog_set_cursor(zdialog *zd, GdkCursor *cursor);                         //  set cursor for dialog window

int zdialog_stuff(zdialog *zd, cchar *name, cchar *data);                        //  stuff string data into widget
int zdialog_stuff(zdialog *zd, cchar *name, int  data);                          //  stuff int data
int zdialog_stuff(zdialog *zd, cchar *name, double data);                        //  stuff double data
int zdialog_stuff(zdialog *zd, cchar *name, double data, cchar *format);         //  stuff double data, formatted
int zdialog_labelfont(zdialog *zd, cchar *lab, cchar *font, cchar *txt);         //  stuff label text with font

int zdialog_fetch(zdialog *zd, cchar *name, char *data, int maxcc);              //  get string data from widget
int zdialog_fetch(zdialog *zd, cchar *name, int  &data);                         //  get int data
int zdialog_fetch(zdialog *zd, cchar *name, double &data);                       //  get double data
int zdialog_fetch(zdialog *zd, cchar *name, float &data);                        //  get float data

int zdialog_cb_app(zdialog *zd, cchar *name, cchar *data);                       //  append entry to combo drop down list
int zdialog_cb_prep(zdialog *zd, cchar *name, cchar *data);                      //  prepend entry to combo drop down list
char * zdialog_cb_get(zdialog *zd, cchar *name, int Nth);                        //  get combo drop down list Nth entry
int zdialog_cb_delete(zdialog *zd, cchar *name, cchar *data);                    //  delete combo drop down list entry
int zdialog_cb_clear(zdialog *zd, cchar *name);                                  //  clear all combo box entries
int zdialog_cb_popup(zdialog *zd, cchar *name);                                  //  show all combo box list entries
int zdialog_cb_save(zdialog *zd, cchar *name, cchar *file);                      //  save combo box list to a file
int zdialog_cb_load(zdialog *zd, cchar *name, cchar *file);                      //  load combo box list from a file

int  zdialog_geometry(cchar *action);                                            //  load/save zdialog window positiion/size
void zdialog_set_position(zdialog *zd, cchar *posn);                             //  set initial/new zdialog window position
void zdialog_save_position(zdialog *zd);                                         //  save zdialog window position

int zdialog_inputs(cchar *action);                                               //  load or save zdialog input fields
int zdialog_save_inputs(zdialog *zd);                                            //  save zdialog input fields when finished
int zdialog_restore_inputs(zdialog *zd);                                         //  restore zdialog inputs from prior use

void window_to_mouse(GtkWidget *window);                                         //  move GtkWidget/window to mouse position

//  write text report in popup window

zdialog * popup_report_open(cchar *title, GtkWidget *parent, int ww, int hh,           //  open popup report - pixel size,
                       int Fheader, textwidget_callbackfunc_t CBfunc, ...);            //    header line, callback function
void popup_report_header(zdialog *zd, int bold, cchar *format, ...);                   //  write non-scrolling header line
void popup_report_write(zdialog *zd, int bold, cchar *format, ...);                    //  write text line
void popup_report_top(zdialog *zd);                                                    //  go to top of report window
void popup_report_bottom(zdialog *zd);                                                 //  go to bottom of report window
void popup_report_clear(zdialog *zd);                                                  //  clear report window
void popup_report_clear(zdialog *zd, int line);                                        //  clear from line to end
void popup_report_insert(zdialog *zd, int bold, int line, cchar *format, ...);         //  insert new line
void popup_report_replace(zdialog *zd, int bold, int line, cchar *format, ...);        //  replace existing line
void popup_report_delete(zdialog *zd, int line);                                       //  delete line
int  popup_report_find(zdialog *zd, char *matchtext, int line1);                       //  find matching line
void popup_report_insert_pixbuf(zdialog *zd, int line, GdkPixbuf *pixbuf);             //  insert pixbuf image after line
void popup_report_scroll(zdialog *zd, int line);                                       //  scroll to make line visible
void popup_report_scroll_top(zdialog *zd, int line);                                   //  scroll to put line at top
void popup_report_get_visible_lines(zdialog *zd, int &top, int &bott);                 //  get visible lines range
char * popup_report_line(zdialog *zd, int line, int strip);                            //  retrieve line (strip \n)
char * popup_report_word(zdialog *zd, int line, int posn, cchar *dlims, char &end);    //  retrieve word
void popup_report_highlight_line(zdialog *zd, int line);                               //  highlight line
void popup_report_highlight_word(zdialog *zd, int line, int posn, int cc);             //  highlight word
void popup_report_underline_word(zdialog *zd, int line, int posn, int cc);             //  underline word
void popup_report_bold_word(zdialog *zd, int line, int posn, int cc);                  //  bold word
void popup_report_font_attributes(zdialog *zd);                                        //  font attributes for entire report
void popup_report_close(zdialog *zd, int secs);                                        //  close window after seconds

//  shell command to popup window

int popup_command(cchar *command, int ww = 400, int hh = 300, GtkWidget *parent = 0, int top = 0);

//  popup message dialogs

void zmessageACK(GtkWidget *parent, cchar *format, ... );                        //  display message, wait for OK
int  zmessageYN(GtkWidget *parent, cchar *format, ... );                         //  display message, wait for YES/NO
zdialog * zmessage_post(GtkWidget *, cchar *loc, int s, cchar *f, ...);          //  show message, timeout or cancel
zdialog * zmessage_post_bold(GtkWidget *, cchar *loc, int s, cchar *f, ...);     //   " " with big red bold font
char * zdialog_text(GtkWidget *parent, cchar *title, cchar *inittext);           //  get short text input from user
int  zdialog_choose(GtkWidget *parent, cchar *where, cchar *message, ...);       //  show message, return choice
void poptext_screen(cchar *text, int px, int py, float s1, float s2);            //  show popup text at screen posn
void poptext_mouse(cchar *text, int dx, int dy, float s1, float s2);             //   " " " at mouse posn + offset
void poptext_window(GtkWindow *, cchar *tx, int x, int y, float s1, float s2);   //   " " " at window posn + offset
void poptext_widget(GtkWidget *, cchar *tx, int x, int y, float s1, float s2);   //   " " " at widget posn + offset
int  poptext_killnow();                                                          //  kill current popup window
int  popup_image(cchar *imagefile, GtkWindow *parent, int Fnewin, int size);     //  popup window with image

//  file chooser dialogs for one file, multiple files, or folder

char  * zgetfile(cchar *title, GtkWindow *parent, cchar *action, cchar *file, int hidden = 0);
char ** zgetfiles(cchar *title, GtkWindow *parent, cchar *action, cchar *file, int hidden = 0);
char  * zgetfolder(cchar *title, GtkWindow *parent, cchar *initfolder);

//  print an image file, choosing printer, paper, orientation, margins, and scale

void print_image_file(GtkWidget *parent, cchar *imagefile);

//  drag and drop functions

typedef char * drag_drop_source_func();                                          //  user function, set drag-drop text
typedef void drag_drop_dest_func(int x, int y, char *text);                      //  user function, get drag-drop text
void drag_drop_source(GtkWidget *window, drag_drop_source_func);                 //  connect source window to user function
void drag_drop_dest(GtkWidget *window, drag_drop_dest_func);                     //  connect dest. window to user function

//  miscellaneous GDK/GTK functions

PIXBUF * get_thumbnail(cchar *fpath, int size);                                  //  get sized thumbnail for image file
GdkCursor * zmakecursor(cchar *iconfile);                                        //  make a cursor from an image file
PIXBUF * gdk_pixbuf_rotate(PIXBUF *, float deg, int alfa = 0);                   //  rotate pixbuf through any angle
PIXBUF * gdk_pixbuf_stripalpha(PIXBUF *pixbuf);                                  //  strip alpha channel from pixbuf
PIXBUF * text_pixbuf(cchar *text, cchar *font, int fsize, GtkWidget *);          //  create pixbuf with text using font

int  move_pointer(GtkWidget *, int px, int py);                                  //  move the mouse pointer to px, py


/********************************************************************************
   C++ classes
*********************************************************************************/

//  dynamic string class ========================================================

class xstring
{
      static int  tcount;                                                        //  total xstring count
      static int  tmem;                                                          //  total memory used
      int         wmi;                                                           //  internal ID
      int         xcc;                                                           //  actual cc (excl. NULL)
      int         xmem;                                                          //  memory allocated cc
      char      * xpp;                                                           //  memory pointer
   public:
      xstring(int cc = 0);                                                       //  default constructor
      xstring(cchar * );                                                         //  string constructor
      xstring(const xstring &);                                                  //  copy constructor
      ~xstring();                                                                //  destructor
      operator cchar * () const { return xpp; }                                  //  conversion operator (cchar *)
      xstring operator= (const xstring &);                                       //  operator =
      xstring operator= (cchar *);                                               //  operator =
      friend xstring operator+ (const xstring &, const xstring &);               //  operator +
      friend xstring operator+ (const xstring &, cchar *);                       //  operator +
      friend xstring operator+ (cchar *, const xstring &);                       //  operator +
      void insert(int pos, cchar * string, int cc = 0);                          //  insert substring at position (expand)
      void overlay(int pos, cchar * string, int cc = 0);                         //  overlay substring (possibly expand)
      static void getStats(int & tcount2, int & tmem2);                          //  get statistics
      void validate() const;                                                     //  verify integrity
      int getcc() const { return xcc; }                                          //  return string length
};

//  vector (array) of xstring ===================================================

class Vxstring
{
   int        nd;                                                                //  count
   xstring  * pdata;                                                             //  xstring[nd]
public:
   Vxstring(int = 0);                                                            //  constructor
   ~Vxstring();                                                                  //  destructor
   Vxstring(const Vxstring &);                                                   //  copy constructor
   Vxstring operator= (const Vxstring &);                                        //  operator =
   xstring & operator[] (int);                                                   //  operator []
   const xstring & operator[] (int) const;                                       //  operator [] (const)
   int search(cchar * string);                                                   //  find element in unsorted Vxstring
   int bsearch(cchar * string);                                                  //  find element in sorted Vxstring
   int sort(int nkeys, int keys[][3]);                                           //  sort by designated subfields
   int sort(int pos = 0, int cc = 0);                                            //  sort by 1 subfield (cc 0 = all)
   int getCount() const { return nd; }                                           //  get current count
};

//  hash table class ============================================================

class HashTab
{
   static int tries1;                                                            //  insert tries
   static int tries2;                                                            //  find/delete tries
   int     cap;                                                                  //  table capacity
   int     count;                                                                //  strings contained
   int     cc;                                                                   //  string length
   char  * table;                                                                //  table[cc][cap]
public:
   HashTab(int cc, int cap);                                                     //  constructor
   ~HashTab();                                                                   //  destructor
   int Add(cchar * string);                                                      //  add a new string
   int Del(cchar * string);                                                      //  delete a string
   int Find(cchar * string);                                                     //  find a string
   int GetCount() { return count; };                                             //  get string count
   int GetNext(int & first, char * string);                                      //  get first/next string
   int Dump();                                                                   //  dump hash table
};

//  Queue class, FIFO, LIFO or mixed ============================================

class  Queue
{
   char        wmi[8];
   Vxstring  * vd;                                                               //  vector of xstrings
   mutex_t     qmutex;                                                           //  for multi-thread access
   int         qcap;                                                             //  queue capacity
   int         qcount;                                                           //  curr. queue count
   int         ent1;                                                             //  first entry pointer
   int         entN;                                                             //  last entry pointer
private:
   void lock();                                                                  //  auto locking and unlocking
   void unlock();                                                                //  (for multi-thread access)
public:
   Queue(int cap);                                                               //  create queue with capacity
   ~Queue();                                                                     //  destroy queue
   int getCount();                                                               //  get current entry count
   int push(const xstring * entry, double secs);                                 //  add new entry with max. wait time
   xstring * pop1();                                                             //  get 1st entry (oldest)
   xstring * popN();                                                             //  get Nth entry (newest)
};

/*  =============================================================================

       Tree class - sparse array indexed by names or numbers
                  - every element of a Tree is a Tree

       put(): cc is data length to store
       get(): cc is max. data length to retrieve
              actual length is returned, = 0 if not found
              nn is array count for nodes[] arguments
*/

class  Tree
{
   int         wmi;                                                              //  for ID checking
   char       *tname;                                                            //  tree name
   int         tmem;                                                             //  tree data memory
   void       *tdata;                                                            //  tree data[tmem]
   int         nsub;                                                             //  no. sub-nodes (Trees)
   Tree      **psub;                                                             //  pointer to sub-nodes
public:
   Tree(cchar * name);                                                           //  create Tree
   ~Tree();                                                                      //  destroy Tree
   int put(void * data, int cc, char * nodes[], int nn);                         //  put data by node names[]
   int put(void * data, int cc, int nodes[], int nn);                            //  put data by node numbers[]
   int get(void * data, int cc, char * nodes[], int nn);                         //  get data by node names[]
   int get(void * data, int cc, int nodes[], int nn);                            //  get data by node numbers[]
   void stats(int nnodes[], int ndata[]);                                        //  get nodes and data per level
   void dump(int level = 0);                                                     //  diagnostic
private:
   Tree * find(char * nodes[], int nn);                                          //  find a sub-node by names[]
   Tree * find(int nodes[], int nn);                                             //  find a sub-node by numbers[]
   Tree * make(char * nodes[], int nn);                                          //  find/create a sub-node by names[]
   Tree * make(int nodes[], int nn);                                             //  find/create a sub-node by numbers[]
};



