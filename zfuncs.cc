/********************************************************************************

   zfuncs.cpp   collection of Linux and GDK/GTK utility functions

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

#include "zfuncs.h"

/********************************************************************************

   Table of Contents
   =================

   System Utility Functions
   ------------------------
   zmalloc zfree zstrdup   replace malloc() etc. to add checks and statistics
   printz                  printf() with immediate fflush()
   zexit                   exit a process and kill all child processes
   zbacktrace              callable backtrace dump
   zappcrash               abort with traceback dump to desktop file
   catch_signals           trap segfault, crash with zappcrash()
   TRACE                   trace() and tracedump() implement the TRACE macro
   beroot                  restart image as root, if password is OK
   runroot                 run a command or program as root user
   timer functions         elapsed time, CPU time, process time functions
   compact_time            convert time_t type to yyyymmddhhmmss format
   pretty_datetime         convert time_t type to yyyy-mm-dd hh:mm:ss format
   proc file functions     parse data from various /proc files
   coretemp                get current processor core temperature
   disktemp                get temperature for given disk drive
   zsleep                  sleep for any amount of time (e.g. 0.1 seconds)
   zloop                   loop for any amount of time
   global_lock             lock/unlock a global resource (all processes/threads)
   resource_lock           lock/unlock a resource within a process + threads
   zget_locked, etc.       safely access parameters from multiple threads
   start_detached_thread   start a detached thread
   start_Jthread           start a joinable thread
   wait_Jthread            wait for thread and join
   synch_threads           make threads pause and resume together
   set_cpu_affinity        set CPU (SMP) affinity for calling process or thread
   Qshell                  run shell command in parallel thread
   command_output          start shell command and read the output as records
   signalProc              pause, resume, or kill a child process
   fgets_trim              fgets() with trim of trailing \r \n and optionally blanks
   samefolder                test if two files/folders have the same folder path
   parsefile               parse filespec into folder, file, extension
   renamez                 like rename() but works across file systems
   check_create_dir        check if folder exists, ask to create if not
   copyFile                copy file to file or file to folder
   zreaddir                return all files in a folder, sorted
   combine_argvs           catenate argv[ii] elements from Nth to last
   zescape_quotes          escape quote marks (") in file names
   cpu_profile             measure CPU time spent per function or code block
   pagefaultrate           monitor and report own process hard page fault rate

   String Functions
   ----------------
   strField                get delimited substrings from input string
   strParms                parse a string in the form "parm1=1.23, parm2=22 ..."
   strHash                 hash string to random number in a range
   strncpy0                strncpy() with insured null delimiter
   strnPad                 add blank padding to specified length
   strTrim                 remove trailing blanks
   strTrim2                remove leading and trailing blanks
   strCompress             remove embedded blanks
   strncatv                catenate multiple strings with length limit
   strmatchV               compare 1 string to N strings
   strToUpper              convert string to upper case
   strToLower              convert string to lower case
   repl_1str               replace substring within string
   repl_Nstrs              replace multiple substrings within string
   breakup_text            insert newline chars to limit text line lengths
   strncpyx                convert string to hex format
   StripZeros              remove trailing zeros (1.23000E+8 >> 1.23E+8)
   blank_null              test string for null pointer, zero length, and all blanks
   clean_escapes           replace 2-character escapes ("\n") with the escaped characters
   UTF-8 functions         deal with UTF-8 multibyte character strings
   zsed                    substitute multiple strings in a file
   zstrstr                 zstrstr() and zcasestrstr() work like strstr() and strcasestr()
   zstrcasestr               but the string "" does NOT match with any string

   Number Conversion and Formatting
   --------------------------------
   convSI            string to integer with optional limits check
   convSD            string to double with optional limits check
   convSF            string to float with optional limits check
   convIS            integer to string with returned length
   convDS            double to string with specified digits of precision
   formatKBMB        format a byte count with specified precision and B/KB/MB/GB units

   Wildcard Functions
   ------------------
   MatchWild               match string to wildcard string (multiple * and ?)
   MatchWildIgnoreCase     works like MatchWild() but ignores case
   SearchWild              wildcard file search (multiple * and ? in path or file name)
   SearchWildCase          works like SearchWild() but ignores case in file name
   zfind                   find function using glob()

   Search and Sort Functions
   -------------------------
   bsearch           binary search of sorted list
   HeapSort          sort list of integer / float / double / records / pointers to records
   MemSort           sort records with multiple keys (data types and sequence)
   zmember           test if a value is a member of a set of values

   Misc Functions
   --------------
   pvlist_create, etc.     functions to manage a list of variable strings
   random numbers          int and double random numbers with improved distributions
   spline1/2               cubic spline curve fitting function
   Qtext                   FIFO queue for text strings, dual thread access

   Application Admin Functions
   ---------------------------
   appimage_install        make desktop and icon files for appimage menu integration
   appimage_unstall        uninstall appimage program and desktop files
   zinitapp etc.           initialize application folder and data files
   zsetfont                set new application font
   widget_font_metrics     get font width and height for given widget
   locale_filespec         find a locale-dependent installation file (translate-xx.po etc.)
   showz_logfile           display application log file
   showz_textfile          show application text file (README, changelog, etc.)
   showz_html              show a local or remote HTML file
   showz_docfile           show a document file topic and associated image
   validate_docfile        validate docfile internal links
   appruns_update          update application appruns file
   phone_home              send anonymous statistics if allowed
   phone_home_allow        dialog to allow/block phone_home messages
   E2X(), etc.             translate GUI and message text strings for non-English locales

   GTK Utility Functions
   ---------------------
   zmainloop               do main loop to process menu events, etc.
   zmainsleep              loop zmainloop and zsleep for designated time
   draw_context_create     get cairo drawing context for GDK window
   textwidget              text report, navigation, line editing
   menus / toolbars        simplified GTK menus, toolbars and status bars
   Gmenuz                  customizable graphic popup menu
   create_popmenu          implement popup menus
   Vmenu                   vertical menu/toolbar in vertical packing box
   zdialog_new                create new zdialog
   zdialog_set_title          change a zdialog title
   zdialog_set_modal          set a zdialog to be modal
   zdialog_set_decorated      set a zdialog to be decorated or not
   zdialog_present            present a zdialog (visible and on top)
   zdialog_can_focus          set zdialog can or cannot have focus
   zdialog_set_focus          set focus on zdialog window or window + widget
   zdialog_add_widget         add widget to existing zdialog
   zdialog_valid              return 1/0 if zdialog is valid/invalid
   zdialog_widget             get GTK widget from zdialog and widget name
   zdialog_add_ttip           add a popup tool tip to a zdialog widget       
   zdialog_set_group          set a common group for a set of radio buttons
   zdialog_resize             resize zdialog greater than initial size
   zdialog_set_limits         set new limits for numeric data entry widget
   zdialog_rescale            expande scale around neutral value
   zdialog_run                run the zdialog and send events to event function
   zdialog_widget_event       respond to zdialog widget events
   zdialog_focus_event        response handler for "focus-in-event" signal
   zdialog_KB_addshortcut     set KB shortcuts for zdialog completion buttons
   zdialog_KB_press           respond to zdialog keyboard inputs
   zdialog_zspin_event        response function for "zspin" widget
   zdialog_send_event         send an event to an active zdialog
   zdialog_send_response      complete a zdialog and assign status
   zdialog_show               show or hide a zdialog window
   zdialog_destroy            destroy a zdialog (data remains available)
   zdialog_free               free zdialog memory (data is gone)
   zdialog_wait               wait for zdialog completion, get status
   zdialog_goto               put cursor at named widget
   zdialog_set_cursor         set zdialog cursor (e.g. busy)
   zdialog_stuff              stuff data into zdialog widget
   zdialog_labelfont          set label text with font
   zdialog_fetch              fetch data from zdialog widget
   zdialog_cb_app             append item to combo box list widget
   zdialog_cb_prep            prepend item to combo box list widget
   zdialog_cb_get             get data from combo box entry widget
   zdialog_cb_delete          delete combo box entry from list
   zdialog_cb_clear           clear combo box list
   zdialog_cb_popup           make combo box show list of all entries
   zdialog_cb_save            save combo box list to a file
   zdialog_cb_load            load combo box list from a file
   zdialog_geometry           load/save zdialog positions at app start/exit
   zdialog_set_position       set zdialog position: null mouse desktop parent save nn/nn
   zdialog_inputs             load/save zdialog inputs at app start/exit
   zdialog_restore_inputs     retrieve prior zdialog input fields
   window_to_mouse         move a GtkWindow to the mouse position
   popup_report            popup window and scrolling text report
   popup_command           run a shell command with output in a popup window
   zmessageACK             popup message, printf format, wait for user ACK
   zmessageYN              popup message, printf format, wait for user Yes / No
   zmessage_post           popup message, printf format, show until killed
   zmessage_post_bold      popup message, printf format, big bold red font
   zdialog_text            popup zdialog to get 1-2 lines of text input from user
   zdialog_choose          popup zdialog to show a message, select a button, return choice
   poptext_screen          popup message at given absolute screen position
   poptext_mouse           popup message at current mouse position + offset
   poptext_window          popup message at given window position + offset
   poptext_widget          popup message at given widget position + offset
   poptext_killnow         kill popup message
   popup_image             show an image in a small popup window
   zgetfile                simplified file chooser zdialog
   print_image_file        zdialog to print an image file using GTK functions
   drag_drop_source        connect window as drag-drop source
   drag_drop_dest          connect window as drag-drop destination
   get_thumbnail           get thumbnail image for given image file
   zmakecursor             make a cursor from an image file (.png .jpg)
   gdk_pixbuf_rotate       rotate a pixbuf through any angle
   gdk_pixbuf_stripalpha   remove an alpha channel from a pixbuf
   text_pixbuf             create pixbuf containing text 
   move_pointer            move the mouse pointer within a widget/window

   C++ Classes
   -----------
   xstring              string manipulation (= / + / insert / overlay)
   Vxstring             array of xstrings with auto growth
   HashTab              hash table: add, delete, find, step through
   Queue                queue of xstrings: push, pop first or last
   Tree                 store / retrieve data by node names or numbers, any depth


*********************************************************************************/


namespace zfuncs
{
   struct timeb   startime;                                                      //  app startup time
   GdkDisplay     *display;                                                      //  workstation (KB, mouse, screen)
   GdkScreen      *screen;                                                       //  screen, N monitors
   GdkDevice      *mouse;                                                        //  pointer device
   GtkSettings    *gtksettings = 0;                                              //  screen settings
   cchar       *build_date_time = __DATE__ " " __TIME__;                         //  build date and time
   char        *progexe = 0;                                                     //  executable image file
   char        *appimagexe = 0;                                                  //  appimage executable image file
   int         monitor_ww, monitor_hh;                                           //  monitor dimensions
   int         appfontsize = 10;                                                 //  application font size
   cchar       *appfont = "sans 10";                                             //  application font defaults
   cchar       *appboldfont = "sans bold 10";
   cchar       *appmonofont = "mono 10";
   cchar       *appmonoboldfont = "mono bold 10";
   char        zappname[40] = "undefined";                                       //  appname without version
   char        zappvers[40] = "undefined";                                       //  appname-N.N
   char        zprefix[200], zdatadir[200], zdocdir[200];                        //  app folders
   char        zlocalesdir[200], zimagedir[200], zhomedir[200];
   char        zlocale[8] = "en";                                                //  "lc" or "lc_RC"
   pthread_t   tid_main = 0;                                                     //  main() thread ID
   int         vmenuclickposn;                                                   //  Vmenu image click posn. 0-100
   int         vmenuclickbutton;                                                 //  button: 1/2/3 = L/M/R mouse
   zdialog     *zdialog_list[zdialog_max];                                       //  active zdialog list
   int         zdialog_count = 0;                                                //  total zdialogs (new - free)
   int         zdialog_busy = 0;                                                 //  open zdialogs (run - destroy)
}

using namespace zfuncs;


/********************************************************************************
   system-level utility functions
*********************************************************************************/

//  malloc() free() and strdup() wrappers with added functionality

void * zmalloc(size_t cc)
{
   double      memavail, fcc;
   static int  ftf = 1, memcheck = 1;
   
   if (ftf) {                                                                    //  first call
      ftf = 0;
      parseprocfile("/proc/meminfo","MemAvailable:",&memavail,0);                //  'MemAvailable' available?
      if (! memavail) memcheck = 0;                                              //  no, memory check not possible
   }
   
   if (memcheck && cc > 10000)                                                   //  large block
   {
      parseprocfile("/proc/meminfo","MemAvailable:",&memavail,0);                //  free memory (+ cache) KB units
      memavail = memavail / 1024;                                                //  do compare in MB units
      fcc = cc / 1024 / 1024;
      if (memavail - fcc < 300) zexit("OUT OF MEMORY");                          //  < 300 MB, quit before OOM killer
   }

   void *pp = malloc(cc);                                                        //  allocate memory
   if (! pp) zexit("OUT OF MEMORY");

   memset(pp,0,cc);                                                              //  clear to zero
   return pp;
}


void zfree(void *puser)
{  
   return free(puser);  
}


char *zstrdup(cchar *string, int addcc)
{
   if (! string) zappcrash("zstrdup() null arg");
   char *pp = (char *) zmalloc(strlen(string) + 1 + addcc);                      //  add additional chars.
   strcpy(pp,string);
   return pp;
}


/********************************************************************************/

//  printf() and flush every output immediately even if stdout is a file

void printz(cchar *format, ...)
{
   va_list  arglist;

   va_start(arglist,format);
   vprintf(format,arglist);
   va_end(arglist);

   fflush(stdout);
   return;
}


/********************************************************************************/

//  Output a status or error message and kill all processes in the process group.
//  Use the function killpg(0,SIGKILL) to kill all processes, including the caller.

void zexit(cchar *errmess, ...)
{
   va_list  arglist;
   char     buff[200];

   if (errmess) {                                                                //  output error message
      va_start(arglist,errmess);
      vsnprintf(buff,200,errmess,arglist);
      printz("zexit: %s\n",buff);
   }
   else printz("zexit\n");

   killpg(0,SIGKILL);                                                            //  kill all processes in group
   sleep(10);                                                                    //  wait here to die
}


/********************************************************************************/

//  produce a backtrace dump to stdout

void zbacktrace()
{
   int      nstack = 100;
   void     *stacklist[100];

   nstack = backtrace(stacklist,nstack);                                         //  get backtrace data
   if (nstack > 100) nstack = 100;
   backtrace_symbols_fd(stacklist,nstack,STDOUT_FILENO);                         //  backtrace records to STDOUT

   return;
}


/********************************************************************************/

//  Write an error message and backtrace dump to a file and to a popup window.
//  Error message works like printf().
//  Depends on library program addr2line().

void zappcrash(cchar *format, ... )
{
   static int     crash = 0;
   struct utsname unbuff;
   va_list        arglist;
   FILE           *fid1, *fid2, *fid3;
   int            fd, ii, err, cc, nstack = 100;
   int            Flinenos = 1;
   void           *stacklist[100];
   char           OS1[60] = "", OS2[60] = "", OS3[60] = "";
   char           message[300], progexe[300];
   char           buff1[300], buff2[300], hexaddr[20];
   char           *arch, *pp1, *pp2, dlim, *pfunc;

   if (crash++) return;                                                          //  re-entry or multiple threads crash

   va_start(arglist,format);
   vsnprintf(message,300,format,arglist);
   va_end(arglist);

   uname(&unbuff);                                                               //  get cpu arch. 32/64 bit
   arch = unbuff.machine;
   fid1 = popen("lsb_release -d","r");                                           //  get Linux flavor and release
   if (fid1) {
      ii = fscanf(fid1,"%s %s %s",OS1,OS2,OS3);
      pclose(fid1);
   }

   printz("\n*** zappcrash: %s %s %s %s %s %s \n",
               arch, OS2, OS3, zappvers, build_date_time, message);

   nstack = backtrace(stacklist,nstack);                                         //  get backtrace data
   if (nstack <= 0) zexit("zappcrash backtrace() failure");
   if (nstack > 100) nstack = 100;

   fid1 = fopen("zbacktrace","w");                                               //  open backtrace data output file
   if (! fid1) zexit("zappcrash fopen() failure");

   fd = fileno(fid1);
   backtrace_symbols_fd(stacklist,nstack,fd);                                    //  write backtrace data
   fclose(fid1);                                                                 //  (use of malloc() is avoided)

   fid1 = fopen("zbacktrace","r");                                               //  open backtrace data file
   if (! fid1) zexit("zappcrash fopen() failure");

   fid2 = fopen("zappcrash","w");                                                //  open zappcrash output file
   if (! fid2) zexit("zappcrash fopen() failure");

   fprintf(fid2,"\n*** zappcrash: %s %s %s %s %s %s \n",
                     arch, OS2, OS3, zappvers, build_date_time, message);
   fprintf(fid2,"*** please send to kornelix@posteo.de *** \n");

   cc = readlink("/proc/self/exe",progexe,300);                                  //  get own program path
   if (cc > 0) progexe[cc] = 0;                                                  //  readlink() quirk
   else {
      fprintf(fid2,"progexe not available \n");
      Flinenos = 0;
   }

   err = Qshell(0,0,"which addr2line >> /dev/null");                             //  check if addr2line() available
   if (err) {
      fprintf(fid2,"addr2line not available \n");
      Flinenos = 0;
   }

   for (ii = 0; ii < nstack; ii++)                                               //  loop backtrace records
   {
      pp1 = pp2 = 0;
      fgets_trim(buff1,300,fid1);                                                //  read backtrace line
      if (! Flinenos) goto output;
      pfunc = 0;
      pp1 = strstr(buff1,"+0x");                                                 //  new format (+0x12345...)
      if (pp1) pp2 = strchr(pp1,')');
      else {
         pp1 = strstr(buff1,"[0x");                                              //  old format [0x12345...]
         if (pp1) pp2 = strchr(pp1,']');
      }
      if (! pp1 || ! pp2) goto output;                                           //  cannot parse
      dlim = *pp2;
      *pp2 = 0;
      strncpy0(hexaddr,pp1+1,20);
      *pp2 = dlim;
      snprintf(buff2,300,"addr2line -i -e %s %s",progexe,hexaddr);               //  convert to source program
      fid3 = popen(buff2,"r");                                                   //    and line number
      if (! fid3) goto output;
      pfunc = fgets(buff2,300,fid3);
      pclose(fid3);
      if (! pfunc) goto output;
      cc = strlen(pfunc);
      if (cc < 10) goto output;
      if (pfunc[cc-1] < ' ') pfunc[cc-1] = 0;                                    //  remove tailing \n if present
      strncatv(buff1,300,"\n--- ",pfunc,null);
   output:
      fprintf(fid2,"%s \n",buff1);                                               //  output
   }

   fclose(fid1);
   fclose(fid2);
   Qshell(0,0,"rm zbacktrace");
   Qshell(0,0,"cat zappcrash");
   Qshell(0,0,"mv zappcrash $(xdg-user-dir DESKTOP)/%s-zappcrash",zappvers);     //  move zappcrash file to desktop
   zexit("exit zappcrash");
}


/********************************************************************************/

//  application initialization function to catch some bad news signals
//  the signal handler calls zappcrash() to output a backtrace dump and exit

void catch_signals()
{
   void sighandler(int signal);
   struct sigaction  sigact;

   sigact.sa_handler = sighandler;
   sigemptyset(&sigact.sa_mask);
   sigact.sa_flags = 0;

   sigaction(SIGTERM,&sigact,0);
   sigaction(SIGSEGV,&sigact,0);
   sigaction(SIGILL,&sigact,0);                                                  //  man page says cannot be caught
   sigaction(SIGFPE,&sigact,0);
   sigaction(SIGBUS,&sigact,0);
   sigaction(SIGABRT,&sigact,0);                                                 //  heap or stack corruption
   return;
}


//  catch fatal signals and produce backtrace dumps on-screen

void sighandler(int signal)
{
   const char  *signame = "unknown";

   if (signal == SIGTERM) zexit("TERMINATED");
   if (signal == SIGKILL) zexit("KILLED");
   if (signal == SIGSEGV) signame = "segment fault";
   if (signal == SIGILL) signame = "illegal operation";
   if (signal == SIGFPE) signame = "arithmetic exception";
   if (signal == SIGBUS) signame = "bus error (bad memory)";
   if (signal == SIGABRT) signame = "abort";

   zappcrash("fatal signal: %s",signame);
   exit(0);
}


/********************************************************************************/

//  Implement the TRACE macro.
//  Trace program execution by function and source code line number.
//  tracedump() dumps last 50 uses of TRACE macro, latest first.

namespace tracenames
{
   char  filebuff[50][100];                                                      //  last 50 TRACE calls
   char  funcbuff[50][60];
   int   linebuff[50];
   void  *addrbuff[50];
   int   ii, ftf = 1;
};


//  Args are source file, source function name, source code line number,
//  caller address. These all come from the GCC compiler and TRACE macro.

void trace(cchar *file, cchar *func, int line, void *addr)
{
   using namespace tracenames;

   if (ftf) {
      ftf = 0;
      for (ii = 0; ii < 50; ii++) {
         filebuff[ii][99] = 0;
         funcbuff[ii][39] = 0;
         linebuff[ii] = 0;
         addrbuff[ii] = 0;
      }
      ii = 0;
   }

   if (line == linebuff[ii] &&
      strmatch(func,funcbuff[ii])) return;                                       //  same as last call, don't duplicate

   if (++ii > 49) ii = 0;                                                        //  add data to list
   strncpy(&filebuff[ii][0],file,99);
   strncpy(&funcbuff[ii][0],func,39);
   linebuff[ii] = line;
   addrbuff[ii] = addr;
   return;
}


//  dump trace records to STDOUT

void tracedump()
{
   using namespace tracenames;

   FILE     *fid;
   int      kk;

   printz(" *** tracedump *** \n");

   kk = ii;
   while (linebuff[kk]) {
      printz("TRACE %s %s %d %p \n",&filebuff[kk][0],
              &funcbuff[kk][0],linebuff[kk],addrbuff[kk]);
      if (--kk == ii) break;
   }

   fid = fopen("tracedump","w");
   if (! fid) {
      perror("tracedump fopen() failure \n");
      return;
   }

   fprintf(fid, " *** tracedump *** \n");

   kk = ii;
   while (linebuff[kk]) {
      fprintf(fid, "TRACE %s %s %d %p \n",&filebuff[kk][0],
                    &funcbuff[kk][0],linebuff[kk],addrbuff[kk]);
      if (--kk == ii) break;
   }

   fclose(fid);
   return;
}


/********************************************************************************/

//  restart the current program as root user (after sudo success)
//  argv[0] must be executable program

void beroot(int argc, char *argv[])
{
   int      err;
   char     *args, command[500];

   if (getuid() == 0) return;                                                    //  already root
   
   err = Qshell(0,0,"which xhost xterm");                                        //  check for necessary programs
   if (err) zexit("xhost and xterm must be installed");

   printz("become root user \n");
   Qshell(1,1,"xhost +si:localuser:root");                                       //  wayland, allow root for x11

   args = combine_argvs(argc,argv,0);
   snprintf(command,500,"xterm -fa 'Mono' -fs 15 -geometry 40x5+400+300"         //  start root process
                                       " -e sudo -b -S %s",args); 
   err = Qshell(1,1,command);

   exit(err);                                                                    //  exit old process
}


/********************************************************************************/

//  run a command or program as root user
//  command:  shell command or filespec of the program to start
//  returns 0 if successfully started, else returns an error code

int runroot(cchar *command)
{
   char     xtcommand[500];
   int      err;

   err = Qshell(0,0,"which xhost xterm");                                        //  check for necessary programs
   if (err) zexit("xhost and xterm must be installed");

   printz("become root user \n");
   Qshell(1,1,"xhost +si:localuser:root");                                       //  wayland, allow root for x11

   snprintf(xtcommand,500,"xterm -fa 'Mono' -fs 15 -geometry 40x5+400+300"
                                       " -e sudo -b -S %s",command);
   err = Qshell(1,1,xtcommand);
   return err;
}


/********************************************************************************/

//  get time in real seconds
//  theoretically uses a precise system clock but the precision is poor

double get_seconds()
{
   timespec    time1;
   double      time2;
   
   clock_gettime(CLOCK_MONOTONIC_RAW,&time1);
   time2 = time1.tv_sec;
   time2 += time1.tv_nsec * 0.000000001;
   return time2;
}


/********************************************************************************/

//  start a timer or get elapsed time with millisecond resolution.

void start_timer(double &time0)
{
   timeval  timev;

   gettimeofday(&timev,0);
   time0 = timev.tv_sec + 0.000001 * timev.tv_usec;
   return;
}

double get_timer(double &time0)
{
   timeval  timev;
   double   time;

   gettimeofday(&timev,0);
   time = timev.tv_sec + 0.000001 * timev.tv_usec;
   return time - time0;
}


/********************************************************************************/

//  perform interval timing better than Linux 4 ms or 40 ms resolution
//  resolution is 1 millisecond

double   ztimer_value = 0;                                                       //  timer value since startup

void ztimer_handler(int signal)                                                  //  timer signal handler
{
   ztimer_value++;
   return;
}

void ztimer_start()                                                              //  start timer
{
   struct itimerval  timerdata;

   ztimer_value = 0;                                                             //  reset to zero

   timerdata.it_value.tv_sec = 0;                                                //  start now
   timerdata.it_value.tv_usec = 1000;
   timerdata.it_interval.tv_sec = 0;                                             //  repeat at 1 millisec. intervals
   timerdata.it_interval.tv_usec = 1000;

   signal(SIGALRM,ztimer_handler);
   setitimer(ITIMER_REAL,&timerdata,NULL);
   return;
}

void ztimer_stop()                                                               //  stop timer
{
   struct itimerval  timerdata;

   timerdata.it_value.tv_sec = 0;
   timerdata.it_value.tv_usec = 0;
   timerdata.it_interval.tv_sec = 0;
   timerdata.it_interval.tv_usec = 0;
   setitimer(ITIMER_REAL,&timerdata,NULL);
   return;
}

int ztimer_milliseconds()                                                        //  get milliseconds since startup
{
   return ztimer_value;
}


/********************************************************************************/

//  start a process CPU timer or get elapsed process CPU time
//  returns seconds with millisecond resolution

void start_CPUtimer(double &time0)
{
   time0 = CPUtime();
   return;
}

double get_CPUtimer(double &time0)
{
   return CPUtime() - time0;
}


/********************************************************************************/

//  get elapsed CPU time used by current process
//  returns seconds with millisecond resolution

double CPUtime()
{
   clock_t ctime = clock();
   double dtime = ctime / 1000000.0;
   return dtime;
}


/********************************************************************************/

//  Get elapsed CPU time used by current process, including all threads.
//  Returns seconds with millisecond resolution.

double CPUtime2()
{
   struct rusage  usage;
   double         utime, stime;
   int            err;

   err = getrusage(RUSAGE_SELF,&usage);
   if (err) return 0.0;
   utime = usage.ru_utime.tv_sec + 0.000001 * usage.ru_utime.tv_usec;
   stime = usage.ru_stime.tv_sec + 0.000001 * usage.ru_stime.tv_usec;
   return utime + stime;
}


/********************************************************************************/

//  get elapsed process time for my process, including threads and child processes.

double jobtime()
{
   double   jiffy = 1.0 / sysconf(_SC_CLK_TCK);                                  //  "jiffy" time slice = 1.0 / HZ
   char     buff[200];
   double   cpu1, cpu2, cpu3, cpu4;
   FILE     *fid;
   char     *pp;

   fid = fopen("/proc/self/stat","r");
   if (! fid) return 0;
   pp = fgets(buff,200,fid);
   fclose(fid);
   if (! pp) return 0;
   
   parseprocrec(pp,14,&cpu1,15,&cpu2,16,&cpu3,17,&cpu4,null);
   return (cpu1 + cpu2 + cpu3 + cpu4) * jiffy;
}


/********************************************************************************/

//  convert a time_t date/time (e.g. st_mtime from stat() call)
//    into a compact date/time format "yyyymmddhhmmss"

void compact_time(const time_t DT, char *compactDT)
{
   struct tm   *fdt;
   int         year, mon, day, hour, min, sec;

   fdt = localtime(&DT);

   year = fdt->tm_year + 1900;
   mon = fdt->tm_mon + 1;
   day = fdt->tm_mday;
   hour = fdt->tm_hour;
   min = fdt->tm_min;
   sec = fdt->tm_sec;

   compactDT[0] = year / 1000 + '0';
   compactDT[1] = (year % 1000) / 100 + '0';
   compactDT[2] = (year % 100) / 10 + '0';
   compactDT[3] = year % 10 + '0';
   compactDT[4] = mon / 10 + '0';
   compactDT[5] = mon % 10 + '0';
   compactDT[6] = day / 10 + '0';
   compactDT[7] = day % 10 + '0';
   compactDT[8] = hour / 10 + '0';
   compactDT[9] = hour % 10 + '0';
   compactDT[10] = min / 10 + '0';
   compactDT[11] = min % 10 + '0';
   compactDT[12] = sec / 10 + '0';
   compactDT[13] = sec % 10 + '0';
   compactDT[14] = 0;

   return;
}


/********************************************************************************/

//  convert a time_t date/time (e.g. st_mtime from stat() call)
//    into a pretty date/time format "yyyy-mm-dd hh:mm:ss"

void pretty_datetime(const time_t DT, char *prettyDT)
{
   struct tm   *fdt;
   int         year, mon, day, hour, min, sec;

   fdt = localtime(&DT);

   year = fdt->tm_year + 1900;
   mon = fdt->tm_mon + 1;
   day = fdt->tm_mday;
   hour = fdt->tm_hour;
   min = fdt->tm_min;
   sec = fdt->tm_sec;

   prettyDT[0] = year / 1000 + '0';
   prettyDT[1] = (year % 1000) / 100 + '0';
   prettyDT[2] = (year % 100) / 10 + '0';
   prettyDT[3] = year % 10 + '0';
   prettyDT[4] = '-';
   prettyDT[5] = mon / 10 + '0';
   prettyDT[6] = mon % 10 + '0';
   prettyDT[7] = '-';
   prettyDT[8] = day / 10 + '0';
   prettyDT[9] = day % 10 + '0';
   prettyDT[10] = ' ';
   prettyDT[11] = hour / 10 + '0';
   prettyDT[12] = hour % 10 + '0';
   prettyDT[13] = ':';
   prettyDT[14] = min / 10 + '0';
   prettyDT[15] = min % 10 + '0';
   prettyDT[16] = ':';
   prettyDT[17] = sec / 10 + '0';
   prettyDT[18] = sec % 10 + '0';
   prettyDT[19] = 0;

   return;
}


/********************************************************************************/

//  Read and parse /proc file with records formatted "parmname xxxxxxx"
//  Find all requested parameters and return their numeric values

int parseprocfile(cchar *pfile, cchar *pname, double *value, ...)                //  EOL = 0
{
   FILE        *fid;
   va_list     arglist;
   char        buff[1000];
   const char  *pnames[20];
   double      *values[20];
   int         ii, fcc, wanted, found;

   pnames[0] = pname;                                                            //  1st parameter
   values[0] = value;
   *value = 0;

   va_start(arglist,value);

   for (ii = 1; ii < 20; ii++)                                                   //  get all parameters
   {
      pnames[ii] = va_arg(arglist,char *);
      if (! pnames[ii]) break;
      values[ii] = va_arg(arglist,double *);
      *values[ii] = 0;                                                           //  initialize to zero
   }

   va_end(arglist);

   if (ii == 20) zappcrash("parseProcFile, too many fields");
   wanted = ii;
   found = 0;

   fid = fopen(pfile,"r");                                                       //  open /proc/xxx file
   if (! fid) return 0;

   while ((fgets(buff,999,fid)))                                                 //  read record, "parmname nnnnn"
   {
      for (ii = 0; ii < wanted; ii++)
      {                                                                          //  look for my fields
         fcc = strlen(pnames[ii]);
         if (strmatchN(buff,pnames[ii],fcc)) {
            *values[ii] = atof(buff+fcc);                                        //  return value
            found++;
            break;
         }
      }

      if (found == wanted) break;                                                //  stop when all found
   }

   fclose(fid);
   return found;
}


//  Parse /proc record of the type  "xxx xxxxx xxxxx xxxxxxxx xxx"
//  Return numeric values for requested fields (starting with 1)

int parseprocrec(char *prec, int field, double *value, ...)                      //  EOL = 0
{
   va_list     arglist;
   int         xfield = 1, found = 0;

   va_start(arglist,value);

   while (*prec == ' ') prec++;                                                  //  skip leading blanks

   while (field > 0)
   {
      while (xfield < field)                                                     //  skip to next wanted field
      {
         prec = strchr(prec,' ');                                                //  find next blank
         if (! prec) break;
         while (*prec == ' ') prec++;                                            //  skip multiple blanks
         xfield++;
      }

      if (! prec) break;
      *value = atof(prec);                                                       //  convert, return double
      found++;

      field = va_arg(arglist,int);                                               //  next field number
      if (! field) break;
      value = va_arg(arglist,double *);                                          //  next output double *
   }

   while (field > 0)
   {
      *value = 0;                                                                //  zero values not found
      field = va_arg(arglist,int);
      value = va_arg(arglist,double *);
   }

   va_end(arglist);
   return found;
}


/********************************************************************************/

//  get current processor core temperature
//  depends on file "temp1_input" somewhere under /sys/devices/*

int coretemp()
{
   FILE         *fid;
   static int   ftf = 1;
   static char  *Tfile = 0;
   static char  buff1[200], buff2[200], *ptemp;
   int          temp;

   if (ftf) {                                                                    //  find file "temp1_input" 
      ftf = 0;                                                                   
      fid = popen("find /sys/devices/ -name temp1_input","r"); 
      if (! fid) return 0;
      Tfile = fgets(buff1,200,fid);
      pclose(fid);
      printz("coretemp file: %s \n",Tfile);
   }
   
   if (! Tfile) return 0;   
   snprintf(buff2,200,"cat %s",Tfile);
   fid = popen(buff2,"r");
   if (! fid) return 0;
   ptemp = fgets_trim(buff2,200,fid);
   pclose(fid);
   if (! ptemp) return 0;
   temp = atoi(ptemp);
   while (temp > 200) temp = temp / 10;                                          //  may be deg. C x 10 or x 100 ...
   if (temp < 10) return 0;
   return temp;
}


/********************************************************************************/

//  get current temperature for given disk, e.g. "/dev/sda"
//  depends on "smartctl" command from package smartmontools

int disktemp(char *disk)
{
   int         id, temp;
   char        *pp, *pp2;
   char        buff[200], command[100];
   FILE        *ffid;

   temp = 0;
   pp2 = 0;
   snprintf(command,100,"smartctl -A %s",disk);
   ffid = popen(command,"r");
   if (! ffid) return 0;

   while (true) {
      pp = fgets(buff,200,ffid);                                                 //  revised for smartctl report
      if (! pp) break;                                                           //    format changes
      if (strmatchN(pp,"ID#",3)) pp2 = strstr(pp,"RAW_VALUE");
      id = atoi(pp);
      if (id != 190 && id != 194) continue;                                      //  Airflow Temp. or Temp.
      if (! pp2) continue;
      temp = atoi(pp2);
      if (temp < 10 || temp > 99) temp = 0;
      break;
   }

   pclose(ffid);
   return temp;
}


/********************************************************************************/

//  sleep for specified time in seconds (double)
//  signals can cause early return

void zsleep(double dsecs)
{
   unsigned    isecs, nsecs;
   timespec    tsecs;

   if (dsecs <= 0) return;
   isecs = unsigned(dsecs);
   nsecs = unsigned(1000000000.0 * (dsecs - isecs));
   tsecs.tv_sec = isecs;
   tsecs.tv_nsec = nsecs;
   nanosleep(&tsecs,null);
   return;
}


/********************************************************************************/

//  loop for specified time in seconds (double)

void zloop(double dsecs)
{
   double  time0, time1;

   if (dsecs <= 0) return;
   time0 = get_seconds();
   time1 = time0 + dsecs;
   while (get_seconds() < time1) continue;
   return;
}


/********************************************************************************/

//  Lock or unlock a multi-process multi-thread resource.
//  Only one process/thread may possess a given lock.
//  A reboot or process exit or crash releases the lock.
//
//  char *lockfile;
//  make_global_lockfile("lockname",&lockfile);
//  fd = global_lock(lockfile);
//  err = global_unlock(fd,lockfile);
//
//  both functions return +N if success, -1 otherwise.


void make_global_lockfile(cchar *lockname, char **lockfile)
{
   int      cc;

   *lockfile = zstrdup(lockname,16);
   cc = strlen(*lockfile);
   snprintf(*lockfile+cc,12,"-%08d",getpid());                                   //  caller name with PID appended
   return;
}


int global_lock(cchar *lockfile)
{
   int       err, fd;
   
   fd = open(lockfile,O_RDWR|O_CREAT,0666);                                      //  open or create the lock file
   if (fd < 0) zappcrash("global_lock() %s",strerror(errno));

   err = flock(fd,LOCK_EX);                                                      //  request exclusive lock
   if (err) {
      close(fd);
      return -1;
   }

   return fd + 1;                                                                //  return value is >= 1
}


int global_unlock(int fd, cchar *lockfile)
{
   int err = close(fd-1);
   if (err < 0) return -1;
// remove(lockfile);                                                             //  lock/unlock 2.5x faster 
   return 1;
}


/********************************************************************************/

//  lock or unlock a resource
//  does not spin or wait, usable within or across threads
//  CANNOT BE USED for coroutines within one thread, e.g. GTK main loop
//  return 0 if already locked, otherwise lock and return 1

mutex_t resource_lock_lock = PTHREAD_MUTEX_INITIALIZER;

int resource_lock(int &resource) 
{
   if (resource) return 0;                                                       //  locked

   mutex_lock(&resource_lock_lock);
   if (resource) {
      mutex_unlock(&resource_lock_lock);                                         //  locked
      return 0;
   }
   resource = 1;
   mutex_unlock(&resource_lock_lock);
   return 1;                                                                     //  locked OK
}

//  unlock a locked resource

void resource_unlock(int &resource)
{
   mutex_lock(&resource_lock_lock);
   if (resource != 1) zappcrash("resource not locked");                          //  bug, not locked
   resource = 0;                                                                 //  unlock
   mutex_unlock(&resource_lock_lock);
   return;
}


/********************************************************************************/

//  Safely access and update parameters from multiple threads.
//  A mutex lock is used to insure one thread at a time has access to the parameter.
//  Many parameters can be used but there is only one mutex lock.
//  CANNOT BE USED for coroutines within one thread, e.g. GTK main loop.

mutex_t zget_lock = PTHREAD_MUTEX_INITIALIZER;

int zget_locked(int &param)                                                      //  lock and return parameter
{
   mutex_lock(&zget_lock);
   return param;
}

void zput_locked(int &param, int value)                                          //  set and unlock parameter
{
   param = value;
   mutex_unlock(&zget_lock);
   return;
}

int zadd_locked(int &param, int incr)                                            //  lock, increment, unlock, return
{
   int      retval;

   mutex_lock(&zget_lock);
   retval = param + incr;
   param = retval;
   mutex_unlock(&zget_lock);
   return retval;
}


/********************************************************************************/

//  Start a detached thread using a simplified protocol.
//  Will not make a zombie if caller exits without checking thread status.
//  Thread should exit with pthread_exit(0);

void start_detached_thread(void * threadfunc(void *), void * arg)
{
   pthread_attr_t pthattr;
   pthread_t      pthtid;
   int            ii, err;
   
   pthread_attr_init(&pthattr);
   pthread_attr_setdetachstate(&pthattr,PTHREAD_CREATE_DETACHED);

   for (ii = 0; ii < 1000; ii++) 
   {
      err = pthread_create(&pthtid,&pthattr,threadfunc,arg);
      if (! err) return;
      zsleep(0.001);
      if (err == EAGAIN) continue;                                               //  this shit happens
      break;
   }

   zexit("pthread_create() failure: %s",strerror(err));
}


/********************************************************************************/

//  Start a thread using a simplified protocol.
//  Caller must call wait_Jthread() to avoid creating a zombie process.

pthread_t start_Jthread(void * threadfunc(void *), void * arg)
{
   pthread_t   tid;
   int         ii, err;

   for (ii = 0; ii < 1000; ii++) 
   {
      err = pthread_create(&tid, null, threadfunc, arg);
      if (! err) return tid;
      zsleep(0.001);
      if (err == EAGAIN) continue;                                               //  this shit happens
      break;
   }

   zexit("pthread_create() failure: %s",strerror(err));
   return 0;
}


//  wait for thread to exit.

int wait_Jthread(pthread_t tid)
{
   int      err;

   err = pthread_join(tid, null);
   if (! err) return 0;
   zexit("pthread_join() failure: %s",strerror(err));
   return 0;
}


/********************************************************************************/

//  Synchronize execution of multiple threads.
//  Simultaneously resume NT calling threads.
//  from main():        synch_threads(NT)    /* setup to synch NT threads */
//  from each thread:   synch_threads()      /* suspend, resume simultaneously */
//
//  Each calling thread will suspend execution until all threads have suspended,
//  then they will all resume execution at the same time. If NT is greater than
//  the number of calling threads, the threads will never resume.

void synch_threads(int NT)
{
   static pthread_barrier_t   barrier;
   static int                 bflag = 0;

   if (NT) {                                                                     //  main(), initialize
      if (bflag) pthread_barrier_destroy(&barrier);
      pthread_barrier_init(&barrier,null,NT);
      bflag = 1;
      return;
   }

   pthread_barrier_wait(&barrier);                                               //  thread(), wait for NT threads
   return;                                                                       //  unblock
}


/********************************************************************************/

//  set a CPU affinity for the calling process or thread
//  cpu is in the range 0 to (processor core count) - 1

void set_cpu_affinity(int cpu)
{
   int         err;
   static int  ftf = 1, Nsmp;
   cpu_set_t   cpuset;

   if (ftf) {                                                                    //  first call
      ftf = 0;
      Nsmp = get_nprocs();                                                       //  get SMP CPU count
   }
   
   if (cpu >= Nsmp) return;

   CPU_ZERO(&cpuset);
   CPU_SET(cpu,&cpuset);
   err = sched_setaffinity(0,sizeof(cpuset),&cpuset);
   if (err) printz("set_cpu_affinity() %s \n",strerror(errno));
   return;
}


/********************************************************************************

   int err = Qshell(int log, int ack, cchar *command, ...)

   Format and perform a shell command, wait for completion, return status.

   The shell command is done in a thread. The wait loop includes zmainsleep(),
   so that long execution does not raise a GTK 'not responding' alarm.

       log:  1 = write command to log file, 0 = do not write
       ack:  1 = popup user ACK message if error, 0 = no message
   command:  shell command with optional '%' printf formats
     ...  :  optional arguments to stuff into printf formats

***/

namespace Qshell_names
{
   char     *command[10];
   int      status[10];
   int      ii2[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   mutex_t  lock = PTHREAD_MUTEX_INITIALIZER;
}

int Qshell(int log, int ack, cchar *Fcommand, ...)                               //  21.0
{
   using namespace Qshell_names;

   void * Qshell_thread(void *);

   va_list     arglist;
   int         ii, cc, cc2, err;
   int         Fthread = 0;
   
   mutex_lock(&lock);                                                            //  block other callers

   for (ii = 0; ii < 10; ii++)                                                      
      if (command[ii] == 0) break;                                               //  find my slot 0-9
   if (ii == 10) zappcrash("Qshell > 10 calls");

   mutex_unlock(&lock);                                                          //  unblock

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  set main or created thread
      Fthread = 1;

   cc = strlen(Fcommand) + 1000;
   command[ii] = (char *) zmalloc(cc+1);                                         //  allocate memory

   va_start(arglist,Fcommand);                                                   //  format command
   cc2 = vsnprintf(command[ii],cc,Fcommand,arglist);
   va_end(arglist);
   if (cc2 >= cc) zappcrash("Qshell: buffer overflow: %d",cc2);

   if (log) printz("Qshell: %s \n",command[ii]);                                 //  command > log file

   status[ii] = -1;                                                              //  status = busy
   start_detached_thread(Qshell_thread,&ii2[ii]);                                //  pass command to thread

   if (Fthread) { while (status[ii] == -1) zsleep(0.01); }                       //  wait for completion - thread
   else { while (status[ii] == -1) zmainsleep(0.01); }                           //   - main
   
   err = status[ii];                                                             //  command status
   if (err == 127) err = 1;                                                      //  "key expired" > "not permitted"

   if (err && log) printz("Qshell error: %s \n",strerror(err));                  //  log error if wanted
   if (err && ack) zmessageACK(0,strerror(err));                                 //  popup to user if wanted

   zfree(command[ii]);                                                           //  free memory
   command[ii] = 0;
   return err;                                                                   //  return completion status
}

void * Qshell_thread(void *arg)                                                  //  thread function
{
   using namespace Qshell_names;
   int      err, ii = *((int *) arg);
   err = system(command[ii]);                                                    //  run command, returns when done
   status[ii] = WEXITSTATUS(err);                                                //  get command status
   return 0;
}


/********************************************************************************

   Run a shell command and get its outputs one record at a time.
   The outputs are returned one record at a time, until a NULL is returned,
   indicating the command has finished and has exited.
   The new line character is removed from the returned output records.
   Use contx = 0 to start a new command. Do not change the returned value.
   Up to 9 commands can run in parallel, with contx values 1-9.
   To get the command exit status: status = command_status(contx).
   If the command is still busy, -1 is returned.
   To kill a command before output is complete: command_kill(contx);

***/

FILE *   CO_contx[10] = { 0,0,0,0,0,0,0,0,0,0 };
int      CO_status[10];

char * command_output(int &contx, cchar *command, ...)                           //  simplify, allow parallel usage
{
   FILE        *fid;
   va_list     arglist;
   char        buff[10000], *prec;

   if (contx == 0)                                                               //  start new command
   {
      for (contx = 1; contx < 10; contx++)
         if (CO_contx[contx] == 0) break;
      if (contx == 10) {
         printz("*** command_output(), parallel usage > 9 \n");
         return 0;
      }

      va_start(arglist,command);                                                 //  format command
      vsnprintf(buff,9999,command,arglist);
      va_end(arglist);
      
      fid = popen(buff,"r");                                                     //  execute command, output to FID
      if (fid == 0) {
         CO_status[contx] = errno;                                               //  failed to start
         printz("*** command_output: %s\n %s\n",buff,strerror(errno));
         return 0;
      }
      CO_contx[contx] = fid + 1000;
      CO_status[contx] = -1;                                                     //  mark context busy
   }

   fid = CO_contx[contx] - 1000;
   prec = fgets_trim(buff,9999,fid,1);                                           //  next output, less trailing \n
   if (prec) return zstrdup(prec);
   CO_status[contx] = pclose(fid);                                               //  EOF, set status
   CO_contx[contx] = 0;                                                          //  mark context free
   return 0;
}

int command_status(int contx)                                                    //  get command exit status
{
   int err = CO_status[contx];
   return WEXITSTATUS(err);                                                      //  special BS for subprocess
}

int command_kill(int contx)                                                      //  kill output before completion 
{
   FILE     *fid;
   if (! CO_contx[contx]) return 0;                                              //  context already closed
   fid = CO_contx[contx] - 1000;
   CO_status[contx] = pclose(fid);                                               //  close context and set status
   CO_contx[contx] = 0;                                                          //  mark context free
   return 0;
}


/********************************************************************************/

//  Signal a running subprocess by name (name of executable or shell command).
//  Signal is "pause", "resume" or "kill". If process is paused, kill may not work,
//  so issue resume first if process is paused.

int signalProc(cchar *pname, cchar *signal)
{
   pid_t       pid;
   FILE        *fid;
   char        buff[100], *pp;
   int         err, nsignal = 0;

   snprintf(buff,100,"ps -C %s h o pid",pname);
   fid = popen(buff,"r");                                                        //  popen() instead of system()
   if (! fid) return 2;
   pp = fgets(buff,100,fid);
   pclose(fid);
   if (! pp) return 4;

   pid = atoi(buff);
   if (! pid) return 5;

   if (strmatch(signal,"pause")) nsignal = SIGSTOP;
   if (strmatch(signal,"resume")) nsignal = SIGCONT;
   if (strmatch(signal,"kill")) nsignal = SIGKILL;

   err = kill(pid,nsignal);
   return err;
}


/********************************************************************************/

//  fgets() with additional feature: trailing \n \r are removed.
//  optional bf flag: true if trailing blanks are to be removed.
//  trailing null character is assured.

char * fgets_trim(char *buff, int maxcc, FILE *fid, int bf)
{
   int      cc;
   char     *pp;

   pp = fgets(buff,maxcc,fid);
   if (! pp) return pp;
   cc = strlen(buff);
   if (bf) while (cc && buff[cc-1] > 0 && buff[cc-1] <= ' ') --cc;
   else    while (cc && buff[cc-1] > 0 && buff[cc-1] < ' ') --cc;
   buff[cc] = 0;
   return pp;
}


/********************************************************************************/

//  Return 1 if both filespecs have the same folder, else return 0.
//  Both folders must be specified, at least one with ending '/'
//  (true if a file name is present)

int samefolder(cchar *file1, cchar *file2) 
{
   cchar    *p1, *p2;
   int      cc1, cc2, cc;

   p1 = strrchr(file1,'/');                                                      //  /dir1/dir2
   p2 = strrchr(file2,'/');                                                      //  /dir1/dir2/file
   cc1 = cc2 = 0;
   if (p1) cc1 = p1 - file1;                                                     //  /dir1/dir2/file
   if (p2) cc2 = p2 - file2;                                                     //  |         |
   if (cc2 > cc1) cc = cc2;                                                      //  0         cc
   else cc = cc1;
   if (cc == 0) return 0;
   if (strmatchN(file1,file2,cc)) return 1;
   return 0;
}


/********************************************************************************

   Parse a pathname (filespec) and return its components.
   Returned strings are allocated in static memory (no zfree needed).
   Next call replaces the data in the static strings.
   Limits: folder: 1000  file: 200  ext: 8
   Missing components are returned as null pointers.

   input ppath         outputs

   /name1/name2/       folder /name1/name2/ with no file
   /name1/name2        folder /name1/name2/ if name2 a folder,
                       otherwise folder /name1/ and file name2
   /name1/name2.xxx    if .xxx < 8 chars, returns file name2 and ext .xxx,
                       otherwise returns file name2.xxx and no ext

   returns 0 if no error, else 1

*********************************************************************************/

int parsefile(cchar *ppath, char **pfolder, char **pfile, char **pext)
{
   STATB          statb;
   static char    folder[1000], file[200], ext[8];
   char           *pp;
   int            err, cc1, cc2;

   *pfolder = *pfile = *pext = null;

   cc1 = strlen(ppath);
   if (cc1 > 999) return 1;                                                      //  ppath too long

   strcpy(folder,ppath);
   *pfolder = folder;

   err = stat(folder,&statb);                                                    //  have folder only
   if (! err && S_ISDIR(statb.st_mode)) return 0;

   pp = (char *) strrchr(folder,'/');
   if (! pp) return 1;                                                           //  illegal

   pp++;
   cc2 = pp - folder;
   if (cc2 < 2 || cc2 == cc1) return 0;                                          //  have /xxxx  or  /xxxx/

   if (strlen(pp) > 199) return 1;                                               //  filename too long

   strcpy(file,pp);                                                              //  file part
   *pfile = file;
   *pp = 0;                                                                      //  remove from folder part

   pp = (char *) strrchr(file,'.');
   if (! pp || strlen(pp) > 7) return 0;                                         //  file part, no .ext

   strcpy(ext,pp);                                                               //  .ext part
   *pext = ext;
   *pp = 0;                                                                      //  remove from file part
   return 0;
}


/********************************************************************************/

//  Move a source file to a destination file and delete the source file.
//  Equivalent to rename(), but the two files MAY be on different file systems.
//  Pathnames must be absolute (start with '/').
//  Returns 0 if OK, +N if not.
//  file names with embedded quote (") will fail

int renamez(cchar *file1, cchar *file2)
{
   char     *pp1, *pp2;
   int      err, Frename = 0;

   if (*file1 != '/' || *file2 != '/') return 1;                                 //  not absolute pathnames

   pp1 = strchr((char *) file1+1,'/');
   pp2 = strchr((char *) file2+1,'/');
   if (! pp1 || ! pp2) return 2;

   *pp1 = *pp2 = 0;
   if (strmatch(file1,file2)) Frename = 1;
   *pp1 = *pp2 = '/';

   if (Frename) {                                                                //  same top folder
      err = rename(file1,file2);
      if (err) return errno;
      else return 0;
   }

   err = Qshell(0,0,"mv -f \"%s\" \"%s\" ",file1,file2);                         //  not
   return err;
}


/********************************************************************************/

//  Check if a folder exists. If not, ask user if it should be created.
//  Returns 0 if OK or +N if error or user refused to create.
//  The user is notified of failure, no other message needed.

int  check_create_dir(char *path)
{
   int      err, yn;
   STATB    statbuf;

   err = stat(path,&statbuf);                                                    //  check status
   if (! err) {
      if (S_ISDIR(statbuf.st_mode)) return 0;                                    //  exists, folder, OK
      else {
         zmessageACK(0,"%s \n %s",path,strerror(ENOTDIR));                       //  exists, not a folder
         return ENOTDIR;
      }
   }

   if (errno != ENOENT) {
      zmessageACK(0,"%s \n %s",path,strerror(errno));                            //  error other than missing
      return errno;
   }

   yn = zmessageYN(0,E2X("create folder? \n %s"),path);                          //  ask to create
   if (! yn) return ENOENT;

   err = Qshell(1,1,"mkdir -p -m 0750 \"%s\" ",path);                            //  create. rwx, rx, no access
   if (! err) return 0;

   zmessageACK(0,"%s \n %s",path,strerror(errno));                               //  failed to create
   return errno;
}


/********************************************************************************/

//  Copy file to file or file to an existing folder.
//  Missing output folders will be created.
//  If inut file is a symlink, copy the symlink, not the file.

int copyFile(cchar *sfile, cchar *dfile)
{
   #define BIOCC (1024*1024)                                                     //  read/write block size
   int         fid1, fid2, err, cc, dlevs;
   char        *pp1, *pp2, buff[BIOCC];
   STATB       statB;
   static char *dfile2 = 0;

   if (dfile2) zfree(dfile2);                                                    //  stop memory leak
   dfile2 = 0;

   err = stat(dfile,&statB);
   if (! err && S_ISDIR(statB.st_mode)) {                                        //  output is an existing folder
      pp1 = (char *) strrchr(sfile,'/');                                         //  get source file base name
      if (pp1) pp1++;
      else pp1 = (char *) sfile;
      cc = strlen(pp1);                                                          //  construct output file path:
      dfile2 = zstrdup(dfile,cc+4);                                              //    output folder + base name
      pp2 = dfile2 + strlen(dfile2);
      if (pp2[-1] != '/') *pp2++ = '/';                                          //  insure '/' after folder
      strcpy(pp2,pp1);
      dfile = dfile2;                                                            //  output file full path
   }
   
   else {                                                                        //  output is a file path
      pp2 = (char *) dfile;
      dlevs = 0;
      while (true) {
         pp2 = strchr(pp2+1,'/');                                                //  create missing folder levels
         if (! pp2) break;                                                       //  (check and create from top down)
         *pp2 = 0;
         err = stat(dfile,&statB);
         if (err) {
            err = mkdir(dfile,0731);
            if (err) {
               printz("%s \n %s \n",strerror(errno),dfile);
               return errno;
            }
            dlevs++;
         }
         *pp2 = '/';
      }
   }

   err = lstat(sfile,&statB);                                                    //  get input file attributes
   if (err) {
      printz("%s \n %s \n",strerror(errno),sfile);
      return errno;
   }

   if (S_ISLNK(statB.st_mode)) {                                                 //  input file is symlink
      cc = readlink(sfile,buff,XFCC);
      if (cc < 0 || cc > XFCC-2) return errno;
      buff[cc] = 0;
      err = symlink(buff,dfile);                                                 //  create output symlink
      if (err) {
         printz("%s \n %s \n %s \n",strerror(errno),buff,dfile);
         return errno;
      }
      return 0;
   }
   
   if (strmatch(sfile,dfile)) return 0;                                          //  source and dest files are same

   fid1 = open(sfile,O_RDONLY);                                                  //  open input file
   if (fid1 == -1) return errno;

   fid2 = creat(dfile,0700);                                                     //  open output file
   if (fid2 == -1) {
      err = errno;
      close(fid1);
      if (err) printz("%s \n %s \n",strerror(err),dfile);
      return err;
   }

   while (true)
   {
      cc = read(fid1,buff,BIOCC);                                                //  read huge blocks
      if (cc == 0) break;
      if (cc == -1) {
         err = errno;
         close(fid1);
         close(fid2);
         if (err) printz("%s \n %s \n",strerror(err),sfile);
         return err;
      }

      cc = write(fid2,buff,cc);                                                  //  write blocks
      if (cc == -1) {
         err = errno;
         close(fid1);
         close(fid2);
         if (err) printz("%s \n %s \n",strerror(err),dfile);
         return err;
      }
   }

   close(fid1);                                                                  //  close input file
   err = close(fid2);                                                            //  close output file
   if (err) {
      printz("%s \n %s \n",strerror(errno),dfile);
      return errno;
   }
   return 0;
}


/********************************************************************************/

//  Return all the file names in a folder, sorted in alphabetic order.
//  Subfolders are not included.
//  The 'files' argument is allocated and filled with pointers to file names.
//  (the names in the folder, not the full path names)
//  The number of files found is returned.
//  -1 is returned if the folder is invalid or other error.
//  If 'files' is returned non-zero, it is subject to zfree()

int zreaddir(cchar *folder, char **&files)
{
   struct dirent  *dirent1;

   int      Nfiles = 0, maxfiles = 100;
   DIR      *direc;
   char     **ufiles, **ufiles2;
   
   files = 0;                                                                    //  nothing returned yet
   
   ufiles = (char **) zmalloc(maxfiles * sizeof(char *));                        //  starting space

   direc = opendir(folder);                                                      //  open caller's folder
   if (! direc) return -1;

   while (true)
   {
      if (Nfiles == maxfiles)                                                    //  out of space
      {
         ufiles2 = (char **) zmalloc(2 * maxfiles * sizeof(char *));             //  allocate new space = 2x old space
         memcpy(ufiles2,ufiles, maxfiles * sizeof(char *));                      //  copy data to new space
         zfree(ufiles);                                                          //  free old space
         ufiles = ufiles2;                                                       //  set new space
         maxfiles *= 2;                                                          //  new capacity
      }

      dirent1 = readdir(direc);                                                  //  get next file in folder
      if (! dirent1) break;
      if (dirent1->d_type != DT_REG) continue;                                   //  skip subfolders
      ufiles[Nfiles] = zstrdup(dirent1->d_name);                                 //  add to file list
      Nfiles++;
      continue;
   }
   
   closedir(direc);
   
   if (Nfiles > 1) HeapSort(ufiles,Nfiles);                                      //  sort file list
   
   files = ufiles;                                                               //  return allocated file list
   return Nfiles;                                                                //  return file count
}


/********************************************************************************/

//  appimage problem: 
//    command line args are separated by blanks even for strings enclosed in 
//    quotes: "aaaa bbbb" becomes two argv[] elements, "aaaa" and "bbbb"
//    this makes it impossible to get file path args with embedded spaces
//
//  char * catenate_argvs(int argc, char *argv[], Nth)
//    combine argv[ii] elements from Nth to last
//    a single space is inserted between each argv[ii] element
//    command ... aaaaa bbbbb ccccc  produces "aaaaa bbbbb ccccc"

char * combine_argvs(int argc, char *argv[], int Nth)
{
   int            ii, ccv, outcc = 0;
   static char    output[XFCC];

   for (ii = Nth; ii < argc; ii++)
   {
      ccv = strlen(argv[ii]);
      if (outcc + ccv > XFCC - 2) return 0;
      strcpy(output+outcc,argv[ii]);
      outcc += ccv;
      output[outcc] = ' ';
      outcc++;
   }
   
   outcc--;
   output[outcc] = 0;
   return output;
}


/********************************************************************************/

//  escape quote marks (") in a file name for use in shell commands
//  returned file is subject for zfree()

char * zescape_quotes(cchar *file1)
{
   char     *file2 = 0;

   if (strchr(file1,'"') == 0) {
      file2 = zstrdup(file1);
      return file2;
   }

   file2 = zstrdup(file1,20);   
   repl_1str(file1,file2,"\"","\\\"");
   return file2;
}


/********************************************************************************

   utility to measure CPU time spent in various functions or code blocks

   cpu_profile_init()            initialize at start of test
   cpu_profile_enter(fnum)       at entry to a function
   cpu_profile_exit(fnum)        at exit from a function
   cpu_profile_report()          report CPU time per function

   Methodology: cpu_profile_init() starts a thread that suspends and runs every
   1 millisecond and updates a timer.
   cpu_profile_enter() and cpu_profile_exit() accumulate the time difference between
   entry and exit of code being measured. This may be zero because of the long interval
   between timer updates. Accuracy comes from statistical sampling over many seconds,
   so that if the time spent in a monitored function is significant, it will be accounted
   for. The accuracy is better than 1% as long as the measured function or code block
   consumes a second or more of CPU time during the measurement period.
   The "fnum" argument (1-99) designates the function or code block being measured.
   cpu_profile_report() stops the timer thread and reports time consumed per function,
   using the "fnum" tags in the report.
   The functions cpu_profile_enter() and cpu_profile_exit() subtract the timer
   difference and add to a counter per fnum, so the added overhead is insignificant.
   They are inline functions defined as follows:
   enter:  cpu_profile_timer = cpu_profile_elapsed;
   exit:   cpu_profile_table[fnum] += cpu_profile_elapsed - cpu_profile_timer;

*********************************************************************************/

VOL double   cpu_profile_table[100];
VOL double   cpu_profile_timer;
VOL double   cpu_profile_elapsed;
VOL int      cpu_profile_kill = 0;

void cpu_profile_init()
{
   void *   cpu_profile_timekeeper(void *);

   for (int ii = 0; ii < 99; ii++)
      cpu_profile_table[ii] = 0;
   cpu_profile_elapsed = 0;
   start_detached_thread(cpu_profile_timekeeper,null);
}

void cpu_profile_report()
{
   cpu_profile_kill++;

   printz("elapsed: %.2f \n",cpu_profile_elapsed);

   for (int ii = 0; ii < 100; ii++)
   {
      double dtime = cpu_profile_table[ii];
      if (dtime) printz("cpu profile func: %d  time: %.2f \n",ii,dtime);
   }
}

void * cpu_profile_timekeeper(void *)
{
   timeval  time0, time1;

   gettimeofday(&time0,0);

   while (true)
   {
      gettimeofday(&time1,0);
      cpu_profile_elapsed = time1.tv_sec - time0.tv_sec
              + 0.000001 * (time1.tv_usec - time0.tv_usec);
      zsleep(0.001);
      if (cpu_profile_kill) break;
   }

   cpu_profile_kill = 0;
   pthread_exit(0);
}


/********************************************************************************/

//  Returns hard page fault rate in faults/second.
//  First call starts a thread that runs every 2 seconds and keeps a
//  weighted average of hard fault rate for the last few intervals.

namespace pagefaultrate_names {
   int      ftf = 1;
   int      samples = 0;
   int      faultrate = 0;
   double   time1, time2;
   void * threadfunc(void *);
}

int pagefaultrate()
{
   using namespace pagefaultrate_names;

   if (ftf) {
      ftf = 0;
      start_detached_thread(threadfunc,0);
      time1 = get_seconds();
   }

   return faultrate;
}

void * pagefaultrate_names::threadfunc(void *)
{
   using namespace pagefaultrate_names;

   FILE        *fid;
   char        *pp, buff[200];
   double      pfs1, pfs2, fps, elaps;

   while (true)
   {
      sleep(2);

      time2 = get_seconds();
      elaps = time2 - time1;
      time1 = time2;

      fid = fopen("/proc/self/stat","r");
      if (! fid) break;
      pp = fgets(buff,200,fid);
      fclose(fid);
      if (! pp) break;
      pp = strchr(pp,')');                                                       //  closing ')' after (short) filename
      if (pp) parseprocrec(pp+1,10,&pfs1,11,&pfs2,null);
      fps = (pfs1 + pfs2) / elaps;
      faultrate = 0.7 * faultrate + 0.3 * fps;
   }

   printz("pagefaultrate() failure \n");
   pthread_exit(0);
}


/********************************************************************************

    strField()

    cchar * strField(cchar *string, cchar *delim, int Nth)

    Get the Nth field in input string, which contains at least N fields
    delimited by the character(s) in delim (e.g. blank, comma).

    Returns a pointer to the found field (actually a pointer to a
    copy of the found field, with a null terminator appended).

    If a delimiter is immediately followed by another delimiter, it is
    considered a field with zero length, and the string "" is returned.

    Leading blanks in a field are omitted from the returned field.
    A field with only blanks is returned as "".

    The last field may be terminated by null or a delimiter.

    Characters within quotes (") are treated as data within a field,
    i.e. blanks and delimiters are not processed as such.
    The quotes are removed from the returned field.

    If there are less than Nth fields, a null pointer is returned.

    The last 100 fields are saved and recycled in a circular manner.
    The caller does not have to free memory. If more memory depth is
    needed, caller must copy the returned data elsewhere.

    The output string may be modified if the length is not increased.
    Fields within the input string must not exceed 2000 characters.

    Example: input string: ,a,bb,  cc,   ,dd"ee,ff"ggg,
             (first and last characters are comma)
             delimiter: comma
             Nth   returned string
              1:   (null string)
              2:   a
              3:   bb
              4:   cc
              5:   (one blank)
              6:   ddee,ffggg
              7:   (null pointer >> no more fields)

*********************************************************************************/

cchar * strField(cchar *string, cchar *delim, int Nth)
{
   static int     ftf = 1, nret = 0;
   static char    *retf[100];                                                    //  100 substring limit
   char           *pf1, pf2[2000];                                               //  2000 char. limit
   cchar          quote = '"';
   int            ii, nf, fcc = 0;

   if (! string || ! *string) return 0;                                          //  bad call
   if (Nth < 1) return 0;

   if (ftf)                                                                      //  overall first call
   {
      ftf = 0;
      for (ii = 0; ii < 100; ii++) retf[ii] = 0;
   }
   
   pf1 = (char *) string - 1;                                                    //  start parse
   nf = 0;

   while (nf < Nth)
   {
      pf1++;                                                                     //  start field
      nf++;
      fcc = 0;

      while (*pf1 == ' ') pf1++;                                                 //  skip leading blanks

      while (true)
      {
         if (*pf1 == quote) {                                                    //  pass chars between single quotes
            pf1++;                                                               //  (but without the quotes)
            while (*pf1 && *pf1 != quote) pf2[fcc++] = *pf1++;
            if (*pf1 == quote) pf1++;
         }

         else if (strchr(delim,*pf1) || *pf1 == 0) break;                        //  found delimiter or null

         else pf2[fcc++] = *pf1++;                                               //  pass normal character

         if (fcc > 1999) zappcrash("strField() too long");
      }

      if (*pf1 == 0) break;                                                      //  end of input string
   }

   if (nf < Nth) return 0;                                                       //  no Nth field

   if (fcc == 0) {                                                               //  empty field
      if (*pf1 == 0) return 0;                                                   //  no field
      return "";                                                                 //  return null string
   }

   if (++nret == 100) nret = 0;                                                  //  use next return slot
   if (retf[nret]) zfree(retf[nret]);
   retf[nret] = (char *) zmalloc(fcc+2);
   strncpy0(retf[nret],pf2,fcc+1);
   return retf[nret];
}

cchar * strField(cchar *string, cchar delim, int Nth)                            //  alternative with one delimiter
{
   char     delims[2] = "x";
   *delims = delim;
   return strField(string,delims,Nth);
}


/********************************************************************************

   stat = strParms(begin, input, pname, maxcc, pval)

   Parse an input string with parameter names and values:
     "pname1=pval1 | pname2 | pname3=pval3 | pname4 ..."

   begin    int &          must be 1 to start new string, is modified
   input    cchar *   input string
   pname    char *         output parameter name
   maxcc    int            max. length for pname, including null
   pval     double &       output parameter value
   stat     int            status: 0=OK, -1=EOL, 1=parse error

   Each call returns the next pname and pval.
   A pname with no pval is assigned a value of 1 (present).
   Input format:  pname1 | pname2=pval2 | pname3 ... null
   Leading blanks are ignored, and pnames may have embedded blanks.
   pvals must convert to double using convSD (accepts decimal point or comma)

***/

int strParms(int &begin, cchar *input, char *pname, int maxcc, double &pval)
{
   static int     ii, beginx = 3579246;
   cchar          *pnamex, *delim;
   int            cc, err;

   if (begin == 1) {                                                             //  start new string
      begin = ++beginx;
      ii = 0;
   }

   if (begin != beginx) zappcrash("strParms call error");                        //  thread safe, not reentrant

   *pname = 0;                                                                   //  initz. outputs to nothing
   pval = 0;

   while (input[ii] == ' ') ii++;                                                //  skip leading blanks
   if (input[ii] == 0) return -1;                                                //  no more data

   pnamex = input + ii;                                                          //  next pname

   for (cc = 0; ; cc++)
   {                                                                             //  look for delimiter
      if (pnamex[cc] == '=') break;
      if (pnamex[cc] == '|') break;
      if (pnamex[cc] == 0) break;
   }

   if (cc == 0) return 1;                                                        //  err: 2 delimiters
   if (cc >= maxcc) return 1;                                                    //  err: pname too big

   strncpy0(pname,pnamex,cc+1);                                                  //  pname >> caller
   strTrim(pname);                                                               //  remove trailing blanks

   if (pnamex[cc] == 0) {                                                        //  pname + null
      ii += cc;                                                                  //  position for next call
      pval = 1.0;                                                                //  pval = 1 >> caller
      return 0;
   }

   if (pnamex[cc] == '|') {                                                      //  pname + |
      ii += cc + 1;                                                              //  position for next call
      pval = 1.0;                                                                //  pval = 1 >> caller
      return 0;
   }

   ii += cc + 1;                                                                 //  pname = pval
   err = convSD(input + ii, pval, &delim);                                       //  parse pval   (was strtod()
   if (err > 1) return 1;
   while (*delim == ' ') delim++;                                                //  skip poss. trailing blanks
   if (*delim && *delim != '|') return 1;                                        //  err: delimiter not | or null
   ii = delim - input;
   if (*delim) ii++;                                                             //  position for next call
   return 0;
}


/********************************************************************************/

//  Produce random value from hashed input string.
//  Output range is 0 to max-1.
//  Benchmark: 0.8 usec for 99 char. string  3 GHz Core i5

int strHash(cchar *string, int max)
{
   uint     hash = 1;
   uchar    byte;

   while ((byte = *string++))
   {
      hash *= byte;
      hash = hash ^ (hash >> 7);
      hash = hash * 7;
      hash = hash & 0x00FFFFFF;
   }

   hash = hash % max;
   return hash;
}

int64 strHash64(cchar *string, int64 max)
{
   uint64   hash = 1;
   uchar    byte;

   while ((byte = *string++))
   {
      hash *= byte;
      hash = hash ^ (hash >> 7);
      hash = hash * 7;
      hash = hash & 0x00FFFFFFFFFFFFFF;
   }

   hash = hash % max;
   return hash;
}


/********************************************************************************/

//  Copy string with specified max. length (including null terminator).
//  truncate if needed. null terminator is always supplied.
//  Returns 0 if no truncation, 1 if input string was truncated to fit.

int strncpy0(char *dest, cchar *source, uint cc)
{
   strncpy(dest,source,cc);
   dest[cc-1] = 0;
   if (strlen(source) >= cc) return 1;                                           //  truncated
   else return 0;
}


/********************************************************************************/

//  Copy string with blank pad to specified length.  No null is added.

void strnPad(char *dest, cchar *source, int cc)
{
   strncpy(dest,source,cc);
   int ii = strlen(source);
   for (int jj = ii; jj < cc; jj++) dest[jj] = ' ';
}


/********************************************************************************/

//  Remove trailing blanks from a string. Returns remaining length.

int strTrim(char *dest, cchar *source)
{
   if (dest != source) strcpy(dest,source);
   return strTrim(dest);
}

int strTrim(char *dest)
{
   int  ii = strlen(dest);
   while (ii && (dest[ii-1] == ' ')) dest[--ii] = 0;
   return ii;
}


/********************************************************************************/

//  Remove leading and trailing blanks from a string.
//  Returns remaining length, possibly zero.

int strTrim2(char *dest, cchar *source)
{
   cchar       *pp1, *pp2;
   int         cc;

   pp1 = source;
   pp2 = source + strlen(source) - 1;
   while (*pp1 == ' ') pp1++;
   if (*pp1 == 0) {
      strcpy(dest,"");                                                           //  null or blank input
      return 0;
   }
   while (*pp2 == ' ' && pp2 > pp1) pp2--;
   cc = pp2 - pp1 + 1;
   memmove(dest,pp1,cc);
   dest[cc] = 0;
   return cc;
}

int strTrim2(char *string)
{
   return strTrim2(string,(cchar *) string);
}


/********************************************************************************/

//  Remove all blanks from a string. Returns remaining length.

int strCompress(char *dest, cchar *source)
{
   if (dest != source) strcpy(dest,source);
   return strCompress(dest);
}

int strCompress(char *string)
{
   int   ii, jj;

   for (ii = jj = 0; string[ii]; ii++)
   {
      if (string[ii] != ' ')
      {
         string[jj] = string[ii];
         jj++;
      }
   }
   string[jj] = 0;
   return jj;
}


/********************************************************************************/

//  Concatenate multiple strings, staying within a specified overall length.
//  The destination string is also the first source string.
//  Null marks the end of the source strings (omission --> crash).
//  Output is truncated to fit within the specified length.
//  A final null is assured and is included in the length.
//  Returns 0 if OK, 1 if truncation was needed.

int strncatv(char *dest, int maxcc, cchar *source, ...)
{
   cchar       *ps;
   va_list     arglist;

   maxcc = maxcc - strlen(dest) - 1;
   if (maxcc < 0) return 1;
   va_start(arglist,source);
   ps = source;

   while (ps)
   {
      strncat(dest,ps,maxcc);
      maxcc = maxcc - strlen(ps);
      if (maxcc < 0) break;
      ps = va_arg(arglist,cchar *);
   }

   va_end(arglist);
   if (maxcc < 0) return 1;
   return 0;
}


/********************************************************************************/

//  Match 1st string to N additional strings.
//  Return matching string number 1 to N or 0 if no match.
//  Supply a null argument for end of list.

int strmatchV(cchar *string, ...)
{
   int         match = 0;
   char        *stringN;
   va_list     arglist;

   va_start(arglist,string);

   while (1)
   {
      stringN = va_arg(arglist, char *);
      if (stringN == null)
      {
         va_end(arglist);
         return 0;
      }

      match++;
      if (strmatch(string,stringN))
      {
         va_end(arglist);
         return match;
      }
   }
}


/********************************************************************************/

//  convert string to upper case

void strToUpper(char *string)
{
   int         ii;
   char        jj;
   const int   delta = 'A' - 'a';

   for (ii = 0; (jj = string[ii]); ii++)
        if ((jj >= 'a') && (jj <= 'z')) string[ii] += delta;
}

void strToUpper(char *dest, cchar *source)
{
   strcpy(dest,source);
   strToUpper(dest);
}


/********************************************************************************/

//  convert string to lower case

void strToLower(char *string)
{
   int         ii;
   char        jj;
   const int   delta = 'a' - 'A';

   for (ii = 0; (jj = string[ii]); ii++)
        if ((jj >= 'A') && (jj <= 'Z')) string[ii] += delta;
}

void strToLower(char *dest, cchar *source)
{
   strcpy(dest,source);
   strToLower(dest);
}


/********************************************************************************/

//  Copy string strin to strout, replacing every occurrence
//  of the substring ssin with the substring ssout.
//  Returns the count of replacements, if any.
//  Replacement strings may be longer or shorter or have zero length.

int repl_1str(cchar *strin, char *strout, cchar *ssin, cchar *ssout)
{
   int         ccc, cc1, cc2, nfound;
   cchar       *ppp;

   cc1 = strlen(ssin);
   cc2 = strlen(ssout);
   nfound = 0;

   while ((ppp = strstr(strin,ssin)))
   {
      nfound++;
      ccc = ppp - strin;
      memcpy(strout,strin,ccc);                                                  //  memcpy instead of strncpy
      strout += ccc;
      strin += ccc;
      memcpy(strout,ssout,cc2);
      strin += cc1;
      strout += cc2;
   }

   strcpy(strout,strin);
   return nfound;
}


/********************************************************************************/

//  Copy string strin to strout, replacing multiple substrings with replacement strings.
//  Multiple pairs of string arguments follow strout, a substring and a replacement string.
//  Last pair of string arguments must be followed by a null argument.
//  Returns the count of replacements, if any.
//  Replacement strings may be longer or shorter or have zero length.

int repl_Nstrs(cchar *strin, char *strout, ...)
{
   va_list     arglist;
   cchar       *ssin, *ssout;
   char        ftemp[XFCC];
   int         ftf, nfound;

   ftf = 1;
   nfound = 0;
   va_start(arglist,strout);

   while (true)
   {
      ssin = va_arg(arglist, char *);
      if (! ssin) break;
      ssout = va_arg(arglist, char *);

      if (ftf) {
         ftf = 0;
         nfound += repl_1str(strin,strout,ssin,ssout);
      }

      else {
         strcpy(ftemp,strout);
         nfound += repl_1str(ftemp,strout,ssin,ssout);
      }
   }

   va_end(arglist);
   return nfound;
}


/********************************************************************************/

//  Break up a long text string into lines no longer than cc2 chars.
//  If fake newlines ("\n") are found, replace them with real newlines.
//  Break unconditionally where newlines are found and remove them.
//  Break at last blank char between cc1 and cc2 if present.
//  Break at last delimiter char between cc1 and cc2 if present.
//  Break unconditionally at cc2 if none of the above.
//  Returns text lines in txout[*] with count as returned function value.
//  txout and txout[*] are subjects for zfree().

int breakup_text(cchar *txin0, char **&txout, cchar *delims, int cc1, int cc2)
{
   char     *txin;
   uchar    ch;
   int      p1, p2, cc3, Nout;
   int      Np, Bp, Sp;

   txin = zstrdup(txin0);
   txout = (char **) zmalloc(100 * sizeof(char *));

   if (strstr(txin0,"\\n"))                                                      //  replace "\n" with real newline chars
      repl_1str(txin0,txin,"\\n","\n");

   Nout = p1 = 0;

   while (true)
   {
      p2 = p1;                                                                   //  input line position
      cc3 = 0;                                                                   //  output line cc

      Np = Bp = Sp = 0;

      while (txin[p2]) {                                                         //  scan further up to cc2 chars
         ch = txin[p2];
         if (ch == '\n') { Np = p2; break; }                                     //  break out if newline found
         if (cc3 >= cc1) {
            if (ch == ' ') Bp = p2;                                              //  remember last ' ' found after cc1 chars
            if (strchr(delims,ch))  Sp = p2;                                     //  remember last delimiter found after cc1
         }
         if (ch < 0)
            while ((ch = txin[p2+1]) < 0xC0) p2++;
         p2++;
         cc3++;
         if (cc3 == cc2) break;
      }

      if (! cc3 && ! Np) break;
      if (Np) cc3 = Np - p1;
      else if (Bp) cc3 = Bp - p1 + 1;
      else if (Sp) cc3 = Sp - p1 + 1;
      else cc3 = p2 - p1;
      txout[Nout] = (char *) zmalloc(cc3+1);
      strncpy0(txout[Nout],txin+p1,cc3+1);
      Nout++;
      p2 = p1 + cc3;
      if (Np) p2++;
      p1 = p2;
      if (Nout == 100) break;
   }

   zfree(txin);
   return Nout;
}


/********************************************************************************/

//  Copy and convert string to hex string.
//  Each input character 'A' >> 3 output characters "41 "

void strncpyx(char *out, cchar *in, int ccin)
{
   int      ii, jj, c1, c2;
   char     cx[] = "0123456789ABCDEF";

   if (! ccin) ccin = strlen(in);

   for (ii = 0, jj = 0; ii < ccin; ii++, jj += 3)
   {
      c1 = (uchar) in[ii] >> 4;
      c2 = in[ii] & 15;
      out[jj] = cx[c1];
      out[jj+1] = cx[c2];
      out[jj+2] = ' ';
   }
   out[jj] = 0;
   return;
}


/********************************************************************************/

//  Strip trailing zeros from ascii floating numbers
//    (e.g. 1.230000e+02  -->  1.23e+02)

void StripZeros(char *pNum)
{
   int     ii, cc;
   int     pp, k1, k2;
   char    work[20];

   cc = strlen(pNum);
   if (cc >= 20) return;

   for (ii = 0; ii < cc; ii++)
   {
      if (pNum[ii] == '.')
      {
         pp = ii;
         k1 = k2 = 0;
         for (++ii; ii < cc; ii++)
         {
            if (pNum[ii] == '0')
            {
               if (! k1) k1 = k2 = ii;
               else k2 = ii;
               continue;
            }

            if ((pNum[ii] >= '1') && (pNum[ii] <= '9'))
            {
               k1 = 0;
               continue;
            }

            break;
         }

         if (! k1) return;

         if (k1 == pp + 1) k1++;
         if (k2 < k1) return;
         strcpy(work,pNum);
         strcpy(work+k1,pNum+k2+1);
         strcpy(pNum,work);
         return;
      }
   }
}


/********************************************************************************/

//  test for blank/null string
//  Returns status depending on input string:
//    0 not a blank or null string
//    1 argument string is NULL
//    2 string has zero length (*string == 0)
//    3 string is all blanks

int blank_null(cchar *string)
{
   if (! string) return 1;                                                       //  null string
   if (! *string) return 2;                                                      //  zero length string
   int cc = strlen(string);
   for (int ii = 0; ii < cc; ii++)
      if (string[ii] != ' ') return 0;                                           //  non-blank string
   return 3;                                                                     //  blank string
}


/********************************************************************************/

//  clean \x escape sequences and replace them with the escaped character
//    \n >> newline  \" >> doublequote  \\ >> backslash   etc.
//  see  $ man ascii  for the complete list

int clean_escapes(char *string)
{
   char     *pp1 = string, *pp2 = string, *pp;
   char     char1;
   char     escapes[] = "abtnvfr";
   int      count = 0;

   while (true)
   {
      char1 = *pp1++;

      if (char1 == 0) {
         *pp2 = 0;
         return count;
      }

      else if (char1 == '\\')  {
         char1 = *pp1++;
         pp = strchr(escapes,char1);
         if (pp) char1 = pp - escapes + 7;
         count++;
      }

      *pp2++ = char1;
   }
}


/********************************************************************************/

//  Compute the graphic character count for a UTF8 character string.
//  Depends on UTF8 rules:
//    - ascii characters are positive (0x00 to 0x7F)
//    - 1st char of multichar sequence is negative (0xC0 to 0xFD)
//    - subsequent multichars are in the range 0x80 to 0xBF

int utf8len(cchar *utf8string)
{
   int      ii, cc;
   char     xlimit = 0xC0;

   for (ii = cc = 0; utf8string[ii]; ii++)
   {
      if (utf8string[ii] < 0)                                                    //  multibyte character
         while (utf8string[ii+1] < xlimit) ii++;                                 //  skip extra bytes
      cc++;
   }

   return cc;
}


/********************************************************************************/

//  Extract a UTF8 substring with a specified count of graphic characters.
//    utf8in     input UTF8 string
//    utf8out    output UTF8 string, which must be long enough
//    pos        initial graphic character position to get (0 = first)
//    cc         max. count of graphic characters to get
//    returns    number of graphic characters extracted, <= cc
//  Output string is null terminated after last extracted character.

int utf8substring(char *utf8out, cchar *utf8in, int pos, int cc)
{
   int      ii, jj, kk, posx, ccx;
   char     xlimit = 0xC0;

   for (ii = posx = 0; posx < pos && utf8in[ii]; ii++)
   {
      if (utf8in[ii] < 0)
         while (utf8in[ii+1] < xlimit) ii++;
      posx++;
   }

   jj = ii;

   for (ccx = 0; ccx < cc && utf8in[jj]; jj++)
   {
      if (utf8in[jj] < 0)
         while (utf8in[jj+1] < xlimit) jj++;
      ccx++;
   }

   kk = jj - ii;

   strncpy(utf8out,utf8in+ii,kk);
   utf8out[kk] = 0;

   return   ccx;
}


/********************************************************************************/

//  check a string for valid utf8 encoding
//  returns:  0 = OK,  1 = bad string

int utf8_check(cchar *string)
{
   cchar             *pp;
   unsigned char     ch1, ch2, nch;

   for (pp = string; *pp; pp++)
   {
      ch1 = *pp;
      if (ch1 < 0x7F) continue;
      if (ch1 > 0xBF && ch1 < 0xE0) nch = 1;
      else if (ch1 < 0xF0) nch = 2;
      else if (ch1 < 0xF8) nch = 3;
      else if (ch1 < 0xFC) nch = 4;
      else if (ch1 < 0xFE) nch = 5;
      else return 1;
      while (nch) {
         pp++;
         ch2 = *pp;
         if (ch2 < 0x80 || ch2 > 0xBF) return 1;
         nch--;
      }
   }

   return 0;
}


/********************************************************************************/

//  Find the Nth graphic character position within a UTF8 string
//    utf8in      input UTF8 string
//    Nth         graphic character position, zero based
//  returns starting character (byte) position of Nth graphic character
//  returns -1 if Nth is beyond the string length

int utf8_position(cchar *utf8in, int Nth)
{
   int      ii, posx;
   char     xlimit = 0xC0;

   for (ii = posx = 0; posx < Nth && utf8in[ii]; ii++)
   {
      if (utf8in[ii] < 0)                                                        //  multi-byte character
         while (utf8in[ii+1] && utf8in[ii+1] < xlimit) ii++;                     //  traverse member bytes
      posx++;
   }

   if (utf8in[ii]) return ii;
   return -1;
}


/********************************************************************************/

//  err = zsed(file, string1, string2 ... null)
//
//  replace string1/3/5... with string2/4/6... in designated file 
//  returns    N  lines changed
//            -1  file not found
//            -2  other error (with message)

int zsed(cchar *infile ...)
{
   int         err, ftf, nn;
   FILE        *fid1, *fid2;
   char        *outfile, *pp;
   char        buffin[1000], buffout[1000], buffxx[1000];
   cchar       *stringin, *stringout;
   va_list     arglist;

   fid1 = fopen(infile,"r");
   if (! fid1) return -1;
   
   outfile = zstrdup(infile,8);
   strcat(outfile,"-temp");
   fid2 = fopen(outfile,"w");
   if (! fid2) {
      printz("%d \n",strerror(errno));
      zfree(outfile);
      return -2;
   }
   
   nn = 0;

   while (true)
   {
      pp = fgets(buffin,500,fid1);
      if (! pp) break;

      va_start(arglist,infile);

      ftf = 1;

      while (true)
      {
         stringin = va_arg(arglist, char *);
         if (! stringin) break;
         stringout = va_arg(arglist, char *);
         if (! stringout) break;

         if (ftf) {
            ftf = 0;
            nn += repl_1str(buffin,buffout,stringin,stringout);
         }
         else {
            strcpy(buffxx,buffout);
            nn += repl_1str(buffxx,buffout,stringin,stringout);
         }
      }

      va_end(arglist);
      
      fputs(buffout,fid2);
   }
   
   fclose(fid1);
   err = fclose(fid2);
   if (err) {
      printz("%s \n",strerror(errno));
      zfree(outfile);
      return -2;
   }
   
   rename(outfile,infile);
   
   zfree(outfile);
   return nn;
}


/********************************************************************************/

//  zstrstr() and zstrcasestr() work like strstr() and strcasestr()
//  but the needle string "" does NOT match any haystack string.

const char * zstrstr(const char *haystack, const char *needle)
{
   if (! needle || ! *needle) return 0;
   return strstr(haystack,needle);   
}

const char * zstrcasestr(const char *haystack, const char *needle)
{
   if (! needle || ! *needle) return 0;
   return strcasestr(haystack,needle);   
}


/********************************************************************************

   Conversion Utilities

   convSI(string, inum, delim)                     string to int
   convSI(string, inum, low, high, delim)          string to int with range check

   convSD(string, dnum, delim)                     string to double
   convSD(string, dnum, low, high, delim)          string to double with range check

   convSF(string, fnum, delim)                     string to float
   convSF(string, fnum, low, high, delim)          string to float with range check

   convIS(inum, string, cc)                        int to string with returned cc

   convDS(fnum, digits, string, cc)                double to string with specified
                                                     digits of precision and returned cc

   string      input (cchar *) or output (char *)
   inum        input (int) or output (int &)
   dnum        input (double) or output (double &)
   delim       optional returned delimiter (null or cchar **)
   low, high   input range check (int or double)
   cc          output string length (int &)
   digits      input digits of precision (int) to be used for output string

   NOTE: decimal point may be comma or period.
         1000's separators must NOT be present.

   convIS and convDS also return the length cc of the string output.
   convDS accepts same formats as atof. Decimal point can be comma or period.
   convDS will use whatever format (f/e) gives the shortest result.
   Outputs like "e03" or "e+03" will be shortened to "e3".

   function status returned:
       0    normal conversion, no invalid digits, blank/null termination
       1    successful conversion, but trailing non-numeric found
       2    conversion OK, but outside specified limits
       3    null or blank string, converted to zero       (obsolete, now status 4)
       4    conversion error, invalid data in string
   overlapping statuses have following precedence: 4 3 2 1 0

*********************************************************************************/

//  Convert string to integer

int convSI(cchar *string, int &inum, cchar **delim)                              //  use glib function
{
   char     *ddelim = 0;
   int      err;
   
   inum = strtol(string,&ddelim,10);                                             //  convert next characters
   if (delim) *delim = ddelim;
   if (ddelim == string) err = 4;                                                //  no valid digits
   else if (*ddelim == '\0') err = 0;                                            //  null delimiter
   else if (*ddelim == ' ') err = 0;                                             //  blank delimiter
   else err = 1;                                                                 //  other delimiter
   return err;
}


int convSI(cchar *string, int &inum, int lolim, int hilim, cchar **delim)
{
   int stat = convSI(string,inum,delim);

   if (stat > 2) return stat;                                                    //  invalid or null/blank
   if (inum < lolim) return 2;                                                   //  return 2 if out of limits
   if (inum > hilim) return 2;                                                   //  (has precedence over status 1)
   return stat;                                                                  //  limits OK, return 0 or 1
}


//  Convert string to double   ***  status 3 --> status 4  ***

int convSD(cchar *string, double &dnum, cchar **delim)                           //  use glib function
{
   char     *ddelim = 0;
   int      err;

   dnum = strtod(string,&ddelim);
   if (delim) *delim = ddelim;
   if (ddelim == string) err = 4;                                                //  no valid digits
   else if (*ddelim == '\0') err = 0;                                            //  OK, null delimiter
   else if (*ddelim == ' ') err = 0;                                             //  OK, blank delimiter
   else err = 1;                                                                 //  OK, other delimiter
   return err;
}


int convSD(cchar *string, double &dnum, double lolim, double hilim, cchar **delim)
{
   int stat = convSD(string,dnum,delim);

   if (stat > 2) return stat;                                                    //  invalid or null/blank
   if (dnum < lolim) return 2;                                                   //  return 2 if out of limits
   if (dnum > hilim) return 2;                                                   //  (has precedence over status 1)
   return stat;                                                                  //  limits OK, return 0 or 1
}


int convSF(cchar *string, float &fnum, cchar **delim)
{
   double   dnum;
   int      err;
   err = convSD(string,dnum,delim);
   fnum = dnum;
   return err;
}


int convSF(cchar *string, float &fnum, float lolim, float hilim, cchar **delim)
{
   double   dnum, dlolim = lolim, dhilim = hilim;
   int      err;
   err = convSD(string,dnum,dlolim,dhilim,delim);
   fnum = dnum;
   return err;
}


//  Convert int to string with returned length.
//  (will never exceed 12 characters)

int convIS(int inum, char *string, int *cc)
{
   int   ccc;

   ccc = snprintf(string,12,"%d",inum);
   if (cc) *cc = ccc;
   return 0;
}


//  Convert double to string with specified digits of precision.
//  Shortest length format (f/e) will be used.
//  Output length is returned in optional argument cc.
//  (will never exceed 20 characters)

int convDS(double dnum, int digits, char *string, int *cc)
{
   char     *pstr;

   snprintf(string,20,"%.*g",digits,dnum);

   pstr = strstr(string,"e+");                                                   //  1.23e+12  >  1.23e12
   if (pstr) strcpy(pstr+1,pstr+2);

   pstr = strstr(string,"e0");                                                   //  1.23e02  >  1.23e2
   if (pstr) strcpy(pstr+1,pstr+2);

   pstr = strstr(string,"e0");
   if (pstr) strcpy(pstr+1,pstr+2);

   pstr = strstr(string,"e-0");                                                  //  1.23e-02  >  1.23e-2
   if (pstr) strcpy(pstr+2,pstr+3);

   pstr = strstr(string,"e-0");
   if (pstr) strcpy(pstr+2,pstr+3);

   if (cc) *cc = strlen(string);

   return 0;
}


//  format a number as "123 B" or "12.3 KB" or "1.23 MB" etc.
//  prec is the desired digits of precision to output.
//  WARNING: only the last 100 conversions remain available in memory.
//  Example formats for 3 digits of precision:
//    123 B,  999 B,  1.23 KB,  98.7 KB,  456 KB,  2.34 MB,  45.6 GB,  1.23 GB

char * formatKBMB(double fnum, int prec)
{
   #define kilo 1024
   #define bmega (kilo*kilo)
   #define bgiga (kilo*kilo*kilo)

   cchar          *units;
   static char    *output[100];
   static int     ftf = 1, ii;
   double         gnum;

   if (ftf) {                                                                    //  keep last 100 conversions
      ftf = 0;
      for (ii = 0; ii < 100; ii++)
         output[ii] = (char *) zmalloc(20);
   }

   gnum = fabs(fnum);

   if (gnum > bgiga) {
      fnum = fnum / bgiga;
      units = "GB";
   }
   else if (gnum > bmega) {
      fnum = fnum / bmega;
      units = "MB";
   }
   else if (gnum > kilo) {
      fnum = fnum / kilo;
      units = "KB";
   }
   else units = "B ";

   gnum = fabs(fnum);
   if (prec == 2 && gnum >= 99.5) prec++;                                        //  avoid e+nn formats
   if (prec == 3 && gnum >= 999.5) prec++;
   if (prec == 4 && gnum >= 9999.5) prec++;
   if (prec == 5 && gnum >= 99999.5) prec++;
   if (prec == 6 && gnum >= 999999.5) prec++;

   if (++ii > 99) ii = 0;
   snprintf(output[ii],20,"%.*g %s",prec,fnum,units);
   return output[ii];
}


/********************************************************************************

    Wildcard string match

    Match candidate string to wildcard string containing any number of
    '*' or '?' wildcard characters. '*' matches any number of characters,
    including zero characters. '?' matches any one character.
    Returns 0 if match, 1 if no match.

    Benchmark: 0.032 usec.       wild = *asdf*qwer?yxc
               3.3 GHz Core i5   match = XXXasdfXXXXqwerXyxc

*********************************************************************************/

int MatchWild(cchar *pWild, cchar *pString)
{
   int   ii, star;

new_segment:

   star = 0;
   while (pWild[0] == '*')
   {
      star = 1;
      pWild++;
   }

test_match:

   for (ii = 0; pWild[ii] && (pWild[ii] != '*'); ii++)
   {
      if (pWild[ii] != pString[ii])
      {
         if (! pString[ii]) return 1;
         if (pWild[ii] == '?') continue;
         if (! star) return 1;
         pString++;
         goto test_match;
      }
   }

   if (pWild[ii] == '*')
   {
      pString += ii;
      pWild += ii;
      goto new_segment;
   }

   if (! pString[ii]) return 0;
   if (ii && pWild[ii-1] == '*') return 0;
   if (! star) return 1;
   pString++;
   goto test_match;
}


/********************************************************************************

    Wildcard string match - ignoring case
    Works like MatchWild() above, but case is ignored.

***/

int MatchWildIgnoreCase(cchar *pWild, cchar *pString)
{
   int   ii, star;

new_segment:

   star = 0;
   while (pWild[0] == '*')
   {
      star = 1;
      pWild++;
   }

test_match:

   for (ii = 0; pWild[ii] && (pWild[ii] != '*'); ii++)
   {
      if (! strmatchcaseN(pWild+ii,pString+ii,1))                                //  the only difference
      {
         if (! pString[ii]) return 1;
         if (pWild[ii] == '?') continue;
         if (! star) return 1;
         pString++;
         goto test_match;
      }
   }

   if (pWild[ii] == '*')
   {
      pString += ii;
      pWild += ii;
      goto new_segment;
   }

   if (! pString[ii]) return 0;
   if (ii && pWild[ii-1] == '*') return 0;
   if (! star) return 1;
   pString++;
   goto test_match;
}


/********************************************************************************

   SearchWild  - wildcard file search

   Find all files with total /pathname/filename matching a pattern,
   which may have any number of the wildcard characters '*' and '?'
   in either or both the pathname and filename.

   cchar * SearchWild(cchar *wfilespec, int &flag)

   inputs:  flag = 1 to start a new search
            flag = 2 abort a running search
            *** do not modify flag within a search ***

            wfilespec = filespec to search with optional wildcards
               e.g. "/name1/na*me2/nam??e3/name4*.ext?"

   return:  a pointer to one matching file is returned per call,
            or null when there are no more matching files.

   The search may be aborted before completion, but make a final
   call with flag = 2 to clean up temp file. A new search with
   flag = 1 will also finish the cleanup.

   NOT THREAD SAFE - do not use in parallel threads

   (#) is used in place of (*) in comments below to prevent
       the compiler from interpreting (#/) as end of comments

   GNU find peculiarities:
     find /path/#      omits "." files
     find /path/       includes "." files
     find /path/#      recurses folders under /path/
     find /path/#.txt  does not recurse folders
     find /path/#/     finds all files under /path/
     find /path/#/#    finds files >= 1 folder level under /path/
     find /path/xxx#   never finds anything

   SearchWild uses simpler rules:
     '/' and '.' are treated like all other characters and match '#' and '?'
     no files are excluded except pure folders
     /path/#.txt finds all xxx.txt files under /path/ at all levels
     (because #.txt matches aaa.txt, /aaa/bbb.txt, etc.)

   Benchmark: search for /usr/share/#/README, find 365 from 119K files
              first time:  3.13 secs.   (3.5 GHz Core i5  SSD disk)
              second time: 0.25 secs.
***/

cchar * SearchWild(cchar *wpath, int &uflag)
{
   static FILE    *fid = 0;
   static char    matchfile[XFCC];
   char           searchpath[XFCC];
   char           command[XFCC];
   int            cc, err;
   char           *pp;

   if ((uflag == 1) || (uflag == 2)) {                                           //  first call or stop flag
      if (fid) pclose(fid);                                                      //  if file open, close it
      fid = 0;
      if (uflag == 2) return 0;                                                  //  stop flag, done
   }

   if (uflag == 1)                                                               //  first call flag
   {
      cc = strlen(wpath);
      if (cc == 0) return 0;
      if (cc > XFCC-20) zappcrash("SearchWild: wpath > XFCC");

      repl_Nstrs(wpath,searchpath,"\"","\\\"","$","\\$",0);                      //  escape " and $ chars.

      pp = strchr(searchpath,'*');
      if (pp) {                                                                  //  not efficient but foolproof
         while ((*pp != '/') && (pp > searchpath)) pp--;                         //  /aaa/bbb/cc*cc... >>> /aaa/bbb/
         if (pp > searchpath) *(pp+1) = 0;
      }
      
      snprintf(command,XFCC,"find -L \"%s\" -type f",searchpath);                //  find files (ordinary, symlink)
      fid = popen(command,"r");
      if (! fid) zappcrash(strerror(errno));
      uflag = 763568954;                                                         //  begin search
   }

   if (uflag != 763568954) zappcrash("SearchWild, uflag invalid");

   while (true)
   {
      pp = fgets(matchfile,XFCC-2,fid);                                          //  next matching file
      if (! pp) {
         pclose(fid);                                                            //  no more
         fid = 0;
         return 0;
      }

      cc = strlen(matchfile);                                                    //  get rid of trailing \n
      matchfile[cc-1] = 0;

      err = MatchWild(wpath,matchfile);                                          //  wildcard match?
      if (err) continue;                                                         //  no

      return matchfile;                                                          //  return file
   }
}


/********************************************************************************

    SearchWildCase  - wildcard file search - ignoring case
    Works like SearchWild() above, but case of file name is ignored.

    Actually, the trailing part of the path name is also case-insensitive,
    meaning that it is possible to get more matches than technically correct
    if folders like this are present:
       /AAA/BBB/.../filename
       /AAA/bbb/.../filename

***/

cchar * SearchWildCase(cchar *wpath, int &uflag) 
{
   static FILE    *fid = 0;
   static char    matchfile[XFCC];
   char           searchpath[XFCC];
   char           command[XFCC];
   int            cc, err;
   char           *pp;

   if ((uflag == 1) || (uflag == 2)) {                                           //  first call or stop flag
      if (fid) pclose(fid);                                                      //  if file open, close it
      fid = 0;
      if (uflag == 2) return 0;                                                  //  stop flag, done
   }

   if (uflag == 1)                                                               //  first call flag
   {
      cc = strlen(wpath);
      if (cc == 0) return 0;
      if (cc > XFCC-20) zappcrash("SearchWild: wpath > XFCC");

      repl_Nstrs(wpath,searchpath,"\"","\\\"","$","\\$",0);                      //  escape " and $ chars.

      pp = strchr(searchpath,'*');
      if (pp) {                                                                  //  not efficient but foolproof
         while ((*pp != '/') && (pp > searchpath)) pp--;                         //  /aaa/bbb/cc*cc... >>> /aaa/bbb/
         if (pp > searchpath) *(pp+1) = 0;
      }

      snprintf(command,XFCC,"find -L \"%s\" -type f",searchpath);                //  find files (ordinary, symlink) 
      fid = popen(command,"r");
      if (! fid) zappcrash(strerror(errno));
      uflag = 763568954;                                                         //  begin search
   }

   if (uflag != 763568954) zappcrash("SearchWild, uflag invalid");

   while (true)
   {
      pp = fgets(matchfile,XFCC-2,fid);                                          //  next matching file
      if (! pp) {
         pclose(fid);                                                            //  no more
         fid = 0;
         return 0;
      }

      cc = strlen(matchfile);                                                    //  get rid of trailing \n
      matchfile[cc-1] = 0;

      err = MatchWildIgnoreCase(wpath,matchfile);                                //  wildcard match?
      if (err) continue;                                                         //  no

      return matchfile;                                                          //  return file
   }
}


/********************************************************************************

   Find all files matching a given pattern
   
   int zfind(cchar *pattern, char **&flist, int &NF)

      pattern     pattern to match, with wildcards
      flist       list of files returned
      NF          count of files returned

   Returns 0 if OK, +N if error (errno is set).
   flist and flist[*] are subjects for zfree().

   zfind() works for files containing quotes (") 

*********************************************************************************/

int zfind(cchar *pattern, char **&flist, int &NF)
{
   char     **zfind_filelist = 0;                                                //  list of filespecs returned
   int      globflags = GLOB_PERIOD;                                             //  include dotfiles
   int      err, cc;
   glob_t   globdata;

   globdata.gl_pathc = 0;                                                        //  glob() setup
   globdata.gl_offs = 0;
   globdata.gl_pathc = 0;

   NF = 0;                                                                       //  empty output
   flist = 0;

   err = glob(pattern,globflags,null,&globdata);                                 //  find all matching files

   if (err) {
      if (err == GLOB_NOMATCH) err = 0;
      else if (err == GLOB_ABORTED) err = 1;
      else if (err == GLOB_NOSPACE) err = 2;
      else err = 3;
      if (err) printz("zfind() error: %d \n",err);
      globfree(&globdata);                                                       //  free glob memory
      return err;
   }

   NF = globdata.gl_pathc;
   if (! NF) {
      globfree(&globdata);
      return 0;
   }

   cc = NF * sizeof(char *);
   zfind_filelist = (char **) zmalloc(cc);

   for (int ii = 0; ii < NF; ii++)                                               //  loop found files
      zfind_filelist[ii] = zstrdup(globdata.gl_pathv[ii]);                       //  add file to output list

   flist = zfind_filelist;
   globfree(&globdata);                                                          //  free glob memory
   return 0;
}


/********************************************************************************/

//  perform a binary search on sorted list of integers
//  return matching element or -1 if not found
//  Benchmark: search a list of 10 million sorted integers
//             0.35 usecs.  3.3 GHz Core i5

int bsearch(int seekint, int nn, int list[])
{
   int      ii, jj, kk, rkk;

   ii = nn / 2;                                                                  //  next element to search
   jj = (ii + 1) / 2;                                                            //  next increment
   nn--;                                                                         //  last element
   rkk = 0;

   while (true)
   {
      kk = list[ii] - seekint;                                                   //  check element

      if (kk > 0)
      {
         ii -= jj;                                                               //  too high, go down
         if (ii < 0) return -1;
      }

      else if (kk < 0)
      {
         ii += jj;                                                               //  too low, go up
         if (ii > nn) return -1;
      }

      else if (kk == 0) return ii;                                               //  matched

      jj = jj / 2;                                                               //  reduce increment

      if (jj == 0)
      {
         jj = 1;                                                                 //  step by 1 element
         if (! rkk) rkk = kk;                                                    //  save direction
         else
         {
            if (rkk > 0) { if (kk < 0) return -1; }                              //  if change direction, fail
            else if (kk > 0) return -1;
         }
      }
   }
}


//  Perform a binary search on sorted set of records in memory.
//  Return matching record number (0 based) or -1 if not found.
//  Benchmark: search 10 million sorted records of 20 chars.
//             0.61 usecs.  3.3 GHz Core i5

int bsearch(cchar *seekrec, cchar *allrecs, int recl, int nrecs)
{
   int      ii, jj, kk, rkk;

   ii = nrecs / 2;                                                               //  next element to search
   jj = (ii + 1) / 2;                                                            //  next increment
   nrecs--;                                                                      //  last element
   rkk = 0;

   while (true)
   {
      kk = strcmp(allrecs+ii*recl,seekrec);                                      //  compare member rec to seek rec

      if (kk > 0)
      {
         ii -= jj;                                                               //  too high, go down in set
         if (ii < 0) return -1;
      }

      else if (kk < 0)
      {
         ii += jj;                                                               //  too low, go up in set
         if (ii > nrecs) return -1;
      }

      else if (kk == 0) return ii;                                               //  matched

      jj = jj / 2;                                                               //  reduce increment

      if (jj == 0)
      {
         jj = 1;                                                                 //  step by 1 element
         if (! rkk) rkk = kk;                                                    //  save direction
         else
         {
            if (rkk > 0) { if (kk < 0) return -1; }                              //  if change direction, fail
            else if (kk > 0) return -1;
         }
      }
   }
}


//  Perform a binary search on sorted set of pointers to records in memory.
//  Return matching record number (0 based) or -1 if not found.
//  The pointers are sorted in the order of the records starting at char N.
//  The records need not be sorted.
//  The string length of seekrec is compared.

int bsearch(cchar *seekrec, cchar **allrecs, int N, int nrecs)
{
   int      ii, jj, kk, rkk;

   ii = nrecs / 2;                                                               //  next element to search
   jj = (ii + 1) / 2;                                                            //  next increment
   nrecs--;                                                                      //  last element
   rkk = 0;

   while (true)
   {
      kk = strcmp(allrecs[ii]+N,seekrec);                                        //  compare member rec to seek rec

      if (kk > 0)
      {
         ii -= jj;                                                               //  too high, go down in set
         if (ii < 0) return -1;
      }

      else if (kk < 0)
      {
         ii += jj;                                                               //  too low, go up in set
         if (ii > nrecs) return -1;
      }

      else if (kk == 0) return ii;                                               //  matched

      jj = jj / 2;                                                               //  reduce increment

      if (jj == 0)
      {
         jj = 1;                                                                 //  step by 1 element
         if (! rkk) rkk = kk;                                                    //  save direction
         else
         {
            if (rkk > 0) { if (kk < 0) return -1; }                              //  if change direction, fail
            else if (kk > 0) return -1;
         }
      }
   }
}


/********************************************************************************
   heap sort functions

   void HeapSort(int list[], int nn)
   void HeapSort(float flist[], int nn)
   void HeapSort(double dlist[], int nn)
   -------------------------------------
   Sort list of nn integers, floats, or doubles.
   Numbers are sorted in ascending order. 

   void HeapSort(char *plist[], int nn)
   ------------------------------------
   Pointers are sorted in order of the strings they point to. 
   The strings are not changed.

   void HeapSort(char *plist1[], char *plist2[], int nn)
   -----------------------------------------------------
   Sort two lists of pointers to two sets of strings.
   Both lists are sorted in order of the first set of strings.

   void HeapSort(char *plist[], int nn, compfunc)
   ----------------------------------------------
   Sort list of pointers to strings in user-defined order.
   Pointers are sorted, strings are not changed.

   void HeapSort(char *recs, int RL, int NR, compfunc)
   ---------------------------------------------------
   Sort an array of records in memory using a caller-supplied compare function.
      recs  pointer to 1st record in array
      RL    record length
      NR    no. of records

   int compfunc(cchar *rec1, cchar *rec2)
   --------------------------------------
   compare rec1 to rec2, return -1 0 +1  if rec1 < = > rec2  in sort order.

   Benchmarks: (3.3 GHz Core i5)
      10 million integers: 1.5 secs
      10 million doubles: 2.4 secs
      2 million pointers to 100 character recs: 1.8 secs

*********************************************************************************/

#define SWAP(x,y) (temp = (x), (x) = (y), (y) = temp)


//  heapsort for array of integers

static void adjust(int vv[], int n1, int n2)
{
   int   *bb, jj, kk, temp;

   bb = vv - 1;
   jj = n1;
   kk = n1 * 2;

   while (kk <= n2)
   {
      if (kk < n2 && bb[kk] < bb[kk+1]) kk++;
      if (bb[jj] < bb[kk]) SWAP(bb[jj],bb[kk]);
      jj = kk;
      kk *= 2;
   }
}

void HeapSort(int vv[], int nn)
{
   int   *bb, jj, temp;

   for (jj = nn/2; jj > 0; jj--) adjust(vv,jj,nn);

   bb = vv - 1;

   for (jj = nn-1; jj > 0; jj--)
   {
      SWAP(bb[1], bb[jj+1]);
      adjust(vv,1,jj);
   }
}


//  heapsort for array of floats

static void adjust(float vv[], int n1, int n2)
{
   float    *bb, temp;
   int      jj, kk;

   bb = vv - 1;
   jj = n1;
   kk = n1 * 2;

   while (kk <= n2)
   {
      if (kk < n2 && bb[kk] < bb[kk+1]) kk++;
      if (bb[jj] < bb[kk]) SWAP(bb[jj],bb[kk]);
      jj = kk;
      kk *= 2;
   }
}

void HeapSort(float vv[], int nn)
{
   float    *bb, temp;
   int      jj;

   for (jj = nn/2; jj > 0; jj--) adjust(vv,jj,nn);

   bb = vv - 1;

   for (jj = nn-1; jj > 0; jj--)
   {
      SWAP(bb[1], bb[jj+1]);
      adjust(vv,1,jj);
   }
}


//  heapsort for array of doubles

static void adjust(double vv[], int n1, int n2)
{
   double   *bb, temp;
   int      jj, kk;

   bb = vv - 1;
   jj = n1;
   kk = n1 * 2;

   while (kk <= n2)
   {
      if (kk < n2 && bb[kk] < bb[kk+1]) kk++;
      if (bb[jj] < bb[kk]) SWAP(bb[jj],bb[kk]);
      jj = kk;
      kk *= 2;
   }
}

void HeapSort(double vv[], int nn)
{
   double   *bb, temp;
   int      jj;

   for (jj = nn/2; jj > 0; jj--) adjust(vv,jj,nn);

   bb = vv - 1;

   for (jj = nn-1; jj > 0; jj--)
   {
      SWAP(bb[1], bb[jj+1]);
      adjust(vv,1,jj);
   }
}


//  heapsort array of pointers to strings in ascending order of strings
//  pointers are sorted, strings are not changed.

static void adjust(char *vv[], int n1, int n2)
{
   char     **bb, *temp;
   int      jj, kk;

   bb = vv - 1;
   jj = n1;
   kk = n1 * 2;

   while (kk <= n2)
   {
      if (kk < n2 && strcmp(bb[kk],bb[kk+1]) < 0) kk++;
      if (strcmp(bb[jj],bb[kk]) < 0) SWAP(bb[jj],bb[kk]);
      jj = kk;
      kk *= 2;
   }
}

void HeapSort(char *vv[], int nn)
{
   char     **bb, *temp;
   int      jj;

   for (jj = nn/2; jj > 0; jj--) adjust(vv,jj,nn);

   bb = vv;

   for (jj = nn-1; jj > 0; jj--)
   {
      SWAP(bb[0], bb[jj]);
      adjust(vv,1,jj);
   }
}


//  Heapsort 2 lists of pointers to 2 parallel sets of strings
//    in ascending order of the first set of strings.
//  Both lists of pointers are sorted together in tandem.
//  Pointers are sorted, strings are not changed.

static void adjust(char *vv1[], char *vv2[], int n1, int n2)
{
   char     **bb1, **bb2, *temp;
   int      jj, kk;

   bb1 = vv1 - 1;
   bb2 = vv2 - 1;
   jj = n1;
   kk = n1 * 2;

   while (kk <= n2)
   {
      if (kk < n2 && strcmp(bb1[kk],bb1[kk+1]) < 0) kk++;
      if (strcmp(bb1[jj],bb1[kk]) < 0) {
         SWAP(bb1[jj],bb1[kk]);
         SWAP(bb2[jj],bb2[kk]);
      }
      jj = kk;
      kk *= 2;
   }
}

void HeapSort(char *vv1[], char *vv2[], int nn)
{
   char     **bb1, **bb2, *temp;
   int      jj;

   for (jj = nn/2; jj > 0; jj--) adjust(vv1,vv2,jj,nn);

   bb1 = vv1;
   bb2 = vv2;

   for (jj = nn-1; jj > 0; jj--)
   {
      SWAP(bb1[0], bb1[jj]);
      SWAP(bb2[0], bb2[jj]);
      adjust(vv1,vv2,1,jj);
   }
}


//  heapsort array of pointers to strings in user-defined order.
//  pointers are sorted, strings are not changed.

static void adjust(char *vv[], int n1, int n2, HeapSortUcomp fcomp)
{
   char     **bb, *temp;
   int      jj, kk;

   bb = vv - 1;
   jj = n1;
   kk = n1 * 2;

   while (kk <= n2)
   {
      if (kk < n2 && fcomp(bb[kk],bb[kk+1]) < 0) kk++;
      if (fcomp(bb[jj],bb[kk]) < 0) SWAP(bb[jj],bb[kk]);
      jj = kk;
      kk *= 2;
   }
}

void HeapSort(char *vv[], int nn, HeapSortUcomp fcomp)
{
   char     **bb, *temp;
   int      jj;

   for (jj = nn/2; jj > 0; jj--) adjust(vv,jj,nn,fcomp);

   bb = vv;

   for (jj = nn-1; jj > 0; jj--)
   {
      SWAP(bb[0], bb[jj]);
      adjust(vv,1,jj,fcomp);
   }
}


//  heapsort for array of records,
//  using caller-supplied record compare function.
//  HeapSortUcomp returns [ -1 0 +1 ]  for  rec1 [ < = > ] rec2
//  method: build array of pointers and sort these, then
//  use this sorted array to re-order the records at the end.

static int     *vv1, *vv2;

static void adjust(char *recs, int RL, int n1, int n2, HeapSortUcomp fcomp)
{
   int      *bb, jj, kk, temp;
   char     *rec1, *rec2;

   bb = vv1 - 1;
   jj = n1;
   kk = n1 * 2;

   while (kk <= n2)
   {
      rec1 = recs + RL * bb[kk];
      rec2 = recs + RL * bb[kk+1];
      if (kk < n2 && fcomp(rec1,rec2) < 0) kk++;
      rec1 = recs + RL * bb[jj];
      rec2 = recs + RL * bb[kk];
      if (fcomp(rec1,rec2) < 0) SWAP(bb[jj],bb[kk]);
      jj = kk;
      kk *= 2;
   }
}

void HeapSort(char *recs, int RL, int NR, HeapSortUcomp fcomp)
{
   int      *bb, jj, kk, temp, flag;
   char     *vvrec;

   vv1 = (int *) malloc((NR+1) * sizeof(int));
   for (jj = 0; jj < NR; jj++) vv1[jj] = jj;

   for (jj = NR/2; jj > 0; jj--) adjust(recs,RL,jj,NR,fcomp);

   bb = vv1 - 1;

   for (jj = NR-1; jj > 0; jj--)
   {
      SWAP(bb[1], bb[jj+1]);
      adjust(recs,RL,1,jj,fcomp);
   }

   vv2 = (int *) malloc((NR+1) * sizeof(int));
   for (jj = 0; jj < NR; jj++) vv2[vv1[jj]] = jj;

   vvrec = (char *) malloc(RL);
   flag = 1;
   while (flag)
   {
      flag = 0;
      for (jj = 0; jj < NR; jj++)
      {
         kk = vv2[jj];
         if (kk == jj) continue;
         memmove(vvrec,recs+jj*RL,RL);
         memmove(recs+jj*RL,recs+kk*RL,RL);
         memmove(recs+kk*RL,vvrec,RL);
         SWAP(vv2[jj],vv2[kk]);
         flag = 1;
      }
   }

   free(vv1);
   free(vv2);
   free(vvrec);
}


/********************************************************************************

   int MemSort (char *RECS, int RL, int NR, int KEYS[][3], int NK)

   RECS is an array of records, to be sorted in-place.
   (record length = RL, record count = NR)

   KEYS[NK][3]  is an integer array defined as follows:
        [N][0]    starting position of Nth key field in RECS
        [N][1]    length of Nth key field in RECS
        [N][2]    type of sort for Nth key:
                    1 = char ascending
                    2 = char descending
                    3 = int*4 ascending
                    4 = int*4 descending
                    5 = float*4 ascending
                    6 = float*4 descending
                    7 = float*8 ascending (double)
                    8 = float*8 descending
   Benchmark: 2 million recs of 40 bytes with 4 sort keys:
              2.5 secs (3.3 GHz Core i5).

***/

int MemSortComp(cchar *rec1, cchar *rec2);
int MemSortKeys[10][3], MemSortNK;

int MemSort(char *RECS, int RL, int NR, int KEYS[][3], int NK)
{
   int   ii;

   if (NR < 2) return 1;

   if (NK > 10) zappcrash("MemSort, bad NK");
   if (NK < 1) zappcrash("MemSort, bad NK");

   MemSortNK = NK;

   for (ii = 0; ii < NK; ii++)
   {
      MemSortKeys[ii][0] = KEYS[ii][0];
      MemSortKeys[ii][1] = KEYS[ii][1];
      MemSortKeys[ii][2] = KEYS[ii][2];
   }

   HeapSort(RECS,RL,NR,MemSortComp);
   return 1;
}

int MemSortComp(cchar *rec1, cchar *rec2)
{
   int            ii, stat, kpos, ktype, kleng;
   int            inum1, inum2;
   float          rnum1, rnum2;
   double         dnum1, dnum2;
   cchar          *p1, *p2;

   for (ii = 0; ii < MemSortNK; ii++)                                            //  loop each key
   {
      kpos = MemSortKeys[ii][0];                                                 //  relative position
      kleng = MemSortKeys[ii][1];                                                //  length
      ktype = MemSortKeys[ii][2];                                                //  type

      p1 = rec1 + kpos;                                                          //  absolute position
      p2 = rec2 + kpos;

      switch (ktype)
      {
         case 1:                                                                 //  char ascending
            stat = strncmp(p1,p2,kleng);                                         //  compare 2 key values
            if (stat) return stat;                                               //  + if rec1 > rec2, - if <
            break;                                                               //  2 keys are equal, check next key

         case 2:                                                                 //  char descending
            stat = strncmp(p1,p2,kleng);
            if (stat) return -stat;
            break;

         case 3:                                                                 //  int ascending
            memmove(&inum1,p1,4);
            memmove(&inum2,p2,4);
            if (inum1 > inum2) return 1;
            if (inum1 < inum2) return -1;
            break;

         case 4:                                                                 //  int descending
            memmove(&inum1,p1,4);
            memmove(&inum2,p2,4);
            if (inum1 > inum2) return -1;
            if (inum1 < inum2) return 1;
            break;

         case 5:                                                                 //  float ascending
            memmove(&rnum1,p1,4);
            memmove(&rnum2,p2,4);
            if (rnum1 > rnum2) return 1;
            if (rnum1 < rnum2) return -1;
            break;

         case 6:                                                                 //  float descending
            memmove(&rnum1,p1,4);
            memmove(&rnum2,p2,4);
            if (rnum1 > rnum2) return -1;
            if (rnum1 < rnum2) return 1;
            break;

         case 7:                                                                 //  double ascending
            memmove(&dnum1,p1,8);
            memmove(&dnum2,p2,8);
            if (dnum1 > dnum2) return 1;
            if (dnum1 < dnum2) return -1;
            break;

         case 8:                                                                 //  double descending
            memmove(&dnum1,p1,8);
            memmove(&dnum2,p2,8);
            if (dnum1 > dnum2) return -1;
            if (dnum1 < dnum2) return 1;
            break;

         default:                                                                //  key type not 1-8
            zappcrash("MemSort, bad KEYS sort type");
      }
   }

   return 0;                                                                     //  records match on all keys
}


/********************************************************************************/

//  test if an integer value matches any in a list of values
//  returns the matching value or zero if nothing matches
//  list of values must end with zero
//  zero cannot be one of the values to match

int zmember(int testval, int matchval1, ...)
{
   va_list     arglist;
   int         matchval;
   
   va_start(arglist,matchval1);
   matchval = matchval1;
   
   while (matchval)
   {
      if (testval == matchval) break;
      matchval = va_arg(arglist,int);
   }
   
   va_end(arglist);
   return matchval;
}


/********************************************************************************
   variable string list functions - array / list of strings

   pvlist * pvlist_create(int max)
   void pvlist_free(pvlist *pv)
   int pvlist_append(pvlist *pv, cchar *entry, int unique)
   int pvlist_prepend(pvlist *pv, cchar *entry, int unique)
   int pvlist_find(pvlist *pv, cchar *entry)
   int pvlist_remove(pvlist *pv, cchar *entry)
   int pvlist_remove(pvlist *pv, int Nth)
   int pvlist_count(pvlist *pv)
   int pvlist_replace(pvlist *pv, int Nth, char *entry)
   cchar * pvlist_get(pvlist *pv, int Nth)
   int pvlist_sort(pvlist *pv)

   These functions manage a variable length list of variable length strings.
   Declare such a list as: pvlist *pv;

*********************************************************************************/

//  Creates a pvlist with a capacity of max strings and returns a pointer.
//  String lengths are unlimited, but the count of strings is limited to max.
//  Memory is allocated for max pointers at first. Memory for the strings is
//  allocated and freed as the strings are added or removed.

pvlist * pvlist_create(int max)
{
   pvlist      *pv;

   pv = (pvlist *) zmalloc(sizeof(pvlist));
   pv->max = max;
   pv->act = 0;
   pv->list = (char **) zmalloc(max * sizeof(char *));
   return pv;
}


//  free memory for variable list and contained strings

void pvlist_free(pvlist *pv)
{
   int      ii;

   for (ii = 0; ii < pv->act; ii++)
      zfree(pv->list[ii]);
   zfree(pv->list);
   zfree(pv);
}


//  append new entry to end of list (optional if unique)
//  if list if full, first entry is removed and rest are packed down
//  return: N >= 0: new entry added at position N
//          N = -1: not unique, not added

int pvlist_append(pvlist *pv, cchar *entry, int unique)
{
   int      ii;

   if (unique && pvlist_find(pv,entry) >= 0) return -1;                          //  not unique

   if (pv->act == pv->max) pvlist_remove(pv,0);                                  //  if list full, remove 1st entry

   ii = pv->act;
   pv->list[ii] = zstrdup(entry);                                                //  add to end of list
   pv->act++;
   return ii;
}


//  prepend new entry to list (optional if unique)
//  prior list entries are pushed down to make room
//  if list is full, last entry is removed first
//  return: N = 0: new entry added at position 0
//          N = -1: not unique, not added

int pvlist_prepend(pvlist *pv, cchar *entry, int unique)
{
   int      ii;

   if (unique && pvlist_find(pv,entry) >= 0) return -1;                          //  not unique

   if (pv->act == pv->max) pvlist_remove(pv,pv->act-1);                          //  if list full, remove last entry

   for (ii = pv->act; ii > 0; ii--)                                              //  push all list entries down
      pv->list[ii] = pv->list[ii-1];
   pv->list[0] = zstrdup(entry);                                                 //  add to start of list
   pv->act++;
   return 0;
}


//  find list entry by name, return entry (0 based)
//  return -1 if not found

int pvlist_find(pvlist *pv, cchar *entry)
{
   int      ii;

   for (ii = 0; ii < pv->act; ii++)
      if (strmatch(entry,pv->list[ii])) break;
   if (ii < pv->act) return ii;
   return -1;
}


//  remove an entry by name and repack list
//  return (former) entry or -1 if not found

int pvlist_remove(pvlist *pv, cchar *entry)
{
   int      ii;

   ii = pvlist_find(pv,entry);
   if (ii < 0) return -1;
   pvlist_remove(pv,ii);
   return ii;
}


//  remove an entry by number and repack list
//  returns -1 if entry is beyond list end

int pvlist_remove(pvlist *pv, int ii)
{
   if (ii < 0 || ii >= pv->act) return -1;
   zfree(pv->list[ii]);
   for (++ii; ii < pv->act; ii++)                                                //  pre-increment
      pv->list[ii-1] = pv->list[ii];
   pv->act--;
   return 0;
}


//  return entry count

int pvlist_count(pvlist *pv)
{
   return pv->act;
}


//  replace Nth entry with new one

int pvlist_replace(pvlist * pv, int ii, cchar *entry)
{
   if (ii < 0 || ii >= pv->act) return -1;
   zfree(pv->list[ii]);
   pv->list[ii] = zstrdup(entry);
   return 0;
}


//  return Nth entry or null

char * pvlist_get(pvlist *pv, int Nth)
{
   if (Nth >= pv->act) return 0;
   return pv->list[Nth];
}


//  sort list in ascending order

int pvlist_sort(pvlist *pv)
{
   HeapSort(pv->list,pv->act);
   return 0;
}


/********************************************************************************/

//  Random number generators with explicit context
//  and improved randomness over a small series.
//  Benchmark: lrandz 0.012 usec  drandz 0.014 usec  3.3 GHz Core i5

int lrandz(int64 *seed)                                                          //  returns 0 to 0x7fffffff
{
   *seed = *seed ^ (*seed << 17);
   *seed = *seed ^ (*seed << 20);
   return nrand48((unsigned int16 *) seed);
}

int lrandz()                                                                     //  implicit seed, repeatable sequence
{
   static int64   seed = 12345678;
   return lrandz(&seed);
}

double drandz(int64 *seed)                                                       //  returns 0.0 to 0.99999...
{
   *seed = *seed ^ (*seed << 17);
   *seed = *seed ^ (*seed << 20);
   return erand48((unsigned int16 *) seed);
}

double drandz()                                                                  //  automatic seed, volatile
{
   static int64 seed = get_seconds();
   return drandz(&seed);
}


/********************************************************************************

   spline1: define a curve using a set of data points (x and y values)
   spline2: for a given x-value, return a y-value fitting the curve

   For spline1, the no. of curve-defining points must be < 100.
   For spline2, the given x-value must be within the range defined in spline1.

   The algorithm was taken from the book "Numerical Recipes"
   (Cambridge University Press) and converted from Fortran to C++.

***/

namespace splinedata
{
   int      nn;
   float    px1[100], py1[100], py2[100];
}


void spline1(int dnn, float *dx1, float *dy1)
{
   using namespace splinedata;

   float    sig, p, u[100];
   int      ii;

   nn = dnn;
   if (nn > 100) zappcrash("spline1(), > 100 data points");

   for (ii = 0; ii < nn; ii++)
   {
      px1[ii] = dx1[ii];
      py1[ii] = dy1[ii];
      if (ii && px1[ii] <= px1[ii-1])
         zappcrash("spline1(), x-value not increasing");
   }

   py2[0] = u[0] = 0;

   for (ii = 1; ii < nn-1; ii++)
   {
      sig = (px1[ii] - px1[ii-1]) / (px1[ii+1] - px1[ii-1]);
      p = sig * py2[ii-1] + 2;
      py2[ii] = (sig - 1) / p;
      u[ii] = (6 * ((py1[ii+1] - py1[ii]) / (px1[ii+1] - px1[ii]) - (py1[ii] - py1[ii-1])
            / (px1[ii] - px1[ii-1])) / (px1[ii+1] - px1[ii-1]) - sig * u[ii-1]) / p;
   }

   py2[nn-1] = 0;

   for (ii = nn-2; ii >= 0; ii--)
      py2[ii] = py2[ii] * py2[ii+1] + u[ii];

   return;
}


float  spline2(float x)
{
   using namespace splinedata;

   int      kk, klo = 0, khi = nn-1;
   float    h, a, b, y;

   while (khi - klo > 1)
   {
      kk = (khi + klo) / 2;
      if (px1[kk] > x) khi = kk;
      else klo = kk;
   }

   h = px1[khi] - px1[klo];
   a = (px1[khi] - x) / h;
   b = (x - px1[klo]) / h;
   y = a * py1[klo] + b * py1[khi] + ((a*a*a - a) * py2[klo]
                                   + (b*b*b - b) * py2[khi]) * (h*h) / 6;

   return y;
}


/********************************************************************************/

//  Add text strings to a FIFO queue, retrieve text strings.
//  Can be used by one or two threads.
//  thread 1: open queue, get strings, close queue.
//  thread 2: put strings into queue.


//  create and initialize Qtext queue, empty status

void Qtext_open(Qtext *qtext, int cap)
{
   int      cc;

   qtext->qcap = cap;
   qtext->qnewest = -1;
   qtext->qoldest = -1;
   qtext->qdone = 0;
   cc = cap * sizeof(char *);
   qtext->qtext = (char **) zmalloc(cc);
   memset(qtext->qtext,0,cc);
   return;
}


//  add new text string to Qtext queue
//  if queue full, sleep until space is available

void Qtext_put(Qtext *qtext, cchar *format, ...)
{
   int      qnext;
   va_list  arglist;
   char     message[200];

   va_start(arglist,format);
   vsnprintf(message,199,format,arglist);
   va_end(arglist);

   qnext = qtext->qnewest + 1;
   if (qnext == qtext->qcap) qnext = 0;
   while (qtext->qtext[qnext]) zsleep(0.01);
   qtext->qtext[qnext] = zstrdup(message);
   qtext->qnewest = qnext;
   return;
}


//  remove oldest text string from Qtext queue
//  if queue empty, return a null string
//  returned string is subject for zfree()

char * Qtext_get(Qtext *qtext)
{
   int      qnext;
   char     *text;

   if (qtext->qcap == 0) return 0;
   qnext = qtext->qoldest + 1;
   if (qnext == qtext->qcap) qnext = 0;
   text = qtext->qtext[qnext];
   if (! text) return 0;
   qtext->qtext[qnext] = 0;
   qtext->qoldest = qnext;
   return text;
}


//  close Qtext, zfree() any leftover strings

void Qtext_close(Qtext *qtext)
{
   for (int ii = 0; ii < qtext->qcap; ii++)
      if (qtext->qtext[ii]) zfree(qtext->qtext[ii]);
   zfree(qtext->qtext);
   qtext->qcap = 0;
   return;
}


/********************************************************************************

  Create appimage desktop file and icon file in /home/<user>/.local/...
  This will make the appimage work normally within the system menus.
  Executed at application startup time.

   Source files in AppImage file system (.../AppDir/)
      /usr/share/appname/appname.desktop
      /usr/share/appname/appname.png

   Destination files in /home/<user>/
      /home/<user>/.local/bin/appname-NN.N-appimage                              //  appimage executable file
      /home/<user>/.local/share/applications/appname.desktop                     //  XDG desktop file
      /home/<user>/.local/share/icons/appname.png                                //  XDG icon file
   
   returns: 1  already done
            2  new desktop file created OK
            3  not an appimage
            4  failure

*****/

namespace make_appimage_names
{
   char     appname1[60], appname2[40];
}


int appimage_install(cchar *appname)
{
   using namespace make_appimage_names;
   
   FILE     *fid;
   int      err, cc, nn;
   char     *pp, *homedir;
   char     buff[300];
   char     desktopfile1[100], desktopfile2[100];
   char     iconfile1[100], iconfile2[100];
   char     exectext1[100], exectext2[100];
   char     icontext1[100], icontext2[100];

   pp = getenv("APPIMAGE");                                                      //  appimage executable file
   if (! pp) return 3;
   if (! strstr(pp,appname)) return 3;                                           //  not my appimage
   appimagexe = pp;
   pp = strrchr(appimagexe,'/');
   if (! pp) return 4;
   strncpy0(appname1,pp+1,60);                                                   //  appname1: appname-NN.N-appimage
   
   pp = strchr(appname1,'-');
   if (! pp) return 4;
   cc = pp - appname1;
   strncpy(appname2,appname1,cc);                                                //  appname2: appname
   appname2[cc] = 0;
   
   cc = readlink("/proc/self/exe",buff,300);                                     //  get own program path
   if (cc <= 0) return 4;
   buff[cc] = 0;                                                                 //  /tmp/mountpoint/usr/bin/appname

   pp = strstr(buff,"/usr/");
   if (! pp) return 4;
   pp[4] = 0;                                                                    //  /tmp/mountpoint/usr/

   homedir = getenv("HOME");
   if (strchr(homedir,' ')) {                                                    //  check /home/user has no blanks
      printz("user home \"%s\" has embedded blank",homedir);
      return 4;
   }

   //  /.../usr/share/appname/appname.desktop  >>  /home/<user>/.local/share/applications/appname.desktop

   snprintf(desktopfile1,100,"%s/share/%s/%s.desktop",buff,appname2,appname2);
   snprintf(desktopfile2,100,"%s/.local/share/applications/%s.desktop",homedir,appname2);

   //  /.../usr/share/appname/appname.png  >>  /home/<user>/.local/share/icons/appname.png

   snprintf(iconfile1,100,"%s/share/%s/icons/%s.png",buff,appname2,appname2);
   snprintf(iconfile2,100,"%s/.local/share/icons/%s.png",homedir,appname2);

   fid = fopen(desktopfile2,"r");                                                //  open existing desktop file
   if (fid) {
      while (true) {                                                             //  read desktop file
         pp = fgets(buff,300,fid);
         if (! pp) break;
         if (strmatchN(buff,"Exec=",5)) break;                                   //  look for "Exec=[my appimage]"
      }
      fclose(fid);
      
      if (pp) {
         cc = strlen(appimagexe);                                                //  appimage executable file
         nn = strmatchN(appimagexe,buff+5,cc);                                   //  compare to Exec= appimage file
         if (nn) return 1;                                                       //  same, do nothing
      }
   }
   
   err = copyFile(desktopfile1,desktopfile2);                                    //  /home/<user>/.local/share/
   if (err) {                                                                    //    /applications/appname.desktop
      printz("cannot create %s \n",desktopfile2);                                
      return 4;
   }

   err = copyFile(iconfile1,iconfile2);                                          //  /home/<user>/.local/share/
   if (err) {                                                                    //    /icons/appname.png
      printz("cannot create %s \n",iconfile2);
      return 4;
   }
   
   snprintf(exectext1,100,"Exec=%s",appname2);                                   //  Exec=appname2
   snprintf(exectext2,100,"Exec=%s",appimagexe);                                 //  Exec=appimagexe

   snprintf(icontext1,100,"Icon=/usr/share/%s/icons/%s.png",appname2,appname2);
   snprintf(icontext2,100,"Icon=%s",iconfile2);
   
   err = zsed(desktopfile2,exectext1,exectext2,icontext1,icontext2,null);        //  make text substitutions
   if (err < 0) {                                                                //  failure
      printz("cannot update %s \n",desktopfile2);
      return 4;
   }

   chmod(desktopfile2,0751);                                                     //  make fotoxx.desktop executable

   printz("appimage desktop file created at %s \n", desktopfile2);
   printz("appimage icon file created at %s \n", iconfile2);

   return 2;
}


/********************************************************************************/

//  Uninstall AppImage program, desktop and icon files
//  exits if program is uninstalled
//  returns if not (user cancels, program is not an appimage)

void appimage_unstall()
{
   using namespace make_appimage_names;

   char     *homedir;
   char     desktopfileloc[200];
   char     iconfileloc[200];
   
   if (! appimagexe) {
      printz("not an appimage, nothing was done \n");
      return;
   }
   
   homedir = getenv("HOME");
   
   snprintf(desktopfileloc,200,"%s/.local/share/applications/",homedir);
   snprintf(iconfileloc,200,"%s/.local/share/icons/",homedir);

   Qshell(1,1,"rm -f %s/%s.desktop",desktopfileloc,appname2);
   Qshell(1,1,"rm -f %s/%s.png",iconfileloc,appname2);
   Qshell(1,1,"rm -f %s",appimagexe);
   
   zexit("appimage uninstalled");
}


/********************************************************************************

   Initialize application files according to following conventions:              //  new version
     + binary executable is at:  /prefix/bin/appname                             //  = PREFIX/bin/appname
     + other application folders are derived as follows:
         /prefix/share/appname/data/            desktop, parameters, userguide  ...
         /prefix/share/doc/appname/             README, changelog, appname.man ...
         /prefix/share/appname/icons/           application icon files, filename.png
         /prefix/share/appname/images/          application image files
         /prefix/share/appname/locales/         translate-xx.po ... (original)
         /home/user/.appname/                   some installation files are copied here
         /home/user/.appname/logfile            log file with error messages

   zprefix        install location              normally /usr  subdirs: /bin /share /doc
   zdatadir       installed data files          /prefix/share/appname/data/
   zdocdir        documentation files           /prefix/share/doc/appname/
   zimagedir      images                        /prefix/share/appname/images 
   zlocalesdir    translation files             /prefix/share/appname/locales/
   zhomedir       local app files               /home/<user>/.appname

   If it does not already exist, an application folder for the current user is
   created at /home/username/.appname (following common Linux convention).
   If this folder was created for the first time, copy specified files
   (following the 1st argument) from the install folder into the newly created
   user-specific folder. The assumption is that all initial data files for the
   application (e.g. parameters) will be in the install data folder, and these are
   copied to the user folder where the user or application can modify them.

   If the running program is not connected to a terminal device, stdout and stderr are
   redirected to the log file at /home/user/.appname/logfile

***/

cchar * get_zprefix() { return zfuncs::zprefix; }                                //  /usr or /home/<user>
cchar * get_zhomedir() { return zfuncs::zhomedir; }                              //  /home/<user>/.appname or /root/.appname
cchar * get_zdatadir() { return zfuncs::zdatadir; }                              //  data files
cchar * get_zdocdir()  { return zfuncs::zdocdir;  }                              //  documentation files
cchar * get_zimagedir()  { return zfuncs::zimagedir;  }                          //  image files
cchar * get_zlocalesdir()  { return zfuncs::zlocalesdir;  }                      //  translation files


int zinitapp(cchar *appvers, cchar *homedir)                                     //  appname-N.N, opt. home dir
{
   char        logfile[200], oldlog[200];
   char        buff[300], Phomedir[200];
   char        *pp, *chTnow;
   int         cc, err;
   time_t      Tnow;
   STATB       statb;
   FILE        *fid;
   
   ftime(&startime);                                                             //  app startup time
   Tnow = startime.time;

   catch_signals();                                                              //  catch signals, do backtrace

   progexe = 0;   
   cc = readlink("/proc/self/exe",buff,300);                                     //  get own program path
   if (cc > 0) {
      buff[cc] = 0;                                                              //  readlink() quirk
      progexe = zstrdup(buff);
   }
   
   if (appimagexe) printz("program exe: %s \n",appimagexe);
   else printz("program exe: %s \n",progexe);
   printz("build date/time: %s \n",build_date_time);

   strncpy0(zappvers,appvers,40);                                                //  appname-N.N 
   strncpy0(zappname,appvers,40);                                                //  appneme   without version
   pp = strchr(zappname,'-');
   if (pp && strmatchN(pp,"-maps",5)) pp = strchr(pp+1,'-');                     //  special case fotoxx-maps-N.N
   if (pp) *pp = 0;

   if (homedir && *homedir == '/')                                               //  homedir from caller
      strncpy0(zhomedir,homedir,199);
   else
   {
      snprintf(zhomedir,199,"%s/.%s",getenv("HOME"),zappname);                   //  use /home/<user>/.appname
      snprintf(Phomedir,200,"%s-home",zhomedir);                                 //  check /home/<user>/.appname-home 
      fid = fopen(Phomedir,"r");
      if (fid) {
         pp = fgets_trim(Phomedir,200,fid);                                      //  if found, read pointer to homedir
         if (pp) strncpy0(zhomedir,pp,200);
         fclose(fid);
      }
   }
   
   printz("%s home: %s \n",zappname,zhomedir);                                   //  forbid space in home folder
   if (strchr(zhomedir,' ')) zexit("home folder name contains a space");

   cc = strlen(zhomedir);                                                        //  stop humongous username
   if (cc > 160) zexit("home folder name too big");
   
   err = stat(zhomedir,&statb);                                                  //  does app home exist already?
   if (err) {
      err = mkdir(zhomedir,0750);                                                //  no, create and initialize
      if (err) zexit("cannot create %s: %s",zhomedir,strerror(errno));
   }

   chTnow = ctime(&Tnow);
   chTnow[19] = 0;                                                               //  eliminate hundredths of seconds

   if (! isatty(fileno(stdin))) {                                                //  not attached to a terminal
      snprintf(logfile,199,"%s/logfile",zhomedir);                               //  /home/<user>/logfile
      snprintf(oldlog,199,"%s/logfile.old",zhomedir);
      err = stat(logfile,&statb);
      if (! err) rename(logfile,oldlog);                                         //  rename old log file
      fid = freopen(logfile,"a",stdout);                                         //  redirect output to log file
      fid = freopen(logfile,"a",stderr);
      if (! fid) printz("*** cannot redirect stdout and stderr \n");
   }

   printz("start %s %s \n",zappname,chTnow);
   fflush(0);

   cc = readlink("/proc/self/exe",zprefix,200);                                  //  get my executable path
   if (cc <= 0) {
      printz("readlink() /proc/self/exe) failed \n");                            //  if failed, assume /usr/bin/
      strcpy(zprefix,"/usr/bin/");
   }
   else zprefix[cc] = 0;                                                         //  readlink() quirk
   pp = strstr(zprefix,"/bin/");                                                 //  get install prefix (e.g. /usr)
   if (pp) *pp = 0;
   else (strcpy(zprefix,"/usr"));                                                //  if /xxxxx/bin --> /xxxxx

   strncatv(zdatadir,199,zprefix,"/share/",zappname,"/data",null);               //  /prefix/share/appname/data
   strncatv(zimagedir,199,zprefix,"/share/",zappname,"/images",null);            //  /prefix/share/appname/images
   strncatv(zlocalesdir,199,zprefix,"/share/",zappname,"/locales",null);         //  /prefix/share/appname/locales
   strncatv(zdocdir,199,zprefix,"/share/doc/",zappname,null);                    //  /prefix/share/doc/appname

   #ifdef DOCDIR
      strncpy0(zdocdir,DOCDIR,199);                                              //  flexible DOCDIR location (SUSE) 
   #endif

   if (! strmatch(zappname,"fotoxx-maps"))                                       //  omit fotoxx-maps
      Qshell(0,0,"cp -R -n %s/* %s",zdatadir,zhomedir);                          //  copy MISSING files > user home 

   tid_main = pthread_self();                                                    //  thread ID of main() process
   
   appruns_update();                                                             //  usage statistics

   if (! gtk_init_check(0,null)) {                                               //  non GUI jobs exit here
      printz("gtk_init() not done, zinitapp() return \n");                       //  this does not print (??) FIXME 
      return 0;
   }

   //  remaining code is for GUI process only

   display = gdk_display_get_default();                                          //  get hardware info
   screen = gdk_screen_get_default();

   #if GTK_CHECK_VERSION(3,22,0)

   GdkRectangle   rect;
   GdkMonitor     *monitor;

   monitor = gdk_display_get_primary_monitor(display);
   gdk_monitor_get_geometry(monitor,&rect);
   monitor_ww = rect.width;
   monitor_hh = rect.height;

   #else

   monitor_ww = gdk_screen_get_width(screen);                                    //  Ubuntu 16.10
   monitor_hh = gdk_screen_get_height(screen);

   #endif

   if (! monitor_ww) zexit("GTK cannot get monitor data");
   
   #if GTK_CHECK_VERSION(3,20,0) 

   GdkSeat    *gdkseat = 0;                                                      //  screen / KB / pointer associations
   if (screen) gdkseat = gdk_display_get_default_seat(display);                  //  Ubuntu 16.10
   if (screen) gtksettings = gtk_settings_get_for_screen(screen);
   if (gdkseat) mouse = gdk_seat_get_pointer(gdkseat);

   #else

   GdkDeviceManager  *devmanager = 0;                                            //  knows screen / mouse associations
   if (screen) devmanager = gdk_display_get_device_manager(display);             //  Ubuntu 16.04
   if (screen) gtksettings = gtk_settings_get_for_screen(screen);
   if (devmanager) mouse = gdk_device_manager_get_client_pointer(devmanager);
   if (! mouse) printz("*** GTK cannot get pointer device \n");

   #endif

   if (! mouse) zexit("GTK cannot get pointer device");

   if (gtksettings) {                                                            //  get default font
      g_object_get(gtksettings,"gtk_font_name",&appfont,null);
      zsetfont(appfont);                                                         //  set mono and bold versions
   }

   return 1;
}


//  set a new application font via GtkSettings
//  newfont should be something like "sans 11"
//  use generic monospace font since app font may not have a mono version

void zsetfont(cchar *newfont)
{
   char     font[40], bfont[48], mfont[48], mbfont[56];
   char     junk[40];
   int      nn, size;

   if (! gtksettings) return;
   
   nn = sscanf(newfont,"%s %d",font,&size);                                      //  "sans 11"
   if (nn != 2) {
      nn = sscanf(newfont,"%s %s %d",font,junk,&size);
      if (nn != 3) goto fail;
   }
   if (size < 5 || size > 30) goto fail;

   g_object_set(gtksettings,"gtk-font-name",newfont,null);                       //  set dialog font

   snprintf(bfont,48,"%s bold %d",font,size);                                    //  "sans bold 11"
   snprintf(mfont,48,"mono %d",size-1);                                          //  "mono 10"
   snprintf(mbfont,56,"mono bold %d",size-1);                                    //  "mono bold 10"
   appfont = zstrdup(newfont);
   appboldfont = zstrdup(bfont);
   appmonofont = zstrdup(mfont);
   appmonoboldfont = zstrdup(mbfont);
   appfontsize = size;
   return;

fail:
   printz("cannot set font: %s \n",newfont);
   return;
}


//  get the font character width and height for a given widget
//  returns 0 if OK, +N if error

int widget_font_metrics(GtkWidget *widget, int &fontwidth, int &fontheight)
{
   PangoContext         *pangocontext;
   PangoFontDescription *pangofontdesc;
   PangoFontMetrics     *pangofontmetrics;
   PangoLanguage        *pangolanguage;

   pangocontext = gtk_widget_get_pango_context(widget);
   pangofontdesc = pango_context_get_font_description(pangocontext);
   pangolanguage = pango_language_get_default();
   pangofontmetrics = pango_context_get_metrics(pangocontext,pangofontdesc,pangolanguage);
   if (! pangofontmetrics) {
      printz("widget_font_metrics() failed \n");
      return 1;
   }

   fontwidth = pango_font_metrics_get_approximate_char_width(pangofontmetrics);
   fontheight = pango_font_metrics_get_ascent(pangofontmetrics)
              + pango_font_metrics_get_descent(pangofontmetrics);
   fontwidth /= PANGO_SCALE;
   fontheight /= PANGO_SCALE;

   return 0;
}


//  Find a locale-dependent installation file or user file.
//    file type: doc, data, locale, user
//    file name: README, changelog, userguide.html, parameters, translate.po ...
//  Returns complete file name, e.g. /usr/share/appname/locales/translate-de.po
//  Output filespec should be 200 bytes (limit for all installation files).
//  Returns 0 if OK, +N if not found.

int locale_filespec(cchar *filetype, cchar *filename, char *filespec)
{
   char     *pp, fname[20], fext[8];
   char     lc_RC[8];                                                            // -lc or -lc_RC
   int      cc, err;
   STATB    statb;

   filespec[0] = '/';
   strcat(filespec,filetype);                                                    //  leave /type as default

   if (strmatch(filetype,"doc")) strcpy(filespec,zdocdir);                       //  /usr/share/doc/appname
   if (strmatch(filetype,"data")) strcpy(filespec,zdatadir);                     //  /usr/share/appname/data
   if (strmatch(filetype,"locale")) strcpy(filespec,zlocalesdir);                //  /usr/share/appname/locales
   if (strmatch(filetype,"user")) strcpy(filespec,zhomedir);                     //  /home/<user>/.appname

   strncpy0(fname,filename,20);
   pp = strchr(fname,'.');
   if (pp) {
      strcpy(fext,pp);                                                           //  file type  .fext
      *pp = 0;
   }
   else *fext = 0;                                                               //  no type

   lc_RC[0] = '-';
   strncpy0(lc_RC+1,zlocale,6);                                                  //  locale with region code: -lc_RC

tryextras:

   cc = strlen(filespec);
   filespec[cc] = '/';                                                           //  /folders.../
   strcpy(filespec+cc+1,fname);                                                  //  /folders.../fname
   cc = strlen(filespec);                                                        //                   |
   pp = filespec + cc;                                                           //                   pp

   strcpy(pp,lc_RC);                                                             //  /folders.../fname-lc_RC.fext
   strcat(pp,fext);
   err = stat(filespec,&statb);
   if (! err) return 0;

   strcpy(pp+3,fext);                                                            //  /folders.../fname-lc.fext
   err = stat(filespec,&statb);
   if (! err) return 0;

   strcpy(pp,"-en");                                                             //  /folders.../fname-en.fext
   strcat(pp,fext);
   err = stat(filespec,&statb);
   if (! err) return 0;

   strcpy(pp,fext);                                                              //  /folders.../fname.fext
   err = stat(filespec,&statb);
   if (! err) return 0;

   if (strmatch(filetype,"doc")) {                                               //  these files may be placed in
      strcpy(filespec,zdocdir);                                                  //    /usr/share/doc/appname/extras
      strcat(filespec,"/extras");                                                //       due to Linux chaos
      filetype = "";
      goto tryextras;                                                            //  try again using /extras
   }

   return 1;                                                                     //  not found
}


/********************************************************************************/

//  display application log file in a popup window
//  The log file is /home/<user>/.appname/logfile

void showz_logfile(GtkWidget *parent)
{
   char buff[200];

   fflush(0); 
   snprintf(buff,199,"cat %s/logfile",zhomedir);
   popup_command(buff,800,600,parent);
   return;
}


//  find and show a text file in /usr/share/doc/appname/
//                            or /usr/share/appname/data
//  the text file may also be a compressed .gz file
//  type is "doc" or "data"

void showz_textfile(const char *type, const char *file, GtkWidget *parent)
{
   char     filex[40], filespec[200], command[200];
   int      err;

   strncpy0(filex,file,36);                                                      //  look for gzip file first
   strcat(filex,".gz");

   err = locale_filespec(type,filex,filespec);
   if (! err) {
      snprintf(command,200,"zcat %s",filespec);
      popup_command(command,700,500,parent,1);
      return;
   }

   strncpy0(filex,file,36);                                                      //  look for uncompressed file

   err = locale_filespec(type,filex,filespec);
   if (! err) {
      snprintf(command,200,"cat %s",filespec);
      popup_command(command,700,500,parent,1);
      return;
   }

   zmessageACK(0,"file not found: %s %s",type,file);
   return;
}


//  show a local or remote html file using the user's preferred browser
//  to show a local file starting at an internal live link location:
//    url = "file://folder/.../filename#livelink

void showz_html(cchar *url)
{
   static char    prog[40];
   static int     ftf = 1, err;

   if (ftf) {
      ftf = 0;
      *prog = 0;
      err = Qshell(0,0,"which firefox");                                         //  use xdg-open only as last resort
      if (! err) strcpy(prog,"firefox"); 
      else {
         err = Qshell(0,0,"which chromium-browser");
         if (! err) strcpy(prog,"chromium-browser --new-window");
         else {
            err = Qshell(0,0,"which xdg-open");
            if (! err) strcpy(prog,"xdg-open");
         }
      }
   }

   if (! *prog) {
      zmessageACK(null,"html file reader not found");
      return;
   }

   Qshell(1,1,"%s %s &",prog,url);
   return;
}


/********************************************************************************

   void showz_docfile(GtkWidget *parent, char *docfile, char *topic)
   
   Show docfile in popup scrolling text window with 'topic' at the top.
   docfile is located in data folder: get_zdatadir()
   images are located in image folder: get_zimagedir() 

   docfile format:

   plain text file, topics in col. 1, text indented.
   images in a separate line: +image: filename.jpg  +image: ...
   embedded links to topics: |topic name|
   underlined words: _underline  (underline to next blank) 

   TOPIC 1                                                              linkable topics in col. 0
      text text text text text text text text text text text text 
      text text text text text text text text text text text text 
      ...
   TOPIC 2  
      +image: file1.png  +image file2.jpg                               embedded images

      text text text text text text text text |TOPIC 1| text text       link to topic
      text text text text text text text text text text text text

      \_subtopic line                                                   underline total line
      text text text text text text text text text text text text
      text text text text text \_UNDERLINED TEXT\_ text text text       underline subtext     
      text text text text text \bBOLD TEXT\b text text text             bold subtext     
      text text text text text http*******  text text text              web link
      text text text text text text text text text text text text
      ...

*********************************************************************************/

namespace showz_docfile_names
{
   zdialog     *zd = 0;
   int         currline, lastline;
   GtkWidget   *popwidget;
   GtkWidget   *textwidget;
   char        *topicnames[1000];
   int         topiclines[1000];
   int         Ntopics = 0;
   int         backtab[10];
   int         maxback = 10, backpos = 0;
}


void showz_docfile(GtkWidget *parent, cchar *docfile, cchar *utopic)
{
   using namespace showz_docfile_names;

   void showz_docfile_clickfunc(GtkWidget *widget, int line, int pos, int kbkey);
   void validate_docfile(cchar *docfile);
   
   FILE        *fid;
   char        filespec[200], buff1[200], buff2[200];
   char        topic[50], image[100];
   char        *pp1, *pp2, *pp3;
   int         ii, cc, line, posn, remv1, remv2;
   int         Fmarkups;
   GdkPixbuf   *pixbuf;
   GError      *gerror;
   
   if (utopic && strmatch(utopic,"validate")) {                                  //  check for errors
      validate_docfile(docfile);
      return;
   }
   
   if (zd && zdialog_valid(zd,docfile))
      goto initz_done;
   
   for (ii = 0; ii < Ntopics; ii++) 
      zfree(topicnames[ii]);
   
   for (ii = 0; ii < maxback; ii++)
      backtab[ii] = -1;
   
   currline = lastline = 0;
   Ntopics = 0;

   snprintf(filespec,200,"%s/%s",get_zdatadir(),docfile);                        //  open docfile
   fid = fopen(filespec,"r");
   if (! fid) {
      zmessageACK(parent,"%s %s",filespec,strerror(errno));
      return;
   }

   zd = popup_report_open(docfile,parent,900,700,0,showz_docfile_clickfunc,      //  popup window for docfile text display
                                                 "<",">","hide","find",0);       //    buttons
   if (! zd) {
      fclose(fid);
      return;
   }
   
   zdialog_show(zd,0);
   
   for (line = 0; line < 99999; line++)                                          //  read docfile text lines
   {
      pp1 = fgets_trim(buff1,200,fid);                                           //  line without \n EOL
      if (! pp1) break;                                                          //  EOF
      if (strmatchN(pp1,"EOF",3)) break;                                         //  end of visible text

      pp1 = strstr(buff1,"+image:");
      if (pp1) {
         while (pp1) {
            popup_report_write(zd,0,"   ",0);                                    //  3 spaces
            pp2 = strstr(pp1+7,"+image:");                                       //  next image file
            if (pp2) *pp2 = 0;
            strncpy0(image,pp1+7,100);
            strTrim2(image);
            snprintf(filespec,200,"%s/%s",get_zimagedir(),image);                //  full filespec
            gerror = 0;
            pixbuf = gdk_pixbuf_new_from_file(filespec,&gerror);                 //  convert to pixbuf image
            if (pixbuf) {
               popup_report_insert_pixbuf(zd,line,pixbuf);                       //  write image to output line
               g_object_unref(pixbuf);
            }
            else printz("cannot load image file: %s \n",image);
            pp1 = pp2;
         }
         popup_report_write(zd,0,"\n",0);                                        //  write image line EOL
         continue;                                                               //  next line
      }

      if (buff1[0] > ' ')                                                        //  topic line
      {      
         popup_report_write(zd,1,"%s\n",buff1);                                  //  write topic line to output, bold

         strncpy0(topic,buff1,50);                                               //  add topic and line number to list
         strTrim(topic);
         topicnames[Ntopics] = zstrdup(topic);
         topiclines[Ntopics] = line;
         if (++Ntopics < 1000) continue;
         zexit("showz_docfile(): exceed 1000 topics");
      }

      strncpy0(buff2,buff1,200);                                                 //  not a topic line
      pp1 = buff2;                                                               //  copy line with embedded markups \*

      Fmarkups = 0;

      while ((pp1 = strchr(pp1,'\\')))                                           //  remove all "\_" and "\b"
      {
         if (pp1[1] == '_' || pp1[1] == 'b') {
            cc = strlen(pp1+2);
            memmove(pp1,pp1+2,cc+1);
            Fmarkups = 1;                                                        //  markups found in this line
         }
         else pp1++;
      }

      popup_report_write(zd,0,"%s\n",buff2);                                     //  write text line to output

      if (! Fmarkups) continue;                                                  //  no markups, next line
      
      //  buff1 = original line with \_ and \b markups
      //  buff2 = buff1 with markups removed

      pp1 = buff1;
      remv1 = 0;

      while (true) 
      {
         pp1 = strchr(pp1,'\\');                                                 //  find next markup "\*"
         if (! pp1) break;
 
         if (pp1[1] != '_') {                                                    //  not underline "\_"
            pp1 += 2;
            remv1 += 2;                                                          //  cc to remove for prior markup
            continue;
         }
         pp2 = strstr(pp1+2,"\\_");                                              //  underline end posn
         if (pp2) cc = pp2 - pp1 - 2;                                            //  length of underline
         else cc = strlen(pp1) - 2;                                              //  (with poss. embedded markups)

         remv2 = 0;
         pp3 = pp1 + 2;
         while (true) {                                                          //  find embedded markups "\*"
            pp3 = strchr(pp3,'\\');
            if (! pp3) break;
            if (pp3 >= pp1 + 2 + cc) break;
            remv2 += 2;                                                          //  reduce cc for embedded markups
            pp3 += 2;
         }

         posn = pp1 - buff1 - remv1;                                             //  underline posn
         cc = cc - remv2;                                                        //  underline cc
         popup_report_underline_word(zd,line,posn,cc);                           //  underline the text

         remv1 += 4;                                                             //  cc to remove for following markup
         if (! pp2) break;
         pp1 = pp2 + 2;                                                          //  search for next underline
         remv1 += remv2;
      }

      pp1 = buff1;                                                               //  repeat for bold markups "\b"
      remv1 = 0;

      while (true) 
      {
         pp1 = strchr(pp1,'\\');
         if (! pp1) break;
 
         if (pp1[1] != 'b') {
            pp1 += 2;
            remv1 += 2;
            continue;
         }
         pp2 = strstr(pp1+2,"\\b");
         if (pp2) cc = pp2 - pp1 - 2;
         else cc = strlen(pp1) - 2;

         remv2 = 0;
         pp3 = pp1 + 2;
         while (true) {
            pp3 = strchr(pp3,'\\');
            if (! pp3) break;
            if (pp3 >= pp1 + 2 + cc) break;
            remv2 += 2;
            pp3 += 2;
         }

         posn = pp1 - buff1 - remv1;
         cc = cc - remv2;
         popup_report_bold_word(zd,line,posn,cc);

         remv1 += 4;
         if (! pp2) break;
         pp1 = pp2 + 2;
         remv1 += remv2;
      }

      continue;                                                                  //  next line
   }

   lastline = line;
   fclose(fid);

   popwidget = zdialog_widget(zd,"dialog");                                      //  popup zdialog

initz_done:

   currline = 0;                                                                 //  docfile line for topic

   if (utopic)                                                                   //  initial topic from caller
   {  
      strncpy0(topic,utopic,50);
      cc = strlen(topic);

      for (ii = 0; ii < cc; ii++)                                                //  conv. topic to uppercase
         topic[ii] = toupper(topic[ii]);
     
      for (ii = 0; ii < Ntopics; ii++) {                                         //  search docfile topics
         if (strcmp(topic,topicnames[ii]) == 0) {
            currline = topiclines[ii];                                           //  get line of matching topic
            break;
         }
      }

      if (ii == Ntopics) 
         printz("topic not found: %s %s \n",utopic,topic);
   }
   
   popup_report_font_attributes(zd);                                             //  high contrast
   
   popup_report_scroll_top(zd,currline);                                         //  scroll to topic line
   textwidget = zdialog_widget(zd,"text");                                       //  text widget in zdialog
   zdialog_show(zd,1);

   return;
}


void showz_docfile_clickfunc(GtkWidget *textwidget, int line, int pos, int kbkey)
{
   using namespace showz_docfile_names;
   
   int      ii, jj, cc;
   int      top, bott, page;
   char     *text, *pp1, *pp2;
   char     topic[50], weblink[200];
   
   textwidget_get_visible_lines(textwidget,top,bott);                            //  range of lines on screen

   if (line >= 0 || kbkey == GDK_KEY_Return)                                     //  line clicked or ENTER key at line
   {
      text = textwidget_line(textwidget,line,1);                                 //  selected line
      if (text) 
      {
         pp2 = strchr(text + pos,'|');                                              
         if (pp2) {                                                              //  link to topic found
            for (ii = 2; ii < 50; ii++)
               if (pp2[-ii] == '|') break;                                       //  text ... |topic name| ...
            if (pp2[-ii] == '|') {                                               //            :         :
               pp1 = pp2 - ii + 1;                                               //            pp1       pp2
               cc = pp2 - pp1;
               strncpy0(topic,pp1,cc+1);
               for (ii = 0; ii < Ntopics; ii++) {
                  if (topiclines[ii] == line) continue;
                  if (strcmp(topic,topicnames[ii]) == 0) break;
               }
               if (ii < Ntopics) {                                               //  found linked topic
                  if (backpos == maxback-1) {
                     for (jj = 0; jj < maxback-1; jj++)                          //  backup table full, discard oldest
                        backtab[jj] = backtab[jj+1];
                     backpos--;
                  }
                  backtab[backpos] = top;                                        //  top line >> backtab
                  backpos++;                                                     //  advance backtap position
                  currline = topiclines[ii];                                     //  new topic top line
                  backtab[backpos] = currline;                                   //       >> backtab
                  textwidget_scroll_top(textwidget,currline);                    //  scroll to top of window
               }
            }
         }
         
         else {                                                                  //  look for web link
            for ( ; pos >= 0; pos--)
               if ( *(text+pos) == ' ') break;                                   //  click position, preceding blank
            if (pos < 0) return;
            pp1 = text + pos + 1;
            pp2 = strchr(pp1,' ');                                               //  following blank or EOL
            if (pp2) cc = pp2 - pp1;
            else cc = strlen(pp1);
            if (pp1[cc-1] == '.') cc--;                                          //  remove trailing period
            if (cc > 199) return;
            strncpy0(weblink,pp1,cc+1);                                          //  copy clicked text string
            if (strmatchN(pp1,"http",4)) showz_html(weblink);                    //  if "http..." assume a web link
         }
      }

      return;
   }

   if (kbkey >= 0xfd00)                                                          //  navigation key
   {
      textwidget_get_visible_lines(textwidget,top,bott);                         //  range of lines on screen
      page = bott - top - 2;                                                     //  page size, lines
      if (page < 0) page = 0;
      currline = 0;                                                              //  default
      if (kbkey == GDK_KEY_Up) currline = top - 1;                               //  handle some navigation keys
      else if (kbkey == GDK_KEY_Down) currline = bott + 1;
      else if (kbkey == GDK_KEY_Page_Up) currline = top - page;
      else if (kbkey == GDK_KEY_Page_Down) currline = bott + page;
      else if (kbkey == GDK_KEY_Home) currline = 0;
      else if (kbkey == GDK_KEY_End) currline = 999999;
      if (currline < 0) currline = 0;
      textwidget_scroll(textwidget,currline);                                    //  put line on screen
   }

   if (kbkey == GDK_KEY_Left || kbkey == '<') {                                  //  left arrow, go back
      backtab[backpos] = currline;
      if (backpos > 0) backpos--;
      currline = backtab[backpos];
      textwidget_scroll_top(textwidget,currline);                                //  scroll line to top of window
      return;
   }
   
   if (kbkey == GDK_KEY_Right || kbkey == '>') {                                 //  right arrow, go forward
      backtab[backpos] = currline;
      if (backpos < maxback-1 && backtab[backpos+1] >= 0)
         currline = backtab[++backpos];
      textwidget_scroll_top(textwidget,currline);                                //  scroll line to top of window
      return;
   }

   return;
}


//  validate the F1_help_topic links and the internal links in a docfile

void validate_docfile(cchar *docfile)
{
   char        *textlines[10000]; 
   char        *topicnames[1000];
   char        filespec[200], buff[200], image[100];
   char        topic[50];
   char        *pp1, *pp2, *pp3;
   FILE        *fid;
   int         Ntext, Ntopics, Nerrs;
   int         ii, cc, line;
   GdkPixbuf   *pixbuf;
   GError      *gerror;
   
   printz("validate docfile %s \n",docfile);
   
   Ntext = Ntopics = Nerrs = 0;

   snprintf(filespec,200,"%s/%s",get_zdatadir(),docfile);                        //  open docfile
   fid = fopen(filespec,"r");
   if (! fid) {
      printz("%s %s",filespec,strerror(errno));
      return;
   }

   for (line = 0; line < 10000; line++)                                          //  read docfile text lines
   {
      pp1 = fgets_trim(buff,200,fid);                                            //  line without \n EOL
      if (! pp1) break;                                                          //  EOF

      textlines[Ntext] = zstrdup(pp1);                                           //  copy text line to memory
      if (++Ntext == 10000) zexit("exceed 10000 text recs");

      if (pp1[0] <= ' ') continue;                                               //  not a topic line

      strncpy0(topic,pp1,50);                                                    //  add topic and line number to list
      strTrim(topic);
      topicnames[Ntopics] = zstrdup(topic);
      if (++Ntopics == 1000) zexit("exceed 1000 topics");
   }

   fclose(fid);

   for (line = 0; line < Ntext; line++)                                          //  process text lines
   {
      pp1 = textlines[line]; 
      
      pp2 = strstr(pp1,"+image:");
      if (pp2)                                                                   //  line contains images
      {
         while (pp2) 
         {
            pp3 = strstr(pp2+7,"+image:");                                       //  next image file
            if (pp3) *pp3 = 0;
            strncpy0(image,pp2+7,100);
            strTrim2(image);
            snprintf(filespec,200,"%s/%s",get_zimagedir(),image);                //  full filespec
            gerror = 0;
            pixbuf = gdk_pixbuf_new_from_file(filespec,&gerror);                 //  convert to pixbuf image
            if (pixbuf) g_object_unref(pixbuf);
            else {
               printz("cannot load image file: %s \n",image);
               Nerrs++;
            }
            pp2 = pp3;
         }

         continue;                                                               //  next line
      }

      if (pp1[0] <= ' ')                                                         //  not a topic line
      {
         pp1 = strchr(pp1,'|');                                                  //  get topic links in line

         while (pp1)
         {
            pp2 = strchr(pp1+1,'|');                                             //  ... |topic name| ...
            if (! pp2) break;                                                    //       :         :
            pp1++;                                                               //       pp1       pp2
            cc = pp2 - pp1;
            if (cc > 50) {
               printf("bad topic, line %d: %s \n",line,pp1);                     //  topic name > 50 char.
               Nerrs++;
               break;
            }
            
            strncpy0(topic,pp1,cc+1);
            for (ii = 0; ii < Ntopics; ii++)
               if (strcmp(topic,topicnames[ii]) == 0) break;
            if (ii == Ntopics) {                                                 //  topic not found
               printf("bad topic, line %d: %s \n",line,topic);
               Nerrs++;
            }
            pp1 = strchr(pp2+1,'|'); 
         }

         continue;                                                               //  next line
      }
   }
   
   printz(" %d errors \n",Nerrs);
   
   for (ii = 0; ii < Ntext; ii++)                                                //  free memory
      zfree(textlines[ii]);

   for (ii = 0; ii < Ntopics; ii++)
      zfree(topicnames[ii]);
   
   return;
}


/********************************************************************************/

/***

   Called at each application startup.
   Update appruns file if >1 day since last update.
   Also do phone_home() if user allows this. 

   appruns file:  Uxxxxxxxx Rxxx Px
                   |         |    |___ phone home permission: 0/1 = no/yes
                   |         |________ appruns file update counter
                   |__________________ unique ID, random 8-digit hex number

***/


void appruns_update()
{
   FILE     *fid;
   char     apprunsfile[200], uuid[12];
   int      nn, runs, err, permit;
   STATB    statb;
   time_t   mtime, today, fage = 999;
   uint64   random;

   snprintf(apprunsfile,200,"%s/appruns",get_zhomedir());                        //  /home/<user>/.appname/appruns

   err = stat(apprunsfile,&statb);                                               //  look for appruns file
   if (! err) {
      mtime = statb.st_mtime;                                                    //  file mod date/time
      today = time(0);
      fage = (today - mtime) / 3600;                                             //  file age in hours
   }
   
   fid = fopen(apprunsfile,"r");                                                 //  open appruns file
   if (fid) {
      nn = fscanf(fid,"U%8s R%d P%d",uuid,&runs,&permit);                        //  read "Uxxxxxxxx Rxxx Px"
      fclose(fid);
      if (nn != 3) fid = 0;                                                      //  failed
   }
   
   if (fid && fage < 24) return;                                                 //  < 1 day since last update

   if (! fid) {                                                                  //  appruns not found or invalid
      random = 0x100000000 * drandz();                                           //  uuid: random 8-digit hex number
      snprintf(uuid,12,"%08llx",random);                                         //  (range 4.2 billion)
      runs = 0;                                                                  //  run count
      permit = 1;                                                                //  permit phone home
   }

   runs += 1;                                                                    //  increment run count

   fid = fopen(apprunsfile,"w");                                                 //  update appruns file
   if (! fid) return;
   fprintf(fid,"U%8s R%d P%d",uuid,runs,permit);
   fclose(fid);
   
   if (! permit) return;                                                         //  phone home blocked
   if (runs == 1) return;                                                        //  not if first run
   phone_home();                                                                 //  do phone home
   return;
}


/********************************************************************************/

//  Send a log message to web host for application usage statistics. 
//  No data tracable to a person, computer, or location is sent. 
//  The IP address can be traced by a state actor (like any internet access). 

void phone_home()
{
   FILE     *fid;
   char     apprunsfile[200], uuid[12];
   int      nn, runs, permit;
   char     buff1[140], buff2[100];

   cchar *wget1 = "wget -b -q -O /dev/null %s 1>/dev/null";                      //  stuff wget2
   cchar *wget2 = "https://kornelix.net/phone_home/%s/%s/";                      //  stuff appname, uuid
   
   snprintf(apprunsfile,200,"%s/appruns",get_zhomedir());
   fid = fopen(apprunsfile,"r");                                                 //  open appruns file
   if (! fid) return;
   nn = fscanf(fid,"U%8s R%d P%d",uuid,&runs,&permit);                           //  read "Uxxxxxxxx Rxxx Px"
   fclose(fid);
   if (nn != 3) return;                                                          //  failed

   snprintf(buff2,100,wget2,zappvers,uuid);                                      //  build message to web host
   snprintf(buff1,140,wget1,buff2);
   Qshell(0,0,buff1);                                                            //  send

   return;
}


/********************************************************************************/

//  phone home dialog
//  Display basic information for user decision to allow or block phone home.
//  Application: provide "phone home" menu or button which calls this function.
//  If user allows, phone_home() will be called each Ith run.

void phone_home_allow(GtkWidget *parent)
{
   FILE     *fid;
   char     apprunsfile[200], uuid[12];
   int      nn, runs, permit;
   uint64   random;
   int      zstat;
   cchar    *infomess = E2X("If you permit, a message is occasionally \n"
                            "sent to the web host for usage statistics. \n"
                            "Nothing is retained that can be associated \n"
                            "with a person or computer or location.");

   zdialog *zd = zdialog_new(E2X("Phone Home"),parent,"OK","NO",0);              //  query user
   zdialog_add_widget(zd,"hbox","hbinfo","dialog",0,"space=10");
   zdialog_add_widget(zd,"label","labinfo","hbinfo",infomess,"space=5");
   if (parent) zdialog_run(zd,0,"parent");
   else zdialog_run(zd,0,"mouse");
   zstat = zdialog_wait(zd);                                                     //  1/2 = OK/NO from user
   zdialog_destroy(zd);

   snprintf(apprunsfile,200,"%s/appruns",get_zhomedir());
   fid = fopen(apprunsfile,"r");                                                 //  open appruns file
   if (fid) {
      nn = fscanf(fid,"U%8s R%d P%d",uuid,&runs,&permit);                        //  read "Uxxxxxxxx Rxxx Px"
      fclose(fid);
      if (nn != 3) fid = 0;                                                      //  failed
   }
   
   if (! fid) {                                                                  //  appruns not found or invalid
      random = 0x100000000 * drandz();                                           //  random 8-digit hex number
      snprintf(uuid,12,"%08llx",random);                                         //  (range 4.2 billion)
      runs = 0;                                                                  //  run count
      permit = 1;                                                                //  permit phone home
   }
   
   if (zstat == 2) permit = 0;
   else permit = 1;

   fid = fopen(apprunsfile,"w");                                                 //  update appruns file
   if (! fid) return;
   fprintf(fid,"U%8s R%d P%d",uuid,runs,permit);
   fclose(fid);
   
   if (permit) phone_home();                                                     //  phone home if allowed
   return;
}


/********************************************************************************

   Translation Functions

   Translation files are standard .po files as used in the Gnu gettext
   system. However the .po files are used directly, and there is no need
   to merge and compile them into a binary format (.mo file).

   A translation file is one of: <zlocalesdir>/translate-lc.po  or  *-lc_RC.po
   where "lc" is a standard language code and "lc_RC" a language and region code.
   The file may also be compressed with the file type .po.gz

   Translation files contain two record types:
      msgid "english text"
      msgstr "translation text"

   The text strings may continue on multiple lines, each such segment enclosed in quotes.
   The strings may contain C-format codes (%s %d etc.) and may contain escaped special
   characters (\n etc.).

   A text string may have a context part "context::string", where "context" is any short
   string, "::" is a separator, and "string" is the string to translate or the translation
   of a string. This is to handle the case where a single English string may need multiple
   translations, depending on context. The English string may be present multiple times in
   a .po file, each one marked with a different context and having a different translation.
   The context part is optional in the translation strings and is not displayed in the GUI.

   Initialize translations:  int E2Xinit(cchar *locale, int Fdump)
      locale is "lc" or "lc_RC" or null (current locale will be used)

   Initializes the running application for the translation of text messages. It reads the
   translation file for the user's locale and builds a translation table for use by E2X().

   locale:  2-character language code 'lc' ("de" "fr" "es" etc.)
            or 5-character language and region code 'lc_RC' ("de_AT" etc.)
            or null to use the current locale

   Fdump:   flag, dump missing translations to log file

   Status returned: 0 = OK, 1 = unable to find or process translation file.

   Translate a text string:  cchar *translation = E2X(cchar *english)
      english: text string which may have printf formats (%d %s ...)
      translation: the returned equivalent translation

   If the user language is English or if no translation is found,
   the input string is returned, else the translated string.

   example:

      program code:

         printf(E2X("answer: %d %s \n next line"), 123, "qwerty");

      A German .po file (translate-de.po) would have the following:

         msgid ""
         "answer: %d %s \n"
         " next line"

         msgstr ""
         "Antwort: %d %s \n"
         " nächste Zeile"

*********************************************************************************/

namespace E2Xnames
{
   FILE        *fidr;
   char        buff[E2Xmaxcc], *ppq1, *ppq2;
   char        *porec, *wporec;
   char        Etext[E2Xmaxcc], Ttext[E2Xmaxcc];                                 //  .po text: "line 1 %s \n" "line 2"
   char        **etext, **ttext;                                                 //  arrays, english and translations
   char        **estring, **tstring;                                             //  merged, un-quoted, un-escaped
   int         Ntext = 0;                                                        //  array counts
   void        E2Xgettext(char *text);
   char        *E2Xmergetext(cchar *text);
}


//  read and process .po file at application startup
//  prepare english strings and translations for quick access

void E2Xinit(cchar *locale, int Fdump)                                           //  initialize translations
{
   using namespace E2Xnames;

   int      ii, err;
   char     installpo[200], localpo[200];
   char     *usepo, *pp;
   double   itime, ltime;
   STATB    statb;

   if (Ntext) {                                                                  //  free prior translation
      for (ii = 0; ii < Ntext; ii++) {
         zfree(etext[ii]);
         zfree(ttext[ii]);
         zfree(estring[ii]);
         zfree(tstring[ii]);
      }
      zfree(etext);
      zfree(ttext);
      zfree(estring);
      zfree(tstring);
      Ntext = 0;
   }

   etext = (char **) zmalloc(E2Xmaxent * sizeof(char *));                        //  english text and translations
   ttext = (char **) zmalloc(E2Xmaxent * sizeof(char *));                        //  (segmented, quoted, escaped)
   estring = (char **) zmalloc(E2Xmaxent * sizeof(char *));                      //  english strings and translations
   tstring = (char **) zmalloc(E2Xmaxent * sizeof(char *));                      //  (merged, un-quoted, un-escaped)

   if (locale && *locale) strncpy0(zlocale,locale,6);                            //  use language from caller
   else {                                                                        //  help Linux chaos
      pp = getenv("LANG");                                                       //  use $LANG if defined
      if (! pp) pp = getenv("LANGUAGE");                                         //  use $LANGUAGE if defined
      if (! pp) pp = setlocale(LC_MESSAGES,"");                                  //  use locale if defined
      if (pp) strncpy0(zlocale,pp,6);                                            //  "lc_RC" language/region code
      else strcpy(zlocale,"en");                                                 //  use English
   }

   if (*zlocale < 'a') strcpy(zlocale,"en");                                     //  use English if garbage
   printz("language: %s \n",zlocale);

   if (strmatchN(zlocale,"en",2)) return;                                        //  English, do nothing

   *installpo = *localpo = 0;                                                    //  no installed or local .po file
   usepo = 0;

   err = locale_filespec("locale","translate.po",installpo);                     //  look for installed .po file
   if (err) err = locale_filespec("locale","translate.po.gz",installpo);         //  may be compressed (debian)
   if (! err) usepo = installpo;

   err = locale_filespec("user","translate.po",localpo);                         //  look for local .po file
   if (! err && *installpo) {                                                    //  both are present
      stat(installpo,&statb);                                                    //  compare mod date/time for
      itime = statb.st_mtime;                                                    //    installed and local .po files
      stat(localpo,&statb);
      ltime = statb.st_mtime;
      if (ltime >= itime) usepo = localpo;                                       //  use local .po if newer
   }
   
   pp = strstr(usepo,".gz");                                                     //  uncompress if needed
   if (pp) {
      printz("uncompress installed .po file \n");
      err = Qshell(1,1,"gunzip -c %s > %s",installpo,localpo);                   //  /usr/share/fotoxx/locale/translate-xx.po.gz
      usepo = localpo;                                                           //    >>  <fotoxx home>/translate-xx.po
   }
   
   if (usepo) printz("using translation file: %s \n",usepo);
   else return;

   fidr = fopen(usepo,"r");                                                      //  open .po file
   if (! fidr) {
      printz("*** cannot open .po file: %s \n",usepo);
      return;
   }

   porec = 0;                                                                    //  no .po record yet
   *Etext = *Ttext = 0;                                                          //  no text yet

   while (true)
   {
      if (! porec) porec = fgets_trim(buff,E2Xmaxcc,fidr);                       //  get next .po record
      if (! porec) break;                                                        //  EOF

      if (blank_null(porec)) {                                                   //  blank record
         porec = 0;
         continue;
      }
      if (*porec == '#') {                                                       //  comment
         porec = 0;
         continue;
      }

      if (strmatchN(porec,"msgid",5))                                            //  start new english string
      {
         if (*Etext) {                                                           //  two in a row
            printz("no translation: %s \n",Etext);
            *Etext = 0;
         }

         if (*Ttext) {
            printz("orphan translation: %s \n",Ttext);
            *Ttext = 0;
         }

         porec += 5;                                                             //  get segmented text string
         E2Xgettext(Etext);                                                      //  "segment1 %s \n" "segment2" ...
      }

      else if (strmatchN(porec,"msgstr",6))                                      //  start new translation
      {
         porec += 6;                                                             //  get segmented string
         E2Xgettext(Ttext);

         if (! *Etext) {
            printz("orphan translation: %s \n",Ttext);
            *Ttext = 0;
            continue;
         }

         if (strlen(Ttext) < 3)                                                  //  translation is "" (quotes included)
            if (Fdump) printz("no translation: %s \n",Etext);
      }

      else
      {
         printz("unrecognized .po record: %s \n",porec);
         porec = 0;
         continue;
      }

      if (*Etext && *Ttext)                                                      //  have an english/translation pair
      {
         etext[Ntext] = zstrdup(Etext);                                          //  add to translation tables
         ttext[Ntext] = zstrdup(Ttext);
         *Etext = *Ttext = 0;
         Ntext++;
         if (Ntext == E2Xmaxent)                                                 //  cannot continue
            zappcrash("more than %d translations",E2Xmaxent);
      }
   }

   fclose(fidr);

   printz(".po file has %d entries \n",Ntext);

   for (ii = 0; ii < Ntext; ii++)
   {
      pp = E2Xmergetext(etext[ii]);                                              //  merge segmented text strings
      estring[ii] = zstrdup(pp);
      pp = E2Xmergetext(ttext[ii]);
      tstring[ii] = zstrdup(pp);
   }

   HeapSort(estring, tstring, Ntext);                                            //  sort both strings, english order
   return;
}


//  private function
//  read and combine multiple 'msgid' or 'msgstr' quoted strings
//  output is one string with one or more quoted segments:
//    "text line 1 %s \n" "text line 2" ...
//  each segment comes from a different line in the input .po file

void E2Xnames::E2Xgettext(char *pstring)
{
   using namespace E2Xnames;

   int      cc, scc = 0;

   while (true)                                                                  //  join multiple quoted strings
   {
      while (*porec && *porec != '"') porec++;                                   //  find opening string quote
      if (! *porec) {
         porec = fgets_trim(buff,E2Xmaxcc,fidr);                                 //  get next .po record
         if (! porec) return;
         if (strmatchN(porec,"msgid",5)) return;                                 //  end of this string
         if (strmatchN(porec,"msgstr",6)) return;
      }
      ppq1 = porec;                                                              //  opening quote
      ppq2 = ppq1 + 1;
      while ((*ppq2 && *ppq2 != '"') ||                                          //  find closing (non-escaped) quote
             (*ppq2 == '"' && *(ppq2-1) == '\\')) ppq2++;
      if (! *ppq2) return;
      cc = ppq2 - ppq1 + 1;                                                      //  min. case is ""
      if (cc + 1 + scc >= E2Xmaxcc)
         printz("*** string is too long %s \n",pstring);
      else {
         strncpy0(pstring+scc,ppq1,cc+1);                                        //  accum. substrings, minus quotes
         scc += cc;
      }
      porec = ppq2 + 1;
   }

   return;
}


//  private function
//  convert quoted string segments into binary form that
//  matches the compiled string in the source program
//  (remove quotes, merge strings, un-escape \n \" etc.)

char * E2Xnames::E2Xmergetext(cchar *dirtystring)
{
   static char    cleanstring[E2Xmaxcc];
   int            ii, jj;

   strncpy0(cleanstring,dirtystring,E2Xmaxcc);
   clean_escapes(cleanstring);

   for (ii = jj = 0; cleanstring[ii]; ii++)
      if (cleanstring[ii] != '"')
         cleanstring[jj++] = cleanstring[ii];
   cleanstring[jj] = 0;
   return cleanstring;
}


//  Translate the input english string or return the input string.
//  Look for "context::string" and return "string" only if context found.

cchar * E2X(cchar *english)
{
   using namespace E2Xnames;

   cchar    *pp, *pp2;
   int      ii;

   if (! Ntext) return english;                                                  //  no translations

   ii = bsearch(english,(cchar **) estring,0,Ntext);                             //  binary search, < 1 microsec.
   if (ii < 0) pp = english;
   else pp = tstring[ii];

   if (strlen(pp) == 0) pp = english;                                            //  translation is ""

   for (pp2 = pp; *pp2 && pp2 < pp+30; pp2++)                                    //  remove context if present
      if (*pp2 == ':' && *(pp2+1) == ':') return pp2+2;

   return pp;
}


//  Find all untranslated strings and return them one per call.
//  Set ftf = 1 for first call, will be returned = 0.
//  Returns null after last untranslated string.

cchar * E2X_missing(int &ftf)
{
   using namespace E2Xnames;

   int         ii;
   static int  next;

   if (ftf) ftf = next = 0;

   for (ii = next; ii < Ntext; ii++)
      if (strlen(tstring[ii]) == 0) break;                                       //  translation is ""

   next = ii + 1;
   if (ii < Ntext) return estring[ii];                                           //  return english

   return 0;                                                                     //  EOL
}


/********************************************************************************
   GTK utility functions
********************************************************************************/

//  Iterate main loop every "skip" calls.
//  If called within the main() thread, does a GTK main loop to process menu events, etc.
//  You must do this periodically within long-running main() thread tasks if you wish to
//  keep menus, buttons, output windows, etc. alive and working. The skip argument will
//  cause the function to do nothing for skip calls, then perform the normal function.
//  This allows it to be embedded in loops with little execution time penalty.
//  If skip = 100, zmainloop() will do nothing for 100 calls, execute normally, etc.
//  If called from a thread, zmainloop() does nothing.

void zmainloop(int skip)
{
   static int  xskip = 0;

   if (skip) {
      if (++xskip < skip) return;
      xskip = 0;
   }

   if (! pthread_equal(pthread_self(),zfuncs::tid_main)) return;                 //  thread caller, do nothing

   while (gtk_events_pending())                                                  //  gdk_flush() removed
      gtk_main_iteration_do(0);                                                  //  use gtk_main_iteration_do 

   return;
}


//  Iterate the main loop and sleep for designated time

void zmainsleep(float secs)
{
   while (secs > 0) {
      zmainloop();
      zsleep(0.001);
      secs = secs - 0.001;
   }
   
   return;
}


/********************************************************************************/

//  cairo drawing context for GDK window       GTK 3.21 version 

#if GTK_CHECK_VERSION(3,22,0)

cairo_t * draw_context_create(GdkWindow *gdkwin, draw_context_t &context)
{
   if (context.dcr) zappcrash("draw_context_create(): nested call");
   context.win = gdkwin;
   context.rect.x = 0;
   context.rect.y = 0;
   context.rect.width = gdk_window_get_width(gdkwin);
   context.rect.height = gdk_window_get_height(gdkwin);
   context.reg = cairo_region_create_rectangle(&context.rect);
   context.ctx = gdk_window_begin_draw_frame(gdkwin,context.reg);
   context.dcr = gdk_drawing_context_get_cairo_context(context.ctx);
   return context.dcr;
}

void draw_context_destroy(draw_context_t &context)
{
   if (! context.dcr) zappcrash("draw_context_destroy(): not created");
   gdk_window_end_draw_frame(context.win,context.ctx);
   cairo_region_destroy(context.reg);
   /*  cairo_destroy(context.dcr);              this is fatal  */
   context.dcr = 0;
   return;
}

#else

cairo_t * draw_context_create(GdkWindow *gdkwin, draw_context_t &context)
{
   if (context.dcr) zappcrash("draw_context_create(): nested call");
   context.dcr = gdk_cairo_create(gdkwin);
   if (! context.dcr) 
      zappcrash("draw_context_create(): gdk_cairo_create() failed");
   return context.dcr;
}

void draw_context_destroy(draw_context_t &context)
{
   if (! context.dcr) zappcrash("draw_context_destroy(): not created");
   if (context.dcr) cairo_destroy(context.dcr);
   context.dcr = 0;
   return;
}

#endif


/********************************************************************************/

//  textwidget functions
//  --------------------
//
//  High-level use of GtkTextView widget for text reports, line editing, text selection
//  In functions below, textwidget = zdialog_widget(zd,"widgetname"),
//    where "widgetname" is a zdialog "text" widget type.
//  All line numbers and line positions are zero based.


//  clear the text widget to blank

void textwidget_clear(GtkWidget *textwidget)
{
   GtkTextBuffer  *textBuff;
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;
   gtk_text_buffer_set_text(textBuff,"",-1);
   return;
}


//  clear the text widget from given line to end

void textwidget_clear(GtkWidget *textwidget, int line)
{
   GtkTextBuffer           *textBuff;
   GtkTextIter             iter1, iter2;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;
   gtk_text_buffer_get_iter_at_line(textBuff,&iter1,line);                       //  iter at line start
   gtk_text_buffer_get_end_iter(textBuff,&iter2);
   gtk_text_buffer_delete(textBuff,&iter1,&iter2);                               //  delete existing line and rest of buffer
   return;
}


//  get the current line count

int textwidget_linecount(GtkWidget *textwidget)
{
   GtkTextBuffer  *textBuff;
   int            nlines;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return 0;
   nlines = gtk_text_buffer_get_line_count(textBuff);
   return nlines;
}


//  append a new line of text to the end of existing text lines
//  line should normally include trailing \n
//  if existing last line is without trailing \n, text is appended to this line

void textwidget_append(GtkWidget *textwidget, int bold, cchar *format, ...)
{
   va_list        arglist;
   char           textline[2000];
   GtkTextBuffer  *textBuff;
   GtkTextIter    enditer;
   GtkTextTag     *fontag = 0;
   cchar          *normfont = zfuncs::appmonofont;
   cchar          *boldfont = zfuncs::appmonoboldfont;

   va_start(arglist,format);
   vsnprintf(textline,1999,format,arglist);
   va_end(arglist);

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   gtk_text_buffer_get_end_iter(textBuff,&enditer);                              //  end of text

   if (bold) fontag = gtk_text_buffer_create_tag(textBuff,0,"font",boldfont,0);  //  prepare bold/norm tag
   else fontag = gtk_text_buffer_create_tag(textBuff,0,"font",normfont,0);
   gtk_text_buffer_insert_with_tags(textBuff,&enditer,textline,-1,fontag,null);  //  insert line

   zmainloop();
   return;
}


//  same as above, with scroll to last line added (slower) 

void textwidget_append2(GtkWidget *textwidget, int bold, cchar *format, ...)
{
   va_list        arglist;
   char           textline[2000];
   GtkTextBuffer  *textBuff;
   GtkTextIter    enditer;
   GtkTextTag     *fontag = 0;
   cchar          *normfont = zfuncs::appmonofont;
   cchar          *boldfont = zfuncs::appmonoboldfont;
   GtkAdjustment  *vadjust;
   double         upperlimit;

   va_start(arglist,format);
   vsnprintf(textline,1999,format,arglist);
   va_end(arglist);

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   gtk_text_buffer_get_end_iter(textBuff,&enditer);                              //  end of text

   if (bold) fontag = gtk_text_buffer_create_tag(textBuff,0,"font",boldfont,0);  //  prepare bold/norm tag
   else fontag = gtk_text_buffer_create_tag(textBuff,0,"font",normfont,0);
   gtk_text_buffer_insert_with_tags(textBuff,&enditer,textline,-1,fontag,null);  //  insert line

   vadjust = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(textwidget));
   upperlimit = gtk_adjustment_get_upper(vadjust);                               //  does not work within a zdialog FIXME
   gtk_adjustment_set_value(vadjust,upperlimit);

   zmainloop();
   return;
}


//  insert a new line of text after designated line
//  use line -1 to insert before line 0
//  line should normally include trailing \n

void textwidget_insert(GtkWidget *textwidget, int bold, int line, cchar *format, ...)
{
   va_list        arglist;
   char           textline[2000];
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter;
   int            nlines;
   GtkTextTag     *fontag = 0;
   cchar          *normfont = zfuncs::appmonofont;
   cchar          *boldfont = zfuncs::appmonoboldfont;

   va_start(arglist,format);
   vsnprintf(textline,1999,format,arglist);
   va_end(arglist);
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;
   
   if (line < 0) gtk_text_buffer_get_start_iter(textBuff,&iter);                 //  insert before line 0

   if (line >= 0) {
      nlines = gtk_text_buffer_get_line_count(textBuff);                         //  insert after line
      if (line < nlines - 1)
         gtk_text_buffer_get_iter_at_line(textBuff,&iter,line+1);                //  start of next line
      else gtk_text_buffer_get_end_iter(textBuff,&iter);                         //  or end of text
   }

   if (bold) fontag = gtk_text_buffer_create_tag(textBuff,0,"font",boldfont,0);  //  prepare bold/norm tag
   else fontag = gtk_text_buffer_create_tag(textBuff,0,"font",normfont,0);
   gtk_text_buffer_insert_with_tags(textBuff,&iter,textline,-1,fontag,null);     //  insert line

   zmainloop();
   return;
}


//  replace a given line with a new line
//  line = -1: replace last line.  -2: replace last-1 line, etc.
//  new line should normally include trailing \n

void textwidget_replace(GtkWidget *textwidget, int bold, int line, cchar *format, ...)
{
   va_list        arglist;
   char           textline[2000];
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   int            nlines;
   GtkTextTag     *fontag = 0;
   cchar          *normfont = zfuncs::appmonofont;
   cchar          *boldfont = zfuncs::appmonoboldfont;

   va_start(arglist,format);
   vsnprintf(textline,1999,format,arglist);
   va_end(arglist);

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   nlines = gtk_text_buffer_get_line_count(textBuff);                            //  lines now in buffer
   if (line < 0) line = nlines + line - 1;
   if (line >= nlines) line = nlines - 1;

   gtk_text_buffer_get_iter_at_line(textBuff,&iter1,line);                       //  line start
   iter2 = iter1;
   gtk_text_iter_forward_line(&iter2);                                           //  end
   gtk_text_buffer_delete(textBuff,&iter1,&iter2);                               //  delete line
   gtk_text_buffer_get_iter_at_line(textBuff,&iter1,line);

   if (bold) fontag = gtk_text_buffer_create_tag(textBuff,0,"font",boldfont,0);  //  prepare bold/norm tag
   else fontag = gtk_text_buffer_create_tag(textBuff,0,"font",normfont,0);
   gtk_text_buffer_insert_with_tags(textBuff,&iter1,textline,-1,fontag,null);    //  insert line

   zmainloop();
   return;
}


//  delete a given line including the trailing \n

void textwidget_delete(GtkWidget *textwidget, int line)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   int            nlines;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   nlines = gtk_text_buffer_get_line_count(textBuff);                            //  lines now in buffer
   if (line < 0 || line >= nlines) return;

   gtk_text_buffer_get_iter_at_line(textBuff,&iter1,line);                       //  line start
   iter2 = iter1;
   gtk_text_iter_forward_line(&iter2);                                           //  end
   gtk_text_buffer_delete(textBuff,&iter1,&iter2);                               //  delete line

   zmainloop();
   return;
}


//  find first line of text containing characters matching input string
//  search is from line1 to end, then from 0 to line1-1
//  returns first matching line or -1 if none
//  comparison is not case sensitive

int textwidget_find(GtkWidget *textwidget, char *matchtext, int line1)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   int            line, nlines, cc;
   char           *textline = 0, *pp1, *pp2;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return -1;
   
   nlines = gtk_text_buffer_get_line_count(textBuff);                            //  lines now in buffer
   if (! nlines) return -1;
   
   if (line1 < 0) line1 = 0;                                                     //  starting line to search
   if (line1 >= nlines) line1 = 0;
   line = line1;

   while (true)
   {
      gtk_text_buffer_get_iter_at_line(textBuff,&iter1,line);                    //  line start
      iter2 = iter1;
      gtk_text_iter_forward_line(&iter2);                                        //  end
      textline = gtk_text_buffer_get_text(textBuff,&iter1,&iter2,0);             //  get text
      if (textline) {
         pp1 = strcasestr(textline,matchtext);                                   //  look for matching text
         if (pp1) break;                                                         //  found
         free(textline);
      }
      line++;
      if (line == nlines) line = 0;
      if (line == line1) return -1;                                              //  wrapped around, not found
   }
   
   cc = strlen(matchtext);                                                       //  highlight matching text
   pp2 = pp1 + cc - 1;      
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter1,line,pp1-textline);
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter2,line,pp2-textline+1);
   gtk_text_buffer_select_range(textBuff,&iter1,&iter2);

   free(textline);
   return line;
}


//  insert a pixbuf image at designated line

void textwidget_insert_pixbuf(GtkWidget *textwidget, int line, GdkPixbuf *pixbuf)
{
   int            nlines;
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   nlines = gtk_text_buffer_get_line_count(textBuff);                            //  insert after line
   if (line < nlines - 1)
      gtk_text_buffer_get_iter_at_line(textBuff,&iter,line+1);                   //  start of next line
   else gtk_text_buffer_get_end_iter(textBuff,&iter);                            //  or end of text

   gtk_text_buffer_insert_pixbuf(textBuff,&iter,pixbuf);
   return;
}


//  scroll a textwidget to put a given line on screen
//  1st line = 0.  for last line use line = -1.

void textwidget_scroll(GtkWidget *textwidget, int line)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter;
   GtkTextMark    *mark;
   GtkAdjustment  *vadjust;
   double         upperlimit;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   vadjust = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(textwidget));
   
   if (line < 0) {                                                               //  bottom
      upperlimit = gtk_adjustment_get_upper(vadjust);                            //  does not work within a zdialog FIXME
      gtk_adjustment_set_value(vadjust,upperlimit);
   }
   
   else {
      gtk_text_buffer_get_iter_at_line(textBuff,&iter,line);
      mark = gtk_text_buffer_create_mark(textBuff,0,&iter,0); 
      gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(textwidget),mark);
   }

   return;
}


//  scroll a textwidget to put a given line at the top of the window

void textwidget_scroll_top(GtkWidget *textwidget, int line)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter;
   GtkTextMark    *mark;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;
   gtk_text_buffer_get_iter_at_line(textBuff,&iter,line);
   mark = gtk_text_buffer_create_mark(textBuff,0,&iter,0); 
   gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(textwidget),mark,0,1,0,0);

   return;
}


//  get the range of textwidget lines currently visible in the window

void textwidget_get_visible_lines(GtkWidget *textwidget, int &top, int &bott)
{
   GdkRectangle   rect;
   GtkTextIter    iter1, iter2;
   int            y1, y2;

   gtk_text_view_get_visible_rect(GTK_TEXT_VIEW(textwidget),&rect);
   y1 = rect.y;
   y2 = y1 + rect.height;
   gtk_text_view_get_line_at_y(GTK_TEXT_VIEW(textwidget), &iter1, y1, 0);
   gtk_text_view_get_line_at_y(GTK_TEXT_VIEW(textwidget), &iter2, y2, 0);
   top = gtk_text_iter_get_line(&iter1);
   bott = gtk_text_iter_get_line(&iter2) - 1;
   return;
}


//  dump the entire textwidget contents into a file

void textwidget_dump(GtkWidget *widget, cchar *filename)
{
   FILE        *fid;
   char        *prec;
   int         line, err;

   fid = fopen(filename,"w");                                                    //  open file
   if (! fid) {
      zmessageACK(0,E2X("cannot open file %s"),filename);
      return;
   }

   for (line = 0; ; line++)
   {
      prec = textwidget_line(widget,line,1);                                     //  get text line, strip \n
      if (! prec) break;
      fprintf(fid,"%s\n",prec);                                                  //  output with \n
   }

   err = fclose(fid);                                                            //  close file
   if (err) zmessageACK(0,"file close error");
   return;
}


//  dump the entire textwidget contents into a file, using a save-as dialog

void textwidget_save(GtkWidget *widget, GtkWindow *parent)
{
   char        *file;

   file = zgetfile(E2X("save text to file"),parent,"save","noname");
   if (! file) return;
   textwidget_dump(widget,file);
   zfree(file);
   return;
}


//  Get a line of text. Returned text is subject for zfree().
//  trailing \n is included if strip == 0

char * textwidget_line(GtkWidget *textwidget, int line, int strip)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   int            cc, nlines;
   char           *textline, *ztext;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return 0;
   
   nlines = gtk_text_buffer_get_line_count(textBuff);                            //  lines now in buffer
   if (line < 0 || line >= nlines) return 0;
   
   gtk_text_buffer_get_iter_at_line(textBuff,&iter1,line);                       //  line start
   iter2 = iter1;
   gtk_text_iter_forward_line(&iter2);                                           //  end
   textline = gtk_text_buffer_get_text(textBuff,&iter1,&iter2,0);                //  get text line
   if (! textline) return 0;
   ztext = zstrdup(textline);
   free(textline);
   if (strip) {
      cc = strlen(ztext);
      if (cc && ztext[cc-1] == '\n') ztext[cc-1] = 0;
   }
   return ztext;
}


//  highlight a given line of text

void textwidget_highlight_line(GtkWidget *textwidget, int line)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   int            nlines;

   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;
   
   nlines = gtk_text_buffer_get_line_count(textBuff);                            //  lines now in buffer
   if (line < 0 || line >= nlines) return;
   
   gtk_text_buffer_get_iter_at_line(textBuff,&iter1,line);                       //  line start
   iter2 = iter1;
   gtk_text_iter_forward_line(&iter2);                                           //  end
   gtk_text_buffer_select_range(textBuff,&iter1,&iter2);                         //  highlight
   return;
}


//  get the word at the given position within the line
//  words are defined by line starts and ends, and the given delimiters
//  returns word and delimiter (&end)

char * textwidget_word(GtkWidget *textwidget, int line, int posn, cchar *dlims, char &end)
{
   GtkTextBuffer  *textBuff;
   char           *txline, *pp1, *pp2, *ztext;
   int            pos, cc;
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return 0;

   txline = textwidget_line(textwidget,line,0);
   if (! txline) return 0;

   pos = utf8_position(txline,posn);                                             //  graphic position to byte position
   if (pos < 0) return 0;

   pp1 = txline + pos;
   if (! *pp1 || strchr(dlims,*pp1)) return 0;                                   //  reject edge position or delimiter
   while (pp1 > txline && ! strchr(dlims,pp1[-1])) pp1--;                        //  find start of word
   pp2 = pp1;
   while (pp2[1] && ! strchr(dlims,pp2[1])) pp2++;                               //  find following delimiter or EOL
   end = pp2[1];                                                                 //  return delimiter
   while (*pp1 == ' ') pp1++;                                                    //  no leading or trailing blanks
   while (*pp2 == ' ') pp2--;
   cc = pp2 - pp1 + 1;
   if (cc < 1) return 0;                                                         //  all blanks?
   ztext = (char *) zmalloc(cc+1);
   strncpy0(ztext,pp1,cc+1);
   zfree(txline);
   return ztext;
}


//  highlight text at line and positiion, length cc

void textwidget_highlight_word(GtkWidget *textwidget, int line, int posn, int cc)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   char           *txline, *pp1, *pp2;
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   txline = textwidget_line(textwidget,line,0);
   if (! txline) return;

   pp1 = txline + posn;
   pp2 = pp1 + cc - 1;
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter1,line,pp1-txline);
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter2,line,pp2-txline+1);
   gtk_text_buffer_select_range(textBuff,&iter1,&iter2);

   zfree(txline);
   return;
}


//  convert text to bold text at line, positiion, cc

void textwidget_bold_word(GtkWidget *textwidget, int line, int posn, int cc)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   GtkTextTag     *fontag = 0;
   cchar          *boldfont = zfuncs::appmonoboldfont;
   char           *txline, *pp1, *pp2;
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   txline = textwidget_line(textwidget,line,0);
   if (! txline) return;

   fontag = gtk_text_buffer_create_tag(textBuff,0,"font",boldfont,0);

   /***
   fontag = gtk_text_buffer_create_tag(textBuff,0,"font",boldfont,               //  example
                       "foreground","red","background","light blue",0);
   ***/
   
   pp1 = txline + posn;
   pp2 = pp1 + cc - 1;
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter1,line,pp1-txline);
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter2,line,pp2-txline+1);
   gtk_text_buffer_apply_tag(textBuff,fontag,&iter1,&iter2);

   zfree(txline);
   return;
}


//  convert text to underlined text at line, positiion, cc

void textwidget_underline_word(GtkWidget *textwidget, int line, int posn, int cc)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   GtkTextTag     *fontag = 0;
   char           *txline, *pp1, *pp2;
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   txline = textwidget_line(textwidget,line,0);
   if (! txline) return;

   fontag = gtk_text_buffer_create_tag(textBuff,0,"underline",PANGO_UNDERLINE_SINGLE,0);

   pp1 = txline + posn;
   pp2 = pp1 + cc - 1;
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter1,line,pp1-txline);
   gtk_text_buffer_get_iter_at_line_index(textBuff,&iter2,line,pp2-txline+1);
   gtk_text_buffer_apply_tag(textBuff,fontag,&iter1,&iter2);

   zfree(txline);
   return;
}


//  set font attributes for the entire textwidget (black on white)               //  temp. kludge

void textwidget_font_attributes(GtkWidget *textwidget)
{
   GtkTextBuffer  *textBuff;
   GtkTextIter    iter1, iter2;
   GtkTextTag     *fontag = 0;
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textwidget));
   if (! textBuff) return;

   fontag = gtk_text_buffer_create_tag(textBuff,0,                               //  high contrast
                        "foreground","black","background","white",0);

   gtk_text_buffer_get_start_iter(textBuff,&iter1);
   gtk_text_buffer_get_end_iter(textBuff,&iter2);

   gtk_text_buffer_apply_tag(textBuff,fontag,&iter1,&iter2);

   return;
}


//  set an event function for mouse and KB events in textwidget
//    + line selection via mouse click or keyboard up/down arrow key
//    + line and word selection via mouse click
//
//  optional user callback function looks like this:
//    void userfunc(GtkWidget *textwidget, int line, int position, int KBkey)
//  this function receives KB keys and mouse click text line/position

void textwidget_set_eventfunc(GtkWidget *textwidget, textwidget_callbackfunc_t userfunc)
{
   int textwidget_eventfunc(GtkWidget *textwidget, GdkEvent *event, textwidget_callbackfunc_t userfunc);
   
   gtk_widget_add_events(textwidget,GDK_BUTTON_PRESS_MASK);
   gtk_widget_add_events(textwidget,GDK_KEY_PRESS_MASK);
   gtk_widget_add_events(textwidget,GDK_POINTER_MOTION_MASK);
   gtk_widget_add_events(textwidget,GDK_FOCUS_CHANGE_MASK);
   G_SIGNAL(textwidget,"key-press-event",textwidget_eventfunc,userfunc);
   G_SIGNAL(textwidget,"button-press-event",textwidget_eventfunc,userfunc);
   G_SIGNAL(textwidget,"motion-notify-event",textwidget_eventfunc,userfunc);
   G_SIGNAL(textwidget,"focus-in-event",textwidget_eventfunc,userfunc);
   return;
}


//  textwidget event function:
//  if no user callback function, process KB navigation keys (arrow, page, home/end)
//  if user callback func, send all KB keys to user callback function
//  process mouse clicks, send clicked line and position to user callback function

int textwidget_eventfunc(GtkWidget *textwidget, GdkEvent *event, textwidget_callbackfunc_t userfunc)
{
   #define TEXT GTK_TEXT_WINDOW_TEXT
   #define VIEW GTK_TEXT_VIEW

   static GdkCursor  *arrowcursor = 0;
   GdkWindow         *gdkwin;
   GtkTextIter       iter1;
   int               mpx, mpy, tbx, tby;
   int               line, pos, top, bott, page, KBkey;
   
   if (! arrowcursor)                                                            //  first call, get arrow cursor
      arrowcursor = gdk_cursor_new_for_display(display,GDK_TOP_LEFT_ARROW);
   gdkwin = gtk_text_view_get_window(VIEW(textwidget),TEXT);                     //  set arrow cursor for window
   if (gdkwin) gdk_window_set_cursor(gdkwin,arrowcursor);                        //  (must reset every event)

   gtk_widget_grab_focus(textwidget);

   if (event->type == GDK_KEY_PRESS)                                             //  KB key press event
   {
      KBkey = ((GdkEventKey *) event)->keyval;
      
      if (userfunc) {
         userfunc(textwidget,-1,-1,KBkey);
         return 1;
      }

      if (KBkey >= 0xfd00)                                                       //  navigation key
      {
         textwidget_get_visible_lines(textwidget,top,bott);                      //  range of lines on screen
         page = bott - top - 2;                                                  //  page size, lines
         if (page < 0) page = 0;
         line = 0;                                                               //  default
         if (KBkey == GDK_KEY_Up) line = top - 1;                                //  handle some navigation keys
         else if (KBkey == GDK_KEY_Down) line = bott + 1;
         else if (KBkey == GDK_KEY_Page_Up) line = top - page;
         else if (KBkey == GDK_KEY_Page_Down) line = bott + page;
         else if (KBkey == GDK_KEY_Home) line = 0;
         else if (KBkey == GDK_KEY_End) line = 999999;
         if (line < 0) line = 0;
         textwidget_scroll(textwidget,line);                                     //  put line on screen
      }

      return 1;
   }
   
   if (! userfunc) return 1;

   if (event->type == GDK_BUTTON_PRESS) {                                        //  mouse button press
      mpx = int(((GdkEventButton *) event)->x);                                  //  mouse click position
      mpy = int(((GdkEventButton *) event)->y);
      mpx -= appfontsize / 2;                                                    //  more accurate 
      if (mpx < 0) mpx = 0;
      gtk_text_view_window_to_buffer_coords(VIEW(textwidget),TEXT,mpx,mpy,&tbx,&tby);
      if (tbx && tby) {                                                          //  can happen
         gtk_text_view_get_iter_at_location(VIEW(textwidget),&iter1,tbx,tby);
         line = gtk_text_iter_get_line(&iter1);                                  //  clicked textwidget line
         pos = gtk_text_iter_get_line_offset(&iter1);                            //  clicked position
      }
      else line = pos = 0;
      userfunc(textwidget,line,pos,-1);                                          //  pass line and posn to user func
      return 1;
   }
   
   return 1;
}


/********************************************************************************

   simplified GTK menu bar, tool bar, status bar functions

   These functions simplify the creation of GTK menus and toolbars.
   The functionality is limited but adequate for most purposes.

   mbar = create_menubar(vbox)                               create menubar
   mitem = add_menubar_item(mbar, label, func)               add menu item to menubar
   msub = add_submenu_item(mitem, label, func, tip)          add submenu item to menu or submenu

   tbar = create_toolbar(vbox, iconsize)                     create toolbar
   add_toolbar_button(tbar, label, tip, icon, func)          add button to toolbar

   stbar = create_stbar(vbox)                                create status bar
   stbar_message(stbar, message)                             display message in status bar

   These functions to the following:
      * create a menu bar and add to existing window vertical packing box
      * add menu item to menu bar
      * add submenu item to menu bar item or submenu item
      * create a toolbar and add to existing window
      * add button to toolbar, using stock icon or custom icon
      * create a status bar and add to existing window
      * display a message in the status bar

   argument definitions:
     vbox         GtkWidget *    a vertical packing box (in a window)
     mbar         GtkWidget *    reference for menu bar
     popup        GtkWidget *    reference for popup menu
     mitem        GtkWidget *    reference for menu item (in a menu bar)
     msub         GtkWidget *    reference for submenu item (in a menu)
     label        cchar *        menu or toolbar name or label
     tbar         GtkWidget *    reference for toolbar
     tip          cchar *        tool button tool tip (popup text via mouse-over)
     icon         cchar *        stock icon name or custom icon file name (see below)
     func         see below      menu or tool button response function
     arg          cchar *        argument to response function
     stbar        int            reference for status bar
     message      cchar *        message to display in status bar

   The icon argument for the function add_toolbar_button() has two forms.
   For a GTK stock item referenced with a macro like GTK_STOCK_OPEN, use the
   corresponding text name, like "gtk-open".

   For a custom icon, use the icon's file name like "my-icon.png".
   The file is expected to be in  get_zdatadir()/icons.
   The icon file may be any size, and is resized for use on the toolbar.
   If the file is not found, the stock icon "gtk-missing-image" is used
   (".png" and ".jpg" files both work).

   For a button with no icon (text label only), use 0 or null for the icon argument.
   For a menu separator, use the menu name "separator".
   For a toolbar separator, use the label "separator".
   For a title menu (no response function), set the response function to null.

   The response function for both menus and toolbar buttons looks like this:
       void func(GtkWidget *, cchar *)

   The following macro is also supplied to simplify the coding of response functions:
   G_SIGNAL(window,event,func,arg)   which expands to:
   g_signal_connect(G_OBJECT(window),event,G_CALLBACK(func),(void *) arg)

*********************************************************************************/

//  create menu bar and add to vertical packing box

GtkWidget * create_menubar(GtkWidget *vbox)
{
   GtkWidget   *wmbar;

   wmbar = gtk_menu_bar_new();
   gtk_box_pack_start(GTK_BOX(vbox),wmbar,0,0,0);
   return wmbar;
}


//  add menu item to menu bar, with optional response function

GtkWidget * add_menubar_item(GtkWidget *wmbar, cchar *mname, cbFunc func)
{
   GtkWidget   *wmitem;

   wmitem = gtk_menu_item_new_with_label(mname);
   gtk_menu_shell_append(GTK_MENU_SHELL(wmbar),wmitem);
   if (func) G_SIGNAL(wmitem,"activate",func,mname);
   return  wmitem;
}


//  add submenu item to menu item, with optional response function

GtkWidget * add_submenu_item(GtkWidget *wmitem, cchar *mlab, cbFunc func, cchar *mtip)
{
   GtkWidget      *wmsub, *wmsubitem;

   wmsub = gtk_menu_item_get_submenu(GTK_MENU_ITEM(wmitem));                     //  add submenu if not already
   if (wmsub == null) {
      wmsub = gtk_menu_new();
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(wmitem),wmsub);
   }

   if (strmatch(mlab,"separator"))
      wmsubitem = gtk_separator_menu_item_new();
   else  wmsubitem = gtk_menu_item_new_with_label(mlab);                         //  add menu item with label only

   gtk_menu_shell_append(GTK_MENU_SHELL(wmsub),wmsubitem);                       //  append submenu item to submenu

   if (func) G_SIGNAL(wmsubitem,"activate",func,mlab);                           //  connect optional response function

   if (mtip) g_object_set(G_OBJECT(wmsubitem),"tooltip-text",mtip,null);         //  add optional popup menu tip

   return  wmsubitem;
}


/********************************************************************************/

//  create toolbar and add to vertical packing box

int      tbIconSize = 32;                                                        //  default if not supplied

GtkWidget * create_toolbar(GtkWidget *vbox, int iconsize)
{
   GtkWidget   *wtbar;

   wtbar = gtk_toolbar_new();
   gtk_box_pack_start(GTK_BOX(vbox),wtbar,0,0,0);
   tbIconSize = iconsize;
   return  wtbar;
}


//  add toolbar button with label and icon ("iconfile.png") and tool tip
//  at least one of label and icon should be present

GtkWidget * add_toolbar_button(GtkWidget *wtbar, cchar *blab, cchar *btip, cchar *icon, cbFunc func)
{
   GtkToolItem    *tbutton;
   GError         *gerror = 0;
   PIXBUF         *pixbuf;
   GtkWidget      *wicon = 0;
   char           iconpath[300], *pp;
   STATB          statb;
   int            err, cc;

   if (blab && strmatch(blab,"separator")) {
      tbutton = gtk_separator_tool_item_new();
      gtk_toolbar_insert(GTK_TOOLBAR(wtbar),GTK_TOOL_ITEM(tbutton),-1);
      return  (GtkWidget *) tbutton;
   }

   if (icon && *icon) {                                                          //  get icon pixbuf
      *iconpath = 0;
      strncatv(iconpath,199,zimagedir,"/",icon,null);
      err = stat(iconpath,&statb);
      if (err) {                                                                 //  alternative path
         cc = readlink("/proc/self/exe",iconpath,300);                           //  get own program path
         if (cc > 0) iconpath[cc] = 0;                                           //  readlink() quirk
         pp = strrchr(iconpath,'/');                                             //  folder of program
         if (pp) *pp = 0;
         strncatv(iconpath,300,"/icons/",icon,null);                             //  .../icons/iconfile.png
      }
      pixbuf = gdk_pixbuf_new_from_file_at_scale(iconpath,tbIconSize,tbIconSize,1,&gerror);
      if (pixbuf) wicon = gtk_image_new_from_pixbuf(pixbuf);
   }

   tbutton = gtk_tool_button_new(wicon,blab);
   if (! wicon) gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(tbutton),"gtk-missing-image");

   if (btip) gtk_tool_item_set_tooltip_text(tbutton,btip);
   gtk_tool_item_set_homogeneous(tbutton,0);
   gtk_toolbar_insert(GTK_TOOLBAR(wtbar),GTK_TOOL_ITEM(tbutton),-1);
   if (func) G_SIGNAL(tbutton,"clicked",func,blab);
   return  (GtkWidget *) tbutton;
}


/********************************************************************************/

//  create a status bar and add to the start of a packing box

GtkWidget * create_stbar(GtkWidget *pbox)
{
   GtkWidget      *stbar;

   stbar = gtk_statusbar_new();
   gtk_box_pack_start(GTK_BOX(pbox),stbar,0,0,0);
   gtk_widget_show(stbar);
   return  stbar;
}


//  display message in status bar

int stbar_message(GtkWidget *wstbar, cchar *message)
{
   static int     ctx = -1;

   if (ctx == -1)
      ctx = gtk_statusbar_get_context_id(GTK_STATUSBAR(wstbar),"all");
   gtk_statusbar_pop(GTK_STATUSBAR(wstbar),ctx);
   gtk_statusbar_push(GTK_STATUSBAR(wstbar),ctx,message);
   return 0;
}


/********************************************************************************

   Customizable Graphic Popup Menu

   void Gmenuz(GtkWidget *parent, cchar *configfile, callbackfunc) 

   Open a popup window with a customizable graphic menu.
      parent         parent window or null
      configfile     menu configuration file, will be created if missing
      callbackfunc   callback function to receive clicked menu entry:
                        typedef void callbackfunc(cchar *menu)

   This function allows an application to offer a customizable menu which a
   user can populate with frequently used (menu) functions, arranged as desired.
   A menu entry selected by the user is passed to the application for execution.

   The initial popup window is blank. Right click an empty space on the popup
   window to define a new menu entry. Right click an existing entry to modify it.
   Use the resulting dialog to define or change the menu entry.
      menu text     optional text appearing in the popup window
      menu func     text that is passed to the application callback function
      menu icon     optional menu icon: an optional .png file can be selected
      icon size     rendered icon size in the popup window, 24x24 to 64x64 pixels
      close         checkbox: option to close the popup window when menu is used

   Left drag a menu entry to move it somewhere else on the popup window.
   The popup window can be resized to fit the contained menu entries.
   Left click a menu entry to select the menu.
   The callback function will be called to execute the menu function.
   If "close" was checked, the popup window will close.

   All menu settings are saved in the supplied configuration file whenever the
   popup window is closed, if any changes were made since it was opened.
   Icon files are copied into the same folder as the configuration file and
   these copies are used. The icon selected when the menu entry is created can
   disappear without consequence. The file name NNN.png is used (000-999).

   If the [x] window kill button is pressed, the window is closed and the
   calling program is informed by passing "quit" to the callback function.

   layout of menu configuration file:

      popup xpos ypos width height     popup window position and size

      posn  xpos ypos ww hh            menu position in popup window
      menu  menu text                  menu text on popup window
      func  funcname                   argument for user callback function
      icon  NNN.png                    optional menu icon file
      size  NN                         optional icon size (default 24)
      kill                             optional flag, kill window when used

      menu  menu text                  next menu entry
      ...

*********************************************************************************/

namespace gmenuznames
{
   #define maxME 200                                                             //  max. menu entries
   #define maxText 1000                                                          //  max. text size, all menu fields

   typedef void callbackfunc(cchar *menu);                                       //  user callback function
   callbackfunc    *gmenucallback;

   cchar       *menuFont, *menuBoldFont;
   char        *menufile = 0;                                                    //  menu configuration file from user
   char        *menudir = 0;
   char        iconfile[200];                                                    //  icon file full path < 200

   GtkWidget   *mWin, *layout;                                                   //  popup and drawing windows
   GtkWidget   *pWin;                                                            //  parent window

   int         winposx=100, winposy=100, winww=400, winhh=300;                   //  initial popup WRT parent window
   int         mdcc;

   struct      menuent {                                                         //  menu entry on popup window
      int         xpos, ypos, ww, hh;                                            //  layout position, extent
      char        *menu;                                                         //  text on window or null
      int         bold;                                                          //  text is bold
      char        *func;                                                         //  func name for user callback
      char        *icon;                                                         //  icon file "NNN.png" or null
      PIXBUF      *pixbuf;                                                       //  icon pixbuf or null
      int         size;                                                          //  icon size or zero
      int         kill;                                                          //  kill popup window when menu used
      int         Fnewicon;                                                      //  flag, icon was changed 
   };
   menuent     menus[maxME];                                                     //  menu entries
   int         NME = 0;                                                          //  entry count

   zdialog     *zdedit = 0;                                                      //  active edit dialog
   int         mpx, mpy;                                                         //  mouse click/drag position
   int         me;                                                               //  current menu entry
   int         Fchanged = 0;                                                     //  flag, menu edited or resized
   int         Fpopquit = 0;                                                     //  popup is being closed
   int         Fpopbusy = 0;                                                     //  popup is active
   int         deficonsize = 32;                                                 //  default icon size or last size set

   void  wpaint(GtkWidget *, cairo_t *);                                         //  window repaint - draw event
   void  resize();                                                               //  window resize event
   void  quit();                                                                 //  kill window and exit
   void  update_configfile();                                                    //  update menu configuration file
   void  mouse_event(GtkWidget *, GdkEventButton *, void *);                     //  mouse event function
   void  KB_event(GtkWidget *, GdkEventKey *, void *);                           //  KB event function
   void  draw_text(cairo_t *, char *, int px, int py, int &ww, int &hh);         //  draw text and return pixel extent
   void  draw_bold_text(cairo_t *, char *, int px, int py, int &ww, int &hh);    //  draw text and return pixel extent
   void  edit_menu();                                                            //  dialog to create/edit menu entry
   int   edit_menu_event(zdialog *zd, cchar *event);                             //  dialog event function
   void  drag_drop_event(int mx, int my, char *file);                            //  drag-drop event function
}


//  user callable function to build the menu from user's configuration file

void Gmenuz(GtkWidget *parent, cchar *title, cchar *ufile, gmenuznames::callbackfunc ufunc)
{
   using namespace gmenuznames;

   FILE        *fid;
   int         nn, xx, yy, ww, hh, size;
   int         pposx, pposy;
   int         xpos, ypos;
   char        *pp, buff[maxText];
   PIXBUF      *pixbuf;
   GError      *gerror;

   if (Fpopbusy) return;                                                         //  don't allow multiple popups

   menuFont = zfuncs::appfont;
   menuBoldFont = zfuncs::appboldfont;

   pWin = parent;                                                                //  get parent window

   if (menufile) {
      zfree(menufile);
      zfree(menudir);
   }
   menufile = zstrdup(ufile);                                                    //  get menu configuration file
   menudir = zstrdup(ufile);                                                     //  folder of same
   pp = strrchr(menudir,'/');
   if (! pp || pp < menudir + 10) zappcrash("Gmenuz() bad menu: %s",menufile);
   *(pp+1) = 0;
   mdcc = strlen(menudir);
   if (mdcc > 190) zappcrash("Gmenuz() too big: %s",menufile);
   
   gmenucallback = ufunc;                                                        //  get user callback function

   NME = 0;

   fid = fopen(menufile,"r");                                                    //  read window geometry
   if (fid)
   {
      nn = fscanf(fid," popup %d %d %d %d",&xx,&yy,&ww,&hh);                     //  get popup window position and size
      if (nn == 4 && ww > 50 && ww < 1000 && hh > 50 && hh < 1000) {
         winposx = xx;                                                           //  OK to use
         winposy = yy;
         winww = ww;
         winhh = hh;
      }

      while (true)
      {
         pp = fgets_trim(buff,maxText-1,fid,1);                                  //  read next menu entry
         if (! pp) break;

         if (strmatchN(pp,"posn ",5)) {                                          //  position in popup window
            if (NME == maxME) {
               zmessageACK(mWin,"exceeded %d menu entries",maxME);
               break;
            }

            me = NME;                                                            //  new entry
            NME++;                                                               //  entry count
            memset(&menus[me],0,sizeof(menuent));                                //  clear all menu data

            nn = sscanf(pp+5," %d %d ",&xpos,&ypos);                             //  position in popup window
            if (nn != 2) xpos = ypos = 100;
            if (xpos > 1000) xpos = 1000;
            if (ypos > 1000) ypos = 1000;
            menus[me].xpos = xpos;
            menus[me].ypos = ypos;
         }

         if (strmatchN(pp,"menu ",5)) {                                          //  menu text
            if (strlen(pp+5) > 0)
               menus[me].menu = zstrdup(pp+5);                                   //  get menu text
            else menus[me].menu = 0;
         }
         
         if (strmatchN(pp,"bold",4))                                             //  bold text
            menus[me].bold = 1;

         if (strmatchN(pp,"func ",5)) {                                          //  function name
            if (strlen(pp+5)) menus[me].func = zstrdup(pp+5);
            else menus[me].func = 0;
         }

         if (strmatchN(pp,"icon ",5)) {                                          //  menu icon file  NNN.png
            pp += 5;
            if (*pp) {                                                           //  icon file present
               if (*pp == '/') {                                                 //  old style icon file /.../filename.png 
                  menus[me].Fnewicon = 1;                                        //  new icon is needed
                  Fchanged = 1;
                  gerror = 0;
                  pixbuf = gdk_pixbuf_new_from_file(pp,&gerror);
                  if (! pixbuf && gerror) printz("*** %s \n",gerror->message);
                  menus[me].pixbuf = pixbuf;
                  if (strmatchN(menudir,pp,mdcc)) remove(pp);                    //  delete file if in /menudir/
               }
               else {                                                            //  new style icon file  NNN.png
                  menus[me].icon = zstrdup(pp);                                  //  (relocatable if menudir moved)
                  strncpy0(iconfile,menudir,200);
                  strcpy(iconfile+mdcc,pp);                                      //  /menudir/NNN.png
                  gerror = 0;
                  pixbuf = gdk_pixbuf_new_from_file(iconfile,&gerror);
                  if (! pixbuf && gerror) printz("*** %s \n",gerror->message);
                  menus[me].pixbuf = pixbuf;
               }
            }
            else menus[me].pixbuf = 0;                                           //  no icon file
         }

         if (strmatchN(pp,"size ",5)) {
            size = atoi(pp+5);
            if (size < 24) size = 24;
            if (size > 256) size = 256;
            menus[me].size = size;
         }

         if (strmatchN(pp,"kill",4))                                             //  kill window flag
            menus[me].kill = 1;
      }

      fclose(fid);
   }

   if (Fchanged) update_configfile();

   mWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);                                   //  create popup window for menu entries

   if (! pWin) {                                                                 //  no parent window
      pposx = pposy = 0;
      gtk_window_set_focus_on_map(GTK_WINDOW(mWin),1);                           //  put on top when created 
   }
   else {
      gtk_window_get_position(GTK_WINDOW(pWin),&pposx,&pposy);                   //  parent window position (NW corner)
      gtk_window_set_transient_for(GTK_WINDOW(mWin),GTK_WINDOW(pWin));           //  popup window belongs to parent
   }

   if (title) gtk_window_set_title(GTK_WINDOW(mWin),title);

   winposx += pposx;                                                             //  popup position relative to parent
   winposy += pposy;

   gtk_window_set_default_size(GTK_WINDOW(mWin),winww,winhh);                    //  set size and position
   gtk_window_move(GTK_WINDOW(mWin),winposx,winposy);

   layout = gtk_layout_new(0,0);                                                 //  create drawing window
   gtk_container_add(GTK_CONTAINER(mWin),layout);                                //  add to popup window

   G_SIGNAL(mWin,"destroy",quit,0);                                              //  connect signals to windows
   G_SIGNAL(mWin,"delete-event",quit,0);
   G_SIGNAL(mWin,"notify",resize,0);
   G_SIGNAL(mWin,"configure-event",resize,0);                                    //  GTK 3.18 API changed 
   G_SIGNAL(mWin,"key-press-event",KB_event,0);                                  //  connect KB key event
   G_SIGNAL(layout,"draw",wpaint,0);

   gtk_widget_add_events(layout,GDK_BUTTON_PRESS_MASK);                          //  connect mouse events
   gtk_widget_add_events(layout,GDK_BUTTON_RELEASE_MASK);
   gtk_widget_add_events(layout,GDK_BUTTON_MOTION_MASK);
   gtk_widget_add_events(layout,GDK_POINTER_MOTION_MASK);
   G_SIGNAL(layout,"button-press-event",mouse_event,0);
   G_SIGNAL(layout,"button-release-event",mouse_event,0);
   G_SIGNAL(layout,"motion-notify-event",mouse_event,0);
   
   drag_drop_dest(mWin,drag_drop_event);

   gtk_widget_show_all(mWin);                                                    //  show all widgets

   zmainloop();                                                                  //  after initial window events 
   Fchanged = 0;                                                                 //  no changes yet
   Fpopquit = 0;                                                                 //  not being closed
   Fpopbusy = 1;

   return;
}


//  paint window when created, exposed, resized

void gmenuznames::wpaint(GtkWidget *, cairo_t *cr)
{
   using namespace gmenuznames;

   PIXBUF      *pixbuf;
   char        *text, *text2;
   int         xpos, ypos, ww, hh, size, yadd;
   int         bold;

   for (int me = 0; me < NME; me++)                                              //  loop all menu entries
   {
      xpos = menus[me].xpos;                                                     //  window position
      ypos = menus[me].ypos;
      text = menus[me].menu;                                                     //  menu text
      bold = menus[me].bold;                                                     //  bold text flag
      pixbuf = menus[me].pixbuf;                                                 //  icon
      size = menus[me].size;                                                     //  size

      if (pixbuf) {                                                              //  draw icon at window position
         gdk_cairo_set_source_pixbuf(cr,pixbuf,xpos,ypos);
         cairo_paint(cr);
         if (! size) size = deficonsize;                                         //  use default if not specified
      }
      else size = 0;

      yadd = 0;
      if (pixbuf) yadd = gdk_pixbuf_get_height(pixbuf) + 2;                      //  icon height + extra space

      if (text) {
         text2 = (char *) zmalloc(strlen(text)+100);                             //  replace "\n" with newline
         repl_1str(text,text2,"\\n","\n");
         if (bold) draw_bold_text(cr,text2,xpos,ypos+yadd,ww,hh);                //  bold text
         else draw_text(cr,text2,xpos,ypos+yadd,ww,hh);
         zfree(text2);
      }
      else ww = hh = 0;

      if (ww < size) ww = size;                                                  //  menu entry enclosing rectangle
      hh += yadd;
      menus[me].ww = ww;
      menus[me].hh = hh;
   }

   return;
}


//  resize event - save current window size

void gmenuznames::resize()
{
   using namespace gmenuznames;

   int            xx, yy, ww, hh;
   
   if (Fpopquit) return;                                                         //  ignore bogus call when killed

   gtk_window_get_position(GTK_WINDOW(mWin),&xx,&yy);
   gtk_window_get_size(GTK_WINDOW(mWin),&ww,&hh);
   
   if (xx != winposx || yy != winposy) Fchanged = 1;                             //  window size or position changed
   if (ww != winww || hh != winhh) Fchanged = 1;
   
   winposx = xx;                                                                 //  save window position
   winposy = yy;
   winww = ww;                                                                   //  save new size
   winhh = hh;
   
   return;
}


//  [x] kill window
//  Save current menu status for next session.

void gmenuznames::quit()
{
   using namespace gmenuznames;

   Fpopquit = 1;
   Fpopbusy = 0;
   if (Fchanged) update_configfile();
   gtk_widget_destroy(mWin);
   gmenucallback("quit");                                                        //  inform host program 
   return;
}


//  menu changed, save all menu data to menu config. file

void gmenuznames::update_configfile()
{
   using namespace gmenuznames;

   static int     ftf = 1;
   int64          DT;
   int            err, ii, me;
   char           *pp;
   int            pposx, pposy;
   FILE           *fid;
   STATB          statb;
   GError         *gerror;
   
   if (ftf) {                                                                    //  initialize random generator
      ftf = 0;
      DT = time(null);
      drandz(&DT);
   }
   
   if (pWin)
      gtk_window_get_position(GTK_WINDOW(pWin),&pposx,&pposy);                   //  parent window position (may have moved)
   else pposx = pposy = 0;

   winposx -= pposx;                                                             //  popup position relative to parent
   winposy -= pposy;

   fid = fopen(menufile,"w");                                                    //  open for write
   if (! fid) {
      zmessageACK(mWin," %s \n %s",menufile,strerror(errno));                    //  diagnose permissions error 
      return;
   }

   fprintf(fid,"popup %d %d %d %d \n",winposx,winposy,winww,winhh);

   for (me = 0; me < NME; me++)                                                  //  write all menu entries to file
   {
      if (! menus[me].menu && ! menus[me].pixbuf) {                              //  no text and no icon
         printz("*** Gmenuz: empty menu entry \n");
         continue;
      }

      fprintf(fid,"\n");                                                         //  blank line separator

      fprintf(fid,"posn %d %d \n",menus[me].xpos, menus[me].ypos);               //  menu position in window

      if (menus[me].menu)                                                        //  menu text
         fprintf(fid,"menu %s \n",menus[me].menu);
      
      if (menus[me].bold)                                                        //  bold text flag 
         fprintf(fid,"bold \n");
      
      if (menus[me].func)                                                        //  menu function (text)
         fprintf(fid,"func %s \n",menus[me].func);

      if (menus[me].pixbuf) {                                                    //  menu icon 
         if (menus[me].Fnewicon) {                                               //  new or changed icon
            strncpy0(iconfile,menudir,200);                                      //  menu file folder
            pp = iconfile + mdcc;                                                //  pp --> NNN.png
            for (ii = 0; ii < 999; ii++) {
               sprintf(pp,"%03d.png",ii);                                        //  find first available NNN.png file
               err = stat(iconfile,&statb);
               if (err) break;                                                   //  (if full, 999.png is re-used)
            }
            gerror = 0;
            gdk_pixbuf_save(menus[me].pixbuf,iconfile,"png",&gerror,null);       //  write pixbuf to icon file
            if (gerror) printz("*** %s %s \n",pp,gerror->message);
            else fprintf(fid,"icon %s \n",pp);                                   //  file name is NNN.png
            menus[me].icon = zstrdup(pp);
            menus[me].Fnewicon = 0;
         }            
         else if (menus[me].icon)                                                //  no change
            fprintf(fid,"icon %s \n",menus[me].icon);
      }

      if (menus[me].size)                                                        //  icon size
         fprintf(fid,"size %d \n",menus[me].size);

      if (menus[me].kill) fprintf(fid,"kill \n");                                //  kill window flag
   }

   fclose(fid);
   Fchanged = 0;
   return;
}


//  mouse event function - capture buttons and drag movements

void gmenuznames::mouse_event(GtkWidget *, GdkEventButton *event, void *)
{
   using namespace gmenuznames;

   static int     bdtime = 0, butime = 0;
   static int     Lmouse = 0, Rmouse = 0, Fdrag = 0;
   static int     elapsed, mpx0 = 0, mpy0 = 0;
   int            Fclick, dx, dy, xpos, ypos, size;
   int            raster = 10;                                                   //  alignment raster

   mpx = int(event->x);                                                          //  mouse position in window
   mpy = int(event->y);

   if (event->type == GDK_BUTTON_PRESS)
   {
      Lmouse = Rmouse = Fdrag = 0;
      if (event->button == 1) Lmouse++;                                          //  left or right mouse button
      if (event->button == 3) Rmouse++;
      bdtime = event->time;

      for (me = 0; me < NME; me++)                                               //  look for clicked menu entry
      {
         if (mpx < menus[me].xpos) continue;
         if (mpy < menus[me].ypos) continue;
         if (mpx > menus[me].xpos + menus[me].ww) continue;
         if (mpy > menus[me].ypos + menus[me].hh) continue;
         break;
      }

      if (me < NME) {                                                            //  menu item clicked (selected)
         mpx0 = mpx;                                                             //  set new drag origin
         mpy0 = mpy;
      }
      else me = -1;                                                              //  indicate empty space clicked
   }

   if (event->type == GDK_BUTTON_RELEASE)
   {
      Fclick = 0;
      butime = event->time;
      elapsed = butime - bdtime;                                                 //  button down time, milliseconds
      bdtime = 0;
      if (elapsed < 500 && ! Fdrag) Fclick = 1;                                  //  mouse clicked

      if (me >= 0 && Fclick && Lmouse) {                                         //  menu entry was left-clicked
         if (menus[me].kill) quit();                                             //  close menu if specified
         if (menus[me].func) gmenucallback(menus[me].func);                      //  caller user function(func)
      }

      else if (Fclick && Rmouse)                                                 //  menu entry or empty space right-clicked
         edit_menu();                                                            //  edit menu

      else if (me >= 0 && Fdrag)                                                 //  menu entry drag ended
         Fchanged = 1;                                                           //  mark menu revised

      Lmouse = Rmouse = Fdrag = 0;                                               //  mouse click action completed
   }

   if (event->type == GDK_MOTION_NOTIFY)                                         //  mouse movement
   {
      if (me >= 0 && Lmouse && bdtime) {                                         //  menu drag underway
         dx = mpx - mpx0;
         dy = mpy - mpy0;
         if (abs(dx) + abs(dy) > 4)                                              //  ignore small drags
         {
            Fdrag++;
            mpx0 = mpx;                                                          //  set new drag origin
            mpy0 = mpy;

            menus[me].xpos = mpx;                                                //  center menu on mouse
            menus[me].ypos = mpy;
            size = menus[me].size;
            if (size) {
               menus[me].xpos -= size / 2;
               menus[me].ypos -= size / 2;
            }
            else {
               menus[me].xpos -= 15;
               menus[me].ypos -= 8;
            }

            xpos = menus[me].xpos;                                               //  align to raster
            ypos = menus[me].ypos;
            xpos = raster * (xpos / raster);
            ypos = raster * (ypos / raster);
            menus[me].xpos = xpos;
            menus[me].ypos = ypos;

            if (menus[me].xpos < 0) menus[me].xpos = 0;                          //  not off the window
            if (menus[me].xpos > winww-20) menus[me].xpos = winww-20;
            if (menus[me].ypos < 0) menus[me].ypos = 0;
            if (menus[me].ypos > winhh-20) menus[me].ypos = winhh-20;
            gtk_widget_queue_draw(layout);                                       //  repaint window
         }
      }
   }

   return;
}


//  KB press event function - send certain keys to main app

void gmenuznames::KB_event(GtkWidget *, GdkEventKey *kbevent, void *)
{
   using namespace gmenuznames;

   int   KBkey = kbevent->keyval;
   if (KBkey == GDK_KEY_F1) KBevent(kbevent);
   if (KBkey == GDK_KEY_Escape) quit();                                          //  escape = cancel
   return;
}


//  draw text into layout and return pixel dimensions of enclosing rectangle

void gmenuznames::draw_text(cairo_t *cr, char *text, int x, int y, int &w, int &h)
{
   using namespace gmenuznames;

   static PangoFontDescription   *pfont = 0;
   static PangoLayout            *playout = 0;

   if (! pfont) {
      pfont = pango_font_description_from_string(menuFont);                      //  first call, get font sizing poop
      playout = gtk_widget_create_pango_layout(layout,0);
      pango_layout_set_font_description(playout,pfont);
   }

   pango_layout_set_text(playout,text,-1);                                       //  compute layout
   pango_layout_get_pixel_size(playout,&w,&h);                                   //  pixel width and height of layout

   cairo_move_to(cr,x,y);                                                        //  draw layout with text
   cairo_set_source_rgb(cr,0,0,0);
   pango_cairo_show_layout(cr,playout);

   return;
}


//  draw bold text into layout and return pixel dimensions of enclosing rectangle

void gmenuznames::draw_bold_text(cairo_t *cr, char *text, int x, int y, int &w, int &h)
{
   using namespace gmenuznames;

   static PangoFontDescription   *pfont = 0;
   static PangoLayout            *playout = 0;

   if (! pfont) {
      pfont = pango_font_description_from_string(menuBoldFont);                  //  first call, get font sizing poop
      playout = gtk_widget_create_pango_layout(layout,0);
      pango_layout_set_font_description(playout,pfont);
   }

   pango_layout_set_text(playout,text,-1);                                       //  compute layout
   pango_layout_get_pixel_size(playout,&w,&h);                                   //  pixel width and height of layout

   cairo_move_to(cr,x,y);                                                        //  draw layout with text
   cairo_set_source_rgb(cr,0,0,0);
   pango_cairo_show_layout(cr,playout);

   return;
}


//  dialog to create a new menu entry from user inputs

void gmenuznames::edit_menu()
{
   using namespace gmenuznames;

   if (me < 0) {                                                                 //  new menu entry
      if (NME == maxME) {
         zmessageACK(mWin,"capacity limit exceeded");
         return;
      }
      me = NME;
      memset(&menus[me],0,sizeof(menuent));                                      //  clear all data
   }

/**
         menu text [________________] [x] Bold
         menu func [________________________]
         menu icon [_______________] [Browse]
         icon size [___]  [x] close window

                    [Apply] [Delete] [Cancel]
**/

   if (! zdedit)                                                                 //  create dialog if not already
   {
      zdedit = zdialog_new(E2X("edit menu entry"),mWin,E2X("Apply"),E2X("Delete"),E2X("Cancel"),null);
      zdialog_add_widget(zdedit,"hbox","hb1","dialog");
      zdialog_add_widget(zdedit,"vbox","vb1","hb1",0,"homog|space=3");
      zdialog_add_widget(zdedit,"vbox","vb2","hb1",0,"homog|expand");
      zdialog_add_widget(zdedit,"label","lab11","vb1",E2X("menu text"));
      zdialog_add_widget(zdedit,"label","lab12","vb1",E2X("menu func"));
      zdialog_add_widget(zdedit,"label","lab13","vb1",E2X("menu icon"));
      zdialog_add_widget(zdedit,"label","lab14","vb1",E2X("icon size"));
      zdialog_add_widget(zdedit,"hbox","hb2","vb2");
      zdialog_add_widget(zdedit,"zentry","text","hb2",0,"size=30|space=2");
      zdialog_add_widget(zdedit,"check","bold","hb2",E2X("Bold"),"space=5");
      zdialog_add_widget(zdedit,"zentry","func","vb2",0,"size=30|space=2");
      zdialog_add_widget(zdedit,"hbox","hb3","vb2",0,"expand|space=2");
      zdialog_add_widget(zdedit,"zentry","icon","hb3",0,"expand");
      zdialog_add_widget(zdedit,"zbutton","browse","hb3",E2X("Browse"),"space=5");
      zdialog_add_widget(zdedit,"hbox","hb4","vb2",0,"space=2");
      zdialog_add_widget(zdedit,"zspin","size","hb4","24|256|1|32");
      zdialog_add_widget(zdedit,"check","kill","hb4",E2X("close window"),"space=30");
      zdialog_run(zdedit,edit_menu_event,"mouse");
   }

   if (menus[me].menu)                                                           //  stuff menu text into dialog
      zdialog_stuff(zdedit,"text",menus[me].menu);
   else zdialog_stuff(zdedit,"text","");

   if (menus[me].func)                                                           //  stuff menu function
      zdialog_stuff(zdedit,"func",menus[me].func);
   else zdialog_stuff(zdedit,"func","");

   zdialog_stuff(zdedit,"bold",menus[me].bold);                                  //  stuff bold text flag

   if (menus[me].icon)                                                           //  stuff icon file
      zdialog_stuff(zdedit,"icon",menus[me].icon);
   else zdialog_stuff(zdedit,"icon","");

   if (menus[me].size)                                                           //  stuff icon size
      zdialog_stuff(zdedit,"size",menus[me].size);

   if (menus[me].kill)                                                           //  stuff window kill flag
      zdialog_stuff(zdedit,"kill",1);
   else zdialog_stuff(zdedit,"kill",0);

   if (me == NME) {                                                              //  new menu entry
      menus[me].xpos = mpx;                                                      //  initial position from mouse
      menus[me].ypos = mpy;
   }

   return;
}


//  menu entry dialog event function

int gmenuznames::edit_menu_event(zdialog *zd, cchar *event)
{
   using namespace gmenuznames;

   char        text[maxText], *pp;
   int         size;
   PIXBUF      *pixbuf;
   GError      *gerror;
   GtkWindow   *parent = GTK_WINDOW(zd->dialog);
   
   if (strmatch(event,"browse")) {                                               //  browse for icon file
      pp = zgetfile(E2X("select icon"),parent,"file",menudir);
      if (pp) {
         zdialog_stuff(zd,"icon",pp);
         zfree(pp);
      }
   }
   
   if (! zd->zstat) return 1;                                                    //  wait for dialog completion

   if (zd->zstat == 2) {                                                         //  [delete] - delete menu entry
      if (me < 0 || me >= NME) return 0;
      for (int me2 = me; me2 < NME-1; me2++)                                     //  remove menu entry and close hole
         menus[me2] = menus[me2+1];
      NME--;
      Fchanged = 1;                                                              //  mark menu revised
      gtk_widget_queue_draw(layout);                                             //  repaint window
   }

   if (zd->zstat != 1) {                                                         //  not [apply] - kill dialog
      zdialog_free(zdedit);
      return 1;
   }
                                                                                 //  [apply] - update menu from dialog data
   zdialog_fetch(zd,"text",text,maxText);
   if (*text) menus[me].menu = zstrdup(text);                                    //  menu text, optional
   else menus[me].menu = 0;
   
   zdialog_fetch(zd,"bold",menus[me].bold);                                      //  bold text flag
   
   zdialog_fetch(zd,"func",text,maxText);                                        //  menu function name
   strTrim2(text);

   pp = strstr(text,"mystuff");
   if (pp) {                                                                     //  this is a mystuff link
      strcpy(text,pp);                                                           //  "/usr/bin/mystuff" --> "mystuff"
      if (strlen(text) < 9) {
         strcat(text," [unique-name].txt");                                      //  remind unique menu file name
         zdialog_stuff(zd,"func",text);
         zmessageACK(mWin,"assign a unique menu name");
         zd->zstat = 0;
         return 1;
      }
   }

   if (*text) menus[me].func = zstrdup(text);
   else menus[me].func = 0;

   zdialog_fetch(zd,"icon",text,maxText);                                        //  menu icon file, optional
   strTrim2(text);
   if (*text) {
      if (strlen(text) < 8) {                                                    //  have local "NNN.png"
         strncpy0(iconfile,menudir,200);
         strcpy(iconfile+mdcc,text);                                             //  make /menudir/NNN.png
      }
      else strncpy0(iconfile,text,200);                                          //  new icon file selected
      zdialog_fetch(zd,"size",size);                                             //  icon size
      gerror = 0;
      pixbuf = gdk_pixbuf_new_from_file_at_size(iconfile,size,size,&gerror);
      if (! pixbuf) {
         if (gerror) zmessageACK(mWin,gerror->message);                          //  bad icon file
         zd->zstat = 0;                                                          //  keep dialog open
         return 0;                                                               //  do nothing
      }
      menus[me].icon = zstrdup(text);
      menus[me].pixbuf = pixbuf;
      menus[me].size = size;
      deficonsize = size;                                                        //  set new default 
      menus[me].Fnewicon = 1;                                                    //  mark icon changed
   }
   else {                                                                        //  no icon
      menus[me].icon = 0;
      menus[me].pixbuf = 0;
      menus[me].size = 0;
      menus[me].Fnewicon = 0;
   }

   zdialog_fetch(zd,"kill",menus[me].kill);                                      //  popup window kill flag

   if (me == NME) NME++;                                                         //  if new menu entry, incr. count

   Fchanged = 1;                                                                 //  mark menu revised
   zdialog_free(zdedit);                                                         //  destroy dialog
   gtk_widget_queue_draw(layout);                                                //  repaint window

   return 1;
}


//  function to accept drag-drop of a .desktop file or file name.

void gmenuznames::drag_drop_event(int mpx, int mpy, char *file)
{
   using namespace gmenuznames;
   
   FILE     *fid;
   char     buff[200], filetype[60], dtfile[200];
   char     *pp, name[100], exec[200], icon[200];
   char     wildiconfile[200];
   GError   *gerror = 0;
   PIXBUF   *pixbuf = 0;
   int      ii, jj, uflag;
   int      size = deficonsize;                                                  //  default or last size set by user
   
   #define Ndirs 8

   cchar    *icondir[Ndirs] = {                                                  //  compensate Linux chaos
      "/usr/share/app-install/icons*",
      "/usr/share/pixmaps*",
      "/usr/share/icons/*64*",
      "/usr/share/icons/*48*",
      "/usr/share/icons/*32*",
      "/usr?share/icons/*scalable*",
      "/usr/local/share/icons*",
      "/usr/local/share/pixmaps*" 
   };
   
   if (! file) return;                                                           //  drag motion event

   #define Next 3
   cchar    *iconext[Next] = { "png", "svg", "xpm" };                            //  file extensions searched
   
   if (NME == maxME) {
      zmessageACK(mWin,"capacity limit exceeded");
      return;
   }

   me = NME;
   memset(&menus[me],0,sizeof(menuent));                                         //  clear all data
   
   pp = strrchr(file,'.');                                                       //  look if .desktop file

   if (pp && strmatch(pp,".desktop")) {
      strncpy0(dtfile,file,200);                                                 //  have a .desktop file
      file = 0;                                                                  //  and no data file
   }

   else                                                                          //  find .desktop file for given data file
   {
      snprintf(buff,200,"xdg-mime query filetype \"%s\"",file);                  //  xdg-mime query filetype <filename>
      fid = popen(buff,"r");
      if (! fid) {
         zmessageACK(mWin,strerror(errno));
         return;
      }
      pp = fgets_trim(buff,200,fid);                                             //  should get major/minor file type
      pclose(fid);
      if (! pp) {
         zmessageACK(mWin,strerror(errno));
         return;
      }
      strncpy0(filetype,pp,60);

      snprintf(buff,200,"xdg-mime query default %s",filetype);                   //  zdg-mime query default <major/minor>
      fid = popen(buff,"r");
      if (! fid) {
         zmessageACK(mWin,strerror(errno));
         return;
      }
      pp = fgets_trim(buff,200,fid);                                             //  should get appname.desktop
      pclose(fid);
      if (! pp) {
         zmessageACK(mWin,strerror(errno));
         return;
      }
      
      snprintf(dtfile,200,"/usr/share/applications/%s",pp);                      //  /usr/share/applications/appname.desktop

      pp = strrchr(dtfile,'.');
      if (! strmatch(pp,".desktop")) {
         zmessageACK(mWin,".desktop file not found");
         return;
      }
   }

   fid = fopen(dtfile,"r");                                                      //  read .desktop file
   if (! fid) {
      zmessageACK(mWin,strerror(errno));
      return;
   }
   
   *name = *exec = *icon = 0;
   
   while (true) {                                                                //  save .desktop parameters
      pp = fgets_trim(buff,200,fid);
      if (! pp) break;
      if (strmatchN(buff,"Name=",5) && ! *name)                                  //  app name
         strncpy0(name,buff+5,100);
      if (strmatchN(buff,"Exec=",5) && ! *exec)                                  //  executable
         strncpy0(exec,buff+5,200);
      if (strmatchN(buff,"Icon=",5) && ! *icon)                                  //  usr/share/app-install/icons/*.png
         strncpy0(icon,buff+5,200);
   }
   
   fclose(fid);

   if (file) {
      pp = strrchr(file,'/');                                                    //  menu name = file base name
      if (! pp) pp = file;
      else pp++;
      menus[me].menu = zstrdup(pp);
      pp = strrchr(menus[me].menu,'.');
      if (pp) *pp = 0;
      pp = strrchr(exec,'%');                                                    //  get rid of %f %u etc.
      if (pp) *pp = 0;
      snprintf(buff,200,"%s \"%s\"",exec,file);                                  //  menu function = executable + filename
      menus[me].func = zstrdup(buff);
   }
   else {
      menus[me].menu = zstrdup(name);                                            //  menu name = appname
      menus[me].func = zstrdup(exec);                                            //  menu function = executable
   }
   
   if (file && strmatchN(filetype,"image",5)) {                                  //  if image file, make icon from image
      pixbuf = gdk_pixbuf_new_from_file_at_size(file,size,size,&gerror);
      if (pixbuf) {
         menus[me].icon = zstrdup(file);
         menus[me].pixbuf = pixbuf;
         menus[me].size = size;
         menus[me].Fnewicon = 1;
      }
   }

   else if (*icon)                                                               //  make icon from .desktop file
   {
      if (*icon == '/') {                                                        //  icon filespec is given 
         pp = icon;
         goto found;
      }
      
      for (ii = 0; ii < Ndirs; ii++)                                             //  search icon locations
      for (jj = 0; jj < Next; jj++)                                              //  try known icon file types
      {
         snprintf(wildiconfile,200,"%s/%s.%s",icondir[ii],icon,iconext[jj]);
         uflag = 1;
         pp = (char *) SearchWild(wildiconfile,uflag);
         if (pp) {
            uflag = 2;
            SearchWild(null,uflag);
            goto found;                                                          //  break out 2 loops
         }
      }

   found:
      if (! pp) zmessageACK(mWin,"icon file not found: %s \n",icon);
      else {
         pixbuf = gdk_pixbuf_new_from_file_at_size(pp,size,size,&gerror);
         if (! pixbuf) zmessageACK(mWin,"icon file error: %s \n",pp);
         else {
            menus[me].icon = zstrdup(pp);
            menus[me].pixbuf = pixbuf;
            menus[me].size = size;
            menus[me].Fnewicon = 1;
         }
      }
   }

   menus[me].xpos = mpx;                                                         //  position from mouse
   menus[me].ypos = mpy;

   menus[me].kill = 1;                                                           //  popup window kill flag

   NME++;                                                                        //  incr. menu count
   Fchanged = 1;                                                                 //  mark menu revised
   gtk_widget_queue_draw(layout);                                                //  repaint window
   
   edit_menu();                                                                  //  user can edit dropped menu
   return;
}


/********************************************************************************

   Popup Menu

   GtkWidget   *popup, *mitem
   cchar       *label, *arg, *tip
   void func(GtkWidget *, cchar *arg)

   popup = create_popmenu()                                  create a popup menu
   mitem = add_popmenu_item(popup, label, func, arg, tip)    add menu item to popup menu
   popup_menu(GtkWidget *parent, popup)                      popup the menu at mouse position

   Call 'create_popmenu' and then 'add_popmenu_item' for each item in the menu.
   'label' is the menu name, 'func' the response function, 'arg' an argument
   for 'func', and 'tip' is a tool-tip. 'arg' and 'tip' may be null.
   A call to 'popup_menu' will show all menu entries at the mouse position.
   Clicking an entry will call the respective response function.
   Hovering on the entry will show the tool-tip.

   The response function looks like this:
       void func(GtkWidget *, cchar *menu)

***/

//  create a popup menu

GtkWidget * create_popmenu()
{
   int popmenu_event(GtkWidget *, GdkEvent *);

   GtkWidget   *popmenu;
   popmenu = gtk_menu_new();

   gtk_widget_add_events(popmenu,GDK_BUTTON_PRESS_MASK);
   G_SIGNAL(popmenu,"button-press-event",popmenu_event,0);
   return popmenu;
}


//  handle mouse button event in a popup menu

int popmenu_event(GtkWidget *popmenu, GdkEvent *event)
{
   if (((GdkEventButton *) event)->button != 1)                                  //  if not left mouse, kill menu
      gtk_menu_popdown(GTK_MENU(popmenu));
   return 0;
}


//  add a menu item to a popup menu

GtkWidget   *popmenu_widget;

GtkWidget * add_popmenu_item(GtkWidget *popmenu, cchar *mname, cbFunc func, cchar *arg, cchar *mtip)
{
   void popmenu_item_select(GtkWidget *, cchar *mtip);
   GtkWidget   *wmitem;

   wmitem = gtk_menu_item_new_with_label(mname);
   gtk_menu_shell_append(GTK_MENU_SHELL(popmenu),wmitem);

   if (func) {
      if (arg) G_SIGNAL(wmitem,"activate",func,arg);                             //  call func with arg 
      else G_SIGNAL(wmitem,"activate",func,mname);                               //  call func with menu name
   }
   
   if (mtip) {
      popmenu_widget = wmitem;                                                   //  trigger popup tool tip
      G_SIGNAL(wmitem,"select",popmenu_item_select,mtip);
      G_SIGNAL(wmitem,"deselect",popmenu_item_select,0);
   }

   return  wmitem;
}


//  show popup tip for selected menu item

void popmenu_item_select(GtkWidget *wmitem, cchar *mtip)                         //  convoluted code but it works
{
   GdkWindow   *window;
   int         xp, yp, mx, my;

   window = gtk_widget_get_window(wmitem);
   gdk_window_get_origin(window,&xp,&yp);                                        //  menu screen origin
   xp += gdk_window_get_width(window);                                           //   + width
   gdk_device_get_position(zfuncs::mouse,0,&mx,&my);                             //  mouse (x,y) screen position
   poptext_screen(mtip,xp,my,0,5);                                               //  popup px = menu + width, py = mouse
   return;
}


//  Show a popup menu at current mouse position
//  GtkWidget *  argument is not used

void popup_menu(GtkWidget *widget, GtkWidget *popmenu)
{
   int      mx, my;
   
   if (mouse) {
      gdk_device_get_position(mouse,&screen,&mx,&my);                            //  offset popup menu from mouse
      gdk_device_warp(mouse,screen,mx+30,my);
   }

   gtk_widget_show_all(popmenu);                                                 //  GTK change: show before popup 

   #if GTK_CHECK_VERSION(3,22,0)
      gtk_menu_popup_at_pointer(GTK_MENU(popmenu),null);
   #else
      int time = gtk_get_current_event_time();
      gtk_menu_popup(GTK_MENU(popmenu),0,0,0,0,1,time);
   #endif

   return;
}


/********************************************************************************

   Vertical Menu / Toolbar

   Build a custom vertical menu and/or toolbar in a vertical packing box

   Vmenu    *vbm;
   cchar    *name, *icon, *desc, *arg;
   int      iww, ihh;

   void func(GtkWidget *, cchar *name);
   void RMfunc(GtkWidget *, cchar *name);

   vbm = Vmenu_new(GtkWidget *vbox, float fgRGB[3], float bgRGB[3]);             //  create base menu

   Vmenu_add(vbm, name, icon, iww, ihh, desc, func, arg);                        //  add menu item

   Vmenu_add_RMfunc(vbm, me, RMfunc);                                            //  add right-click menu item
   
   Vmenu_block(flag)
      int      flag        1 to block Vmenu, 0 to unblock 

   Create a vertical menu / toolbar in a vertical packing box.
   fgRGB and bgRGB are font and background colors, RGB scaled 0-1.0 
   Added items can have a menu name, icon, description, response function,
   and function argument. 'name' and 'icon' can be null but not both.

   name        menu name
   icon        menu icon, filespec for a .png file
   iww, ihh    size of icon in menu display
   desc        optional tool tip if mouse is hovered over displayed menu

   When 'name/icon' is clicked, 'func' is called with 'arg'.
   If 'arg' is null, 'name' is used instead.

   To create a menu entry that is a popup menu with multiple entries, do as follows:
        popup = create_popmenu();
        add_popup_menu_item(popup ...);         see create_popmenu()
        add_popup_menu_item(popup ...);
        ...
        Vmenu_add(vbm, name, icon, ww, hh, desc, create_popmenu, (cchar *) popup);

   i.e. use create_popmenu() as the response function and use the previously
        created menu 'popup' as the argument (cast to cchar *).

***/


namespace Vmenunames
{
   #define margin 5                                                              //  margins for menu text

   PangoFontDescription    *pfont1, *pfont2;
   PangoAttrList           *pattrlist;
   PangoAttribute          *pbackground;
   int      fontheight;
   int      Fblock = 0;

   void  wpaint(GtkWidget *, cairo_t *, Vmenu *);                                //  window repaint - draw event
   void  mouse_event(GtkWidget *, GdkEventButton *, Vmenu *);                    //  mouse event function
   void  paint_menu(cairo_t *cr, Vmenu *vbm, int me, int hilite);                //  paint menu entry, opt. highlight
}


//  create Vmenu

Vmenu *Vmenu_new(GtkWidget *vbox, float fgRGB[3], float bgRGB[3])
{
   using namespace Vmenunames;

   int      cc, ww, hh;
   int      K64 = 65536;
   char     *menufont1, *menufont2;
   PangoLayout   *playout;

   cc = sizeof(Vmenu);
   Vmenu *vbm = (Vmenu *) zmalloc(cc);
   memset(vbm,0,cc);
   vbm->fgRGB[0] = fgRGB[0];                                                     //  background color, RGB 0-1.0
   vbm->fgRGB[1] = fgRGB[1];
   vbm->fgRGB[2] = fgRGB[2];
   vbm->bgRGB[0] = bgRGB[0];                                                     //  background color, RGB 0-1.0
   vbm->bgRGB[1] = bgRGB[1];
   vbm->bgRGB[2] = bgRGB[2];
   vbm->vbox = vbox;
   vbm->topwin = gtk_widget_get_toplevel(vbox);
   vbm->layout = gtk_layout_new(0,0);
   vbm->mcount = 0;
   gtk_box_pack_start(GTK_BOX(vbox),vbm->layout,1,1,0);
   vbm->xmax = vbm->ymax = 10;                                                   //  initial layout size

   pattrlist = pango_attr_list_new();
   pbackground = pango_attr_background_new(K64*bgRGB[0],K64*bgRGB[1],K64*bgRGB[2]);
   pango_attr_list_change(pattrlist,pbackground);

   menufont1 = zstrdup(zfuncs::appfont);                                         //  set menu fonts, normal and bold
   menufont2 = zstrdup(zfuncs::appboldfont);
   
   pfont1 = pango_font_description_from_string(menufont1);
   pfont2 = pango_font_description_from_string(menufont2);

   playout = gtk_widget_create_pango_layout(vbm->layout,0);
   pango_layout_set_font_description(playout,pfont1);
   pango_layout_set_text(playout,"Ayg",-1);
   pango_layout_get_pixel_size(playout,&ww,&hh);
   fontheight = hh;

   gtk_widget_add_events(vbm->layout,GDK_BUTTON_PRESS_MASK);
   gtk_widget_add_events(vbm->layout,GDK_BUTTON_RELEASE_MASK);
   gtk_widget_add_events(vbm->layout,GDK_POINTER_MOTION_MASK);
   gtk_widget_add_events(vbm->layout,GDK_LEAVE_NOTIFY_MASK);
   G_SIGNAL(vbm->layout,"button-press-event",mouse_event,vbm);
   G_SIGNAL(vbm->layout,"button-release-event",mouse_event,vbm);
   G_SIGNAL(vbm->layout,"motion-notify-event",mouse_event,vbm);
   G_SIGNAL(vbm->layout,"leave-notify-event",mouse_event,vbm);
   G_SIGNAL(vbm->layout,"draw",wpaint,vbm);

   return vbm;
}


//  add Vmenu entry with name, icon, description, callback function

void Vmenu_add(Vmenu *vbm, cchar *name, cchar *icon, int iconww, int iconhh, cchar *desc, cbFunc func, cchar *arg)
{
   using namespace Vmenunames;

   int         me, cc, xpos, ww, hh;
   char        iconpath[200], *mdesc, *name__;
   cchar       *blanks = "                    ";                                 //  20 blanks
   PIXBUF      *pixbuf;
   GError      *gerror = 0;

   PangoLayout             *playout;
   PangoFontDescription    *pfont;

   if (! name && ! icon) return;

   me = vbm->mcount++;                                                           //  track no. menu entries

   if (name) vbm->menu[me].name = zstrdup(name);                                 //  create new menu entry from caller data

   if (icon) {
      vbm->menu[me].icon = zstrdup(icon);
      vbm->menu[me].iconww = iconww;
      vbm->menu[me].iconhh = iconhh;
   }

   if (desc) {                                                                   //  pad description with blanks for looks
      cc = strlen(desc);
      mdesc = (char *) zmalloc(cc+3);
      mdesc[0] = ' ';
      strcpy(mdesc+1,desc);
      strcpy(mdesc+cc+1," ");
      vbm->menu[me].desc = mdesc;
   }

   vbm->menu[me].func = func;                                                    //  menu function
   vbm->menu[me].arg = name;                                                     //  argument is menu name or arg if avail.
   if (arg) vbm->menu[me].arg = arg;

   vbm->menu[me].pixbuf = 0;

   if (icon) {                                                                   //  if icon is named, get pixbuf
      *iconpath = 0;
      strncatv(iconpath,199,zfuncs::zimagedir,"/",icon,null);
      pixbuf = gdk_pixbuf_new_from_file_at_scale(iconpath,iconww,iconhh,1,&gerror);
      if (pixbuf) vbm->menu[me].pixbuf = pixbuf;
      else printz("Vmenu no icon: %s \n",iconpath);
   }

   if (me == 0) vbm->ymax = margin;                                              //  first menu, top position

   vbm->menu[me].iconx = 0;
   vbm->menu[me].icony = 0;
   vbm->menu[me].namex = 0;
   vbm->menu[me].namey = 0;

   if (icon) {
      vbm->menu[me].iconx = margin;                                              //     ______
      vbm->menu[me].icony = vbm->ymax;                                           //    |      |
      if (name) {                                                                //    | icon | menu name
         vbm->menu[me].namex = margin + iconww + margin;                         //    |______|
         vbm->menu[me].namey = vbm->ymax + (iconhh - fontheight) / 2;            //
      }
      vbm->menu[me].ylo = vbm->ymax;
      vbm->ymax += iconhh + iconhh / 8;                                          //  position for next menu entry
      vbm->menu[me].yhi = vbm->ymax;
      if (margin + iconww > vbm->xmax) vbm->xmax = margin + iconww;              //  keep track of max. layout width
   }

   else if (name) {
      vbm->menu[me].namex = margin;                                              //  menu name
      vbm->menu[me].namey = vbm->ymax;
      vbm->menu[me].ylo = vbm->ymax;
      vbm->ymax += 1.5 * fontheight;                                             //  more space between text lines
      vbm->menu[me].yhi = vbm->ymax;
   }
   
   vbm->menu[me].playout1 = gtk_widget_create_pango_layout(vbm->layout,0);
   vbm->menu[me].playout2 = gtk_widget_create_pango_layout(vbm->layout,0);

   if (name) {
      xpos = vbm->menu[me].namex;

      cc = strlen(name);                                                         //  menu name with trailing blanks
      name__ = zstrdup(name,22);                                                 //  (long enough to overwrite bold name)
      strncpy0(name__+cc,blanks,20);

      playout = vbm->menu[me].playout1;                                          //  normal font
      pfont = pfont1;
      pango_layout_set_attributes(playout,pattrlist);
      pango_layout_set_font_description(playout,pfont);
      pango_layout_set_text(playout,name__,-1);                                  //  compute layout
      pango_layout_get_pixel_size(playout,&ww,&hh);                              //  pixel width and height of layout

      playout = vbm->menu[me].playout2;                                          //  bold font
      pfont = pfont2;
      pango_layout_set_attributes(playout,pattrlist);
      pango_layout_set_font_description(playout,pfont);
      pango_layout_set_text(playout,name,-1);                                    //  compute layout
      pango_layout_get_pixel_size(playout,&ww,&hh);                              //  pixel width and height of layout
      if (xpos + ww > vbm->xmax) vbm->xmax = xpos + ww;                          //  keep track of max. layout width
   }

   gtk_widget_set_size_request(vbm->layout,vbm->xmax+margin,0);                  //  add right margin to layout width

   return;
}


//  add alternate function for right-mouse click

void Vmenu_add_RMfunc(Vmenu *vbm, int me, cbFunc func, cchar *arg)
{
   if (me > vbm->mcount-1) zappcrash("Vmenu_add_RMfunc() bad me: %d",me);
   vbm->menu[me].RMfunc = func;
   vbm->menu[me].RMarg = arg;
   return;
}


//  block or unblock menu

void Vmenu_block(int flag)
{
   using namespace Vmenunames;
   
   Fblock = flag;
   return;
}


//  paint window when created, exposed, resized

void Vmenunames::wpaint(GtkWidget *widget, cairo_t *cr, Vmenu *vbm)
{
   using namespace Vmenunames;

   cairo_set_source_rgb(cr,vbm->bgRGB[0],vbm->bgRGB[1],vbm->bgRGB[2]);           //  background
   cairo_paint(cr);

   for (int me = 0; me < vbm->mcount; me++)                                      //  paint all menu entries
      paint_menu(cr,vbm,me,0);

   return;
}


//  draw menu icon and text into layout

void Vmenunames::paint_menu(cairo_t *cr, Vmenu *vbm, int me, int hilite)
{
   using namespace Vmenunames;

   PIXBUF         *pixbuf;
   PangoLayout    *playout;
   int            xpos, ypos;
   int            iconww, iconhh;
   cchar          *name;

   pixbuf = vbm->menu[me].pixbuf;                                                //  icon
   if (pixbuf) {                                                                 //  draw menu icon at menu position
      xpos = vbm->menu[me].iconx;
      ypos = vbm->menu[me].icony;
      iconww = vbm->menu[me].iconww;
      iconhh = vbm->menu[me].iconhh;

      if (! hilite) {                                                            //  erase box around icon
         cairo_set_source_rgb(cr,vbm->bgRGB[0],vbm->bgRGB[1],vbm->bgRGB[2]);     //  background
         cairo_rectangle(cr,xpos-1,ypos-1,iconww+2,iconhh+2);
         cairo_fill(cr);
      }

      gdk_cairo_set_source_pixbuf(cr,pixbuf,xpos,ypos);                          //  draw icon
      cairo_paint(cr);

      if (hilite) {
         cairo_set_source_rgb(cr,vbm->fgRGB[0],vbm->fgRGB[1],vbm->fgRGB[2]);     //  draw box around icon
         cairo_set_line_width(cr,1);
         cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
         cairo_rectangle(cr,xpos,ypos,iconww,iconhh);
         cairo_stroke(cr);
      }
   }

   name = vbm->menu[me].name;                                                    //  menu text
   if (name) {                                                                   //  draw menu text at menu position
      xpos = vbm->menu[me].namex;
      ypos = vbm->menu[me].namey;
      cairo_move_to(cr,xpos,ypos);                                               //  draw layout with text
      cairo_set_source_rgb(cr,vbm->fgRGB[0],vbm->fgRGB[1],vbm->fgRGB[2]);
      if (hilite) playout = vbm->menu[me].playout2;
      else playout = vbm->menu[me].playout1;
      pango_cairo_show_layout(cr,playout);
   }

   return;
}


//  mouse event function - capture buttons and drag movements

void Vmenunames::mouse_event(GtkWidget *widget, GdkEventButton *event, Vmenu *vbm)
{
   using namespace Vmenunames;

   GdkWindow   *gdkwin;
   cchar       *desc;
   int         me, mpx, mpy, button, ww, ylo, yhi;
   static int  me0 = -1, Fmyclick = 0, winww = 0;

   static draw_context_t   context;
   static GtkWidget        *pwidget = 0;
   static cairo_t          *cr = 0;
   
   if (widget != pwidget) {                                                      //  widget changed 
      if (pwidget) draw_context_destroy(context);
      gdkwin = gtk_layout_get_bin_window(GTK_LAYOUT(widget));
      cr = draw_context_create(gdkwin,context);
      gdkwin = gtk_widget_get_window(widget);                                    //  get width of menu widget
      winww = gdk_window_get_width(gdkwin);
      pwidget = widget;
   }

   mpx = int(event->x);                                                          //  mouse position
   mpy = int(event->y);
   button = event->button;
   
   if (event->type == GDK_MOTION_NOTIFY)                                         //  mouse inside layout
   {
      for (me = 0; me < vbm->mcount; me++) {                                     //  find menu where mouse is
         ylo = vbm->menu[me].ylo;
         yhi = vbm->menu[me].yhi;
         if (mpy < ylo) continue;
         if (mpy < yhi) break;
      }

      if (me != me0 && me0 >= 0) {                                               //  unhighlight prior
         paint_menu(cr,vbm,me0,0);
         me0 = -1;
      }

      if (me == me0) return;                                                     //  same as before
      if (me == vbm->mcount) return;                                             //  no new menu match

      paint_menu(cr,vbm,me,1);                                                   //  highlight menu entry at mouse
      desc = vbm->menu[me].desc;                                                 //  show tool tip
      if (desc) poptext_widget(widget,desc,winww,mpy,0,5);                       //  px = menu width, py = mouse
      me0 = me;                                                                  //  remember last match
      return;
   }

   if (me0 >= 0)                                                                 //  mouse left layout
   {
      paint_menu(cr,vbm,me0,0);                                                  //  unhighlight prior
      desc = vbm->menu[me0].desc;                                                //  remove prior tool tip
      if (desc) poptext_mouse(0,0,0,0,0);
      me0 = -1;
   }

   if (event->type == GDK_BUTTON_PRESS)                                          //  menu entry clicked
      Fmyclick = 1;                                                              //  button click is mine

   if (event->type == GDK_BUTTON_RELEASE)                                        //  menu entry clicked
   {
      if (Fblock) return;                                                        //  menu is blocked

      if (! Fmyclick) return;                                                    //  ignore unmatched button release
      Fmyclick = 0;                                                              //    (from vanished popup window)

      for (me = 0; me < vbm->mcount; me++) {                                     //  look for clicked menu entry
         ylo = vbm->menu[me].ylo;
         yhi = vbm->menu[me].yhi;
         if (mpy < ylo) continue;
         if (mpy < yhi) break;
      }

      if (me == vbm->mcount) return;                                             //  no menu match

      zfuncs::vmenuclickbutton = button;                                         //  1/2/3 = left/mid/right button

      ww = vbm->menu[me].iconww;                                                 //  get horiz. click posn. on menu icon
      if (ww) mpx = 100 * (mpx - margin) / ww;                                   //    scale 0-100
      else mpx = 0;
      if (mpx < 0) mpx = 0;
      if (mpx > 100) mpx = 100;
      zfuncs::vmenuclickposn = mpx;

      paint_menu(cr,vbm,me,0);                                                   //  unhighlight menu
      
      if (button == 3 && vbm->menu[me].RMfunc)                                   //  if right mouse button,
         vbm->menu[me].RMfunc(widget,vbm->menu[me].RMarg);                       //    call right-mouse function
      
      else if (vbm->menu[me].func)
          vbm->menu[me].func(widget,vbm->menu[me].arg);                          //  caller function(widget,arg)
   }

   return;
}


/********************************************************************************
   simplified GTK dialog functions

   create a dialog with response buttons (OK, cancel ...)
   add widgets to dialog (button, text entry, combo box ...)
   put data into widgets (text or numeric data)
   run the dialog, get user inputs (button press, text entry, checkbox selection ...)
   run caller event function when widgets change from user inputs
   run caller event function when dialog is completed or canceled
   get dialog completion status (OK, cancel, destroyed ...)
   get data from dialog widgets (the user inputs)
   destroy the dialog and free memory

*********************************************************************************/


//  private functions for widget events and dialog completion

int  zdialog_widget_event(GtkWidget *, zdialog *zd);
int  zdialog_delete_event(GtkWidget *, GdkEvent *, zdialog *zd);
int  zdialog_zspin_event(GtkWidget *, GdkEvent *, zdialog *zd);                  //  "zspin" widget

//  create a new zdialog dialog
//  The title and parent arguments may be null.
//  optional arguments: up to zdmaxbutts button labels followed by null
//  returned dialog status: +N = button N (1 to zdmaxbutts)
//                          <0 = [x] button or other GTK destroy action
//  completion buttons are also events like other widgets
//  all dialogs run parallel, use zdialog_wait() if needed
//  The status returned by zdialog_wait() corresponds to the button
//  (1-10) used to end the dialog. Status < 0 indicates the [x] button
//  was used or the dialog was killed by the program itself.

zdialog * zdialog_new(cchar *title, GtkWidget *parent, ...)                      //  parent added
{
   zdialog        *zd;
   GtkWidget      *dialog, *hbox, *vbox, *butt, *hsep;
   cchar          *bulab[zdmaxbutts];
   int            cc, ii, nbu;
   va_list        arglist;
   static int     uniqueID = 1;

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))
      zappcrash("zdialog_new() called from thread");

   va_start(arglist,parent);
   for (nbu = 0; nbu < zdmaxbutts; nbu++) {                                      //  get completion buttons
      bulab[nbu] = va_arg(arglist, cchar *);
      if (! bulab[nbu]) break;
   }
   va_end(arglist);

   if (! title) title = "";

   dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(dialog),title);
   vbox = gtk_box_new(VERTICAL,0);                                               //  vertical packing box
   gtk_container_add(GTK_CONTAINER(dialog),vbox);                                //  add to main window
   gtk_window_set_default_size(GTK_WINDOW(dialog),10,10);                        //  stop auto width of 150 pixels
   
   if (parent)
      gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(parent));
   
   gtk_box_set_spacing(GTK_BOX(vbox),2);
   gtk_container_set_border_width(GTK_CONTAINER(vbox),5);

   cc = sizeof(zdialog);                                                         //  allocate zdialog
   zd = (zdialog *) zmalloc(cc);

   if (zdialog_count == zdialog_max) {                                           //  add to active list
      for (ii = 0; ii < zdialog_count; ii++)
         printz("dialog: %s \n",zdialog_list[ii]->widget[0].data);
      zappcrash("max. zdialogs exceeded");
   }

   zdialog_list[zdialog_count] = zd;
   zdialog_count++;

   if (nbu) {                                                                    //  completion buttons
      hbox = gtk_box_new(HORIZONTAL,2);                                          //  add hbox for buttons at bottom
      gtk_box_pack_end(GTK_BOX(vbox),hbox,0,0,2);
      hsep = gtk_separator_new(HORIZONTAL);                                      //  add separator line
      gtk_box_pack_end(GTK_BOX(vbox),hsep,0,0,3);

      for (ii = nbu-1; ii >= 0; ii--) {                                          //  add buttons to hbox
         butt = gtk_button_new_with_label(bulab[ii]);                            //  reverse order nbu-1...0
         gtk_box_pack_end(GTK_BOX(hbox),butt,0,0,2);
         G_SIGNAL(butt,"clicked",zdialog_widget_event,zd);                       //  connect to event function
         zd->compwidget[ii] = butt;                                              //  save button widgets
         zd->compbutton[ii] = bulab[ii];                                         //  and button labels
      }
   }

   zd->compbutton[nbu] = 0;                                                      //  mark EOL

   zd->dialog = dialog;                                                          //  dialog window
   zd->title = zstrdup(title);                                                   //  dialog title
   zd->parent = parent;                                                          //  parent window
   zd->sentinel1 = zdsentinel | (lrandz() & 0x0000FFFF);                         //  validity sentinels
   zd->sentinel2 = zd->sentinel1;                                                //  fixed part + random part
   zd->uniqueID = uniqueID++;                                                    //  increment unique ID
   zd->eventCB = 0;                                                              //  no user event callback function
   zd->zstat = 0;                                                                //  no zdialog status
   zd->disabled = 1;                                                             //  widget signals disabled
   zd->saveposn = 0;                                                             //  position not saved

   zd->widget[0].name = "dialog";                                                //  set up 1st widget = dialog
   zd->widget[0].type = "dialog";
   zd->widget[0].pname = 0;                                                      //  no parent
   zd->widget[0].data = zstrdup(title);
   zd->widget[0].cblist = 0;
   zd->widget[0].widget = dialog;

   zd->widget[1].type = 0;                                                       //  eof - no contained widgets yet
   return zd;
}


//  change a zdialog title

void zdialog_set_title(zdialog *zd, cchar *title)
{
   gtk_window_set_title(GTK_WINDOW(zd->widget[0].widget),title);
   return;
}


//  set a zdialog to be modal

void zdialog_set_modal(zdialog *zd)
{
   GtkWidget  *widget = zdialog_widget(zd,"dialog");
   gtk_window_set_modal(GTK_WINDOW(widget),1);
   gtk_window_set_keep_above(GTK_WINDOW(widget),1);
   return;
}


//  set a zdialog to be decorated or not

void zdialog_set_decorated(zdialog *zd, int decorated)
{
   void zdialog_drag(GtkWidget *widget, GdkEventButton *event, void *);

   GtkWidget  *widget;

   widget = zdialog_widget(zd,"dialog");
   gtk_window_set_decorated(GTK_WINDOW(widget),decorated);
   if (decorated) return;
   gtk_widget_add_events(widget,GDK_BUTTON_PRESS_MASK);
   gtk_widget_add_events(widget,GDK_BUTTON_RELEASE_MASK);
   gtk_widget_add_events(widget,GDK_POINTER_MOTION_MASK);
   G_SIGNAL(widget,"button-press-event",zdialog_drag,0);                         //  connect mouse events to drag
   G_SIGNAL(widget,"button-release-event",zdialog_drag,0);                       //    undecorated window
   G_SIGNAL(widget,"motion-notify-event",zdialog_drag,0);
   return;
}

void zdialog_drag(GtkWidget *widget, GdkEventButton *event, void *)
{
   static int  bdown = 0, type;
   static int  mx0, my0, mx, my;
   static int  wx0, wy0, wx, wy;

   type = event->type;
   gdk_device_get_position(zfuncs::mouse,0,&mx,&my);                             //  mouse position in monitor

   if (type == GDK_BUTTON_PRESS) {
      bdown = 1;
      mx0 = mx;                                                                  //  drag start
      my0 = my;
      gtk_window_get_position(GTK_WINDOW(widget),&wx0,&wy0);                     //  initial window position
   }

   if (type == GDK_BUTTON_RELEASE)
      bdown = 0;

   if (type == GDK_MOTION_NOTIFY) {
      if (! bdown) return;
      wx = wx0 + mx - mx0;
      wy = wy0 + my - my0;
      gtk_window_move(GTK_WINDOW(widget),wx,wy);
   }
   
   return;
}


//  present a zdialog (visible and on top)

void zdialog_present(zdialog *zd)
{
   GtkWidget  *widget = zdialog_widget(zd,"dialog");
   gtk_window_present(GTK_WINDOW(widget));
   return;
}


//  set zdialog can or can not receive focus (informational or report dialog)

void zdialog_can_focus(zdialog *zd, int Fcan)
{
   gtk_window_set_accept_focus(GTK_WINDOW(zd->dialog),Fcan);
   return;
}


//  set focus on dialog window or window and named widget 
//  (widget name may be null or missing) 
//  see also: gtk_window_activate_focus(GtkWindow *) 

void zdialog_set_focus(zdialog *zd, cchar *wname)
{
   GtkWindow   *window;
   GtkWidget   *widget;
   
   window = GTK_WINDOW(zd->dialog);
   if (wname) widget = zdialog_widget(zd,wname);
   else widget = 0;
   
   if (wname) gtk_window_set_focus(window,widget);
   else gtk_window_activate_focus(window);
   return;
}


//  add widget to existing zdialog
//
//   Arguments after parent are optional and default to 0.
//   zd         zdialog *, created with zdialog_new()
//   type       string, one of the widget types listed below
//   name       string, widget name, used to stuff or fetch widget data
//   parent     string, parent name: "dialog" or a previously added container widget
//   data       string, initial data for widget (label name, entry string, spin value, etc.)
//   size       cc for text entry or pixel size for image widget
//   homog      for hbox or vbox to make even space allocation for contained widgets
//   expand     widget should expand with dialog box expansion
//   space      extra space between this widget and neighbors, pixels
//   wrap       allow text to wrap at right margin
//
//   data can be a string ("initial widget data") or a number in string form ("12.345")
//   data for togbutt / check / radio: use "0" or "1" for OFF or ON
//   data for spin / zspin / hscale / vscale: use "min|max|step|value" (default: 0 | 100 | 1 | 50)
//   data for colorbutt: use "rrr|ggg|bbb" with values 0-255 for each RGB color.
//   This format is used to initialize the control and read back when user selects a color.
//   Multiple radio buttons with same parent are a group: pressing one turns the others OFF.

int zdialog_add_widget (
     zdialog *zd, cchar *type, cchar *name, cchar *pname,                        //  mandatory args
     cchar *data, int size, int homog, int expand, int space, int wrap)          //  optional args (default = 0)
{
   GtkWidget      *widget = 0, *pwidget = 0, *fwidget = 0;
   GtkWidget      *entry, *image, *vbox;
   GtkTextBuffer  *editBuff = 0;
   PIXBUF         *pixbuf = 0;
   GdkRGBA        gdkrgba;
   GError         *gerror = 0;
   cchar          *pp, *ptype = 0;
   char           vdata[30], iconpath[200];
   double         min, max, step, val;
   double         f256 = 1.0 / 256.0;
   int            iiw, iip, kk, err;

   if (! zdialog_valid(zd)) zappcrash("zdialog invalid");

   for (iiw = 1; zd->widget[iiw].type; iiw++);                                   //  find next avail. slot
   if (iiw > zdmaxwidgets-2) zappcrash("too many widgets: %d",iiw);

   zd->widget[iiw].type = zstrdup(type);                                         //  initz. widget struct
   zd->widget[iiw].name = zstrdup(name);
   zd->widget[iiw].pname = zstrdup(pname);
   zd->widget[iiw].data = 0;
   zd->widget[iiw].cblist = 0;
   zd->widget[iiw].size = size;
   zd->widget[iiw].homog = homog;
   zd->widget[iiw].expand = expand;
   zd->widget[iiw].space = space;
   zd->widget[iiw].wrap = wrap;
   zd->widget[iiw].widget = 0;

   zd->widget[iiw+1].type = 0;                                                   //  set new EOF marker

   if (strmatchV(type,"dialog","hbox","vbox","hsep","vsep","frame","scrwin",     //  added 'z' widgets
                      "label","link","entry","zentry","edit","text",
                      "button","zbutton","togbutt","check","radio",
                      "imagebutt","colorbutt","combo","comboE","spin","zspin",
                      "hscale","vscale","icon","image",null) == 0) {
      printz("*** zdialog, bad widget type: %s \n",type);
      return 0;
   }
   
   for (iip = iiw-1; iip >= 0; iip--)                                            //  find parent (container) widget
      if (strmatch(pname,zd->widget[iip].name)) break;
   if (iip < 0) zappcrash("zdialog, no parent for widget: %s",name);

   pwidget = zd->widget[iip].widget;                                             //  parent widget, type
   ptype = zd->widget[iip].type;

   if (strmatchV(ptype,"dialog","hbox","vbox","frame","scrwin",null) == 0)
      zappcrash("zdialog, bad widget parent type: %s",ptype);

   if (strmatch(type,"hbox")) widget = gtk_box_new(HORIZONTAL,space);            //  expandable container boxes
   if (strmatch(type,"vbox")) widget = gtk_box_new(VERTICAL,space);
   if (strstr("hbox vbox",type))
      gtk_box_set_homogeneous(GTK_BOX(widget),homog);

   if (strmatch(type,"hsep")) widget = gtk_separator_new(HORIZONTAL);            //  horiz. & vert. separators
   if (strmatch(type,"vsep")) widget = gtk_separator_new(VERTICAL);

   if (strmatch(type,"frame")) {                                                 //  frame around contained widgets
      widget = gtk_frame_new(data);
      gtk_frame_set_shadow_type(GTK_FRAME(widget),GTK_SHADOW_IN);
      data = 0;
   }

   if (strmatch(type,"scrwin")) {                                                //  scrolled window container
      widget = gtk_scrolled_window_new(0,0);
      gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(widget),
                        GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
      #if GTK_CHECK_VERSION(3,16,0)
      gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(widget),0);
      #endif
      data = 0;
   }

   if (strmatch(type,"label")) {                                                 //  label (static text)
      widget = gtk_label_new(data);
      if (size) gtk_label_set_width_chars(GTK_LABEL(widget),size);
      if (data && strstr(data,"<span")) 
         gtk_label_set_markup(GTK_LABEL(widget),data);
      data = 0;
   }

   if (strmatch(type,"link")) {                                                  //  label is clickable
      if (strmatch(name,"nolabel"))
         widget = gtk_link_button_new(data);                                     //  link is also label
      else
         widget = gtk_link_button_new_with_label(data,name);                     //  label >> link
      G_SIGNAL(widget,"clicked",zdialog_widget_event,zd);
      data = 0;
   }

   if (strmatch(type,"entry")) {                                                 //  text input, single line
      widget = gtk_entry_new();
      if (data) gtk_entry_set_text(GTK_ENTRY(widget),data);
      if (size) gtk_entry_set_width_chars(GTK_ENTRY(widget),size);
      G_SIGNAL(widget,"changed",zdialog_widget_event,zd);
   }

   if (strmatch(type,"zentry")) {                                                //  text input, single line
      widget = gtk_text_view_new();
      #if GTK_CHECK_VERSION(3,18,0)
      gtk_text_view_set_top_margin(GTK_TEXT_VIEW(widget),2);
      gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(widget),2); 
      #endif
      gtk_text_view_set_left_margin(GTK_TEXT_VIEW(widget),5);
      if (! size) size = 10;                                                     //  scale widget for font size
      gtk_widget_set_size_request(widget,size*appfontsize,2*appfontsize);
      gtk_text_view_set_editable(GTK_TEXT_VIEW(widget),1);
      gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(widget),GTK_WRAP_NONE);
      gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(widget),0);
      editBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      if (data) gtk_text_buffer_set_text(editBuff,data,-1);
      G_SIGNAL(editBuff,"changed",zdialog_widget_event,zd);                      //  buffer signals, not widget
   }

   if (strmatch(type,"edit")) {                                                  //  text input, opt. multi-line
      widget = gtk_text_view_new();
      #if GTK_CHECK_VERSION(3,18,0)
      gtk_text_view_set_top_margin(GTK_TEXT_VIEW(widget),2);
      gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(widget),2); 
      #endif
      gtk_text_view_set_left_margin(GTK_TEXT_VIEW(widget),5);
      if (! size) size = 10;                                                     //  scale widget for font size
      gtk_widget_set_size_request(widget,size*appfontsize,2*appfontsize);
      gtk_text_view_set_editable(GTK_TEXT_VIEW(widget),1);
      if (wrap) gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(widget),GTK_WRAP_WORD);
      gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(widget),0);
      editBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      if (data) gtk_text_buffer_set_text(editBuff,data,-1);
      G_SIGNAL(editBuff,"changed",zdialog_widget_event,zd);                      //  buffer signals, not widget
   }

   if (strmatch(type,"text")) {                                                  //  text output (not editable)
      widget = gtk_text_view_new();
      #if GTK_CHECK_VERSION(3,18,0)
      gtk_text_view_set_top_margin(GTK_TEXT_VIEW(widget),2);
      gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(widget),2); 
      #endif
      gtk_text_view_set_left_margin(GTK_TEXT_VIEW(widget),3);
      editBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      if (data) gtk_text_buffer_set_text(editBuff,data,-1);
      gtk_text_view_set_editable(GTK_TEXT_VIEW(widget),0);
      if (wrap) gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(widget),GTK_WRAP_WORD);
   }

   if (strmatch(type,"button")) {                                                //  button
      widget = gtk_button_new_with_label(data);
      G_SIGNAL(widget,"clicked",zdialog_widget_event,zd);
      data = 0;
   }
   
   if (strmatch(type,"zbutton")) {                                               //  checkbox used as small button
      if (data) widget = gtk_check_button_new_with_label(data);
      else  widget = gtk_check_button_new();
      G_SIGNAL(widget,"toggled",zdialog_widget_event,zd);
      data = "0";                                                                //  default data
   }

   if (strmatch(type,"togbutt")) {                                               //  toggle button
      widget = gtk_toggle_button_new_with_label(data);
      G_SIGNAL(widget,"toggled",zdialog_widget_event,zd);
      data = "0";                                                                //  default data
   }
   
   if (strmatch(type,"imagebutt")) {                                             //  button with image
      snprintf(iconpath,200,"%s/%s",get_zimagedir(),data);
      data = 0;
      pixbuf = gdk_pixbuf_new_from_file_at_scale(iconpath,size,size,1,&gerror);
      if (pixbuf) {
         image = gtk_image_new_from_pixbuf(pixbuf);
         g_object_unref(pixbuf);
      }
      else image = gtk_image_new_from_icon_name("missing",GTK_ICON_SIZE_BUTTON);
      widget = gtk_button_new_with_label(data);
      gtk_button_set_image(GTK_BUTTON(widget),image);
      G_SIGNAL(widget,"clicked",zdialog_widget_event,zd);
   }

   if (strmatch(type,"colorbutt")) {                                             //  color edit button
      if (! data) data = "0|0|0";                                                //  data format: "nnn|nnn|nnn" = RGB
      pp = strField(data,'|',1); gdkrgba.red = f256 * atoi(pp);                  //  RGB values are 0-1
      pp = strField(data,'|',2); gdkrgba.green = f256 * atoi(pp);
      pp = strField(data,'|',3); gdkrgba.blue = f256 * atoi(pp);
      gdkrgba.alpha = 1.0;
      widget = gtk_color_button_new_with_rgba(&gdkrgba);
      G_SIGNAL(widget,"color-set",zdialog_widget_event,zd);
   }

   if (strmatch(type,"check")) {                                                 //  checkbox
      if (data) widget = gtk_check_button_new_with_label(data);
      else  widget = gtk_check_button_new();
      G_SIGNAL(widget,"toggled",zdialog_widget_event,zd);
      data = "0";                                                                //  default data
   }

   if (strmatch(type,"combo")) {                                                 //  combo box
      widget = gtk_combo_box_text_new();
      zd->widget[iiw].cblist = pvlist_create(zdcbmax);                           //  for drop-down list
      if (! blank_null(data)) {
         pvlist_append(zd->widget[iiw].cblist,data);                             //  add data to drop-down list
         gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widget),data);
         gtk_combo_box_set_active(GTK_COMBO_BOX(widget),0);
      }
      G_SIGNAL(widget,"changed",zdialog_widget_event,zd);
   }

   if (strmatch(type,"comboE")) {                                                //  combo box with entry box
      widget = gtk_combo_box_text_new_with_entry();
      entry = gtk_bin_get_child(GTK_BIN(widget));
      if (! size) size = 20;
      gtk_entry_set_width_chars(GTK_ENTRY(entry),size);
      zd->widget[iiw].cblist = pvlist_create(zdcbmax);                           //  for drop-down list
      if (! blank_null(data)) {
         gtk_entry_set_text(GTK_ENTRY(entry),data);                              //  entry = initial data
         pvlist_append(zd->widget[iiw].cblist,data);                             //  add data to drop-down list
         gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widget),data);
      }
      G_SIGNAL(widget,"changed",zdialog_widget_event,zd);
   }

   if (strmatch(type,"radio")) {                                                 //  radio button
      for (kk = iip+1; kk <= iiw; kk++)
         if (strmatch(zd->widget[kk].pname,pname) &&                             //  find first radio button
             strmatch(zd->widget[kk].type,"radio")) break;                       //    with same container
      if (kk == iiw)
         widget = gtk_radio_button_new_with_label(null,data);                    //  this one is first
      else
         widget = gtk_radio_button_new_with_label_from_widget                    //  not first, add to group
              (GTK_RADIO_BUTTON(zd->widget[kk].widget),data);
      G_SIGNAL(widget,"toggled",zdialog_widget_event,zd);
      data = "0";                                                                //  default data
   }

   if (strmatchV(type,"spin","hscale","vscale",null)) {                          //  spin button or sliding scale
      if (! data) zappcrash("zdialog_add_widget(): data missing");               //  "min|max|step|value"
      pp = strField(data,'|',1); err = convSD(pp,min);
      pp = strField(data,'|',2); err += convSD(pp,max);
      pp = strField(data,'|',3); err += convSD(pp,step);
      pp = strField(data,'|',4); err += convSD(pp,val);
      if (err) { min = 0; max = 100; step = 1; val = 50; }

      if (*type == 's') {
         widget = gtk_spin_button_new_with_range(min,max,step);
         gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget),val);
      }
      if (*type == 'h') {
         widget = gtk_scale_new_with_range(HORIZONTAL,min,max,step);
         gtk_range_set_value(GTK_RANGE(widget),val);
         gtk_scale_set_draw_value(GTK_SCALE(widget),0);
      }
      if (*type == 'v') {
         widget = gtk_scale_new_with_range(VERTICAL,min,max,step);
         gtk_range_set_value(GTK_RANGE(widget),val);
         gtk_scale_set_draw_value(GTK_SCALE(widget),0);
      }
      G_SIGNAL(widget,"value-changed",zdialog_widget_event,zd);
      snprintf(vdata,30,"%g",val);
      data = vdata;
   }
   
   if (strmatch(type,"zspin")) {                                                 //  "zspin" widget with range
      if (! data) zappcrash("zdialog_add_widget(): data missing");               //  "min|max|step|value"
      pp = strField(data,'|',1); err = convSD(pp,min);
      pp = strField(data,'|',2); err += convSD(pp,max);
      pp = strField(data,'|',3); err += convSD(pp,step);
      pp = strField(data,'|',4); err += convSD(pp,val);
      if (err) { min = 0; max = 100; step = 1; val = 50; }
      zd->widget[iiw].lolim = min;
      zd->widget[iiw].hilim = max;
      zd->widget[iiw].step = step;
      err = convDS(val,6,vdata);                                                 //  initial value >> text
      data = vdata;
      widget = gtk_text_view_new();                                              //  GTK widget is text_view
      #if GTK_CHECK_VERSION(3,18,0)
      gtk_text_view_set_top_margin(GTK_TEXT_VIEW(widget),2);
      #endif
      gtk_text_view_set_left_margin(GTK_TEXT_VIEW(widget),5);
      if (! size) size = 5;                                                      //  scale widget for font size
      gtk_widget_set_size_request(widget,size*appfontsize,2*appfontsize);
      gtk_text_view_set_editable(GTK_TEXT_VIEW(widget),1);
      gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(widget),GTK_WRAP_NONE);
      gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(widget),0);
      gtk_text_view_set_input_purpose(GTK_TEXT_VIEW(widget),GTK_INPUT_PURPOSE_NUMBER);
      editBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      gtk_text_buffer_set_text(editBuff,data,-1);
      G_SIGNAL(widget,"key-press-event",zdialog_zspin_event,zd);
      G_SIGNAL(widget,"focus-out-event",zdialog_zspin_event,zd);
      gtk_widget_add_events(widget,GDK_SCROLL_MASK);
      G_SIGNAL(widget,"scroll-event",zdialog_zspin_event,zd);
   }

   if (strmatch(type,"icon")) {                                                  //  image widget from icon
      snprintf(iconpath,200,"%s/%s",get_zimagedir(),data);
      data = 0;                                                                  //  data not further used
      pixbuf = gdk_pixbuf_new_from_file_at_scale(iconpath,size,size,1,&gerror);
      if (pixbuf) {
         widget = gtk_image_new_from_pixbuf(pixbuf);
         g_object_unref(pixbuf);
      }
      else widget = gtk_image_new_from_icon_name("missing",GTK_ICON_SIZE_BUTTON);
   }

   if (strmatch(type,"image"))                                                   //  image widget from pixbuf
      widget = gtk_image_new_from_pixbuf((GdkPixbuf *) data);                    //  use (cchar *) pixbuf in call

   //  all widget types come here

   zd->widget[iiw].widget = widget;                                              //  set widget in zdialog
   
   if (strmatchV(type,"zentry","zspin","edit",0)) {                              //  add frame around these widgets
      fwidget = gtk_frame_new(0);                                                //  removed "text"
      gtk_frame_set_shadow_type(GTK_FRAME(fwidget),GTK_SHADOW_IN);
      gtk_container_add(GTK_CONTAINER(fwidget),widget);
      widget = fwidget;
   }

   if (strmatch(ptype,"hbox") || strmatch(ptype,"vbox"))                         //  add to hbox/vbox
      gtk_box_pack_start(GTK_BOX(pwidget),widget,expand,expand,space);
   if (strmatch(ptype,"frame"))                                                  //  add to frame
      gtk_container_add(GTK_CONTAINER(pwidget),widget);
   if (strmatch(ptype,"scrwin"))                                                 //  add to scroll window
      gtk_container_add(GTK_CONTAINER(pwidget),widget);
   if (strmatch(ptype,"dialog")) {                                               //  add to dialog box
      vbox = gtk_bin_get_child(GTK_BIN(pwidget));                                //  dialog is a gtkwindow
      gtk_box_pack_start(GTK_BOX(vbox),widget,expand,expand,space);
   }
   if (data) zd->widget[iiw].data = zstrdup(data);                               //  widget memory

   return 0;
}


//  add widget to existing zdialog - alternative form (clearer and easier code)
//  options: "size=nn | homog | expand | space=nn | wrap"  (all optional, any order)

int zdialog_add_widget(zdialog *zd, cchar *type, cchar *name,
                       cchar *parent, cchar *data, cchar *options)
{
   int      stat, size = 0, homog = 0, expand = 0, space = 0, wrap = 0, begin = 1;
   char     pname[8];
   double   pval;

   while (true)
   {
      stat = strParms(begin,options,pname,8,pval);
      if (stat == -1) break;
      if (stat == 1) zappcrash("bad zdialog options: %s",options);
      if (strmatch(pname,"size")) size = (int(pval));
      else if (strmatch(pname,"homog")) homog = 1;
      else if (strmatch(pname,"expand")) expand = 1;
      else if (strmatch(pname,"space")) space = (int(pval));
      else if (strmatch(pname,"wrap")) wrap = 1;
      else zappcrash("bad zdialog options: %s",options);
   }

   stat = zdialog_add_widget(zd,type,name,parent,data,size,homog,expand,space,wrap);
   return stat;
}


//  return 1/0 if zdialog is valid/invalid

int zdialog_valid(zdialog *zd, cchar *title)
{
   int      ok, ii;

   if (! zd) return 0;

   for (ii = 0; ii < zdialog_count; ii++)                                        //  find in valid zdialog list
      if (zd == zdialog_list[ii]) break;
   if (ii == zdialog_count) return 0;

   ok = 1;
   if ((zd->sentinel1 & 0xFFFF0000) != zdsentinel) ok = 0;
   if (zd->sentinel2 != zd->sentinel1) ok = 0;
   if (! ok) {
      printz("*** zdialog sentinel invalid %p \n",zd);
      return 0;
   }
   
   if (title && ! strmatch(title,zd->title)) return 0;

   return 1;
}


//  get GTK widget from zdialog and widget name

GtkWidget * zdialog_widget(zdialog *zd, cchar *name)
{
   if (! zdialog_valid(zd)) return 0;
   for (int ii = 0; zd->widget[ii].type; ii++)
      if (strmatch(zd->widget[ii].name,name))
         return zd->widget[ii].widget;
   return 0;
}


//  set an "image" widget type from a GDK pixbuf
//  returns 0 if OK, else +N

int zdialog_set_image(zdialog *zd, cchar *name, GdkPixbuf *pixbuf)
{
   GtkWidget   *widget;
   int         ii;

   if (! zdialog_valid(zd)) return 1;
   for (ii = 0; zd->widget[ii].type; ii++)
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 2;
   if (! strmatch(zd->widget[ii].type,"image")) return 3;
   widget = zd->widget[ii].widget;
   gtk_image_set_from_pixbuf(GTK_IMAGE(widget),pixbuf);
   return 0;
}


//  add a popup tool tip to a zdialog widget

int zdialog_add_ttip(zdialog *zd, cchar *wname, cchar *ttip)
{
   GtkWidget   *widget;
   int         ii;

   if (! zdialog_valid(zd)) return 0;

   for (ii = 0; zd->compwidget[ii]; ii++)                                        //  search completion buttons
      if (strmatch(zd->compbutton[ii],wname)) {                                  //    for matching name
         gtk_widget_set_tooltip_text(zd->compwidget[ii],ttip);
         return 1;
      }

   widget = zdialog_widget(zd,wname);                                            //  search zdialog widgets
   if (! widget) {
      printz(" *** zdialog widget invalid %s \n",wname);
      return 0;
   }

   gtk_widget_set_tooltip_text(widget,ttip);
   return 1;
}


//  set a common group for a set of radio buttons
//  (GTK, this does not work)

int zdialog_set_group(zdialog *zd, cchar *radio1, ...)
{
   va_list        arglist;
   cchar          *radio2;
   GtkWidget      *gwidget, *widget;
   GSList         *glist;

   gwidget = zdialog_widget(zd,radio1);
   glist = gtk_radio_button_get_group(GTK_RADIO_BUTTON(gwidget));
   if (! glist) zappcrash("no radio button group");

   va_start(arglist,radio1);

   while (true)
   {
      radio2 = va_arg(arglist,cchar *);
      if (! radio2) break;
      widget = zdialog_widget(zd,radio2);
      gtk_radio_button_set_group(GTK_RADIO_BUTTON(widget),glist);
   }

   va_end(arglist);

   return 0;
}


//  resize dialog to a size greater than initial size
//  (as determined by the included widgets)

int zdialog_resize(zdialog *zd, int width, int height)
{
   if (! zdialog_valid(zd)) return 0;

   if (! width) width = 10;                                                      //  stop spurious GTK warnings
   if (! height) height = 10;
   GtkWidget *window = zd->widget[0].widget;
   gtk_window_set_default_size(GTK_WINDOW(window),width,height);
   return 1;
}


//  put data into a zdialog widget
//  private function

int zdialog_put_data(zdialog *zd, cchar *name, cchar *data)
{
   GtkWidget      *widget, *entry;
   GtkTextBuffer  *textBuff;
   GdkRGBA        gdkrgba;
   int            iiw, nn, kk, err, Nsteps;
   cchar          *type, *pp;
   char           *wdata, sdata[12];
   double         dval;
   double         f256 = 1.0 / 256.0;
   double         lval, hval, nval, F, F2;
   double         fdata, lolim, hilim, step;                                     //  double

   if (! zdialog_valid(zd)) return 0;

   if (! name || ! *name) zappcrash("zdialog_put_data(), name null");

   for (iiw = 1; zd->widget[iiw].type; iiw++)                                    //  find widget
      if (strmatch(zd->widget[iiw].name,name)) break;
   if (! zd->widget[iiw].type) {
      printz("*** zdialog_put_data(), bad name %s \n",name);
      return 0;
   }

   type = zd->widget[iiw].type;
   widget = zd->widget[iiw].widget;

   wdata = zd->widget[iiw].data;
   if (wdata) zfree(wdata);                                                      //  free prior data memory
   zd->widget[iiw].data = 0;

   if (data) {
      if (utf8_check(data)) wdata = zstrdup("bad UTF8 data");                    //  replace bad UTF-8 encoding
      else wdata = zstrdup(data);                                                //  set new data for widget
      zd->widget[iiw].data = wdata;
   }

   zd->disabled++;                                                               //  disable for widget stuffing
   
   if (strmatch(type,"label"))
      gtk_label_set_text(GTK_LABEL(widget),data);

   if (strmatch(type,"link"))
      gtk_label_set_text(GTK_LABEL(widget),data);

   if (strmatch(type,"entry"))
      gtk_entry_set_text(GTK_ENTRY(widget),data);

   if (strmatch(type,"zentry")) {                                                //  text input, single line
      textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      gtk_text_buffer_set_text(textBuff,data,-1);
   }

   if (strmatchV(type,"button","zbutton",null))                                  //  change button label
      gtk_button_set_label(GTK_BUTTON(widget),data);

   if (strmatch(type,"edit")) {                                                  //  text input to editable text
      textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      gtk_text_buffer_set_text(textBuff,data,-1);
   }

   if (strmatch(type,"text")) {                                                  //  text output
      textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      gtk_text_buffer_set_text(textBuff,data,-1);
   }

   if (strmatchV(type,"togbutt","check","radio",null))
   {
      if (! data) kk = nn = 0;
      else kk = convSI(data,nn);
      if (kk != 0) nn = 0;                                                       //  data not integer, force zero
      if (nn <= 0) nn = 0; else nn = 1;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),nn);                //  set gtk widget value
   }

   if (strmatch(type,"spin")) {
      kk = convSD(data,dval);
      if (kk != 0) dval = 0.0;
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget),dval);
   }
   
   if (strmatch(type,"zspin")) {                                                 //  "zspin" widget
      lolim = zd->widget[iiw].lolim;
      hilim = zd->widget[iiw].hilim;
      step = zd->widget[iiw].step;
      err = convSD(data,fdata);                                                  //  string --> double
      if (err) goto retx;
      Nsteps = (fdata - lolim) / step + 0.5;                                     //  nearest exact step
      fdata = lolim + Nsteps * step;
      if (fdata < lolim) fdata = lolim;                                          //  enforce limits
      if (fdata > hilim) fdata = hilim;
      convDS(fdata,6,sdata);                                                     //  double --> string, precision 6
      textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      gtk_text_buffer_set_text(textBuff,sdata,-1);
   }

   if (strmatch(type,"colorbutt")) {                                             //  color button data is nnn|nnn|nnn
      pp = strField(data,'|',1);
      if (pp) gdkrgba.red = f256 * atoi(pp);                                     //  RGB range is 0-1
      pp = strField(data,'|',2);
      if (pp) gdkrgba.green = f256 * atoi(pp);
      pp = strField(data,'|',3);
      if (pp) gdkrgba.blue = f256 * atoi(pp);
      gdkrgba.alpha = 1.0;
      gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(widget),&gdkrgba);
   }

   if (strmatchV(type,"hscale","vscale",null))                                   //  slider widget
   {
      kk = convSD(data,dval);                                                    //  zdialog widget value
      if (kk != 0) dval = 0.0;

      if (zd->widget[iiw].rescale)                                               //  widget value --> slider value
      {
         lval = zd->widget[iiw].lval;                                            //  rescaled for more sensitivity
         nval = zd->widget[iiw].nval;                                            //    around neutral value
         hval = zd->widget[iiw].hval;

         if (dval > lval && dval <= nval) {                                      //  if dval == lval or dval == hval
            F2 = (nval - dval) / (nval - lval);                                  //    then dval is not revised
            F = sqrtf(F2);
            dval = nval - F * (nval - lval);
         }
         
         else if (dval >= nval && dval < hval) {
            F2 = (dval - nval) / (hval - nval);
            F = sqrtf(F2);
            dval = nval + F * (hval - nval);
         }
      }

      gtk_range_set_value(GTK_RANGE(widget),dval);
   }

   if (strmatch(type,"combo")) {
      if (! blank_null(data)) {
         kk = pvlist_prepend(zd->widget[iiw].cblist,data,1);                     //  add to drop-down list
         if (kk == 0)                                                            //  (only if unique)
            gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(widget),data);
         kk = pvlist_find(zd->widget[iiw].cblist,data);
         gtk_combo_box_set_active(GTK_COMBO_BOX(widget),kk);                     //  make the active entry
      }
      else gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);                   //  make no active entry
   }

   if (strmatch(type,"comboE")) {
      entry = gtk_bin_get_child(GTK_BIN(widget));
      if (! blank_null(data)) {
         kk = pvlist_prepend(zd->widget[iiw].cblist,data,1);                     //  add to drop-down list
         if (kk == 0)                                                            //  (only if unique)
            gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(widget),data);
         gtk_entry_set_text(GTK_ENTRY(entry),data);                              //  stuff entry box with new data
      }
      else gtk_entry_set_text(GTK_ENTRY(entry),"");                              //  stuff entry box with nothing
   }

retx:
   zd->disabled--;                                                               //  re-enable dialog
   return iiw;
}


//  get data from a dialog widget based on its name
//  private function

cchar * zdialog_get_data(zdialog *zd, cchar *name)
{
   if (! zdialog_valid(zd)) return 0;

   for (int ii = 1; zd->widget[ii].type; ii++)
      if (strmatch(zd->widget[ii].name,name))
            return zd->widget[ii].data;
   return 0;
}


//  set new limits for a numeric data entry widget (spin, zspin, hscale, vscale)

int zdialog_set_limits(zdialog *zd, cchar *name, double min, double max)
{
   GtkWidget   *widget;
   cchar       *type;
   int         iiw;

   if (! zdialog_valid(zd)) return 0;

   for (iiw = 1; zd->widget[iiw].type; iiw++)
      if (strmatch(name,zd->widget[iiw].name)) break;
   if (! zd->widget[iiw].type) {
      printz("*** zdialog_set_limits, %s not found \n",name);
      return 0;
   }

   widget = zd->widget[iiw].widget;
   type = zd->widget[iiw].type;

   if (*type == 's')
      gtk_spin_button_set_range(GTK_SPIN_BUTTON(widget),min,max);

   if (*type == 'h' || *type == 'v')
      gtk_range_set_range(GTK_RANGE(widget),min,max);
   
   if (*type == 'z') {                                                           //  zspin
      zd->widget[iiw].lval = min;
      zd->widget[iiw].hval = max;
   }

   return 1;
}


//  Expand a widget scale in the region around the neutral value.
//  Control small adjustments near the neutral value more precisely.
//  lval and hval: the range of values to be rescaled.
//  nval: the neutral value where the scale will be expanded the most.
//        lval <= nval <= hval 

int zdialog_rescale(zdialog *zd, cchar *name, float lval, float nval, float hval)
{
   int         iiw;

   if (! zdialog_valid(zd)) return 0;

   for (iiw = 1; zd->widget[iiw].type; iiw++)
      if (strmatch(name,zd->widget[iiw].name)) break;
   if (! zd->widget[iiw].type) {
      printz("*** zdialog_rescale, %s not found \n",name);
      return 0;
   }
   
   if (lval > nval || nval > hval) {
      printz("*** zdialog_rescale, bad data: %s \n",name);
      return 0;
   }

   zd->widget[iiw].rescale = 1;
   zd->widget[iiw].lval = lval;
   zd->widget[iiw].nval = nval;
   zd->widget[iiw].hval = hval;

   return 1;
}


//  run the dialog and send events to the event function
//
//  evfunc: int func(zdialog *zd, cchar *event)
//  If present, eventFunc is called when a dialog widget is changed or the dialog
//  is completed. If a widget was changed, event is the widget name.
//  Get the new widget data with zdialog_fetch().
//  If a completion button was pressed, event is "zstat" and zd->zstat will be
//  the button number 1-N.
//  If the dialog was destroyed, event is "zstat" and zd->zstat is negative.
//
//  posn: optional dialog box position:
//    "mouse" = position at mouse
//    "desktop" = center on desktop
//    "parent" = center on parent window
//    "nn/nn" = position NW corner at relative x/y position in parent window,
//              where nn/nn is a percent 0-100 of the parent window dimensions.
//    "save" = save last user-set position and use this whenever the dialog 
//             is repeated, also across sessions.  DEFAULT. 
//
//  KBevent: extern void KBevent(GdkEventKey *event)
//  This function must be supplied by the caller of zdialog.
//  It is called when Ctrl|Shift|Alt|F1 is pressed.

int zdialog_run(zdialog *zd, zdialog_event evfunc, cchar *posn)
{
   int   zdialog_KB_press(GtkWidget *, GdkEventKey *event, zdialog *zd);
   int   zdialog_focus_event(GtkWidget *, GdkEvent *event, zdialog *zd);

   GtkWidget   *dialog;

   if (! zdialog_valid(zd)) zappcrash("zdialog invalid");
   if (zd->zrunning) {
      printz("zdialog is already running \n");
      return 0;
   }

   if (posn) zdialog_set_position(zd,posn);                                      //  put dialog at desired position
   else zdialog_set_position(zd,"save");                                         //  use default

   if (evfunc) zd->eventCB = (void *) evfunc;                                    //  link to dialog event callback

   dialog = zd->widget[0].widget;
   gtk_widget_show_all(dialog);                                                  //  activate dialog

   G_SIGNAL(dialog,"focus-in-event",zdialog_focus_event,zd);                     //  connect focus event function 
   G_SIGNAL(dialog,"key-press-event",zdialog_KB_press,zd);                       //  connect key press event function
   G_SIGNAL(dialog,"delete-event",zdialog_delete_event,zd);                      //  connect delete event function

   zd->zstat = 0;                                                                //  dialog status incomplete
   zd->disabled = 0;                                                             //  enable widget events
   zd->zrunning = 1;                                                             //  dialog is running
   zfuncs::zdialog_busy++;                                                       //  count open zdialogs
   return 0;
}


//  zdialog event handler - called for dialog events.
//  Updates data in zdialog, calls user callback function (if present).
//  private function

int zdialog_widget_event(GtkWidget *widget, zdialog *zd)
{
   zdialog_event  *evfunc = 0;                                                   //  dialog event callback function

   GtkTextView       *textView = 0;
   GtkTextBuffer     *textBuff = 0;
   GtkTextIter       iter1, iter2;
   GdkRGBA           gdkrgba;
   GtkWidget         *entry;
   int               ii, nn;
   cchar             *wname, *wtype, *wdata;
   char              sdata[20];
   double            dval;
   float             lval, nval, hval, F;
   static int        cbadded = 0;
   
   if (! zdialog_valid(zd)) return 1;
   if (zd->disabled) return 1;                                                   //  events disabled

   zd->disabled = 1;

   for (ii = 0; ii < zdmaxbutts; ii++) {                                         //  check completion buttons
      if (zd->compwidget[ii] == null) break;                                     //  EOL
      if (zd->compwidget[ii] != widget) continue;
      zd->zstat = ii+1;                                                          //  zdialog status = button no.
      strncpy0(zd->event,"zstat",40);
      strncpy0(zd->zstat_button,zd->compbutton[ii],40);                          //  button label "cancel" etc.
      wtype = "completion button";
      goto call_evfunc;                                                          //  call zdialog event function
   }

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget in zdialog
      if (zd->widget[ii].widget == widget) goto found_widget;

   for (ii = 1; zd->widget[ii].type; ii++) {                                     //  failed, test if buffer
      if (strmatchV(zd->widget[ii].type,"edit","zentry",null)) {                 //    of text view widget
         textView = GTK_TEXT_VIEW(zd->widget[ii].widget);
         textBuff = gtk_text_view_get_buffer(textView);
         if (widget == (GtkWidget *) textBuff) goto found_widget;
      }
   }

   printz("*** zdialog event ignored \n");                                       //  not found, ignore event
   zd->disabled = 0;
   return 1;

found_widget:

   wname = zd->widget[ii].name;
   wtype = zd->widget[ii].type;
   wdata = 0;

   if (strmatch(wtype,"button")) 
      wdata = gtk_button_get_label(GTK_BUTTON(widget));                          //  button label

   if (strmatch(wtype,"zbutton")) {                                              //  checkbox as smaller button
      wdata = gtk_button_get_label(GTK_BUTTON(widget));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),0);                 //  reset checkmark = off
   }

   if (strmatch(wtype,"edit")) {
      gtk_text_buffer_get_bounds(textBuff,&iter1,&iter2);
      wdata = gtk_text_buffer_get_text(textBuff,&iter1,&iter2,0);
   }

   if (strmatch(wtype,"entry"))
      wdata = gtk_entry_get_text(GTK_ENTRY(widget));

   if (strmatch(wtype,"zentry")) {
      gtk_text_buffer_get_bounds(textBuff,&iter1,&iter2);
      wdata = gtk_text_buffer_get_text(textBuff,&iter1,&iter2,0);
   }

   if (strmatchV(wtype,"radio","check","togbutt",null))
   {
      nn = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
      if (nn == 0) wdata = "0";
      else wdata = "1";
   }

   if (strmatch(wtype,"combo"))
         wdata = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));

   if (strmatch(wtype,"comboE"))
   {
      if (widget == zd->lastwidget && cbadded) {
         pvlist_remove(zd->widget[ii].cblist,0);                                 //  detect multiple edits (keystrokes)
         gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(widget),0);                //    amd replace with new entry
      }
      entry = gtk_bin_get_child(GTK_BIN(widget));
      wdata = gtk_entry_get_text(GTK_ENTRY(entry));
      cbadded = 0;
      if (! blank_null(wdata)) {
         nn = pvlist_prepend(zd->widget[ii].cblist,wdata,1);                     //  add entry to drop-down list
         if (nn == 0) {                                                          //  (only if unique)
            gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(widget),wdata);
            cbadded = 1;
         }
      }
   }

   if (strmatch(wtype,"spin"))
   {
      dval = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
      snprintf(sdata,20,"%g",dval);
      wdata = sdata;
   }
   
   if (strmatch(wtype,"colorbutt"))                                              //  color button
   {
      gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget),&gdkrgba);
      snprintf(sdata,20,"%.0f|%.0f|%.0f",gdkrgba.red*255,gdkrgba.green*255,gdkrgba.blue*255);
      wdata = sdata;
   }

   if (strmatchV(wtype,"hscale","vscale",null))
   {
      dval = gtk_range_get_value(GTK_RANGE(widget));

      if (zd->widget[ii].rescale)                                                //  slider value --> widget value
      {
         lval = zd->widget[ii].lval;
         nval = zd->widget[ii].nval;
         hval = zd->widget[ii].hval;

         if (dval > lval && dval < nval) {                                       //  lval ... nval
            F = (nval - dval) / (nval - lval);                                   //  1 ... 0
            dval = (1.0 - F * F) * (nval - lval) + lval;                         //  lval ... nval
         }

         else if (dval > nval && dval < hval) {                                  //  nval ... hval
            F = (dval - nval) / (hval - nval);                                   //  0 ... 1
            dval = F * F * (hval - nval) + nval;                                 //  nval ... hval
         }
      }

      snprintf(sdata,20,"%g",dval);
      wdata = sdata;
   }

   //  all widgets come here

   if (zd->widget[ii].data) zfree(zd->widget[ii].data);                          //  clear prior data
   zd->widget[ii].data = 0;
   if (wdata) zd->widget[ii].data = zstrdup(wdata);                              //  set new data
   zd->lastwidget = widget;                                                      //  remember last widget updated
   strncpy0(zd->event,wname,40);                                                 //  event = widget name

call_evfunc:                                                                     //  call zdialog event function

   if (zd->eventCB) {
      if (! zdialog_valid(zd)) zexit("%s event for invalid zdialog",wtype);
      evfunc = (zdialog_event *) zd->eventCB;                                    //  do callback function
      evfunc(zd,zd->event);
   }

   if (zdialog_valid(zd)) zd->disabled = 0;                                      //  'event' may cause zdialog_free()
   return 1;
}


//  zdialog response handler for "focus-in-event" signal
//  private function

zdialog  *zdialog_focus_zd;                                                      //  current zdialog

int zdialog_focus_event(GtkWidget *, GdkEvent *event, zdialog *zd)
{
   if (! zdialog_valid(zd)) return 0;
   if (zd->zstat) return 0;                                                      //  already complete
   zdialog_focus_zd = zd;
   zdialog_send_event(zd,"focus");                                               //  notify dialog event function
   return 0;                                                                     //  must be 0
}


//  set KB shortcuts for common zdialog completion buttons

int    Nkbshortcuts = 0;
cchar  *kbshortcuts[10], *kbevents[10];

void zdialog_KB_addshortcut(cchar *shortcut, cchar *event)                       //  e.g. ("Ctrl+D", "Done") 
{
   if (Nkbshortcuts > 9) return;
   kbshortcuts[Nkbshortcuts] = zstrdup(shortcut);
   kbevents[Nkbshortcuts] = zstrdup(event);
   ++Nkbshortcuts;
   return;
}


//  zdialog response handler for keyboard events
//  key symbols can be found at /usr/include/gtk-3.0/gdk/gdkkeysyms.h
//  main app must provide: extern void KBevent(GdkEventKey *event)
//  private function

int zdialog_KB_press(GtkWidget *widget, GdkEventKey *kbevent, zdialog *zd)
{
   void  zdialog_copyfunc(GtkWidget *, GtkClipboard *);
   void  zdialog_pastefunc(GtkClipboard *, cchar *, void *);
   zdialog_event  *evfunc = 0;                                                   //  dialog event callback function

   GtkWidget      *focuswidget;
   int      KB_Ctrl = 0, KB_Alt = 0;                                             //  track state of Ctrl and Alt keys
   int      KBkey = kbevent->keyval;
   cchar    *type;
   int      ii, cc, Ftext;
   
   if (kbevent->state & GDK_CONTROL_MASK) KB_Ctrl = 1;
   if (kbevent->state & GDK_MOD1_MASK) KB_Alt = 1;

   if (KBkey == GDK_KEY_Escape) {                                                //  ESC key, treat as [x]
      zdialog_delete_event(null,null,zd);
      return 1;
   }

   if (KBkey == GDK_KEY_F1) { KBevent(kbevent); return 1; };                     //  these keys handled by main app
   if (KBkey == GDK_KEY_F10) { KBevent(kbevent); return 1; };
   if (KBkey == GDK_KEY_F11) { KBevent(kbevent); return 1; };
   
   for (ii = 0; ii < Nkbshortcuts; ii++) {                                       //  look for dialog button shortcut
      if (strstr(kbshortcuts[ii],"Ctrl") && ! KB_Ctrl) continue;
      if (strstr(kbshortcuts[ii],"Alt") && ! KB_Alt) continue;
      cc = strlen(kbshortcuts[ii]);
      if (KBkey == kbshortcuts[ii][cc-1]) break;                                 //  compare key to last char. in shortcut
      if (KBkey-32 == kbshortcuts[ii][cc-1]) break;                              //  (case indifferent compare)
   }
   
   if (ii < Nkbshortcuts) {
      zdialog_send_event(zd,kbevents[ii]);                                       //  send corresp. event, e.g. "Done"
      return 1;
   }

   focuswidget = gtk_window_get_focus(GTK_WINDOW(widget));                       //  find widget in zdialog

   for (ii = 1; zd->widget[ii].type; ii++)
      if (zd->widget[ii].widget == focuswidget) break;

   type = zd->widget[ii].type;
   if (! type) return 0;
   
   Ftext = strmatchV(type,"zspin","zentry","entry","edit","text","spin",null);   //  text-edit widgets

   if (KBkey == GDK_KEY_Left || KBkey == GDK_KEY_Right) {                        //  left/right arrow key 
      if (! Ftext) {                                                             //  not a text-edit widget,
         KBevent(kbevent);                                                       //    pass key to main()
         return 1; 
      }
   }

   if (KBkey == GDK_KEY_Return && Ftext == 2) {                                  //  zentry widget
      if (zd->eventCB) {
         zd->disabled = 1;
         evfunc = (zdialog_event *) zd->eventCB;                                 //  do callback function
         evfunc(zd,zd->event);
         if (zdialog_valid(zd)) zd->disabled = 0;                                //  'event' may cause zdialog_free()
      }
      return 1;
   }

   return 0;                                                                     //  pass KB key to widget
}


//  event function for "zspin" widget (text entry works as spin button)

int zdialog_zspin_event(GtkWidget *widget, GdkEvent *event, zdialog *zd)
{
   zdialog_event  *evfunc = 0;                                                   //  dialog event callback function

   GtkTextBuffer        *textBuff;
   GtkTextIter          iter1, iter2;

   int         KBkey;
   int         ii, err, Nsteps, state, incr = 0;
   double      fdata, lolim, hilim, step;                                        //  double
   char        *wdata, sdata[20];
   int         time, elaps;
   static int  time0 = 0, time1 = 0;
   
   if (! zdialog_valid(zd)) return 1;
   if (zd->disabled) return 1;                                                   //  zdialog events disabled

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find "zspin" (text view) widget
      if (zd->widget[ii].widget == widget) break;
   if (! zd->widget[ii].type) return 0;                                          //  not found
   
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));                   //  get widget data
   gtk_text_buffer_get_bounds(textBuff,&iter1,&iter2);
   wdata = gtk_text_buffer_get_text(textBuff,&iter1,&iter2,0);
   
   lolim = zd->widget[ii].lolim;                                                 //  limits and step size
   hilim = zd->widget[ii].hilim;
   step = zd->widget[ii].step;

   if (event->type == GDK_SCROLL) {                                              //  mouse wheel event
      gtk_widget_grab_focus(widget);
      incr = - ((GdkEventScroll *) event)->delta_y;
      if (! incr) return 0;
      state = ((GdkEventScroll *) event)->state;                                 //  if shift key held, use 10x step
      if (state & GDK_SHIFT_MASK) incr *= 10;
      goto checklimits;
   }

   if (event->type == GDK_KEY_PRESS) {                                           //  KB button press
      KBkey = ((GdkEventKey *) event)->keyval;
      if (KBkey == GDK_KEY_Return) goto checklimits;                             //  return = entry finished
      if (KBkey == GDK_KEY_Up) incr = 1;
      if (KBkey == GDK_KEY_Down) incr = -1;
      if (! incr) return 0;                                                      //  must return 0
      
      state = ((GdkEventKey *) event)->state;                                    //  if shift key held, use 10x step
      if (state & GDK_SHIFT_MASK) incr *= 10;

      time = ((GdkEventKey *) event)->time;                                      //  track time key is held down
      if (time - time1 > 300) time0 = time;
      time1 = time;
      elaps = time - time0;

      if (elaps > 5000) step = 10 * step;                                        //  acceleration table for
      else if (elaps > 4500) step = 9 * step;                                    //    hold time 1-5+ seconds
      else if (elaps > 4000) step = 8 * step;                                    //  use integer values only
      else if (elaps > 3500) step = 7 * step;
      else if (elaps > 3000) step = 6 * step;
      else if (elaps > 2500) step = 5 * step;
      else if (elaps > 2000) step = 4 * step;
      else if (elaps > 1500) step = 3 * step;
      else if (elaps > 1000) step = 2 * step;
      goto checklimits;
   }

   if (event->type == GDK_FOCUS_CHANGE) goto checklimits;                        //  focus change = entry finished
   return 0;

checklimits:

   convSD(wdata,fdata);                                                          //  ignore bad char. inputs
   fdata += incr * step;

   Nsteps = (fdata - lolim) / step + 0.5;                                        //  set nearest exact step
   fdata = lolim + Nsteps * step;
   
   err = 0;

   if (fdata < lolim) {                                                          //  force within range
      err = 1;
      fdata = lolim;
   }

   if (fdata > hilim) {
      err = 2;
      fdata = hilim;
   }

   if (err) gtk_widget_grab_focus(widget);                                       //  if error, restore focus

   convDS(fdata,6,sdata);                                                        //  round to 6 digits
   gtk_text_buffer_set_text(textBuff,sdata,-1);
   if (zd->widget[ii].data) zfree(zd->widget[ii].data);                          //  clear prior widget data
   zd->widget[ii].data = zstrdup(sdata);                                         //  set new data

   zd->lastwidget = widget;                                                      //  remember last widget updated
   strncpy0(zd->event,zd->widget[ii].name,40);                                   //  event = widget name

   if (zd->eventCB) {
      zd->disabled = 1;
      evfunc = (zdialog_event *) zd->eventCB;                                    //  do callback function
      evfunc(zd,zd->event);
      if (zdialog_valid(zd)) zd->disabled = 0;                                   //  'event' may cause zdialog_free()
   }

   if (event->type == GDK_KEY_PRESS) return 1;                                   //  stop new line from enter key
   return 0;                                                                     //  others OK
}


//  process Ctrl+C (copy text from widget to clipboard)
//  private function

void zdialog_copyfunc(GtkWidget *widget, GtkClipboard *clipboard)
{
   GtkTextView    *textView = 0;
   GtkTextBuffer  *textBuff = 0;
   zdialog        *zd;
   int            ii, cc = 0;
   cchar          *wname;
   char           text[1000];

   widget = gtk_window_get_focus(GTK_WINDOW(widget));
   if (! widget) return;

   zd = zdialog_focus_zd;
   if (! zdialog_valid(zd)) return;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget in zdialog
      if (zd->widget[ii].widget == widget) goto found_widget;
   for (ii = 1; zd->widget[ii].type; ii++) {                                     //  failed, test if buffer
      if (strmatchV(zd->widget[ii].type,"edit","zentry",null)) {                 //    of text view widget
         textView = GTK_TEXT_VIEW(zd->widget[ii].widget);
         textBuff = gtk_text_view_get_buffer(textView);
         if (widget == (GtkWidget *) textBuff) goto found_widget;
      }
   }
   return;                                                                       //  not found

found_widget:
   wname = zd->widget[ii].name;
   zdialog_fetch(zd,wname,text,999);                                             //  current text in widget
   cc = strlen(text);
   gtk_clipboard_set_text(clipboard,text,cc);
   return;
}


//  process Ctrl+V (paste text from clipboard to widget)
//  private function

void zdialog_pastefunc(GtkClipboard *clipboard, cchar *cliptext, void *arg)
{
   GtkWindow      *window;
   GtkWidget      *widget;
   GtkTextView    *textView = 0;
   GtkTextBuffer  *textBuff = 0;
   zdialog        *zd;
   int            ii, cc = 0;
   cchar          *wname;
   char           text[1000];

   window = (GtkWindow *) arg;
   widget = gtk_window_get_focus(window);
   if (! widget) return;                                                         //  widget for pasted text
   if (! cliptext || ! *cliptext) return;                                        //  clipboard text pasted

   zd = zdialog_focus_zd;
   if (! zdialog_valid(zd)) return;

   if (zd->zstat) return;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget in zdialog
      if (zd->widget[ii].widget == widget) goto found_widget;
   for (ii = 1; zd->widget[ii].type; ii++) {                                     //  failed, test if buffer
      if (strmatchV(zd->widget[ii].type,"edit","zentry",null)) {                 //    of text view widget
         textView = GTK_TEXT_VIEW(zd->widget[ii].widget);
         textBuff = gtk_text_view_get_buffer(textView);
         if (widget == (GtkWidget *) textBuff) goto found_widget;
      }
   }
   return;                                                                       //  not found

found_widget:
   wname = zd->widget[ii].name;
   zdialog_fetch(zd,wname,text,999);                                             //  current text in widget
   cc = strlen(text);
   if (cc > 995) return;
   strncpy(text+cc,cliptext,999-cc);                                             //  add clipboard text
   text[999] = 0;
   zdialog_stuff(zd,wname,text);
   return;
}


//  private function called when zdialog is completed.
//  called when dialog is canceled via [x] button or destroyed by GTK (zstat < 0).

int zdialog_delete_event(GtkWidget *, GdkEvent *, zdialog *zd)
{
   zdialog_event  *evfunc = 0;                                                   //  dialog event callback function
   
   if (! zdialog_valid(zd)) return 1;
   if (zd->zstat) return 1;                                                      //  already complete
   if (zd->disabled) return 1;                                                   //  in process

   zd->zstat = -1;                                                               //  set zdialog cancel status

   if (zd->eventCB) {
      evfunc = (zdialog_event *) zd->eventCB;                                    //  do callback function
      zd->disabled = 1;
      evfunc(zd,"zstat");
      if (zdialog_valid(zd)) zd->disabled = 0;                                   //  'event' may cause zdialog_free()
   }
   
   if (zdialog_valid(zd)) zdialog_destroy(zd);                                   //  caller should zdialog_free()
   return 0;
}


//  Send the event "name" to an active zdialog.
//  The response function eventFunc() will be called with this event.

int zdialog_send_event(zdialog *zd, cchar *event)
{
   zdialog_event * evfunc = 0;                                                   //  dialog event callback function

   int      ii;
   cchar    *eventx = E2X(event);                                                //  translated event
   
   if (! zdialog_valid(zd)) return 0;                                            //  canceled
   if (zd->disabled) return 0;                                                   //  busy

   if (strstr(zdialog_button_shortcuts,event)) {                                 //  completion buttons (zfuncs.h)
      for (ii = 0; ii < zdmaxbutts; ii++) {                                      //  find button
         if (! zd->compbutton[ii]) break;                                        //  EOL
         if (strmatch(event,zd->compbutton[ii])) break;                          //  english event
         if (strmatch(eventx,zd->compbutton[ii])) break;                         //  translated event (dialog button)
      }
      if (zd->compbutton[ii]) {                                                  //  found
         zd->zstat = ii+1;                                                       //  zdialog status = button no.
         strcpy(zd->event,"zstat");                                              //  event = "zstat"
      }
      else if (strmatch(event,"Cancel"))                                         //  no [Cancel] button in zdialog, 
         zdialog_destroy(zd);                                                    //  handle same as [x]
   }

   evfunc = (zdialog_event *) zd->eventCB;
   if (! evfunc) return 0;
   zd->disabled = 1;
   evfunc(zd,event);                                                             //  call dialog event function
   if (zdialog_valid(zd)) zd->disabled = 0;                                      //  'event' may cause zdialog_free()
   return 1;
}


//  Complete an active dialog and assign a status.
//  Equivalent to the user pressing a dialog completion button.
//  The dialog completion function is called if defined,
//  and zdialog_wait() is unblocked.
//  returns:  0 = no active dialog or completion function, 1 = OK

int zdialog_send_response(zdialog *zd, int zstat)
{
   zdialog_event  *evfunc = 0;                                                   //  dialog event callback function

   if (! zdialog_valid(zd)) return 0;
   if (zd->disabled) return 0;
   zd->zstat = zstat;                                                            //  set status
   evfunc = (zdialog_event *) zd->eventCB;
   if (! evfunc) return 0;
   zd->disabled = 1;
   evfunc(zd,"zstat");
   if (zdialog_valid(zd)) zd->disabled = 0;                                      //  'event' may cause zdialog_free()
   return 1;
}


//  show or hide a zdialog window
//  returns 1 if successful, 0 if zd does not exist.

int zdialog_show(zdialog *zd, int show)
{
   static GtkWidget  *widget, *pwidget = 0;
   static int        posx, posy;

   if (! zdialog_valid(zd)) return 0;

   widget = zdialog_widget(zd,"dialog");

   if (show) {                                                                   //  show window
      if (widget == pwidget) {                                                   //  restore prior position
         gtk_window_move(GTK_WINDOW(widget),posx,posy);
         pwidget = 0;
      }
      gtk_widget_show_all(widget);
      gtk_window_present(GTK_WINDOW(widget));                                    //  set focus on restored window 
   }
   else {                                                                        //  hide window
      pwidget = widget;
      gtk_window_get_position(GTK_WINDOW(widget),&posx,&posy);                   //  save position
      gtk_widget_hide(widget);
   }
   return 1;
}


//  Destroy the zdialog - must be done by zdialog_run() caller
//  (else dialog continues active even after completion button).
//  Data in widgets remains valid until zdialog_free() is called.

int zdialog_destroy(zdialog *zd)
{
   if (! zdialog_valid(zd)) return 0;

   if (zd->saveposn) zdialog_save_position(zd);                                  //  save position for next use

   if (zd->widget[0].widget) {                                                   //  multiple destroys OK
      gtk_widget_destroy(zd->widget[0].widget);                                  //  destroy GTK dialog
      zd->widget[0].widget = 0;
      zdialog_busy--;
   }

   if (! zd->zstat) zd->zstat = -1;                                              //  status = destroyed
   zd->zrunning = 0;                                                             //  not running
   return 1;
}


//  free zdialog memory (will destroy first, if not already)
//  zd is set to null

int zdialog_free(zdialog *&zd)                                                   //  reference
{
   int      ii;
   
   if (! zdialog_valid(zd)) return 0;                                            //  validate zd pointer

   zdialog_save_inputs(zd);                                                      //  save user inputs for next use

   zdialog_destroy(zd);                                                          //  destroy GTK dialog if there

   zd->sentinel1 = zd->sentinel2 = 0;                                            //  mark sentinels invalid
   zfree(zd->title);                                                             //  free title memory
   zfree(zd->widget[0].data);

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  loop through widgets
   {
      if (strmatchV(zd->widget[ii].type,"combo","comboE",null))                  //  free combo list
         pvlist_free(zd->widget[ii].cblist);
      zfree((char *) zd->widget[ii].type);                                       //  free strings
      zfree((char *) zd->widget[ii].name);
      if (zd->widget[ii].pname) zfree((char *) zd->widget[ii].pname);            //  parent widget name
      if (zd->widget[ii].data) zfree(zd->widget[ii].data);                       //  free data
   }

   for (ii = 0; ii < zdialog_count; ii++)                                        //  remove from valid zdialog list
      if (zd == zdialog_list[ii]) break;
   if (ii < zdialog_count) {
      zdialog_count--;
      for (NOP; ii < zdialog_count; ii++)                                        //  pack down list
         zdialog_list[ii] = zdialog_list[ii+1];
   }
   else printz("*** zdialog_free(), not in zdialog_list \n");

   zfree(zd);                                                                    //  free zdialog memory
   zd = 0;                                                                       //  caller pointer = null
   return 1;
}


//  Wait for a dialog to complete or be destroyed. This is a zmainloop() loop.
//  The returned status is the button 1-N used to complete the dialog, or negative
//  if the dialog was destroyed with [x] or otherwise by GTK. If the status was 1-N and
//  the dialog will be kept active, set zd->zstat = 0 to restore the active state.

int zdialog_wait(zdialog *zd)
{
   while (true)
   {
      zmainloop();
      if (! zd) return -1;
      if (! zdialog_valid(zd)) return -1;
      if (zd->zstat) return zd->zstat;
      zsleep(0.01);
   }
}


//  put cursor at named widget

int zdialog_goto(zdialog *zd, cchar *name)
{
   GtkWidget   *widget;

   if (! zdialog_valid(zd)) return 0;

   widget = zdialog_widget(zd, name);
   if (! widget) return 0;
   gtk_widget_grab_focus(widget);

   return 1;
}


//  set cursor for zdialog (e.g. a busy cursor)

void zdialog_set_cursor(zdialog *zd, GdkCursor *cursor)
{
   GtkWidget   *dialog;
   GdkWindow   *window;

   if (! zdialog_valid(zd)) return;
   dialog = zd->widget[0].widget;
   if (! dialog) return;
   window = gtk_widget_get_window(dialog);
   gdk_window_set_cursor(window,cursor);
   return;
}


//  insert data into a zdialog widget

int zdialog_stuff(zdialog *zd, cchar *name, cchar *data)                         //  stuff a string
{
   if (data) zdialog_put_data(zd,name,data);
   else zdialog_put_data(zd,name,"");                                            //  null > ""
   return 1;
}

int zdialog_stuff(zdialog *zd, cchar *name, int idata)                           //  stuff an integer
{
   char  string[16];

   snprintf(string,16,"%d",idata);
   zdialog_put_data(zd,name,string);
   return 1;
}

int zdialog_stuff(zdialog *zd, cchar *name, double ddata)                        //  stuff a double
{
   char  string[32];

   snprintf(string,32,"%.7g",ddata);                                             //  increase from 6 to 7 digits
   zdialog_put_data(zd,name,string);                                             //  'g' uses decimal or comma
   return 1;                                                                     //      (per locale)
}

int zdialog_stuff(zdialog *zd, cchar *name, double ddata, cchar *format)         //  stuff a double, formatted
{
   char  string[32];

   snprintf(string,32,format,ddata);                                             //  use "%.2g" etc. for
   zdialog_put_data(zd,name,string);                                             //    locale dependent point/comma
   return 1;
}

int zdialog_labelfont(zdialog *zd, cchar *labl, cchar *font, cchar *txt)         //  stuff label text with font
{
   GtkWidget   *widget;
   cchar       *format =  "<span font=\"%s\" >%s</span>";
   char        txt2[1000];
   
   if (! font) font = zfuncs::appfont;                                           //  default font
   snprintf(txt2,1000,format,font,txt);
   widget = zdialog_widget(zd,labl);
   gtk_label_set_markup(GTK_LABEL(widget),txt2);
   return 1;
}


//  get data from a zdialog widget

int zdialog_fetch(zdialog *zd, cchar *name, char *data, int maxcc)               //  fetch string data
{
   cchar  *zdata;

   zdata = zdialog_get_data(zd,name);
   if (! zdata) {
      *data = 0;
      return 0;
   }

   return strncpy0(data,zdata,maxcc);                                            //  0 = OK, 1 = truncation
}

int zdialog_fetch(zdialog *zd, cchar *name, int &idata)                          //  fetch an integer
{
   cchar  *zdata;

   zdata = zdialog_get_data(zd,name);
   if (! zdata) {
      idata = 0;
      return 0;
   }

   idata = atoi(zdata);
   return 1;
}

int zdialog_fetch(zdialog *zd, cchar *name, double &ddata)                       //  fetch a double
{
   int         stat;
   cchar       *zdata;

   zdata = zdialog_get_data(zd,name);
   if (! zdata) {
      ddata = 0;
      return 0;
   }

   stat = convSD(zdata,ddata);                                                   //  period or comma decimal point OK
   if (stat < 4) return 1;
   return 0;
}

int zdialog_fetch(zdialog *zd, cchar *name, float &fdata)                        //  fetch a float
{
   int         stat;
   cchar       *zdata;
   double      ddata;

   zdata = zdialog_get_data(zd,name);
   if (! zdata) {
      fdata = 0;
      return 0;
   }

   stat = convSD(zdata,ddata);                                                   //  period or comma decimal point OK
   fdata = ddata;
   if (stat < 4) return 1;
   return 0;
}


/********************************************************************************

   Combo Box widget with Entry
   ---------------------------
   stat = zdialog_cb_app(zd, name, data)      append to combo box list
   stat = zdialog_cb_prep(zd, name, data)     prepend to combo box list
   data = zdialog_cb_get(zd, name, Nth)       get combo box list item
   data = zdialog_cb_delete(zd, name, data)   delete entry matching "data"
   data = zdialog_cb_clear(zd, name)          clear all entries
   zdialog_cb_popup(zd, name)                 open the combo box entries

   These functions map and track a combo box drop-down list, by maintaining a
   parallel list in memory. The function zdialog-stuff, when called for a comboE
   widget, automatically prepends the stuffed data to the drop-down list.
   The private function zdialog_event(), when processing user input to the edit
   box of a comboE widget, adds the updated entry to the drop-down list.
   The drop-down list is kept free of redundant and blank entries.

*********************************************************************************/


//  append new item to combo box list without changing entry box

int zdialog_cb_app(zdialog *zd, cchar *name, cchar *data)
{
   int      ii, nn;

   if (! zdialog_valid(zd)) return 0;

   if (blank_null(data)) return 0;                                               //  find widget
   for (ii = 1; zd->widget[ii].type; ii++)
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 0;                                          //  not found
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 0;         //  not combo box

   nn = pvlist_append(zd->widget[ii].cblist,data,1);                             //  append unique
   if (nn >= 0)
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(zd->widget[ii].widget),data);

   return 1;
}


//  prepend new item to combo box list without changing entry box

int zdialog_cb_prep(zdialog *zd, cchar *name, cchar *data)
{
   int      ii, nn;

   if (! zdialog_valid(zd)) return 0;

   if (blank_null(data)) return 0;                                               //  find widget
   for (ii = 1; zd->widget[ii].type; ii++)
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 0;                                          //  not found
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 0;         //  not combo box

   nn = pvlist_prepend(zd->widget[ii].cblist,data,1);                            //  prepend unique
   if (nn == 0)
      gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(zd->widget[ii].widget),data);

   return 1;
}


//  get combo box drop-down list entry
//  Nth = 0 = first list entry (not comboE edit box)

char * zdialog_cb_get(zdialog *zd, cchar *name, int Nth)
{
   int      ii;

   if (! zdialog_valid(zd)) return 0;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 0;                                          //  not found
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 0;         //  not combo box
   return pvlist_get(zd->widget[ii].cblist,Nth);
}


//  delete entry by name from combo drop down list

int zdialog_cb_delete(zdialog *zd, cchar *name, cchar *data)
{
   int      ii, nn;

   if (! zdialog_valid(zd)) return 0;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 0;                                          //  not found
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 0;         //  not combo box

   nn = pvlist_find(zd->widget[ii].cblist,data);                                 //  find entry by name
   if (nn < 0) return -1;

   pvlist_remove(zd->widget[ii].cblist,nn);                                      //  remove from memory list
   gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(zd->widget[ii].widget),nn);      //    and from widget
   gtk_combo_box_set_active(GTK_COMBO_BOX(zd->widget[ii].widget),-1);            //  set no active entry

   return 0;
}


//  delete all entries from combo drop down list

int zdialog_cb_clear(zdialog *zd, cchar *name)
{
   int         ii, jj, nn;
   GtkWidget   *widget, *entry;

   if (! zdialog_valid(zd)) return 0;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 0;                                          //  not found
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 0;         //  not combo box

   nn = pvlist_count(zd->widget[ii].cblist);                                     //  entry count
   for (jj = nn-1; jj >= 0; jj--) {
      pvlist_remove(zd->widget[ii].cblist,jj);                                   //  remove from memory list
      gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(zd->widget[ii].widget),jj);   //  remove from widget
   }

   widget = zd->widget[ii].widget;
   gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);                           //  set no active entry
   if (strmatch(zd->widget[ii].type,"comboE")) {                                 //  stuff entry box with nothing
      entry = gtk_bin_get_child(GTK_BIN(widget));
      gtk_entry_set_text(GTK_ENTRY(entry),"");
   }

   return 1;
}


//  make a combo box drop down to show all entries

int zdialog_cb_popup(zdialog *zd, cchar *name)
{
   int      ii;

   if (! zdialog_valid(zd)) return 0;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 0;                                          //  not found
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 0;         //  not combo box

   gtk_combo_box_popup(GTK_COMBO_BOX(zd->widget[ii].widget));
   gtk_combo_box_set_active(GTK_COMBO_BOX(zd->widget[ii].widget),-1);            //  no active entry

   return 1;
}


//  save all combo box entries to a file in /home/<user>/.appname/filename
//  returns 0 = OK, +N = error

int zdialog_cb_save(zdialog *zd, cchar *name, cchar *filename) 
{
   char     file[200], *pp;
   int      ii, jj, nn;
   FILE     *fid;

   if (! zdialog_valid(zd)) return 1;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 2;
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 3;         //  not combo box

   nn = pvlist_count(zd->widget[ii].cblist);                                     //  entry count
   if (! nn) return 0;

   snprintf(file,200,"%s/%s",get_zhomedir(),filename);
   fid = fopen(file,"w");
   if (! fid) return 4;

   for (jj = 0; jj < nn; jj++) {
      pp = pvlist_get(zd->widget[ii].cblist,jj);
      fprintf(fid,"%s\n",pp);
   }
   
   fclose(fid);
   return 0;
}


//  load combo box entries from a file in /home/<user>/.appname/filename
//  returns 0 = OK, +N = error

int zdialog_cb_load(zdialog *zd, cchar *name, cchar *filename)
{
   char     file[200], data[100], *pp;
   int      ii;
   FILE     *fid;

   if (! zdialog_valid(zd)) return 1;

   for (ii = 1; zd->widget[ii].type; ii++)                                       //  find widget
      if (strmatch(zd->widget[ii].name,name)) break;
   if (! zd->widget[ii].type) return 2;
   if (! strmatchV(zd->widget[ii].type,"combo","comboE",null)) return 3;         //  not combo box

   zdialog_cb_clear(zd,name);

   snprintf(file,200,"%s/%s",get_zhomedir(),filename);
   fid = fopen(file,"r");
   if (! fid) return 0;
   
   while (true) {
      pp = fgets_trim(data,99,fid);
      if (! pp) break;
      zdialog_cb_app(zd,name,data);
   }
   
   fclose(fid);
   return 0;
}


/********************************************************************************/

//  functions to save and recall zdialog window positions

namespace zdposn_names
{
   struct zdposn_t {
      char     wintitle[64];                                                     //  window title (ID)
      float    xpos, ypos;                                                       //  window position WRT parent or desktop
      int      xsize, ysize;                                                     //  window size
   }  zdposn[200];                                                               //  space to remember 200 windows

   int      Nzdposn;                                                             //  no. in use
   int      Nzdpmax = 200;                                                       //  table size
}


//  Load zdialog positions table from its file (application startup)
//  or save zdialog positions table to its file (application exit).
//  Action is "load" or "save". Number of table entries is returned.

int zdialog_geometry(cchar *action)
{
   using namespace zdposn_names;

   char     posfile[200], buff[100], wintitle[64], *pp;
   float    xpos, ypos;
   int      xsize, ysize;
   int      ii, nn, cc;
   FILE     *fid;

   snprintf(posfile,199,"%s/zdialog_geometry",zhomedir);                         //  /home/<user>/.appname/zdialog_geometry

   if (strmatch(action,"load"))                                                  //  load dialog positions table from file
   {
      fid = fopen(posfile,"r");
      if (! fid) {
         Nzdposn = 0;
         return 0;
      }

      for (ii = 0; ii < Nzdpmax; )
      {
         pp = fgets(buff,100,fid);
         if (! pp) break;
         pp = strstr(buff,"||");
         if (! pp) continue;
         cc = pp - buff;
         strncpy0(wintitle,buff,cc);
         strTrim(wintitle);
         if (strlen(wintitle) < 3) continue;
         nn = sscanf(pp+2," %f %f %d %d ",&xpos,&ypos,&xsize,&ysize);
         if (nn != 4) continue;

         strcpy(zdposn[ii].wintitle,wintitle);
         zdposn[ii].xpos = xpos;
         zdposn[ii].ypos = ypos;
         zdposn[ii].xsize = xsize;
         zdposn[ii].ysize = ysize;
         ii++;
      }

      fclose(fid);
      Nzdposn = ii;
      return Nzdposn;
   }

   if (strmatch(action,"save"))                                                  //  save dialog positions table to file
   {
      fid = fopen(posfile,"w");
      if (! fid) {
         printz("*** cannot write zdialog_geometry file \n");
         return 0;
      }

      for (ii = 0; ii < Nzdposn; ii++) {
         fprintf(fid,"%s || %0.1f %0.1f %d %d \n",                               //  dialog-title || xpos ypos xsize ysize
                           zdposn[ii].wintitle,
                           zdposn[ii].xpos, zdposn[ii].ypos, 
                           zdposn[ii].xsize, zdposn[ii].ysize);
      }

      fclose(fid);
      return Nzdposn;
   }

   printz("*** zdialog_geometry bad action: %s \n",action);
   return 0;
}


//  Set the initial or new zdialog window position from "posn".
//  Called by zdialog_run().  Private function.
//     null:      window manager decides
//    "mouse"     put dialog at mouse position
//    "desktop"   center dialog in desktop window
//    "parent"    center dialog in parent window
//    "save"      use the same position last set by the user
//    "nn/nn"     put NW corner of dialog in parent window at % size
//                (e.g. "50/50" puts NW corner at center of parent)

void zdialog_set_position(zdialog *zd, cchar *posn)
{
   using namespace zdposn_names;

   int         ii, ppx, ppy, zdpx, zdpy, pww, phh;
   float       xpos, ypos;
   int         xsize, ysize;
   char        wintitle[64], *pp;
   GtkWidget   *parent, *dialog;

   if (! zdialog_valid(zd)) return;

   parent = zd->parent;
   dialog = zd->widget[0].widget;

   if (strmatch(posn,"mouse")) {
      window_to_mouse(zd->dialog);
      return;
   }

   if (strmatch(posn,"desktop")) {
      gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
      return;
   }

   if (strmatch(posn,"parent")) {
      gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER_ON_PARENT);
      return;
   }

   if (! parent) {                                                               //  no parent window
      ppx = ppy = 0;                                                             //  use desktop
      pww = monitor_ww;
      phh = monitor_hh;
   }
   else {
      gtk_window_get_position(GTK_WINDOW(parent),&ppx,&ppy);                     //  parent window NW corner
      gtk_window_get_size(GTK_WINDOW(parent),&pww,&phh);                         //  parent window size
   }

   if (strmatch(posn,"save"))                                                    //  use last saved window position
   {
      zd->saveposn = 1;                                                          //  set flag for zdialog_free()

      pp = (char *) gtk_window_get_title(GTK_WINDOW(dialog));                    //  get window title, used as ID
      if (! pp) return;
      if (strlen(pp) < 2) return;
      strncpy0(wintitle,pp,64);                                                  //  window title, < 64 chars.

      for (ii = 0; ii < Nzdposn; ii++)                                           //  search table for title
         if (strmatch(wintitle,zdposn[ii].wintitle)) break;
      if (ii == Nzdposn) {                                                       //  not found - zdialog_destroy() will add
         zdpx = ppx + 0.8 * pww;                                                 //  supply reasonable defaults
         zdpy = ppx + 0.2 * phh;
         gtk_window_get_size(GTK_WINDOW(dialog),&xsize,&ysize);                  //  use current size
      }
      else {
         zdpx = ppx + 0.01 * zdposn[ii].xpos * pww;                              //  set position for dialog window
         zdpy = ppy + 0.01 * zdposn[ii].ypos * phh;
         xsize = zdposn[ii].xsize;                                               //  set size
         ysize = zdposn[ii].ysize;
      }

      gtk_window_move(GTK_WINDOW(dialog),zdpx,zdpy);
      gtk_window_resize(GTK_WINDOW(dialog),xsize,ysize);
      return;
   }

   else     //  "nn/nn"                                                          //  position from caller
   {
      ii = sscanf(posn,"%f/%f",&xpos,&ypos);                                     //  parse "nn/nn"
      if (ii != 2) return;
      zdpx = ppx + 0.01 * xpos * pww;                                            //  position for dialog window
      zdpy = ppy + 0.01 * ypos * phh;
      gtk_window_move(GTK_WINDOW(dialog),zdpx,zdpy);
      return;
   }
}


//  If the dialog window position is "save" then save
//  its position WRT parent or desktop for next use.
//  called by zdialog_destroy().  Private function.

void zdialog_save_position(zdialog *zd)
{
   using namespace zdposn_names;

   int         ii, ppx, ppy, pww, phh, zdpx, zdpy;
   float       xpos, ypos;
   int         xsize, ysize;
   char        wintitle[64], *pp;
   GtkWidget   *parent, *dialog;

   if (! zdialog_valid(zd)) return;

   dialog = zd->widget[0].widget;
   if (! dialog) return;
   if (! gtk_widget_get_window(dialog)) return;

   gtk_window_get_position(GTK_WINDOW(dialog),&zdpx,&zdpy);                      //  dialog window NW corner
   if (! zdpx && ! zdpy) return;                                                 //  (0,0) ignore
   
   gtk_window_get_size(GTK_WINDOW(dialog),&xsize,&ysize);                        //  window size

   parent = zd->parent;                                                          //  parent window

   if (! parent) {                                                               //  no parent window
      ppx = ppy = 0;                                                             //  use desktop
      pww = monitor_ww;
      phh = monitor_hh;
   }
   else {
      gtk_window_get_position(GTK_WINDOW(parent),&ppx,&ppy);                     //  parent window NW corner
      gtk_window_get_size(GTK_WINDOW(parent),&pww,&phh);                         //  parent window size
   }

   xpos = 100.0 * (zdpx - ppx) / pww;                                            //  dialog window relative position
   ypos = 100.0 * (zdpy - ppy) / phh;                                            //  (as percent of parent size)

   pp = (char *) gtk_window_get_title(GTK_WINDOW(dialog));
   if (! pp) return;
   if (strlen(pp) < 2) return;
   strncpy0(wintitle,pp,64);                                                     //  window title, < 64 chars.

   for (ii = 0; ii < Nzdposn; ii++)                                              //  search table for window
      if (strmatch(wintitle,zdposn[ii].wintitle)) break;
   if (ii == Nzdposn) {                                                          //  not found
      if (ii == Nzdpmax) return;                                                 //  table full
      Nzdposn++;                                                                 //  new entry
   }

   strcpy(zdposn[ii].wintitle,wintitle);                                         //  add window to table
   zdposn[ii].xpos = xpos;                                                       //  save window position
   zdposn[ii].ypos = ypos;
   zdposn[ii].xsize = xsize;                                                     //  and window size
   zdposn[ii].ysize = ysize;
   return;
}


/********************************************************************************/

//  Functions to save and restore zdialog user inputs
//    within an app session or across app sessions.

namespace zdinputs_names
{
   #define  Nwmax zdmaxwidgets                                                   //  max. widgets in a dialog
   #define  Nzdmax 200                                                           //  max. zdialogs 
   #define  ccmax1 100                                                           //  max. widget name length
   #define  ccmax2 400                                                           //  max. widget data length

   struct zdinputs_t {
      char     *zdtitle;                                                         //  zdialog title
      int      Nw;                                                               //  no. of widgets
      char     **wname;                                                          //  list of widget names
      char     **wdata;                                                          //  list of widget data
   }  zdinputs[Nzdmax];                                                          //  space for Nzdmax dialogs

   int      Nzd = 0;                                                             //  no. zdialogs in use
}


//  Load zdialog input fields from its file (app startup)
//  or save zdialog input fields to its file (app shutdown).
//  Action is "load" or "save".
//  Number of zdialogs is returned.

int zdialog_inputs(cchar *action)
{
   using namespace zdinputs_names;

   char     zdinputsfile[200], buff[ccmax2];
   char     zdtitle[ccmax1], wname[Nwmax][ccmax1], wdata[Nwmax][ccmax2];
   char     *pp, *pp1, *pp2, wdata2[ccmax2+50];
   FILE     *fid;
   int      Nw, ii, jj, cc, cc1, cc2;

   snprintf(zdinputsfile,200,"%s/zdialog_inputs",zhomedir);                      //  /home/<user>/.appname/zdialog_inputs

   if (strmatch(action,"load"))                                                  //  load dialog input fields from its file
   {
      Nzd = 0;

      fid = fopen(zdinputsfile,"r");                                             //  no file
      if (! fid) return 0;

      while (true)
      {
         pp = fgets_trim(buff,ccmax2,fid,1);                                     //  read next zdialog title record
         if (! pp) break;
         if (! strmatchN(pp,"zdialog == ",11)) continue;

         strncpy0(zdtitle,pp+11,ccmax1);                                         //  save new zdialog title

         pp = fgets_trim(buff,ccmax2,fid,1);                                     //  read widget count
         if (! pp) break;
         Nw = atoi(pp);
         if (Nw < 1 || Nw > Nwmax) {
            printz("*** zdialog_inputs() bad data: %s \n",zdtitle);
            continue;
         }

         for (ii = 0; ii < Nw; ii++)                                             //  read widget data recs
         {
            pp = fgets_trim(buff,ccmax2,fid,1);
            if (! pp) break;
            pp1 = pp;
            pp2 = strstr(pp1," ==");
            if (! pp2) break;                                                    //  widget has no data
            cc1 = pp2 - pp1;
            pp1[cc1] = 0;
            pp2 += 3;
            if (*pp2 == ' ') pp2++;
            cc2 = strlen(pp2);
            if (cc1 < 1 || cc1 >= ccmax1) break;
            if (cc2 < 1) pp2 = (char *) "";
            if (cc2 >= ccmax2) break;                                            //  do not copy large inputs
            strcpy(wname[ii],pp1);                                               //  save widget name and data
            strcpy(wdata2,pp2);
            repl_1str(wdata2,wdata[ii],"\\n","\n");                              //  replace "\n" with newline chars.
         }

         if (ii < Nw) {
            printz("*** zdialog_inputs() bad data: %s \n",zdtitle);
            continue;
         }

         if (Nzd == Nzdmax) {
            printz("*** zdialog_inputs() too many dialogs \n");
            break;
         }

         zdinputs[Nzd].zdtitle = zstrdup(zdtitle);                               //  save acculumated zdialog data
         zdinputs[Nzd].Nw = Nw;
         cc = Nw * sizeof(char *);
         zdinputs[Nzd].wname = (char **) zmalloc(cc);
         zdinputs[Nzd].wdata = (char **) zmalloc(cc);
         for (ii = 0; ii < Nw; ii++) {
            zdinputs[Nzd].wname[ii] = zstrdup(wname[ii]);
            zdinputs[Nzd].wdata[ii] = zstrdup(wdata[ii]);
         }

         Nzd++;
      }

      fclose(fid);
      return Nzd;
   }

   if (strmatch(action,"save"))                                                  //  save dialog input fields to its file
   {
      fid = fopen(zdinputsfile,"w");
      if (! fid) {
         printz("*** zdialog_inputs() cannot write file \n");
         return 0;
      }

      for (ii = 0; ii < Nzd; ii++)
      {
         fprintf(fid,"zdialog == %s \n",zdinputs[ii].zdtitle);                   //  zdialog == zdialog title
         Nw = zdinputs[ii].Nw;
         fprintf(fid,"%d \n",Nw);                                                //  widget count
         for (jj = 0; jj < Nw; jj++) {
            pp1 = zdinputs[ii].wname[jj];                                        //  widget name == widget data
            pp2 = zdinputs[ii].wdata[jj];
            repl_1str(pp2,wdata2,"\n","\\n");                                    //  replace newline chars. with "\n"
            fprintf(fid,"%s == %s \n",pp1,wdata2);
         }
         fprintf(fid,"\n");
      }

      fclose(fid);
      return Nzd;
   }

   printz("*** zdialog_inputs bad action: %s \n",action);
   return 0;
}


//  Save dialog user input fields when a dialog is finished.
//  Called automatically by zdialog_free(). Private function.

int zdialog_save_inputs(zdialog *zd)
{
   using namespace zdinputs_names;

   char     zdtitle[ccmax1], wname[ccmax1], wdata[ccmax2], *type;
   int      ii, jj, Nw, cc;
   
   if (! zdialog_valid(zd)) return 0;
   if (! zd->saveinputs) return 0;                                               //  zdialog does not use this service

   strncpy0(zdtitle,zd->widget[0].data,ccmax1);                                  //  zdialog title is widget[0].data

   for (ii = 0; ii < Nzd; ii++)                                                  //  find zdialog in zdinputs table
      if (strmatch(zdtitle,zdinputs[ii].zdtitle)) break;

   if (ii < Nzd) {                                                               //  found
      zfree(zdinputs[ii].zdtitle);                                               //  delete obsolete zdinputs data
      for (jj = 0; jj < zdinputs[ii].Nw; jj++) {
         zfree(zdinputs[ii].wname[jj]);
         zfree(zdinputs[ii].wdata[jj]);
      }
      zfree(zdinputs[ii].wname);
      zfree(zdinputs[ii].wdata);
      Nzd--;                                                                     //  decr. zdialog count
      for (NOP; ii < Nzd; ii++)                                                  //  pack down the rest
         zdinputs[ii] = zdinputs[ii+1];
   }

   if (Nzd == Nzdmax) {
      printz("*** zdialog_save_inputs, too many zdialogs \n");
      return 0;
   }

   ii = Nzd;                                                                     //  next zdinputs table entry

   for (Nw = 0, jj = 1; zd->widget[jj].type; jj++) {                             //  count zdialog widgets
      type = (char *) zd->widget[jj].type;
      if (strstr("dialog hbox vbox hsep vsep frame scrwin"                       //  skip non-input widgets
                 "label link button zbutton",type)) continue;
      Nw++;
   }

   if (! Nw) return 0;                                                           //  no input widgets
   if (Nw > Nwmax) {
      printz("*** zdialog_inputs() bad data: %s \n",zdtitle);
      return 0;
   }

   zdinputs[ii].zdtitle = zstrdup(zdtitle);                                      //  set zdialog title

   cc = Nw * sizeof(char *);                                                     //  allocate pointers for widgets
   zdinputs[ii].wname = (char **) zmalloc(cc);
   zdinputs[ii].wdata = (char **) zmalloc(cc);

   for (Nw = 0, jj = 1; zd->widget[jj].type; jj++) {                             //  add widget names and data
      type = (char *) zd->widget[jj].type;
      if (strstr("dialog hbox vbox hsep vsep frame scrwin"                       //  skip non-input widgets 
                 "label link button zbutton",type)) continue;
      strncpy0(wname,zd->widget[jj].name,ccmax1);
      if (zd->widget[jj].data) 
         strncpy0(wdata,zd->widget[jj].data,ccmax2);
      else strcpy(wdata,"");
      zdinputs[ii].wname[Nw] = zstrdup(wname);
      zdinputs[ii].wdata[Nw] = zstrdup(wdata);
      Nw++;
   }

   zdinputs[ii].Nw = Nw;                                                         //  set widget count
   Nzd++;                                                                        //  add zdialog to end of zdinputs

   return 1;
}


//  Restore user input fields from prior use of the same dialog.
//  Call this if wanted after zdialog is built and before it is run.
//  Override old user inputs with zdialog_stuff() where needed.

int zdialog_restore_inputs(zdialog *zd) 
{
   using namespace zdinputs_names;

   char     *zdtitle, *wname, *wdata;
   int      ii, jj;

   zd->saveinputs = 1;                                                           //  flag, save data at zdialog_free()

   zdtitle = (char *) zd->widget[0].data;                                        //  zdialog title

   for (ii = 0; ii < Nzd; ii++)                                                  //  find zdialog in zdinputs
      if (strmatch(zdtitle,zdinputs[ii].zdtitle)) break;
   if (ii == Nzd) return 0;                                                      //  not found

   for (jj = 0; jj < zdinputs[ii].Nw; jj++) {                                    //  stuff all saved widget data
      wname = zdinputs[ii].wname[jj];
      wdata = zdinputs[ii].wdata[jj];
      zdialog_put_data(zd,wname,wdata);
   }

   return 1;
}


/********************************************************************************/

//  move a window to the mouse position
//  widget is a GtkWindow, which may or may not be realized

void window_to_mouse(GtkWidget *window) 
{
   using namespace zfuncs;
   
   int      px, py;

   gdk_device_get_position(mouse,&screen,&px,&py);                               //  get mouse position
   gtk_window_move(GTK_WINDOW(window),px,py);
   return;
}


/********************************************************************************/

//  popup window with scrolling text report 
//  line numbers and line positions are zero based

//  open the report window with given title and pixel dimensions
//  Fheader - add optional non-scrolling header at top of report window
//  CBfunc - optional callback function: 
//             CBfunc(GtkWidget *, int line, int posn, int KBkey)
//  ...   - optional event buttons terminated with null:
//             [ OK ] [cancel] [hide] [find] are processed here
//             others are passed to callback function (1st character) 

zdialog * popup_report_open(cchar *title, GtkWidget *parent, int ww, int hh,
                          int Fheader, textwidget_callbackfunc_t CBfunc, ...)
{
   int popup_report_dialog_event(zdialog *zd, cchar *event);
   
   va_list     arglist;
   cchar       *butn[6];
   int         ii, NB;
   zdialog     *zd;
   GtkWidget   *mHead, *mText;
   
   va_start(arglist,CBfunc);                                                     //  get button args, if any

   for (ii = 0; ii < 5; ii++) {
      butn[ii] = va_arg(arglist,cchar *);
      if (! butn[ii]) break;
   }
   
   NB = ii;                                                                      //  no. buttons
   
   zd = zdialog_new(title,parent,null);
   
   if (Fheader) {                                                                //  non-scrolling header
      zdialog_add_widget(zd,"text","header","dialog");
      zdialog_add_widget(zd,"hsep","hsep","dialog");
   }

   zdialog_add_widget(zd,"scrwin","scroll","dialog",0,"expand");                 //  scrolling text window for report
   zdialog_add_widget(zd,"text","text","scroll",0,"expand");
   
   if (NB) {                                                                     //  optional event buttons
      zdialog_add_widget(zd,"hbox","hbbutn","dialog");
      zdialog_add_widget(zd,"label","space","hbbutn",0,"expand");
      for (ii = 0; ii < NB; ii++)
         zdialog_add_widget(zd,"button",butn[ii],"hbbutn",butn[ii],"space=5");
   }
   
   zdialog_resize(zd,ww,hh);                                                     //  show report dialog box
   zdialog_run(zd,popup_report_dialog_event,"parent");

   if (Fheader) {   
      mHead = zdialog_widget(zd,"header");                                       //  header initially invisible
      gtk_widget_set_visible(mHead,0);
   }

   mText = zdialog_widget(zd,"text");                                            //  report text not editable
   gtk_text_view_set_editable(GTK_TEXT_VIEW(mText),0);
   gtk_widget_grab_focus(mText);

   textwidget_set_eventfunc(mText,CBfunc);                                       //  set mouse/KB event function
   zd->popup_report_CB = (void *) CBfunc;

   return zd;
}


//  dialog event and completion function

int popup_report_dialog_event(zdialog *zd, cchar *event)
{
   textwidget_callbackfunc_t  *CBfunc;
   
   GtkWidget    *mText;
   static char  findtext[40] = "";
   int          linem, line1, line2;
   zdialog      *zdf;
   
   if (strmatch(event,"focus")) return 1;
   
   mText = zdialog_widget(zd,"text");
   
   if (zd->zstat) {                                                              //  [x] cancel
      zdialog_free(zd);
      return 1;
   }
   
   if (strmatch(event,"cancel")) {                                               //  [cancel]
      zdialog_free(zd);
      return 1;
   }
   
   if (strmatch(event,"done")) {                                                 //  [ OK ]
      zdialog_free(zd);
      return 1;
   }
   
   if (strmatch(event,"hide")) {                                                 //  [hide]
      zdialog_show(zd,0);
      return 1;
   }
   
   if (strmatch(event,"find")) {                                                 //  [find]
      zdf = zdialog_new("find text",zd->dialog,"find","cancel",0);               //  popup dialog to enter text
      zdialog_add_widget(zdf,"entry","text","dialog",findtext,"size=20");
      zdialog_run(zdf,0,"mouse");
      linem = -1;                                                                //  no match line yet
      while (true) 
      {
         zdialog_wait(zdf);
         if (zdf->zstat != 1) {                                                  //  [cancel]
            zdialog_free(zdf);
            return 1;
         }
         zdf->zstat = 0;
         zdialog_fetch(zdf,"text",findtext,40);                                  //  get text
         popup_report_get_visible_lines(zd,line1,line2);                         //  lines now visible
         if (linem < 0) linem = line1;                                           //  search from 1st visible line
         linem = popup_report_find(zd,findtext,linem);                           //  search for text
         if (linem < 0) continue;                                                //  not found
         popup_report_scroll_top(zd,linem);                                      //  found, scroll to top
         linem++;                                                                //  next search from line
      }
   }
   
   CBfunc = (textwidget_callbackfunc_t *) zd->popup_report_CB;                   //  other event
   if (CBfunc) CBfunc(mText,-1,-1,*event);                                       //  pass to callback function (1st char.)

   return 1;
}


//  write a non-scrolling header line

void popup_report_header(zdialog *zd, int bold, cchar *format, ...)
{
   va_list     arglist;
   char        message[1000];
   GtkWidget   *mHead;

   va_start(arglist,format);
   vsnprintf(message,999,format,arglist);
   va_end(arglist);

   mHead = zdialog_widget(zd,"header");
   textwidget_append(mHead,bold,message);
   gtk_widget_set_visible(mHead,1);

   return;
}


//  write a new text line at the end

void popup_report_write(zdialog *zd, int bold, cchar *format, ...)
{
   va_list     arglist;
   char        message[1000];
   GtkWidget   *mText;

   va_start(arglist,format);
   vsnprintf(message,999,format,arglist);
   va_end(arglist);

   mText = zdialog_widget(zd,"text");
   textwidget_append(mText,bold,"%s",message);                                   //  speedup, append not append2
   return;
}


//  scroll window back to top line

void popup_report_top(zdialog *zd)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_scroll(mText,0);
   return;
}


//  scroll window back to bottom line

void popup_report_bottom(zdialog *zd)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_scroll(mText,999999);
   return;
}


//  clear the report window

void popup_report_clear(zdialog *zd)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_clear(mText);
   return;
}


//  clear the report window from line to end

void popup_report_clear(zdialog *zd, int line)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_clear(mText,line);
   return;
}


//  insert a new line after a given line

void popup_report_insert(zdialog *zd, int bold, int line, cchar *format, ...)
{
   va_list     arglist;
   char        message[1000];
   GtkWidget   *mText;

   va_start(arglist,format);
   vsnprintf(message,999,format,arglist);
   va_end(arglist);

   mText = zdialog_widget(zd,"text");
   textwidget_insert(mText,bold,line,message);
   return;
}


//  replace a given line

void popup_report_replace(zdialog *zd, int bold, int line, cchar *format, ...)
{
   va_list     arglist;
   char        message[1000];
   GtkWidget   *mText;

   va_start(arglist,format);
   vsnprintf(message,999,format,arglist);
   va_end(arglist);

   mText = zdialog_widget(zd,"text");
   textwidget_replace(mText,bold,line,message);
   return;
}


//  delete a given line

void popup_report_delete(zdialog *zd, int line)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_delete(mText,line);
   return;
}


//  find first line of text containing characters matching input string
//  search is from line1 to end, then from 0 to line1-1
//  returns first matching line or -1 if none
//  comparison is not case sensitive


int  popup_report_find(zdialog *zd, char *matchtext, int line1)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   return textwidget_find(mText,matchtext,line1);
}


//  insert a pixbuf image after a given line

void popup_report_insert_pixbuf(zdialog *zd, int line, GdkPixbuf *pixbuf)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_insert_pixbuf(mText,line,pixbuf);
   return;
}


//  scroll to bring a given line into the report window

void popup_report_scroll(zdialog *zd, int line)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_scroll(mText,line);
   return;
}


//  scroll to bring a given line to the top of the report window

void popup_report_scroll_top(zdialog *zd, int line)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_scroll_top(mText,line);
   return;
}


//  get the range of visible lines in the report window

void popup_report_get_visible_lines(zdialog *zd, int &top, int &bott)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_get_visible_lines(mText,top,bott);
   return;
}


//  retrieve a given line and optionally strip the trailing \n

char * popup_report_line(zdialog *zd, int line, int strip)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   return textwidget_line(mText,line,strip);
}


//  retrieve the word starting at a given position in a given line

char * popup_report_word(zdialog *zd, int line, int posn, cchar *dlims, char &end)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   return textwidget_word(mText,line,posn,dlims,end);
}


//  highlight a given line of text

void popup_report_highlight_line(zdialog *zd, int line)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_highlight_line(mText,line);
   return;
}


//  highlight the text at a given position and length in a given line

void popup_report_highlight_word(zdialog *zd, int line, int posn, int cc)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_highlight_word(mText,line,posn,cc);
   return;
}


//  underline the text at a given position and length in a given line

void popup_report_underline_word(zdialog *zd, int line, int posn, int cc)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_underline_word(mText,line,posn,cc);
   return;
}


//  bold the text at a given position and length in a given line

void popup_report_bold_word(zdialog *zd, int line, int posn, int cc)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_bold_word(mText,line,posn,cc);
   return;
}


//  set font attributes for entire report                                        //  temp. kludge

void popup_report_font_attributes(zdialog *zd)
{
   GtkWidget *mText = zdialog_widget(zd,"text");
   textwidget_font_attributes(mText);
   return;
}


//  close report after given seconds (OK to leave it open until user closes)
//  also connected to window destroy signal (secs = 0) 

void popup_report_close(zdialog *zd, int secs)
{
   void popup_report_timeout(zdialog *zd);

   if (secs < 1) {
      zdialog_destroy(zd);
      return;
   }

   g_timeout_add_seconds(secs,(GSourceFunc) popup_report_timeout,zd);
   return;
}


//  private function for report timeout

void popup_report_timeout(zdialog *zd)
{
   zdialog_destroy(zd);
   return;
}


/********************************************************************************/

//  execute a command and show the output in a scrolling popup window
//  returns: 0 = EOF  1 = command failure

int popup_command(cchar *command, int ww, int hh, GtkWidget *parent, int top)
{
   FILE        *fid;
   char        buff[1000], *pp;
   zdialog     *zd;

   zd = popup_report_open(command,parent,ww,hh,0,0,"find","done",0);
   
   fid = popen(command,"r");
   if (! fid) return 1;
   while (true) {
      pp = fgets_trim(buff,1000,fid);
      if (! pp) break;
      popup_report_write(zd,0,"%s\n",pp);
   }
   pclose(fid);

   if (top) popup_report_top(zd);                                                //  back to top of window
   return 0;
}


/********************************************************************************/

//  Display popup message box and wait for user acknowledgement.
//  May be called from a thread.
//  Messages are presented sequentially from main() and from threads.

void zmessageACK(GtkWidget *parent, cchar *format, ... )
{
   va_list        arglist;
   char           message[1000], xmessage[1000];
   cchar          *posn = "mouse";
   zdialog        *zd;

   va_start(arglist,format);                                                     //  format the message
   vsnprintf(message,1000,format,arglist);
   va_end(arglist);
   
   printz("%s \n",message);                                                      //  output to log file 

   if (! pthread_equal(pthread_self(),zfuncs::tid_main)) {                       //  caller is a thread                 21.0
      snprintf(xmessage,1000,"xmessage -center %s",message);                     //  use xmessage() 
      Qshell(0,0,xmessage);
      return;
   }

   zd = zdialog_new("",parent,"OK",null);                                        //  caller is main() 
   zdialog_add_widget(zd,"hbox","hb1","dialog",0,"space=3");
   zdialog_add_widget(zd,"label","lab1","hb1",message,"space=5");
   zdialog_resize(zd,200,0);
   zdialog_set_modal(zd);
   if (parent) posn = "parent";
   else posn = "desktop";
   zdialog_run(zd,0,posn);
   zdialog_present(zd);                                                          //  help wayland
   zdialog_wait(zd);
   zdialog_free(zd);
   return;
}


/********************************************************************************/

//  display message box and wait for user Yes or No response
//  returns 1 or 0

int zmessageYN(GtkWidget *parent, cchar *format, ... )
{
   va_list        arglist;
   char           message[500];
   cchar          *where;
   zdialog        *zd;
   int            zstat;

   va_start(arglist,format);
   vsnprintf(message,500,format,arglist);
   va_end(arglist);

   printz("%s \n",message);                                                      //  output to log file
   
   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("zmessageYN() called from thread");

   if (parent) where = "parent";
   else where = "desktop";

   zd = zdialog_new("",parent,E2X("Yes"),E2X("No"),null);
   zdialog_add_widget(zd,"hbox","hb1","dialog",0,"space=3");
   zdialog_add_widget(zd,"label","lab1","hb1",message,"space=5");
   zdialog_resize(zd,200,0);
   zdialog_set_modal(zd);
   zdialog_run(zd,0,where);
   zdialog_present(zd);                                                          //  help wayland
   zstat = zdialog_wait(zd);
   zdialog_free(zd);
   if (zstat == 1) return 1;
   return 0;
}


/********************************************************************************/

//  display message until timeout (can be forever) or user cancel
//  or caller kills it with zdialog_free()

typedef struct {
   zdialog     *zd;
   int         uniqueID;
}  zdx_t;


zdialog * zmessage_post(GtkWidget *parent, cchar *loc, int seconds, cchar *format, ... )
{
   int  zmessage_post_timeout(zdx_t *zdx);
   
   va_list           arglist;
   char              message[1000], xmessage[1000];
   static zdx_t      zdx[100];
   static int        ii = 0;
   zdialog           *zd;

   va_start(arglist,format);
   vsnprintf(message,1000,format,arglist);
   va_end(arglist);
   
   printz("%s \n",message);                                                      //  output to log file

   if (! pthread_equal(pthread_self(),zfuncs::tid_main)) {                       //  called from thread, use xmessage
      if (seconds < 1) seconds = 1;                                              //  disallow zero
      snprintf(xmessage,1000,"xmessage -center -timeout %d %s",seconds,message);
      Qshell(0,0,xmessage);
      return 0;
   }

   zd = zdialog_new("post",parent,null);
   zdialog_add_widget(zd,"hbox","hb1","dialog",0,"space=3");
   zdialog_add_widget(zd,"label","lab1","hb1",message,"space=5");
   zdialog_set_decorated(zd,0);
   
   zdialog_run(zd,0,loc);                                                        //  mouse position
   zdialog_present(zd);                                                          //  help wayland
   if (parent) gtk_window_present(GTK_WINDOW(parent));                           //  return focus to parent

   if (seconds) {
      if (ii < 99) ii++;                                                         //  track unique zdialogs 
      else ii = 0;
      zdx[ii].zd = zd;
      zdx[ii].uniqueID = zd->uniqueID;
      g_timeout_add_seconds(seconds,(GSourceFunc) zmessage_post_timeout,&zdx[ii]);
   }

   return zd;
}


//  same as above, but message is big, bold and red

zdialog * zmessage_post_bold(GtkWidget *parent, cchar *loc, int seconds, cchar *format, ... )
{
   int  zmessage_post_timeout(zdx_t *zdx);
   
   va_list           arglist;
   char              message[400], messagebold[460];
   static zdx_t      zdx[100];
   static int        ii = 0;
   zdialog           *zd;

   va_start(arglist,format);
   vsnprintf(message,400,format,arglist);
   va_end(arglist);
   
   printz("%s \n",message);                                                      //  output to log file

   if (! pthread_equal(pthread_self(),zfuncs::tid_main)) return 0;               //  called from thread

   snprintf(messagebold,460,"<span font=\"bold\" color=\"red\">%s</span>",message);
   
   zd = zdialog_new("post",parent,null);
   zdialog_add_widget(zd,"hbox","hb1","dialog",0,"space=3");
   zdialog_add_widget(zd,"label","lab1","hb1",messagebold,"space=5");
   zdialog_set_decorated(zd,0);
   
   zdialog_run(zd,0,loc);                                                        //  mouse position
   zdialog_present(zd);                                                          //  help wayland
   if (parent) gtk_window_present(GTK_WINDOW(parent));                           //  return focus to parent

   if (seconds) {
      if (ii < 99) ii++;                                                         //  track unique zdialogs
      else ii = 0;
      zdx[ii].zd = zd;
      zdx[ii].uniqueID = zd->uniqueID;
      g_timeout_add_seconds(seconds,(GSourceFunc) zmessage_post_timeout,&zdx[ii]);
   }

   return zd;
}


int zmessage_post_timeout(zdx_t *zdx)
{
   zdialog  *zd = zdx->zd;                                                       //  check unique zdialog active
   if (! zdialog_valid(zd,"post")) return 0;
   if (zd->uniqueID != zdx->uniqueID) return 0;
   zdialog_free(zd);
   return 0;
}


/********************************************************************************/

//  get text input from a popup dialog
//  returned text is subject for zfree()
//  null is returned if user presses [cancel] button.

char * zdialog_text(GtkWidget *parent, cchar *title, cchar *inittext)
{
   zdialog     *zd;
   int         zstat;
   char        *text;

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("zdialog_text() called from thread");

   zd = zdialog_new(title,parent,"OK",E2X("cancel"),null);
   zdialog_add_widget(zd,"frame","fred","dialog");
   zdialog_add_widget(zd,"edit","edit","fred");
   if (inittext) zdialog_stuff(zd,"edit",inittext);

   zdialog_resize(zd,300,0);
   zdialog_set_modal(zd);
   zdialog_run(zd,0,"mouse");
   zdialog_present(zd);                                                          //  help wayland
   zstat = zdialog_wait(zd);
   if (zstat == 1)
      text = (char *) zdialog_get_data(zd,"edit");
   else text = 0;
   if (text) text = zstrdup(text);
   zdialog_free(zd);
   return text;
}


/********************************************************************************/

//  Display a dialog with a message and 1-5 choice buttons.
//  Returns choice 1-N corresponding to button selected.
//  nn = zdialog_choose(parent, where, message, butt1, butt2, ... null)
//  'where' is    null:      window manager decides
//                "mouse"     put dialog at mouse position
//                "desktop"   center dialog in desktop window
//                "parent"    center dialog in parent window

int zdialog_choose(GtkWidget *parent, cchar *where, cchar *message, ...)
{
   zdialog     *zd;
   va_list     arglist;
   int         ii, zstat, Nbutn;
   cchar       *butn[6];

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("zmessage_choose() called from thread");

   va_start(arglist,message);

   for (ii = 0; ii < 5; ii++)
   {
      butn[ii] = va_arg(arglist,cchar *);
      if (! butn[ii]) break;
   }

   Nbutn = ii;
   if (! Nbutn) zappcrash("zdialog_choose(), no buttons");

repeat:

   zd = zdialog_new("",parent,butn[0],butn[1],butn[2],butn[3],butn[4],null);
   zdialog_add_widget(zd,"hbox","hbmess","dialog","space=3");
   zdialog_add_widget(zd,"label","labmess","hbmess",message,"space=5");
   zdialog_set_modal(zd);
   zdialog_set_decorated(zd,0);
   zdialog_run(zd,null,where);
   zdialog_present(zd);                                                          //  wayland
   zstat = zdialog_wait(zd);
   zdialog_free(zd);

   if (zstat < 1) goto repeat;
   return zstat;
}


/********************************************************************************/

//  functions to show popup text windows

namespace poptext {
   char        *ptext = 0;
   GtkWidget   *popwin = 0;
   char        *pcurrent = 0;
   #define GSFNORMAL GTK_STATE_FLAG_NORMAL
}


//  timer function to show popup window after a given time

int poptext_show(char *current)
{
   using namespace poptext;

   if (current != pcurrent) return 0;
   if (popwin) gtk_widget_show_all(popwin);
   return 0;
}


//  timer function to kill popup window after a given time

int poptext_timeout(char *current)
{
   using namespace poptext;

   if (current != pcurrent) return 0;
   if (popwin) gtk_widget_destroy(popwin);
   if (ptext) zfree(ptext);
   popwin = 0;
   ptext = 0;
   return 0;
}


//  kill popup window unconditionally

int poptext_killnow()
{
   using namespace poptext;

   if (popwin) gtk_widget_destroy(popwin);
   if (ptext) zfree(ptext);
   popwin = 0;
   ptext = 0;
   return 0;
}


//  Show a popup text message at a given absolute screen position.
//  Any prior popup will be killed and replaced.
//  If text == null, kill without replacement.
//  secs1 is time to delay before showing the popup.
//  secs2 is time to kill the popup after it is shown (0 = never).
//  This function returns immediately.

void poptext_screen(cchar *text, int px, int py, float secs1, float secs2)
{
   using namespace poptext;

   GtkWidget   *label;
   int         cc, millisec1, millisec2;

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("poptext_screen() called from thread");

   poptext_killnow();

   pcurrent++;                                                                   //  make current != pcurrent

   if (! text) return;

   cc = strlen(text) + 4;                                                        //  construct popup window
   ptext = (char *) zmalloc(cc);                                                 //    with caller's text
   *ptext = 0;
   strncatv(ptext,cc," ",text," ",null);                                         //  add extra spaces
   popwin = gtk_window_new(GTK_WINDOW_POPUP);
   label = gtk_label_new(ptext);
   gtk_container_add(GTK_CONTAINER(popwin),label);
   gtk_window_move(GTK_WINDOW(popwin),px,py);

   if (secs1 > 0) {                                                              //  delayed popup display
      millisec1 = secs1 * 1000;
      g_timeout_add(millisec1,(GSourceFunc) poptext_show,pcurrent);
   }
   else gtk_widget_show_all(popwin);                                             //  immediate display

   if (secs2 > 0) {                                                              //  popup kill timer
      millisec2 = (secs1 + secs2) * 1000;
      g_timeout_add(millisec2,(GSourceFunc) poptext_timeout,pcurrent);
   }

   return;
}


//  Show a popup text message at current mouse position + offsets.

void poptext_mouse(cchar *text, int dx, int dy, float secs1, float secs2)
{
   int   mx, my;

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("poptext_mouse() called from thread");

   if (! text) {
      poptext_killnow();
      return;
   }
   gdk_device_get_position(zfuncs::mouse,0,&mx,&my);                             //  mouse screen position
   poptext_screen(text,mx+dx,my+dy,secs1,secs2);                                 //  add displacements
   return;
}


//  Show a popup text message at the given window position. 

void poptext_window(GtkWindow *win, cchar *text, int dx, int dy, float secs1, float secs2)
{
   int      px, py;

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("poptext_window() called from thread");

   if (! text) {
      poptext_killnow();
      return;
   }
   gtk_window_get_position(win,&px,&py);
   poptext_screen(text,px+dx,py+dy,secs1,secs2);
   return;
}


//  Show a popup text message at the given widget position.

void poptext_widget(GtkWidget *widget, cchar *text, int dx, int dy, float secs1, float secs2)
{
   GdkWindow   *win;
   int         px, py;

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("poptext_widget() called from thread");

   if (! text) {
      poptext_killnow();
      return;
   }
   win = gtk_widget_get_window(widget);
   gdk_window_get_origin(win,&px,&py);
   poptext_screen(text,px+dx,py+dy,secs1,secs2);
   return;
}


/********************************************************************************/

//  Show an image file in a popup window at mouse position.
//  Re-use most recent window or create a new one if Fnewin != 0.
//  Returns 0 if OK, +N otherwise.

namespace popup_image_names
{
   GtkWidget  *window[10], *drawarea[10];                                        //  up to 10 popup windows open
   char       *filex[10], reqfull[10], isfull[10];
   int        Nval[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   int        Nw = 0;
}


int popup_image(cchar *file, GtkWindow *parent, int Fnewin, int size)
{
   using namespace popup_image_names;

   int popup_image_draw(GtkWidget *, cairo_t *, int &Nw);
   int popup_image_scroll(GtkWidget *, GdkEvent *event, int &Nw);
   int popup_image_KBevent(GtkWidget *, GdkEventKey *event, int &Nw);
   int popup_image_mousebutt(GtkWidget *, GdkEvent *event, int &Nw);
   int popup_image_state_event(GtkWidget *, GdkEvent *, int &Nw);

   if (! pthread_equal(pthread_self(),zfuncs::tid_main))                         //  21.0
      zappcrash("popup_image() called from thread");

   if (Fnewin)
      if (++Nw == 10) Nw = 0;                                                    //  new window, re-use oldest up to 10
   if (! Fnewin)
      while (Nw > 0 && window[Nw] == 0) Nw--;                                    //  else re-use latest still active

   if (window[Nw]) {
      gtk_widget_destroy(drawarea[Nw]);
      drawarea[Nw] = 0;
      zfree(filex[Nw]);
      filex[Nw] = 0;
   }
   else {
      window[Nw] = gtk_window_new(GTK_WINDOW_TOPLEVEL);                          //  create new popup window
      if (! window[Nw]) return 1;
      if (! size) size = 512;
      gtk_window_set_default_size(GTK_WINDOW(window[Nw]),size,size);
      if (parent) {
         gtk_window_set_transient_for(GTK_WINDOW(window[Nw]),parent);
         gtk_window_set_destroy_with_parent(GTK_WINDOW(window[Nw]),1);
      }
      gtk_window_set_position(GTK_WINDOW(window[Nw]),GTK_WIN_POS_MOUSE);
      gtk_widget_freeze_child_notify(window[Nw]);                                //  improves resize performance
      G_SIGNAL(window[Nw],"destroy",gtk_widget_destroyed,&window[Nw]);           //  set window = null if destroyed
   }

   filex[Nw] = zstrdup(file);
   drawarea[Nw] = gtk_drawing_area_new();                                        //  new drawing area always required
   if (! drawarea[Nw]) return 2;
   gtk_container_add(GTK_CONTAINER(window[Nw]),drawarea[Nw]);
   reqfull[Nw] = isfull[Nw] = 0;                                                 //  not fullscreen

   G_SIGNAL(window[Nw],"draw",popup_image_draw,&Nval[Nw]);
   gtk_widget_add_events(window[Nw],GDK_SCROLL_MASK);
   G_SIGNAL(window[Nw],"scroll-event",popup_image_scroll,&Nval[Nw]);             //  connect events
   gtk_widget_add_events(window[Nw],GDK_KEY_PRESS_MASK);
   G_SIGNAL(window[Nw],"key-press-event",popup_image_KBevent,&Nval[Nw]);
   gtk_widget_add_events(window[Nw],GDK_BUTTON_RELEASE_MASK);
   G_SIGNAL(window[Nw],"button-release-event",popup_image_mousebutt,&Nval[Nw]);
   G_SIGNAL(window[Nw],"window-state-event",popup_image_state_event,&Nval[Nw]);

   gtk_widget_show_all(window[Nw]);
   return 0;
}


//  resize image and repaint window when resized

int popup_image_draw(GtkWidget *window, cairo_t *cr, int &nn)
{
   using namespace popup_image_names;

   PIXBUF      *pixb1, *pixb2;
   GError      *gerror;
   int         ww1, hh1, ww2, hh2;
   int         sww, shh;
   double      area;
   char        *file;
   cchar       *pp;

   file = filex[nn];
   if (! file) return 1;

   pp = strrchr(file,'/');                                                       //  window title = file name
   gtk_window_set_title(GTK_WINDOW(window),pp+1);

   gerror = 0;
   pixb1 = gdk_pixbuf_new_from_file(file,&gerror);                               //  load image file into pixbuf
   if (! pixb1) {
      printz("*** file: %s \n %s \n",file,gerror->message);
      return 1;
   }

   ww1 = gdk_pixbuf_get_width(pixb1);                                            //  image dimensions
   hh1 = gdk_pixbuf_get_height(pixb1);

   sww = monitor_ww;
   shh = monitor_hh;

   gtk_window_get_size(GTK_WINDOW(window),&ww2,&hh2);                            //  current window dimensions
   area = ww2 * hh2;
   ww2 = sqrt(area * ww1 / hh1);                                                 //  fit window to image, keeping same area
   hh2 = area / ww2;
   if (ww2 < sww && hh2 < shh)                                                   //  prevent GTK resize event loop
      gtk_window_resize(GTK_WINDOW(window),ww2,hh2);

   pixb2 = gdk_pixbuf_scale_simple(pixb1,ww2,hh2,GDK_INTERP_BILINEAR);           //  rescale pixbuf to window
   if (! pixb2) return 1;

   gdk_cairo_set_source_pixbuf(cr,pixb2,0,0);                                    //  draw image
   cairo_paint(cr);

   g_object_unref(pixb1);
   g_object_unref(pixb2);
   return 1;
}


//  respond to mouse scroll button and zoom window larger or smaller

int popup_image_scroll(GtkWidget *window, GdkEvent *event, int &nn)
{
   using namespace popup_image_names;

   int         scroll, ww, hh;
   int         sww, shh;
   double      ff = 1.0;

   if (event->type == GDK_SCROLL) {                                              //  mouse wheel event
      scroll = ((GdkEventScroll *) event)->direction;
      if (scroll == GDK_SCROLL_UP) ff = 1.33333;
      if (scroll == GDK_SCROLL_DOWN) ff = 0.75;
   }

   gtk_window_get_size(GTK_WINDOW(window),&ww,&hh);                              //  current window dimensions
   ww *= ff;                                                                     //  new dimensions
   hh *= ff;

   sww = monitor_ww;
   shh = monitor_hh;
   
   if (ww > sww || hh > shh) {                                                   //  request > screen size, fullscreen
      reqfull[nn] = 1;
      gtk_window_fullscreen(GTK_WINDOW(window));
      return 1;
   }

   reqfull[nn] = 0;
   gtk_window_unfullscreen(GTK_WINDOW(window));

   if (ww + hh > 500)
      gtk_window_resize(GTK_WINDOW(window),ww,hh);                               //  rescale up or down
   else
      gtk_widget_destroy(window);                                                //  if very small, delete window

   return 1;
}


//  respond to KB events F11 (fullscreen/unfullscreen) and Escape (destroy)

int popup_image_KBevent(GtkWidget *window, GdkEventKey *event, int &nn)
{
   using namespace popup_image_names;

   int KBkey = event->keyval;

   if (KBkey == GDK_KEY_Escape) gtk_widget_destroy(window);
   if (KBkey != GDK_KEY_F11) return 1;

   if (reqfull[nn]) {
      reqfull[nn] = 0;
      gtk_window_unfullscreen(GTK_WINDOW(window));
   }
   else {
      reqfull[nn] = 1;
      gtk_window_fullscreen(GTK_WINDOW(window));
   }      

   return 1;
}


//  respond to mouse button - destroy window

int popup_image_mousebutt(GtkWidget *window, GdkEvent *event, int &nn) 
{
   gtk_widget_destroy(window);
   return 1;
}


//  track window fullscreen state

int popup_image_state_event(GtkWidget *window, GdkEvent *event, int &nn)
{
   using namespace popup_image_names;
   
   int state = ((GdkEventWindowState *) event)->new_window_state;
   if (state & GDK_WINDOW_STATE_FULLSCREEN) isfull[nn] = 1;
   else isfull[nn] = 0;

   if (isfull[nn] != reqfull[nn]) {                                              //  compensate GTK bug:    FIXME
      if (reqfull[nn]) gtk_window_fullscreen(GTK_WINDOW(window));                //   the window fullscreens itself after
      else gtk_window_unfullscreen(GTK_WINDOW(window));                          //    being requested to unfullscreen
   }

   return 1;
}


/********************************************************************************

    File chooser dialog for one or more files

    Action:  "file"            select an existing file
             "files"           select multiple existing files
             "save"            select an existing or new file
             "folder"          select existing folder
             "folders"         select multiple existing folders
             "create folder"   select existing or new folder

    hidden   if > 0, add button to toggle display of hidden files
             optional, default = 0

    Returns a list of filespecs terminated with null.
    Memory for returned list and returned files are subjects for zfree();

*********************************************************************************/


//  version for 1 file only: file, save, folder, create folder
//  returns one filespec or null
//  returned file is subject for zfree()

char * zgetfile(cchar *title, GtkWindow *parent, cchar *action, cchar *initfile, int hidden)
{
   if (! strmatchV(action,"file","save","folder","create folder",null))
      zappcrash("zgetfile() call error: %s",action);

   char **flist = zgetfiles(title,parent,action,initfile,hidden);
   if (! flist) return 0;
   char *file = *flist;
   zfree(flist);
   return file;
}


//  version for 2 or more files
//  returns a list of filespecs (char **) terminated with null
//  returns null if canceled by user

char ** zgetfiles(cchar *title, GtkWindow *parent, cchar *action, cchar *initfile, int hidden)
{
   void zgetfile_preview(GtkWidget *dialog, GtkWidget *pvwidget);                //  private functions
   int  zgetfile_KBkey(GtkWidget *dialog, GdkEventKey *event, int &fcdes);
   void zgetfile_newfolder(GtkFileChooser *dialog, void *);

   GtkFileChooserAction fcact = GTK_FILE_CHOOSER_ACTION_OPEN;

   GtkWidget   *dialog;
   PIXBUF      *thumbnail;
   GtkWidget   *pvwidget = gtk_image_new();
   GSList      *gslist = 0;
   cchar       *button1 = 0, *buttxx = 0;
   char        *pdir, *pfile;
   int         ii, err, NF, setfname = 0;
   int         fcstat, bcode = 0, hide = 1;
   int         fcdes = 0;
   char        *file1, *file2, **flist = 0;
   STATB       fstat;

   if (strmatch(action,"file")) {
      fcact = GTK_FILE_CHOOSER_ACTION_OPEN;
      button1 = E2X("choose file");
   }

   else if (strmatch(action,"files")) {
      fcact = GTK_FILE_CHOOSER_ACTION_OPEN;
      button1 = E2X("choose files");
   }

   else if (strmatch(action,"save")) {
      fcact = GTK_FILE_CHOOSER_ACTION_SAVE;
      button1 = E2X("Save");
      setfname = 1;
   }

   else if (strmatch(action,"folder")) {
      fcact = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
      button1 = E2X("choose folder");
   }

   else if (strmatch(action,"folders")) {
      fcact = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
      button1 = E2X("choose folders");
   }

   else if (strmatch(action,"create folder")) {
      fcact = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
      button1 = E2X("create folder");
      setfname = 1;
   }

   else zappcrash("zgetfiles() call error: %s",action);

   if (hidden) {
      buttxx = E2X("hidden");
      bcode = 103;
   }
   
   dialog = gtk_file_chooser_dialog_new(title, parent, fcact,                    //  create file selection dialog
                              button1, GTK_RESPONSE_ACCEPT,                      //  parent added
                              E2X("Cancel"), GTK_RESPONSE_CANCEL,
                              buttxx, bcode, null);

   gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(dialog),pvwidget);

   G_SIGNAL(dialog,"update-preview",zgetfile_preview,pvwidget);                  //  create preview for selected file
   G_SIGNAL(dialog,"key-press-event",zgetfile_KBkey,&fcdes);                     //  respond to special KB keys

   gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);                //  put dialog at mouse position
   gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog),0);                 //  default: no show hidden

   if (strmatch(action,"save"))                                                  //  overwrite confirmation
      gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),1);

   if (strmatch(action,"files") || strmatch(action,"folders"))
      gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog),1);          //  select multiple files or folders

   if (initfile) {                                                               //  pre-select filespec
      err = stat(initfile,&fstat);
      if (err) {
         pdir = zstrdup(initfile);                                               //  non-existent file
         pfile = strrchr(pdir,'/');
         if (pfile && pfile > pdir) {
            *pfile++ = 0;                                                        //  set folder name
            gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),pdir);
         }
         if (setfname) {                                                         //  set new file name
            if (! pfile) pfile = (char *) initfile;
            gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),pfile);
         }
         zfree(pdir);
      }
      else if (S_ISREG(fstat.st_mode))                                           //  select given file
         gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),initfile);
      else if (S_ISDIR(fstat.st_mode))                                           //  select given folder
         gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),initfile);
   }

   if (initfile) {
      thumbnail = get_thumbnail(initfile,256);                                   //  preview for initial file
      if (thumbnail) {
         gtk_image_set_from_pixbuf(GTK_IMAGE(pvwidget),thumbnail);
         gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(dialog),1);
         g_object_unref(thumbnail);
      }
      else gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(dialog),0);
   }

   gtk_widget_show_all(dialog);

   while (true)
   {
      fcstat = gtk_dialog_run(GTK_DIALOG(dialog));                               //  run dialog, get status button

      if (fcstat == 103) {                                                       //  show/hide hidden files
         hide = 1 - hide;
         gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog),hide);
         continue;
      }

      else if (fcstat == GTK_RESPONSE_ACCEPT)
      {
         gslist = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
         if (! gslist) continue;

         NF = g_slist_length(gslist);                                            //  no. selected files
         flist = (char **) zmalloc((NF+1)*sizeof(char *));                       //  allocate returned list

         for (ii = 0; ii < NF; ii++)
         {                                                                       //  process selected files
            file1 = (char *) g_slist_nth_data(gslist,ii);
            file2 = zstrdup(file1);                                              //  re-allocate memory
            flist[ii] = file2;
            g_free(file1);
         }
         flist[ii] = 0;                                                          //  EOL marker
         break;
      }

      else break;                                                                //  user bailout
   }

   if (gslist) g_slist_free(gslist);                                             //  return selected file(s)
   if (! fcdes) gtk_widget_destroy(dialog);                                      //  destroy if not already
   return flist;
}


//  zgetfile private function - get preview images for image files

void zgetfile_preview(GtkWidget *dialog, GtkWidget *pvwidget)
{
   PIXBUF      *thumbnail;
   char        *filename;

   filename = gtk_file_chooser_get_preview_filename(GTK_FILE_CHOOSER(dialog));

   if (! filename) {
      gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(dialog),0);
      return;
   }

   thumbnail = get_thumbnail(filename,256);                                      //  256x256 pixels
   g_free(filename);

   if (thumbnail) {
      gtk_image_set_from_pixbuf(GTK_IMAGE(pvwidget),thumbnail);
      gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(dialog),1);
      g_object_unref(thumbnail);
   }
   else gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(dialog),0);

   return;
}


//  zgetfile private function - KB functions

int zgetfile_KBkey(GtkWidget *dialog, GdkEventKey *event, int &fcdes)
{
   int KBkey = event->keyval;

   if (KBkey == GDK_KEY_F1) {                                                    //  F1 = help
      KBevent(event);
      return 1;
   }
   
   if (KBkey == GDK_KEY_Escape) {                                                //  escape = cancel
      gtk_widget_destroy(dialog);
      fcdes = 1;
      return 1;
   }
   
   return 0;
}


/********************************************************************************/

//  select a folder (or create a new folder)
//  returns location (pathname) of selected or created folder.
//  returned location is subject for zfree().

char * zgetfolder(cchar *title, GtkWindow *parent, cchar *initfolder)
{
   GtkWidget      *dialog;
   GtkFileChooser *chooser;
   int      nn;
   char     *pp1, *pp2 = null;

   dialog = gtk_file_chooser_dialog_new(title, parent,
                  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                  E2X("Cancel"), GTK_RESPONSE_CANCEL,
                  E2X("Open"), GTK_RESPONSE_ACCEPT, NULL);

   chooser = GTK_FILE_CHOOSER(dialog);
   gtk_file_chooser_set_filename(chooser, initfolder);

   nn = gtk_dialog_run(GTK_DIALOG(dialog));
   if (nn != GTK_RESPONSE_ACCEPT) {
      gtk_widget_destroy(dialog);
      return null;
   }

   pp1 = gtk_file_chooser_get_filename(chooser);
   if (pp1) {
      pp2 = zstrdup(pp1);
      g_free(pp1);
   }

   gtk_widget_destroy(dialog);
   return pp2;
}


/********************************************************************************

   print_image_file(GtkWidget *parent, cchar *imagefile)

   Print an image file using the printer, paper, orientation,
   margins, and scale set by the user.

   HPLIP problem: Setting paper size was made less flexible.
   GtkPrintSettings paper size must agree with the one in the current
   printer setup. This can only be set in the printer setup dialog, not
   in the application. Also the print size (width, height) comes from
   the chosen paper size and cannot be changed in the application.
   Print margins can be changed to effect printing a smaller or shifted
   image on a larger paper size.

*********************************************************************************/

namespace print_image
{
   #define MM GTK_UNIT_MM
   #define INCH GTK_UNIT_INCH
   #define PRINTOP   GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG
   #define PORTRAIT  GTK_PAGE_ORIENTATION_PORTRAIT
   #define LANDSCAPE GTK_PAGE_ORIENTATION_LANDSCAPE
   #define QUALITY   GTK_PRINT_QUALITY_HIGH

   GtkWidget                  *parent = 0;
   GtkPageSetup               *priorpagesetup = 0;
   GtkPageSetup               *pagesetup;
   GtkPrintSettings           *printsettings = 0;
   GtkPrintOperation          *printop;
   GtkPageOrientation         orientation = PORTRAIT;
   PIXBUF                     *pixbuf;
   cchar                      *printer = 0;
   int                        landscape = 0;                                     //  true if landscape
   double                     width = 21.0, height = 29.7;                       //  paper size, CM (default A4 portrait)
   double                     margins[4] = { 0.5, 0.5, 0.5, 0.5 };               //  margins, CM (default 0.5)
   double                     imagescale = 100;                                  //  image print scale, percent
   double                     pwidth, pheight;                                   //  printed image size

   int   page_setup();
   int   margins_setup();
   int   margins_dialog_event(zdialog *zd, cchar *event);
   void  get_printed_image_size();
   void  print_page(GtkPrintOperation *, GtkPrintContext *, int page);
}


//  user callable function to set paper, margins, scale, and then print

void print_image_file(GtkWidget *pwin, cchar *imagefile)
{
   using namespace print_image;

   GtkPrintOperationResult  printstat;
   GError   *gerror = 0;
   int      err;

   parent = pwin;                                                                //  save parent window

   pixbuf = gdk_pixbuf_new_from_file(imagefile,&gerror);                         //  read image file
   if (! pixbuf) {
      zmessageACK(null,gerror->message);
      return;
   }

   err = page_setup();                                                           //  select size and orientation
   if (err) return;

   err = margins_setup();                                                        //  set margins and scale
   if (err) return;

   printop = gtk_print_operation_new();                                          //  print operation
   gtk_print_operation_set_default_page_setup(printop,pagesetup);
   gtk_print_operation_set_print_settings(printop,printsettings);
   gtk_print_operation_set_n_pages(printop,1);

   g_signal_connect(printop,"draw-page",G_CALLBACK(print_page),0);               //  start print
   printstat = gtk_print_operation_run(printop,PRINTOP,0,0);

   if (printstat == GTK_PRINT_OPERATION_RESULT_ERROR) {
      gtk_print_operation_get_error(printop,&gerror);
      zmessageACK(null,gerror->message);
   }

   g_object_unref(printop);
   return;
}


//  draw the graphics for the print page
//  rescale with cairo

void print_image::print_page(GtkPrintOperation *printop, GtkPrintContext *printcontext, int page)
{
   using namespace print_image;

   cairo_t           *cairocontext;
   double            iww, ihh, pww, phh, scale;

   pww = gtk_print_context_get_width(printcontext);                              //  print context size, pixels
   phh = gtk_print_context_get_height(printcontext);

   iww = gdk_pixbuf_get_width(pixbuf);                                           //  original image size
   ihh = gdk_pixbuf_get_height(pixbuf);

   scale = pww / iww;                                                            //  rescale to fit page
   if (phh / ihh < scale) scale = phh / ihh;

   cairocontext = gtk_print_context_get_cairo_context(printcontext);             //  use cairo to rescale
   cairo_translate(cairocontext,0,0);
   cairo_scale(cairocontext,scale,scale);
   gdk_cairo_set_source_pixbuf(cairocontext,pixbuf,0,0);
   cairo_paint(cairocontext);

   return;
}


//   Do a print paper format selection, after which the page width, height
//   and orientation are available to the caller. Units are CM.
//   (paper width and height are reversed for landscape orientation)

int print_image::page_setup()
{
   using namespace print_image;

   char     printsettingsfile[200], pagesetupfile[200];

   snprintf(printsettingsfile,200,"%s/printsettings",zhomedir);
   snprintf(pagesetupfile,200,"%s/pagesetup",zhomedir);

   if (! printsettings) {                                                        //  start with prior print settings
      printsettings = gtk_print_settings_new_from_file(printsettingsfile,0);
      if (! printsettings)
         printsettings = gtk_print_settings_new();
   }

   if (! priorpagesetup) {                                                       //  start with prior page setup
      priorpagesetup = gtk_page_setup_new_from_file(pagesetupfile,0);
      if (! priorpagesetup)
         priorpagesetup = gtk_page_setup_new();
   }

   pagesetup = gtk_print_run_page_setup_dialog                                   //  select printer, paper, orientation
            (GTK_WINDOW(parent),priorpagesetup,printsettings);                   //  user cancel cannot be detected

   g_object_unref(priorpagesetup);                                               //  save for next call
   priorpagesetup = pagesetup;

   orientation = gtk_print_settings_get_orientation(printsettings);              //  save orientation
   if (orientation == LANDSCAPE) landscape = 1;
   else landscape = 0;

   gtk_print_settings_set_quality(printsettings,QUALITY);                        //  set high quality 300 dpi
   gtk_print_settings_set_resolution(printsettings,300);

   gtk_print_settings_to_file(printsettings,printsettingsfile,0);                //  save print settings to file
   gtk_page_setup_to_file(pagesetup,pagesetupfile,0);                            //  save print settings to file

   return 0;
}


//   Optionally set the print margins and print scale.
//   If canceled the margins are zero (or printer-dependent minimum)
//   and the scale is 100% (fitting the paper and margins).

int print_image::margins_setup()
{
   using namespace print_image;

   zdialog     *zd;
   int         zstat;

/***
       __________________________________________________
      | [x] (-) [_]   Margins                            |
      |                                                  |
      |  Margins   Top      Bottom     Left     Right    |
      |     CM   [ 0.50 ]  [ 0.50 ]  [ 0.50 ]  [ 0.50 ]  |
      |    Inch  [ 0.20 ]  [ 0.20 ]  [ 0.20 ]  [ 0.20 ]  |
      |                                                  |
      |  image scale [ 80 ] percent                      |
      |                                                  |
      |  image  width  height                            |
      |    CM    xx.x   xx.x                             |
      |   Inch   xx.x   xx.x                             |
      |                                 [ OK ] [cancel]  |
      |__________________________________________________|

***/

   zd = zdialog_new(E2X("Margins"),parent,E2X("done"),E2X("cancel"),null);
   zdialog_add_widget(zd,"hbox","hbmlab","dialog");

   zdialog_add_widget(zd,"vbox","vbmarg","hbmlab",0,"homog|space=3");
   zdialog_add_widget(zd,"vbox","vbtop","hbmlab",0,"homog|space=3");
   zdialog_add_widget(zd,"vbox","vbbottom","hbmlab",0,"homog|space=3");
   zdialog_add_widget(zd,"vbox","vbleft","hbmlab",0,"homog|space=3");
   zdialog_add_widget(zd,"vbox","vbright","hbmlab",0,"homog|space=3");

   zdialog_add_widget(zd,"label","labmarg","vbmarg",E2X("Margins"),"space=5");
   zdialog_add_widget(zd,"label","labcm","vbmarg","CM","space=5");
   zdialog_add_widget(zd,"label","labinch","vbmarg","Inch","space=5");

   zdialog_add_widget(zd,"label","labtop","vbtop",E2X("Top"));
   zdialog_add_widget(zd,"zspin","mtopcm","vbtop","0|10|0.01|0");
   zdialog_add_widget(zd,"zspin","mtopin","vbtop","0|4|0.01|0");

   zdialog_add_widget(zd,"label","labbot","vbbottom",E2X("Bottom"));
   zdialog_add_widget(zd,"zspin","mbottcm","vbbottom","0|10|0.01|0");
   zdialog_add_widget(zd,"zspin","mbottin","vbbottom","0|4|0.01|0");

   zdialog_add_widget(zd,"label","lableft","vbleft",E2X("Left"));
   zdialog_add_widget(zd,"zspin","mleftcm","vbleft","0|10|0.01|0");
   zdialog_add_widget(zd,"zspin","mleftin","vbleft","0|4|0.01|0");

   zdialog_add_widget(zd,"label","labright","vbright",E2X("Right"));
   zdialog_add_widget(zd,"zspin","mrightcm","vbright","0|10|0.01|0");
   zdialog_add_widget(zd,"zspin","mrightin","vbright","0|4|0.01|0");

   zdialog_add_widget(zd,"hbox","hbscale","dialog",0,"space=5");
   zdialog_add_widget(zd,"label","labscale","hbscale",E2X("image scale"),"space=5");
   zdialog_add_widget(zd,"zspin","scale","hbscale","5|100|1|100");
   zdialog_add_widget(zd,"label","labpct","hbscale",E2X("percent"),"space=5");

   zdialog_add_widget(zd,"hbox","hbsize","dialog",0,"space=3");
   zdialog_add_widget(zd,"vbox","vbunit","hbsize",0,"space=5");
   zdialog_add_widget(zd,"vbox","vbwidth","hbsize",0,"space=5");
   zdialog_add_widget(zd,"vbox","vbheight","hbsize",0,"space=5");

   zdialog_add_widget(zd,"label","space","vbunit",E2X("Image"));
   zdialog_add_widget(zd,"label","labcm","vbunit","CM");
   zdialog_add_widget(zd,"label","labinch","vbunit","Inch");

   zdialog_add_widget(zd,"label","labwidth","vbwidth",E2X("Width"));
   zdialog_add_widget(zd,"label","labwcm","vbwidth","xx.x");
   zdialog_add_widget(zd,"label","labwin","vbwidth","xx.x");

   zdialog_add_widget(zd,"label","labheight","vbheight",E2X("Height"));
   zdialog_add_widget(zd,"label","labhcm","vbheight","xx.x");
   zdialog_add_widget(zd,"label","labhin","vbheight","xx.x");

   zdialog_restore_inputs(zd);                                                   //  recall prior settings

   zdialog_fetch(zd,"mtopcm",margins[0]);
   zdialog_fetch(zd,"mbottcm",margins[1]);
   zdialog_fetch(zd,"mleftcm",margins[2]);
   zdialog_fetch(zd,"mrightcm",margins[3]);
   zdialog_fetch(zd,"scale",imagescale);

   get_printed_image_size();
   zdialog_stuff(zd,"labwcm",pwidth,"%.2f");                                     //  update image size in dialog
   zdialog_stuff(zd,"labhcm",pheight,"%.2f");
   zdialog_stuff(zd,"labwin",pwidth/2.54,"%.2f");
   zdialog_stuff(zd,"labhin",pheight/2.54,"%.2f");

   gtk_page_setup_set_top_margin(pagesetup,10*margins[0],MM);                    //  set page margins
   gtk_page_setup_set_bottom_margin(pagesetup,10*margins[1],MM);                 //  (cm to mm units)
   gtk_page_setup_set_left_margin(pagesetup,10*margins[2],MM);
   gtk_page_setup_set_right_margin(pagesetup,10*margins[3],MM);
   gtk_print_settings_set_scale(printsettings,imagescale);                       //  set image print scale %

   zdialog_run(zd,margins_dialog_event,"parent");                                //  run dialog
   zstat = zdialog_wait(zd);                                                     //  wait for completion
   zdialog_free(zd);                                                             //  kill dialog

   if (zstat == 1) return 0;
   return 1;
}


//  dialog event function
//  save user margin and scale changes
//  recompute print image size

int print_image::margins_dialog_event(zdialog *zd, cchar *event)
{
   using namespace print_image;

   double   temp;
   
   if (strmatch(event,"escape")) {                                               //  escape = cancel
      zd->zstat = 2;
      return 1;
   }

   if (strmatch(event,"mtopcm")) {                                               //  get cm inputs and set inch values
      zdialog_fetch(zd,"mtopcm",margins[0]);
      zdialog_stuff(zd,"mtopin",margins[0]/2.54);
   }

   if (strmatch(event,"mbottcm")) {
      zdialog_fetch(zd,"mbottcm",margins[1]);
      zdialog_stuff(zd,"mbottin",margins[1]/2.54);
   }

   if (strmatch(event,"mleftcm")) {
      zdialog_fetch(zd,"mleftcm",margins[2]);
      zdialog_stuff(zd,"mleftin",margins[2]/2.54);
   }

   if (strmatch(event,"mrightcm")) {
      zdialog_fetch(zd,"mrightcm",margins[3]);
      zdialog_stuff(zd,"mrightin",margins[3]/2.54);
   }

   if (strmatch(event,"mtopin")) {                                               //  get inch inputs and set cm values
      zdialog_fetch(zd,"mtopin",temp);
      margins[0] = temp * 2.54;
      zdialog_stuff(zd,"mtopcm",margins[0]);
   }

   if (strmatch(event,"mbottin")) {
      zdialog_fetch(zd,"mbottin",temp);
      margins[1] = temp * 2.54;
      zdialog_stuff(zd,"mbottcm",margins[1]);
   }

   if (strmatch(event,"mleftin")) {
      zdialog_fetch(zd,"mleftin",temp);
      margins[2] = temp * 2.54;
      zdialog_stuff(zd,"mleftcm",margins[2]);
   }

   if (strmatch(event,"mrightin")) {
      zdialog_fetch(zd,"mrightin",temp);
      margins[3] = temp * 2.54;
      zdialog_stuff(zd,"mrightcm",margins[3]);
   }

   zdialog_fetch(zd,"scale",imagescale);                                         //  get image scale

   get_printed_image_size();
   zdialog_stuff(zd,"labwcm",pwidth,"%.2f");                                     //  update image size in dialog
   zdialog_stuff(zd,"labhcm",pheight,"%.2f");
   zdialog_stuff(zd,"labwin",pwidth/2.54,"%.2f");
   zdialog_stuff(zd,"labhin",pheight/2.54,"%.2f");

   gtk_page_setup_set_top_margin(pagesetup,10*margins[0],MM);                    //  set page margins
   gtk_page_setup_set_bottom_margin(pagesetup,10*margins[1],MM);                 //  (cm to mm units)
   gtk_page_setup_set_left_margin(pagesetup,10*margins[2],MM);
   gtk_page_setup_set_right_margin(pagesetup,10*margins[3],MM);
   gtk_print_settings_set_scale(printsettings,imagescale);                       //  set image print scale %

   return 1;
}


//  compute printed image size based on paper size,
//    orientation, margins, and scale (percent)

void print_image::get_printed_image_size()
{
   using namespace print_image;

   double   iww, ihh, pww, phh, scale;

   pww = 0.1 * gtk_page_setup_get_paper_width(pagesetup,MM);                     //  get paper size
   phh = 0.1 * gtk_page_setup_get_paper_height(pagesetup,MM);                    //  (mm to cm units)

   pww = pww - margins[2] - margins[3];                                          //  reduce for margins
   phh = phh - margins[0] - margins[1];

   pww = pww / 2.54 * 300;                                                       //  convert to dots @ 300 dpi
   phh = phh / 2.54 * 300;

   iww = gdk_pixbuf_get_width(pixbuf);                                           //  original image size, pixels
   ihh = gdk_pixbuf_get_height(pixbuf);

   scale = pww / iww;                                                            //  rescale image to fit page
   if (phh / ihh < scale) scale = phh / ihh;

   scale = scale * 0.01 * imagescale;                                            //  adjust for user scale setting

   pwidth = iww * scale / 300 * 2.54;                                            //  dots to cm
   pheight = ihh * scale / 300 * 2.54;

   return;
}


/********************************************************************************/

//  connect a user callback function to a drag-drop source widget

void drag_drop_source(GtkWidget *widget, drag_drop_source_func ufunc)
{
   void drag_drop_source2(GtkWidget *, GdkDragContext *, void *ufunc);
   void drag_drop_source3(GtkWidget *, GdkDragContext *, GtkSelectionData *, int, int, void *ufunc);

   gtk_drag_source_set(widget,GDK_BUTTON1_MASK,null,0,GDK_ACTION_COPY);
   gtk_drag_source_add_text_targets(widget);
   gtk_drag_source_add_image_targets(widget);
   G_SIGNAL(widget, "drag-begin", drag_drop_source2, ufunc);
   G_SIGNAL(widget, "drag-data-get", drag_drop_source3, ufunc);
   return;
}


//  private function for "drag-begin" signal

void drag_drop_source2(GtkWidget *widget, GdkDragContext *context, void *ufunc)
{
   drag_drop_source_func  *ufunc2;

   GdkPixbuf   *pixbuf;
   GError      *gerror = 0;
   char        *file = 0;
   
   ufunc2 = (drag_drop_source_func *) ufunc;
   file = ufunc2();
   if (! file) goto cancel;

   pixbuf = gdk_pixbuf_new_from_file_at_size(file,128,128,&gerror);
   if (! pixbuf) {
      if (gerror) printz("%s \n",gerror->message);
      return;
   }

   gtk_drag_set_icon_pixbuf(context,pixbuf,64,64);                               //  hot spot is middle of image
   return;

cancel: 
   printz("drag canceled \n");
   return;
}   


//  private function for "drag-data-get" signal

void drag_drop_source3(GtkWidget *widget, GdkDragContext *context, GtkSelectionData *data, int, int, void *ufunc)
{
   drag_drop_source_func  *ufunc2;

   char        *file = 0;
// char        *files[2] = { file, null };
   
   ufunc2 = (drag_drop_source_func *) ufunc;
   file = ufunc2();
   if (! file) goto cancel;
   gtk_selection_data_set_text(data,file,-1);                                    //  drops text
// gtk_selection_data_set_uris(data,files);                                      //  does nothing     FIXME
   return;

cancel: 
   printz("drag canceled \n");
   return;
}


//  connect a user callback function to a drag-drop destination widget

void drag_drop_dest(GtkWidget *widget, drag_drop_dest_func *ufunc)
{
   int  drag_drop_dest2(GtkWidget *, GdkDragContext *, int, int, void *, int, int time, void *);
   int  drag_drop_dest3(GtkWidget *, void *, int, int, int, void *);
   int  drag_drop_dest4(GtkWidget *, void *, int, void *);

   gtk_drag_dest_set(widget,GTK_DEST_DEFAULT_ALL,null,0,GDK_ACTION_COPY);
   gtk_drag_dest_add_text_targets(widget);
   G_SIGNAL(widget, "drag-data-received", drag_drop_dest2, ufunc);
   G_SIGNAL(widget, "drag-motion", drag_drop_dest3, ufunc);
   G_SIGNAL(widget, "drag-leave", drag_drop_dest4, ufunc);

   return;
}


//  private function for "drag-data-received" signal
//  get dropped file, clean escapes, pass to user function
//  passed filespec is subject for zfree()

int drag_drop_dest2(GtkWidget *, GdkDragContext *context, int mpx, int mpy, void *sdata, int, int time, void *ufunc)
{
   char  * drag_drop_unescape(cchar *escaped_string);
   drag_drop_dest_func  *ufunc2;

   char     *text, *text2, *file, *file2;
   int      cc;

   text = (char *) gtk_selection_data_get_data((GtkSelectionData *) sdata);

   ufunc2 = (drag_drop_dest_func *) ufunc;

   if (strstr(text,"file://"))                                                   //  text is a filespec
   {
      file = zstrdup(text+7);                                                    //  get rid of junk added by GTK
      cc = strlen(file);
      while (file[cc-1] < ' ') cc--;
      file[cc] = 0;
      file2 = drag_drop_unescape(file);                                          //  clean %xx escapes from Nautilus
      zfree(file);
      ufunc2(mpx,mpy,file2);                                                     //  pass file to user function
   }

   else                                                                          //  text is text
   {
      text2 = zstrdup(text);
      ufunc2(mpx,mpy,text2);
   }

   gtk_drag_finish(context,1,0,time); 
   return 1;
}


//  private function for "drag-motion" signal
//  pass mouse position to user function during drag

int drag_drop_dest3(GtkWidget *, void *, int mpx, int mpy, int, void *ufunc)
{
   drag_drop_dest_func  *ufunc2;
   ufunc2 = (drag_drop_dest_func *) ufunc;
   if (! ufunc2) return 0;
   ufunc2(mpx,mpy,null);
   return 0;
}


//  private function for "drag-leave" signal
//  pass mouse position (0,0) to user function

int  drag_drop_dest4(GtkWidget *, void *, int, void *ufunc)
{
   drag_drop_dest_func  *ufunc2;
   ufunc2 = (drag_drop_dest_func *) ufunc;
   if (! ufunc2) return 0;
   ufunc2(0,0,null);
   return 0;
}


//  private function
//  Clean %xx escapes from strange Nautilus drag-drop file names

char * drag_drop_unescape(cchar *inp)
{
   int  drag_drop_convhex(char ch);

   char     inch, *out, *outp;
   int      nib1, nib2;

   out = (char *) zmalloc(strlen(inp)+1);
   outp = out;

   while ((inch = *inp++))
   {
      if (inch == '%')
      {
         nib1 = drag_drop_convhex(*inp++);
         nib2 = drag_drop_convhex(*inp++);
         *outp++ = nib1 << 4 | nib2;
      }
      else *outp++ = inch;
   }

   *outp = 0;
   return out;
}


//  private function - convert character 0-F to number 0-15

int drag_drop_convhex(char ch)
{
   if (ch >= '0' && ch <= '9') return  ch - '0';
   if (ch >= 'A' && ch <= 'F') return  ch - 'A' + 10;
   if (ch >= 'a' && ch <= 'f') return  ch - 'a' + 10;
   return ch;
}


/********************************************************************************
   Miscellaneous GDK/GTK functions
*********************************************************************************/

//  Get thumbnail image for given image file.
//  Returned thumbnail belongs to caller: g_object_unref() is necessary.

PIXBUF * get_thumbnail(cchar *fpath, int size)
{
   PIXBUF      *thumbpxb;
   GError      *gerror = 0;
   int         err;
   char        *bpath;
   STATB       statf;

   err = stat(fpath,&statf);                                                     //  fpath status info
   if (err) return 0;

   if (S_ISDIR(statf.st_mode)) {                                                 //  if folder, return folder image
      bpath = (char *) zmalloc(500);
      *bpath = 0;
      strncatv(bpath,499,zimagedir,"/folder.png",null);
      thumbpxb = gdk_pixbuf_new_from_file_at_size(bpath,size,size,&gerror);
      zfree(bpath);
      return thumbpxb;
   }

   thumbpxb = gdk_pixbuf_new_from_file_at_size(fpath,size,size,&gerror);
   return thumbpxb;                                                              //  return pixbuf to caller
}


//  make a cursor from a graphic file in application folder
//  (see initz_appfiles()).

GdkCursor * zmakecursor(cchar *imagefile)
{
   GError         *gerror = 0;
   PIXBUF         *pixbuf;
   GdkDisplay     *display;
   GdkCursor      *cursor = 0;
   char           imagepath[200];

   display = gdk_display_get_default();
   *imagepath = 0;
   strncatv(imagepath,199,zimagedir,"/",imagefile,null);
   pixbuf = gdk_pixbuf_new_from_file(imagepath,&gerror);
   if (pixbuf && display)
      cursor = gdk_cursor_new_from_pixbuf(display,pixbuf,0,0);
   else printz("*** %s \n",gerror->message);
   return cursor;
}


/********************************************************************************

   PIXBUF * gdk_pixbuf_rotate(PIXBUF *pixbuf, float angle, int acolor)

   Rotate a pixbuf through an arbitrary angle (degrees).

   The returned image has the same size as the original, but the
   pixbuf envelope is increased to accommodate the rotated original
   (e.g. a 100x100 pixbuf rotated 45 deg. needs a 142x142 pixbuf).

   Pixels added around the rotated image have all RGB values = acolor.
   Angle is in degrees. Positive direction is clockwise.
   Pixbuf must have 8 bits per channel and 3 or 4 channels.
   Loss of resolution is about 1/2 pixel.
   Speed is about 28 million pixels/sec. on 3.3 GHz CPU.
   (e.g. a 10 megapix image needs about 0.36 seconds)

   NULL is returned if the function fails for one of the following:
      - pixbuf not 8 bits/channel or < 3 channels
      - unable to create output pixbuf (lack of memory?)

   Algorithm:
      create output pixbuf big enough for rotated input pixbuf
      compute coefficients for affine transform
      loop all output pixels (px2,py2)
         get corresp. input pixel (px1,py1) using affine transform
         if outside of pixbuf
            output pixel = black
            continue
         for 4 input pixels based at (px0,py0) = (int(px1),int(py1))
            compute overlap (0 to 1) with (px1,py1)
            sum RGB values * overlap
         output aggregate RGB to pixel (px2,py2)

   Benchmark: rotate 7 megapixel image 10 degrees
               0.31 secs.  3.3 GHz Core i5

***/

PIXBUF * gdk_pixbuf_rotate(PIXBUF *pixbuf1, float angle, int acolor)
{
   typedef unsigned char  *pixel;                                                //  3 RGB values, 0-255 each

   PIXBUF      *pixbuf2;
   GDKCOLOR    color;

   int      nch, nbits, alpha;
   int      ww1, hh1, rs1, ww2, hh2, rs2;
   int      px2, py2, px0, py0;
   pixel    ppix1, ppix2, pix0, pix1, pix2, pix3;
   float    px1, py1;
   float    f0, f1, f2, f3, red, green, blue, tran = 0;
   float    a, b, d, e, ww15, hh15, ww25, hh25;
   float    PI = 3.141593;

   nch = gdk_pixbuf_get_n_channels(pixbuf1);
   nbits = gdk_pixbuf_get_bits_per_sample(pixbuf1);
   if (nch < 3) return 0;                                                        //  must have 3+ channels (colors)
   if (nbits != 8) return 0;                                                     //  must be 8 bits per channel

   color = gdk_pixbuf_get_colorspace(pixbuf1);                                   //  get input pixbuf1 attributes
   alpha = gdk_pixbuf_get_has_alpha(pixbuf1);
   ww1 = gdk_pixbuf_get_width(pixbuf1);
   hh1 = gdk_pixbuf_get_height(pixbuf1);
   rs1 = gdk_pixbuf_get_rowstride(pixbuf1);

   while (angle < -180) angle += 360;                                            //  normalize, -180 to +180
   while (angle > 180) angle -= 360;
   angle = angle * PI / 180;                                                     //  radians, -PI to +PI

   if (fabsf(angle) < 0.001) {
      pixbuf2 = gdk_pixbuf_copy(pixbuf1);                                        //  angle is zero within my precision
      return pixbuf2;
   }

   ww2 = int(ww1*fabsf(cosf(angle)) + hh1*fabsf(sinf(angle)));                   //  rectangle containing rotated image
   hh2 = int(ww1*fabsf(sinf(angle)) + hh1*fabsf(cosf(angle)));

   pixbuf2 = gdk_pixbuf_new(color,alpha,nbits,ww2,hh2);                          //  create output pixbuf2
   if (! pixbuf2) return 0;
   rs2 = gdk_pixbuf_get_rowstride(pixbuf2);

   ppix1 = gdk_pixbuf_get_pixels(pixbuf1);                                       //  input pixel array
   ppix2 = gdk_pixbuf_get_pixels(pixbuf2);                                       //  output pixel array

   ww15 = 0.5 * ww1;
   hh15 = 0.5 * hh1;
   ww25 = 0.5 * ww2;
   hh25 = 0.5 * hh2;

   a = cosf(angle);                                                              //  affine transform coefficients
   b = sinf(angle);
   d = - sinf(angle);
   e = cosf(angle);

   for (py2 = 0; py2 < hh2; py2++)                                               //  loop through output pixels
   for (px2 = 0; px2 < ww2; px2++)
   {
      px1 = a * (px2 - ww25) + b * (py2 - hh25) + ww15;                          //  (px1,py1) = corresponding
      py1 = d * (px2 - ww25) + e * (py2 - hh25) + hh15;                          //    point within input pixels

      px0 = int(px1);                                                            //  pixel containing (px1,py1)
      py0 = int(py1);

      if (px1 < 0 || px0 >= ww1-1 || py1 < 0 || py0 >= hh1-1) {                  //  if outside input pixel array
         pix2 = ppix2 + py2 * rs2 + px2 * nch;                                   //    output is acolor
         pix2[0] = pix2[1] = pix2[2] = acolor;
         continue;
      }

      pix0 = ppix1 + py0 * rs1 + px0 * nch;                                      //  4 input pixels based at (px0,py0)
      pix1 = pix0 + rs1;
      pix2 = pix0 + nch;
      pix3 = pix0 + rs1 + nch;

      f0 = (px0+1 - px1) * (py0+1 - py1);                                        //  overlap of (px1,py1)
      f1 = (px0+1 - px1) * (py1 - py0);                                          //    in each of the 4 pixels
      f2 = (px1 - px0) * (py0+1 - py1);
      f3 = (px1 - px0) * (py1 - py0);

      red =   f0 * pix0[0] + f1 * pix1[0] + f2 * pix2[0] + f3 * pix3[0];         //  sum the weighted inputs
      green = f0 * pix0[1] + f1 * pix1[1] + f2 * pix2[1] + f3 * pix3[1];
      blue =  f0 * pix0[2] + f1 * pix1[2] + f2 * pix2[2] + f3 * pix3[2];
      if (alpha)
       tran = f0 * pix0[3] + f1 * pix1[3] + f2 * pix2[3] + f3 * pix3[3];         //  4th color = alpha

      if (red == acolor && green == acolor && blue == acolor) {                  //  avoid acolor in image
         if (blue == 0) blue = 1;
         else blue--;
      }

      pix2 = ppix2 + py2 * rs2 + px2 * nch;                                      //  output pixel
      pix2[0] = int(red);
      pix2[1] = int(green);
      pix2[2] = int(blue);
      if (alpha) pix2[3] = int(tran);
   }

   return pixbuf2;
}


/********************************************************************************/

//  strip the alpha channel from a pixbuf
//  returns 0 if no alpha channel or fatal error

PIXBUF * gdk_pixbuf_stripalpha(PIXBUF *pixbuf1)
{
   PIXBUF      *pixbuf2;
   GDKCOLOR    color;
   int         ww, hh, rs1, rs2;
   uint8       *ppix1, *ppix2, *pix1, *pix2;
   int         nch, ac;
   int         px, py;

   ac = gdk_pixbuf_get_has_alpha(pixbuf1);
   if (! ac) return 0;
   nch = gdk_pixbuf_get_n_channels(pixbuf1);
   color = gdk_pixbuf_get_colorspace(pixbuf1);
   ww = gdk_pixbuf_get_width(pixbuf1);
   hh = gdk_pixbuf_get_height(pixbuf1);
   
   pixbuf2 = gdk_pixbuf_new(color,0,8,ww,hh);                                    //  create output pixbuf2
   if (! pixbuf2) return 0;

   ppix1 = gdk_pixbuf_get_pixels(pixbuf1);                                       //  input pixel array
   ppix2 = gdk_pixbuf_get_pixels(pixbuf2);                                       //  output pixel array
   rs1 = gdk_pixbuf_get_rowstride(pixbuf1);
   rs2 = gdk_pixbuf_get_rowstride(pixbuf2);
   
   for (py = 0; py < hh; py++)
   {
      pix1 = ppix1 + py * rs1;
      pix2 = ppix2 + py * rs2;

      for (px = 0; px < ww; px++)
      {
         memcpy(pix2,pix1,nch-1);
         pix1 += nch;
         pix2 += nch-1;
      }
   }
   
   return pixbuf2;
}


/********************************************************************************/

//  Create a pixbuf containing text with designated font and attributes.
//  Text is white on black. Widget is ultimate display destination.

PIXBUF * text_pixbuf(cchar *text, cchar *font, int fontsize, GtkWidget *widget) 
{
   char                    font2[60];
   PangoFontDescription    *pfont;
   PangoLayout             *playout;
   cairo_surface_t         *surface;
   cairo_t                 *cr;
   PIXBUF                  *pixbuf;
   uint8                   *pixels, *cairo_data, *cpix, *pix2;
   int                     ww, hh, rs, px, py;

   if (! font) font = zfuncs::appfont;                                           //  default font 

   snprintf(font2,60,"%s %d",font,fontsize);                                     //  combine font and size

   pfont = pango_font_description_from_string(font2);                            //  make layout with text
   playout = gtk_widget_create_pango_layout(widget,text);
   pango_layout_set_font_description(playout,pfont);
   
   pango_layout_get_pixel_size(playout,&ww,&hh);
   ww += 2 + 0.2 * fontsize;                                                     //  compensate bad font metrics
   hh += 2 + 0.1 * fontsize;

   surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24,ww,hh);               //  cairo output image
   cr = cairo_create(surface);
   pango_cairo_show_layout(cr,playout);                                          //  write text layout to image

   cairo_data = cairo_image_surface_get_data(surface);                           //  get text image pixels
   
   pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,0,8,ww,hh);
   rs = gdk_pixbuf_get_rowstride(pixbuf);
   pixels = gdk_pixbuf_get_pixels(pixbuf);

   for (py = 0; py < hh; py++)                                                   //  copy text image to PXB
   for (px = 0; px < ww; px++)
   {
      cpix = cairo_data + 4 * (ww * py + px);
      pix2 = pixels + py * rs + px * 3;
      pix2[0] = pix2[1] = pix2[2] = cpix[3];
   }

   pango_font_description_free(pfont);                                           //  free resources
   g_object_unref(playout);
   cairo_destroy(cr);
   cairo_surface_destroy(surface);

   return pixbuf;
}


/********************************************************************************/

//  move the mouse pointer to given position in given window
//  widget must be realized

int move_pointer(GtkWidget *widget, int px, int py)
{
   int         rpx, rpy;
   GdkWindow   *window;

   window = gtk_widget_get_window(widget);
   gdk_window_get_root_coords(window,px,py,&rpx,&rpy);
   gdk_device_warp(mouse,screen,rpx,rpy);                                        //  wayland fails   FIXME
   return 1;
}


/********************************************************************************

   xstring class (dynamic length string)

   xstring(int cc = 0)                                            default constructor
   xstring(cchar * )                                              string constructor
   xstring(const xstring &)                                       copy constructor
   ~xstring()                                                     destructor
   operator cchar * () const { return xpp; }                      conversion operator (cchar *)
   xstring operator= (const xstring &)                            operator =
   xstring operator= (cchar *)                                    operator =
   friend xstring operator+ (const xstring &, const xstring &)    operator + (catenate)
   friend xstring operator+ (const xstring &, cchar *)            operator +
   friend xstring operator+ (cchar *, const xstring &)            operator +
   void insert(int pos, cchar *string, int cc = 0)                insert substring at pos (expand)
   void overlay(int pos, cchar *string, int cc = 0)               replace substring (possibly expand)
   static void getStats(int & tcount2, int & tmem2)               get statistics
   void validate() const                                          verify integrity
   int getcc() const { return xcc; }                              return string length

*********************************************************************************/

#define  wmiv  1648734981

int   xstring::tcount = 0;                                                       //  initz. static members
int   xstring::tmem = 0;


xstring::xstring(int cc)                                                         //  new xstring(cc)
{
   wmi = wmiv;
   xmem = (cc & 0x7ffffff8) + 8;                                                 //  mod 8 length
   xpp = new char[xmem];                                                         //  allocate
   if (! xpp) zappcrash("xstring NEW failure",null);
   tcount++;                                                                     //  incr. object count
   tmem += xmem;                                                                 //  incr. allocated memory
   xcc = 0;                                                                      //  string cc = 0
   *xpp = 0;                                                                     //  string = null
}


xstring::xstring(cchar *string)                                                  //  new xstring("initial string")
{
   wmi = wmiv;
   xcc = 0;
   if (string) xcc = strlen(string);                                             //  string length
   xmem = (xcc & 0x7ffffff8) + 8;                                                //  mod 8 length
   xpp = new char[xmem];                                                         //  allocate
   if (! xpp) zappcrash("xstring NEW failure",null);
   tcount++;                                                                     //  incr. object count
   tmem += xmem;                                                                 //  incr. allocated memory
   *xpp = 0;
   if (xcc) strcpy(xpp,string);                                                  //  copy string
}


xstring::xstring(const xstring & xstr)                                           //  new xstring2(xstring1)
{
   wmi = wmiv;
   xmem = xstr.xmem;                                                             //  allocate same length
   xcc = xstr.xcc;
   xpp = new char[xmem];
   if (! xpp) zappcrash("xstring NEW failure",null);
   tcount++;                                                                     //  incr. object count
   tmem += xmem;                                                                 //  incr. allocated memory
   strcpy(xpp,xstr.xpp);                                                         //  copy string
}


xstring::~xstring()                                                              //  delete xstring
{
   validate();
   delete[] xpp;                                                                 //  release allocated memory
   xpp = 0;
   tcount--;                                                                     //  decr. object count
   tmem -= xmem;                                                                 //  decr. allocated memory
   if (tcount < 0) zappcrash("xstring count < 0",null);
   if (tmem < 0) zappcrash("xstring memory < 0",null);
   if (tcount == 0 && tmem > 0) zappcrash("xstring memory leak",null);
}


xstring xstring::operator= (const xstring & xstr)                                //  xstring2 = xstring1
{
   validate();
   xstr.validate();
   if (this == &xstr) return *this;
   xcc = xstr.xcc;
   if (xmem < xcc+1)
   {
      delete[] xpp;                                                              //  expand memory if needed
      tmem -= xmem;
      xmem = (xcc & 0x7ffffff8) + 8;                                             //  mod 8 length
      xpp = new char[xmem];
      if (! xpp) zappcrash("xstring NEW failure",null);
      tmem += xmem;
   }
   strcpy(xpp,xstr.xpp);                                                         //  copy string
   return *this;
}


xstring xstring::operator= (cchar *str)                                          //  xstring = "some string"
{
   validate();
   xcc = 0;
   *xpp = 0;
   if (str) xcc = strlen(str);
   if (xmem < xcc+1)
   {
      delete[] xpp;                                                              //  expand memory if needed
      tmem -= xmem;
      xmem = (xcc & 0x7ffffff8) + 8;                                             //  mod 8 length
      xpp = new char[xmem];
      if (! xpp) zappcrash("xstring NEW failure",null);
      tmem += xmem;
   }
   if (xcc) strcpy(xpp,str);                                                     //  copy string
   return *this;
}


xstring operator+ (const xstring & x1, const xstring & x2)                       //  xstring1 + xstring2
{
   x1.validate();
   x2.validate();
   xstring temp(x1.xcc + x2.xcc);                                                //  build temp xstring
   strcpy(temp.xpp,x1.xpp);                                                      //    with both input strings
   strcpy(temp.xpp + x1.xcc, x2.xpp);
   temp.xcc = x1.xcc + x2.xcc;
   temp.validate();
   return temp;
}


xstring operator+ (const xstring & x1, cchar *s2)                                //  xstring + "some string"
{
   x1.validate();
   int cc2 = 0;
   if (s2) cc2 = strlen(s2);
   xstring temp(x1.xcc + cc2);                                                   //  build temp xstring
   strcpy(temp.xpp,x1.xpp);                                                      //    with both input strings
   if (s2) strcpy(temp.xpp + x1.xcc, s2);
   temp.xcc = x1.xcc + cc2;
   temp.validate();
   return temp;
}


xstring operator+ (cchar *s1, const xstring & x2)                                //  "some string" + xstring
{
   x2.validate();
   int cc1 = 0;
   if (s1) cc1 = strlen(s1);
   xstring temp(cc1 + x2.xcc);                                                   //  build temp xstring
   if (s1) strcpy(temp.xpp,s1);                                                  //    with both input strings
   strcpy(temp.xpp + cc1, x2.xpp);
   temp.xcc = cc1 + x2.xcc;
   temp.validate();
   return temp;
}


void xstring::insert(int pos, cchar *string, int cc)                             //  insert cc chars from string at pos
{                                                                                //  pad if pos > xcc or cc > string
   validate();

   int scc = strlen(string);
   if (! cc) cc = scc;

   int pad = pos - xcc;
   if (pad < 0) pad = 0;

   if (xmem < xcc + cc + pad + 1)                                                //  allocate more memory if needed
   {
      int newmem = xcc + cc + pad;
      newmem = (newmem & 0x7ffffff8) + 8;                                        //  mod 8 length
      char * xpp2 = new char[newmem];
      if (! xpp2) zappcrash("xstring NEW failure",null);
      strcpy(xpp2,xpp);                                                          //  copy to new space
      delete[] xpp;
      xpp = xpp2;
      tmem += newmem - xmem;
      xmem = newmem;
   }

   if (pad) memset(xpp+xcc,' ',pad);                                             //  add blanks up to pos

   for (int ii = xcc + pad; ii >= pos; ii--)                                     //  make hole for inserted string
           *(xpp+ii+cc) = *(xpp+ii);

   if (cc > scc) memset(xpp+pos+scc,' ',cc-scc);                                 //  blank pad if cc > string
   if (cc < scc) scc = cc;
   strncpy(xpp+pos,string,scc);                                                  //  insert string, without null

   xcc += cc + pad;                                                              //  set new length
   xpp[xcc] = 0;
   validate();
}


void xstring::overlay(int pos, cchar *string, int cc)                            //  overlay substring
{
   validate();

   int scc = strlen(string);
   if (! cc) cc = scc;

   if (xmem < pos + cc + 1)                                                      //  allocate more memory if needed
   {
      int newmem = pos + cc;
      newmem = (newmem & 0x7ffffff8) + 8;                                        //  mod 8 length
      char * xpp2 = new char[newmem];
      if (! xpp2) zappcrash("xstring NEW failure",null);
      strcpy(xpp2,xpp);                                                          //  copy to new space
      delete[] xpp;
      xpp = xpp2;
      tmem += newmem - xmem;
      xmem = newmem;
   }

   if (pos > xcc) memset(xpp+xcc,' ',pos-xcc);                                   //  add blanks up to pos

   if (cc > scc) memset(xpp+pos+scc,' ',cc-scc);                                 //  blank pad if cc > string
   if (cc < scc) scc = cc;
   strncpy(xpp+pos,string,scc);                                                  //  insert string, without null

   if (pos + cc > xcc) xcc = pos + cc;                                           //  set new length
   xpp[xcc] = 0;
   validate();
}


void xstring::getStats(int & tcount2, int & tmem2)                               //  get statistics
{
   tcount2 = tcount;
   tmem2 = tmem;
}


void xstring::validate() const                                                   //  validate integrity
{
   if (wmi != wmiv) zappcrash("xstring bad wmi",null);
   if (xmem < xcc+1) zappcrash("xstring xmem < xcc+1",null);
   if (xcc != (int) strlen(xpp)) zappcrash("xstring xcc != strlen(xpp)",null);
}


/********************************************************************************

   Vxstring class (array or vector of xstring)

   Vxstring(int = 0);                                 constructor
   ~Vxstring();                                       destructor
   Vxstring(const Vxstring &);                        copy constructor
   Vxstring operator= (const Vxstring &);             operator =
   xstring & operator[] (int);                        operator []
   const xstring & operator[] (int) const;            operator [] (const)
   int search(cchar *string);                         find element in unsorted Vxstring
   int bsearch(cchar *string);                        find element in sorted Vxstring
   int sort(int nkeys, int keys[][3]);                sort elements by designated subfields
   int sort(int pos = 0, int cc = 0);                 sort elements by 1 subfield (cc 0 = all)
   int getCount() const { return nd; }                get current count

   Sort with keys:
       keys[N][0] = key position (0 based) of key N
       keys[N][1] = key length
       keys[N][2] = sort type: 1/2 = ascending/descending, 3/4 = same, ignoring case

*********************************************************************************/

Vxstring::Vxstring(int ii)                                                       //  constructor
{
   pdata = 0;
   nd = ii;
   if (nd) pdata = new xstring[nd];
   if (nd && !pdata) zappcrash("Vxstring NEW fail",null);
}


Vxstring::~Vxstring()                                                            //  destructor
{
   if (nd) delete[] pdata;
   pdata = 0;
   nd = 0;
}


Vxstring::Vxstring(const Vxstring & pold)                                        //  copy constructor
{
   pdata = 0;
   nd = pold.nd;                                                                 //  set size
   if (nd) pdata = new xstring[nd];                                              //  allocate memory
   if (nd && !pdata) zappcrash("Vxstring NEW fail");
   for (int ii = 0; ii < nd; ii++) pdata[ii] = pold[ii];                         //  copy defined elements
}


Vxstring Vxstring::operator= (const Vxstring & vdstr)                            //  operator =
{
   if (nd) delete[] pdata;                                                       //  delete old memory
   pdata = 0;
   nd = vdstr.nd;
   if (nd) pdata = new xstring[nd];                                              //  allocate new memory
   if (nd && !pdata) zappcrash("Vxstring NEW fail",null);
   for (int ii = 0; ii < nd; ii++) pdata[ii] = vdstr.pdata[ii];                  //  copy elements
   return *this;
}


xstring & Vxstring::operator[] (int ii)                                          //  operator []
{
   static xstring xnull(0);
   if (ii < nd) return pdata[ii];                                                //  return reference
   zappcrash("Vxstring index invalid %d %d",nd,ii,null);
   return xnull;
}


const xstring & Vxstring::operator[] (int ii) const                              //  operator []
{
   static xstring xnull(0);
   if (ii < nd) return pdata[ii];                                                //  return reference
   zappcrash("Vxstring index invalid %d %d",nd,ii,null);
   return xnull;
}


int Vxstring::search(cchar *string)                                              //  find element in unsorted Vxstring
{
   for (int ii = 0; ii < nd; ii++)
        if (strmatch(pdata[ii],string)) return ii;
   return -1;
}


int Vxstring::bsearch(cchar *string)                                             //  find element in sorted Vxstring
{                                                                                //   (binary search)
   int   nn, ii, jj, kk, rkk;

   nn = nd;
   if (! nn) return 0;                                                           //  empty list

   ii = nn / 2;                                                                  //  next element to search
   jj = (ii + 1) / 2;                                                            //  next increment
   nn--;                                                                         //  last element
   rkk = 0;

   while (1)
   {
      kk = strcmp(pdata[ii],string);                                             //  check element

      if (kk > 0)
      {
         ii -= jj;                                                               //  too high, go down
         if (ii < 0) return -1;
      }

      else if (kk < 0)
      {
         ii += jj;                                                               //  too low, go up
         if (ii > nn) return -1;
      }

      else if (kk == 0) return ii;                                               //  matched

      jj = jj / 2;                                                               //  reduce increment

      if (jj == 0)
      {
         jj = 1;                                                                 //  step by 1 element
         if (! rkk) rkk = kk;                                                    //  save direction
         else
         {
            if (rkk > 0) { if (kk < 0) return -1; }                              //  if change direction, fail
            else if (kk > 0) return -1;
         }
      }
   }
}


static int  VDsortKeys[10][3], VDsortNK;

int Vxstring::sort(int NK, int keys[][3])                                        //  sort elements by subfields
{                                                                                //  key[ii][0] = position
   int     NR, RL, ii;                                                           //         [1] = length
   HeapSortUcomp  VDsortComp;                                                    //         [2] = 1/2 = ascending/desc.
                                                                                 //             = 3/4 =  + ignore case
   NR = nd;
   if (NR < 2) return 1;

   RL = sizeof(xstring);

   if (NK < 1) zappcrash("Vxstring::sort, bad NK",null);
   if (NK > 10) zappcrash("Vxstring::sort, bad NK",null);
   VDsortNK = NK;

   for (ii = 0; ii < NK; ii++)
   {
      VDsortKeys[ii][0] = keys[ii][0];
      VDsortKeys[ii][1] = keys[ii][1];
      VDsortKeys[ii][2] = keys[ii][2];
   }

   HeapSort((char *) pdata,RL,NR,VDsortComp);

   return 1;
}


int VDsortComp(cchar *r1, cchar *r2)
{
   xstring     *d1, *d2;
   cchar       *p1, *p2;
   int         ii, nn, kpos, ktype, kleng;

   d1 = (xstring *) r1;
   d2 = (xstring *) r2;
   p1 = *d1;
   p2 = *d2;

   for (ii = 0; ii < VDsortNK; ii++)                                             //  compare each key
   {
      kpos = VDsortKeys[ii][0];
      kleng = VDsortKeys[ii][1];
      ktype = VDsortKeys[ii][2];

      if (ktype == 1)
      {
         nn = strncmp(p1+kpos,p2+kpos,kleng);
         if (nn) return nn;
         continue;
      }

      else if (ktype == 2)
      {
         nn = strncmp(p1+kpos,p2+kpos,kleng);
         if (nn) return -nn;
         continue;
      }

      else if (ktype == 3)
      {
         nn = strncasecmp(p1+kpos,p2+kpos,kleng);
         if (nn) return nn;
         continue;
      }

      else if (ktype == 4)
      {
         nn = strncasecmp(p1+kpos,p2+kpos,kleng);
         if (nn) return -nn;
         continue;
      }

      zappcrash("Vxstring::sort, bad KEYS sort type",null);
   }

   return 0;
}


int Vxstring::sort(int pos, int cc)                                              //  sort elements ascending
{
   int   key[3];

   if (! cc) cc = 999999;
   key[0] = pos;
   key[1] = cc;
   key[2] = 1;

   sort(1,&key);

   return 1;
}


/********************************************************************************

   Hash Table class

   HashTab(int cc, int cap);                       constructor
   ~HashTab();                                     destructor
   int Add(cchar *string);                         add a new string
   int Del(cchar *string);                         delete a string
   int Find(cchar *string);                        find a string
   int GetCount() { return count; }                get string count
   int GetNext(int & first, char *string);         get first/next string
   int Dump();                                     dump hash table to std. output

   constructor: cc = string length of table entries, cap = table capacity
   cap should be set 30% higher than needed to reduce collisions and improve speed

   Benchmark: 0.056 usec. to find 19 char string in a table of 100,000
              which is 80% full.    3.3 GHz Core i5

*********************************************************************************/

//  static members (robust for tables up to 60% full)

int HashTab::tries1 = 100;                                                       //  Add() tries
int HashTab::tries2 = 200;                                                       //  Find() tries


HashTab::HashTab(int _cc, int _cap)                                              //  constructor
{
   cc = 4 * (_cc + 4) / 4;                                                       //  + 1 + mod 4 length
   cap = _cap;
   int len = cc * cap;
   table = new char [len];
   if (! table) zappcrash("HashTab() new %d fail",len,null);
   memset(table,0,len);
}


HashTab::~HashTab()                                                              //  destructor
{
   delete [] table;
   table = 0;
}


//  Add a new string to table

int HashTab::Add(cchar *string)
{
   int   pos, fpos, tries;

   pos = strHash(string,cap);                                                    //  get random position
   pos = pos * cc;

   for (tries = 0, fpos = -1; tries < tries1; tries++, pos += cc)                //  find next free slot at/after position
   {
      if (pos >= cap * cc) pos = 0;                                              //  last position wraps to 1st

      if (! table[pos])                                                          //  empty slot: string not found
      {
         if (fpos != -1) pos = fpos;                                             //  use prior deleted slot if there
         strncpy(table+pos,string,cc);                                           //  insert new string
         table[pos+cc-1] = 0;                                                    //  insure null terminator
         return (pos/cc);                                                        //  return rel. table entry
      }

      if (table[pos] == -1)                                                      //  deleted slot
      {
         if (fpos == -1) fpos = pos;                                             //  remember 1st one found
         continue;
      }

      if (strmatch(string,table+pos)) return -2;                                 //  string already present
   }

   return -3;                                                                    //  table full (tries1 exceeded)
}


//  Delete a string from table

int HashTab::Del(cchar *string)
{
   int   pos, tries;

   pos = strHash(string,cap);                                                    //  get random position
   pos = pos * cc;

   for (tries = 0; tries < tries2; tries++, pos += cc)                           //  search for string at/after position
   {
      if (pos >= cap * cc) pos = 0;                                              //  last position wraps to 1st

      if (! table[pos]) return -1;                                               //  empty slot, string not found

      if (strmatch(string,table+pos))                                            //  string found
      {
         table[pos] = -1;                                                        //  delete table entry
         return (pos/cc);                                                        //  return rel. table entry
      }
   }

   zappcrash("HashTab::Del() bug",null);                                         //  exceed tries2, must not happen
   return 0;                                                                     //  (table too full to function)
}


//  Find a table entry.

int HashTab::Find(cchar *string)
{
   int   pos, tries;

   pos = strHash(string,cap);                                                    //  get random position
   pos = pos * cc;

   for (tries = 0; tries < tries2; tries++, pos += cc)                           //  search for string at/after position
   {
      if (pos >= cap * cc) pos = 0;                                              //  last position wraps to 1st
      if (! table[pos]) return -1;                                               //  empty slot, string not found
      if (strmatch(string,table+pos)) return (pos/cc);                           //  string found, return rel. entry
   }

   zappcrash("HashTab::Find() bug",null);                                        //  cannot happen
   return 0;
}


//  return first or next table entry

int HashTab::GetNext(int & ftf, char *string)
{
   static int    pos;

   if (ftf)                                                                      //  initial call
   {
      pos = 0;
      ftf = 0;
   }

   while (pos < (cap * cc))
   {
      if ((table[pos] == 0) || (table[pos] == -1))                               //  empty or deleted slot
      {
         pos += cc;
         continue;
      }

      strcpy(string,table+pos);                                                  //  return string
      pos += cc;
      return 1;
   }

   return -4;                                                                    //  EOF
}


int HashTab::Dump()
{
   int   ii, pos;

   for (ii = 0; ii < cap; ii++)
   {
      pos = ii * cc;

      if (table[pos] && table[pos] != -1)
         printz("%d, %s \n", ii, table + pos);

      if (table[pos] == -1)
         printz("%d, deleted \n", pos);
   }

   return 1;
}


/********************************************************************************

   class for queue of dynamic strings

   Queue(int cap);                                 create queue with capacity
   ~Queue();                                       destroy queue
   int getCount();                                 get current entry count
   int push(const xstring *entry, double secs);    add new entry with max. wait time
   xstring *pop1();                                get 1st entry (oldest)
   xstring *popN();                                get Nth entry (newest)

   constructor: cap is queue capacity
   push: secs is max. time to wait if queue is full.
   This makes sense if the queue is being pop'd from another thread.
   Use zero otherwise.

   Execution time: 0.48 microsecs per push + pop on queue with 100 slots kept full.
   (2.67 GHz Intel Core i7)

*********************************************************************************/

Queue::Queue(int cap)                                                            //  constructor
{
   int   err;

   err = mutex_init(&qmutex, 0);                                                 //  create mutex = queue lock
   if (err) zappcrash("Queue(), mutex init fail",null);

   qcap = cap;                                                                   //  queue capacity
   ent1 = entN = qcount = 0;                                                     //  state = empty
   vd = new Vxstring(qcap);                                                      //  create vector of xstring's
   if (! vd) zappcrash("Queue(), NEW fail %d",cap,null);
   strcpy(wmi,"queue");
   return;
}


Queue::~Queue()                                                                  //  destructor
{
   if (! strmatch(wmi,"queue")) zappcrash("~Queue wmi fail",null);
   wmi[0] = 0;
   mutex_destroy(&qmutex);                                                       //  destroy mutex
   qcount = qcap = ent1 = entN = -1;
   delete vd;
   vd = 0;
   return;
}


void Queue::lock()                                                               //  lock queue (private)
{
   int   err;
   err = mutex_lock(&qmutex);                                                    //  reserve mutex or suspend
   if (err) zappcrash("Queue mutex lock fail",null);
   return;
}


void Queue::unlock()                                                             //  unlock queue (private)
{
   int   err;
   err = mutex_unlock(&qmutex);                                                  //  release mutex
   if (err) zappcrash("Queue mutex unlock fail",null);
   return;
}


int Queue::getCount()                                                            //  get current entry count
{
   if (! strmatch(wmi,"queue")) zappcrash("Queue getCount wmi fail",null);
   return qcount;
}


int Queue::push(const xstring *newEnt, double wait)                              //  add entry to queue, with max. wait
{
   double  elaps = 0.0;
   int     count;

   if (! strmatch(wmi,"queue")) zappcrash("Queue::push wmi fail",null);

   lock();                                                                       //  lock queue
   while (qcount == qcap) {                                                      //  queue full
      unlock();                                                                  //  unlock queue
      if (elaps >= wait) return -1;                                              //  too long, return -1 status
      usleep(1000);                                                              //  sleep in 1 millisec. steps
      elaps += 0.001;                                                            //  until queue not full
      lock();                                                                    //  lock queue
   }

   (* vd)[entN] = *newEnt;                                                       //  copy new entry into queue
   entN++;                                                                       //  incr. end pointer
   if (entN == qcap) entN = 0;
   qcount++;                                                                     //  incr. queue count
   count = qcount;
   unlock();                                                                     //  unlock queue
   return count;                                                                 //  return curr. queue count
}


xstring *Queue::pop1()                                                           //  get 1st (oldest) entry and remove
{
   xstring    *entry;

   if (! strmatch(wmi,"queue")) zappcrash("Queue::pop1 wmi fail",null);

   lock();                                                                       //  lock queue

   if (qcount == 0) entry = 0;                                                   //  queue empty
   else {
      entry = &(* vd)[ent1];                                                     //  get first entry
      ent1++;                                                                    //  index pointer to next
      if (ent1 == qcap) ent1 = 0;
      qcount--;                                                                  //  decr. queue count
   }

   unlock();                                                                     //  unlock queue
   return entry;
}


xstring *Queue::popN()                                                           //  get last (newest) entry and remove
{
   xstring   *entry;

   if (! strmatch(wmi,"queue")) zappcrash("Queue::popN wmi fail",null);

   lock();                                                                       //  lock queue

   if (qcount == 0) entry = 0;                                                   //  queue empty
   else {
      if (entN == 0) entN = qcap;                                                //  index pointer to prior
      entN--;
      qcount--;                                                                  //  decr. queue count
      entry = &(* vd)[entN];                                                     //  get last entry
   }

   unlock();                                                                     //  unlock queue
   return entry;
}


/********************************************************************************

   Tree class, tree-structured data storage without limits

   Store any amount of data at any depth within a tree-structure with named nodes.
   Data can be found using an ordered list of node names or node numbers.

   Tree(cchar *name);                                       create Tree
   ~Tree();                                                 destroy Tree
   int put(void *data, int dd, char *nodes[], int nn);      put data by node names[]
   int put(void *data, int dd, int nodes[], int nn);        put data by node numbers[]
   int get(void *data, int dd, char *nodes[], int nn);      get data by node names[]
   int get(void *data, int dd, int nodes[], int nn);        get data by node numbers[]

   A Tree can also be thought of as an N-dimensional array with the cells or nodes
   having both names and numbers. Data can be stored and retrieved with a list of node
   names or numbers. The nodes are created as needed. Nodes are sparse: those with no data
   do not use any memory. Node numbers are created when data is stored by node numbers.
   Node numbers are also added when data is stored by node names: the numbers are assigned
   sequentially from zero at each level in the tree.

   nodes         array of node names or numbers
   nn            no. of nodes used for a put() or get() call
   dd            data length to put, or the max. data length to get (i.e. the space available)
   put()         returns 1 if successful and crashes with a message if not (out of memory)
   get()         returns the length of the data retrieved (<= dd) or 0 if not found
                 there is no assumption that the data is character data and no null is appended
                 data returned has the same length as the data stored (if dd arg is big enough)

   example:
   char     *snodes[10];                     // up to 10 node names (max tree depth)
   int      knodes[10];                      // up to 10 node numbers
   char     mydata[20];                      // data length up to 20

   Tree *mytree = new Tree("myname");        // create Tree
   snodes[0] = "name1";
   snodes[1] = "name2";
   mytree->put("string1",8,snodes,2);        // put "string1" at ["name1","name2"]
   snodes[1] = "name3";
   mytree->put("string22",9,snodes,2);       // put "string22" at ["name1","name3"]
   snodes[1] = "name2";
   mytree->get(mydata,20,snodes,2);          // get data at ["name1","name2"] ("string1")
   knodes[0] = 0;
   knodes[1] = 0;
   mytree->get(mydata,20,knodes,2);          // get data at [0,0] ("string1")
   knodes[1] = 1;
   mytree->get(mydata,20,knodes,2);          // get data at [0,1] ("string22")

   When data was stored at ["name1","name2"] these node names were created along with the
   corresponding node numbers [0,0]. When data was stored at ["name1","name3"] a new node
   "name3" was created under the existing node "name1", and assigned the numbers [0,1].

   Benchmark Execution times:
       2.67 GHz Intel Core i7
       Tree with 1 million nodes and average depth of 8 levels (peak 15 levels)
       put() all data by node names: 2.1 secs
       get() all data by node names: 1.5 secs
       put() all data by node numbers: 2.0 secs
       get() all data by node numbers: 0.72 secs

   Internal code conventions:
      - caller level is node 0, next level is node 1, etc.
      - node names and numbers in calls to get() and put() refer to next levels
      - number of levels = 1+nn, where nn is max. in calls to put(...nodes[], nn)

*********************************************************************************/

#define wmid 1374602859                                                          //  integrity check key

//  constructor

Tree::Tree(cchar *name)
{
   wmi = wmid;
   tname = 0;
   tmem = 0;
   tdata = 0;
   nsub = 0;
   psub = 0;

   if (name)
   {
      int cc = strlen(name);
      tname = new char[cc+1];
      if (! tname) zappcrash("Tree, no memory",null);
      strcpy(tname,name);
   }
}


//  destructor

Tree::~Tree()
{
   if (wmi != wmid) zappcrash("not a Tree",null);
   if (tname) delete [] tname;
   tname = 0;
   if (tmem) zfree(tdata);
   tmem = 0;
   tdata = 0;
   for (int ii = 0; ii < nsub; ii++) delete psub[ii];
   if (psub) zfree(psub);
   nsub = 0;
   psub = 0;
}


//  put data by node names[]

int Tree::put(void *data, int dd, char *nodes[], int nn)
{
   Tree    *tnode;

   if (wmi != wmid) zappcrash("not a Tree",null);
   tnode = make(nodes,nn);
   if (tnode->tdata) zfree(tnode->tdata);
   tnode->tdata = new char[dd];
   if (! tnode->tdata) zappcrash("Tree, no memory",null);
   tnode->tmem = dd;
   memmove(tnode->tdata,data,dd);
   return 1;
}


//  put data by node numbers[]

int Tree::put(void *data, int dd, int nodes[], int nn)
{
   Tree    *tnode;

   if (wmi != wmid) zappcrash("not a Tree",null);
   tnode = make(nodes,nn);
   if (tnode->tdata) zfree(tnode->tdata);
   tnode->tdata = new char[dd];
   if (! tnode->tdata) zappcrash("Tree, no memory",null);
   tnode->tmem = dd;
   memmove(tnode->tdata,data,dd);
   return 1;
}


//  get data by node names[]

int Tree::get(void *data, int dd, char *nodes[], int nn)
{
   Tree *tnode = find(nodes,nn);
   if (! tnode) return 0;
   if (! tnode->tmem) return 0;
   if (dd > tnode->tmem) dd = tnode->tmem;
   memmove(data,tnode->tdata,dd);
   return dd;
}


//  get data by node numbers[]

int Tree::get(void *data, int dd, int nodes[], int nn)
{
   Tree *tnode = find(nodes,nn);
   if (! tnode) return 0;
   if (! tnode->tmem) return 0;
   if (dd > tnode->tmem) dd = tnode->tmem;
   memmove(data,tnode->tdata,dd);
   return dd;
}


//  find a given node by names[]

Tree * Tree::find(char *nodes[], int nn)
{
   int      ii;

   for (ii = 0; ii < nsub; ii++)
      if (psub[ii]->tname && strmatch(nodes[0],psub[ii]->tname)) break;
   if (ii == nsub) return 0;
   if (nn == 1) return psub[ii];
   return psub[ii]->find(&nodes[1],nn-1);
}


//  find a given node by numbers[]

Tree * Tree::find(int nodes[], int nn)
{
   int ii = nodes[0];
   if (ii >= nsub) return 0;
   if (! psub[ii]) return 0;
   if (nn == 1) return psub[ii];
   return psub[ii]->find(&nodes[1],nn-1);
}


//  find or create a given node by names[]

Tree * Tree::make(char *nodes[], int nn)
{
   int      ii;
   Tree   **psub2;

   for (ii = 0; ii < nsub; ii++)
      if (psub[ii]->tname && strmatch(nodes[0],psub[ii]->tname)) break;

   if (ii == nsub)
   {
      psub2 = new Tree * [nsub+1];
      if (! psub2) zappcrash("Tree, no memory",null);
      for (ii = 0; ii < nsub; ii++) psub2[ii] = psub[ii];
      delete [] psub;
      psub = psub2;
      nsub++;
      psub[ii] = new Tree(nodes[0]);
      if (! psub[ii]) zappcrash("Tree, no memory",null);
   }

   if (nn == 1) return psub[ii];
   return psub[ii]->make(&nodes[1],nn-1);
}


//  find or create a given node by numbers[]

Tree * Tree::make(int nodes[], int nn)
{
   Tree   **psub2;
   int      ii, jj;

   ii = nodes[0];
   if ((ii < nsub) && psub[ii])
   {
      if (nn == 1) return psub[ii];
      return psub[ii]->make(&nodes[1],nn-1);
   }

   if (ii >= nsub)
   {
      psub2 = new Tree * [ii+1];
      if (psub2 == null) zappcrash("Tree, no memory",null);
      for (jj = 0; jj < nsub; jj++) psub2[jj] = psub[jj];
      for (jj = nsub; jj < ii; jj++) psub2[jj] = 0;
      delete [] psub;
      psub = psub2;
      nsub = ii + 1;
   }

   psub[ii] = new Tree("noname");
   if (! psub[ii]) zappcrash("Tree, no memory",null);

   if (nn == 1) return psub[ii];
   return psub[ii]->make(&nodes[1],nn-1);
}


//  dump tree data to stdout (call with level 0)

void Tree::dump(int level)
{
   cchar    *name;

   if (! tname) name = "noname";
   else name = tname;
   printz("%*s level: %d  name: %s  subs: %d  mem: %d \n",
                        level*2,"",level,name,nsub,tmem);
   for (int ii = 0; ii < nsub; ++ii){
       if (psub[ii]) psub[ii]->dump(level+1);
   }
}


//  get node counts and total data per level
//  level 0 + nn more levels, as given in calls to put(...nodes[],nn)
//  caller must initialize counters to zero

void Tree::stats(int nn[], int nd[])
{
   nn[0] += 1;
   nd[0] += tmem;
   for (int ii = 0; ii < nsub; ++ii){
       if (psub[ii]) psub[ii]->stats(&nn[1],&nd[1]);
   }
}



