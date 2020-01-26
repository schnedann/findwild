/**************************************************************************
   findwild      general wildcard file and string search

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

***************************************************************************/

#include "zfuncs.h"

#define findwild_release "findwild-2.7"                  //  version
#define Tmax 500                                         //  max. dialog entry text cc
#define Smax 10                                          //  max. search/ignore string count
                                                         //  file matching and ignoring rules:
#define match_any          1                             //  find files with any search string
#define match_all          2                             //  find files with all search strings
#define match_rec_all      3                             //  find files with all search strings in one record
#define ignore_any         1                             //  ignore files with any ignore string
#define ignore_all         2                             //  ignore files with all ignore strings
#define ignore_rec_all     3                             //  ignore files with all ignore strings in one record
#define ignore_match_any   4                             //  ignore match record with any ignore string
#define ignore_match_all   5                             //  ignore match record with all ignore strings

PangoFontDescription    *font;
GtkWidget      *mWin, *mVbox, *mScroll, *mLog;                                   //  main window widgets
GtkTextBuffer  *textBuff;
GtkWidget      *toolbar, *stbar;

#define MWIN GTK_WINDOW(mWin)

int initfunc(void * data);                                                       //  gtk_main() initial function

void buttfunc(GtkWidget *item, cchar *menu);                                     //  toolbar button response functions
void m_search();
void m_save();
void m_kill();
void m_clear();
void m_help();
void m_quit();

int search_dialog_event(zdialog *zd, cchar *event);                              //  widget event response function
void break_criteria(char *string, char *strings[Smax], int &count);              //  break search/ignore strings into substrings
void filescan();                                                                 //  directory / file name search
int filesearch(cchar *file);                                                     //  search file for matching string
int search_dialog_stuff(zdialog *zd);                                            //  search criteria >> dialog widgets
int search_dialog_fetch(zdialog *zd);                                            //  dialog widgets >> search criteria
void load_file(zdialog *zd);                                                     //  load criteria from a file
int load_file2(cchar *file);
void save_file(zdialog *zd);                                                     //  save criteria to a file

int         matchrule, ignorerule;                                               //  match and ignore rules from dialog
char        sr_path[Tmax], sr_file[Tmax], sr_string[Tmax];                       //  search strings from dialog
char        ig_file[Tmax], ig_string[Tmax];                                      //  ignore strings
char        delims[100];                                                         //  string delimiters
char        date_from[20], date_to[20];                                          //  date range, string format
time_t      dt_from, dt_to;                                                      //  date range, binary format
int         Fhits;                                                               //  flag, search prior search hits
bool        FignorecaseF = false;                                                //  flag, ignore case searching files
bool        FignorecaseS = false;                                                //  flag, ignore case searching strings

cchar  *mstext[3] = { "any search string", "all search strings",
                      "all search strings in same record" };

cchar  *igtext[5] = { "any ignore string", "all ignore strings",
                      "all ignore strings in same record",
                      "with any ignore string",
                      "with all ignore strings" };

cchar  defaultdelims[] = " =()[]{}.,;:'<>!-+*/|~`%^&?\\\"";

char        *srfiles[Smax], *srstrings[Smax];                                    //  0-Smax search files and strings
char        *igfiles[Smax], *igstrings[Smax];                                    //  0-Smax ignore files and strings
int         nsrf, nsrs, nigf, nigs;                                              //  actual counts

int         dialogbusy = 0, killsearch = 0;                                      //  flags
int         listmatch = 0;
int         listprec = 0;
int         listfoll = 0;

char        criteriaFile[1000];                                                  //  file - save search criteria
char        hitsFile[1000];                                                      //  file - save search hits (found files)
char        hitsFile2[1000];                                                     //  file - read search hits
char        workbuff[1000];
char        workbuff2[1000];

/**
 * @brief main - main windowing program
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
   appimage_install("findwild");                                                 //  if appimage, menu integration      2.4

   if (argc > 1 && strmatch(argv[1],"-uninstall"))                               //  uninstall appimage                 2.4
      appimage_unstall();                                                        //  (does not return if uninstalled)

   setenv("GDK_BACKEND","x11",1);                                                //  fedora/wayland
   setenv("GTK_THEME","default",0);                                              //  KDE window  manager

   gtk_init(&argc, &argv);                                                       //  GTK command line options

   zinitapp(findwild_release);                                                   //  set up app directories

   zdialog_inputs("load");                                                       //  1.6

   if (argc > 1) strncpy0(criteriaFile,argv[1],999);                             //  get opt. command line file
   else *criteriaFile = 0;

   mWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);                                   //  main window
   gtk_window_set_title(GTK_WINDOW(mWin),findwild_release);
   gtk_window_set_position(GTK_WINDOW(mWin),GTK_WIN_POS_CENTER);
   gtk_window_set_default_size(GTK_WINDOW(mWin),800,400);

   mVbox = gtk_box_new(VERTICAL,0);                                              //  vertical packing box
   gtk_container_add(GTK_CONTAINER(mWin),mVbox);                                 //  add to main window

   toolbar = create_toolbar(mVbox,32);                                           //  create tool bar and buttons

   add_toolbar_button(toolbar,"search","start search","find.png",buttfunc);
   add_toolbar_button(toolbar,"save","save output","save.png",buttfunc);
   add_toolbar_button(toolbar,"kill","kill search","stop.png",buttfunc);
   add_toolbar_button(toolbar,"clear","clear output","clear.png",buttfunc);
   add_toolbar_button(toolbar,"quit","quit findwild","quit.png",buttfunc);
   add_toolbar_button(toolbar,"stats","statistics","stats.png",buttfunc);        //  2.7
   add_toolbar_button(toolbar,"help","show user guide","help.png",buttfunc);

   mScroll = gtk_scrolled_window_new(0,0);                                       //  scrolled window
   gtk_box_pack_start(GTK_BOX(mVbox),mScroll,1,1,0);                             //  add to main window mVbox
   mLog = gtk_text_view_new();                                                   //  text window
   gtk_text_view_set_left_margin(GTK_TEXT_VIEW(mLog),2);
   gtk_container_add(GTK_CONTAINER(mScroll),mLog);                               //  add to scrolled window
   textBuff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mLog));                     //  get related text buffer

   stbar = create_stbar(mVbox);                                                  //  add status bar

   gtk_widget_show_all(mWin);                                                    //  show all widgets
   G_SIGNAL(mWin,"destroy",m_quit,0);                                            //  connect window destroy event
   g_timeout_add(0,initfunc,0);                                                  //  setup initial call from gtk_main()
   gtk_main();                                                                   //  process window events
   return 0;
}

/**
 * @brief initfunc - initial function called from gtk_main() at startup
 * @param data
 * @return
 */
int initfunc(void * data){
  int      ii, err;

  *sr_path = 0;
  *sr_file = 0;
  *sr_string = 0;
  *ig_file = 0;
  *ig_string = 0;
  strcpy(delims,defaultdelims);
  *date_from = 0;
  *date_to = 0;

  for (ii = 0; ii < Smax; ++ii){
    srfiles[ii] = 0;
    srstrings[ii] = 0;
    igfiles[ii] = 0;
    igstrings[ii] = 0;
  }

  dt_from = dt_to = 0;

  *hitsFile = 0;                                                                //  set up search hits save file
  strncatv(hitsFile,999,get_zhomedir(),"/search_hits",null);

  if (*criteriaFile) {
    err = load_file2(criteriaFile);                                            //  load command line file
    if (err) {
       printf("%s \n %s \n",criteriaFile,strerror(err));
       *criteriaFile = 0;
    }
  }

  if (! *criteriaFile)
  strncatv(criteriaFile,999,get_zhomedir(),"/search_criteria",null);

  return 0;
}

/**
 * @brief buttfunc - toolbar button response functions
 * @param item
 * @param menu
 */
void buttfunc(GtkWidget *item, cchar *menu)
{
   if (strmatch(menu,"search")) m_search();
   if (strmatch(menu,"save")) m_save();
   if (strmatch(menu,"kill")) m_kill();
   if (strmatch(menu,"clear")) m_clear();
   if (strmatch(menu,"help")) m_help();
   if (strmatch(menu,"stats")) phone_home_allow(mWin);                           //  2.7
   if (strmatch(menu,"quit")) m_quit();
   return;
}

/**
 * @brief m_save - save screen to file
 */
void m_save(){
   textwidget_save(mLog,MWIN);
   return;
}

/**
 * @brief m_kill - kill running search
 */
void m_kill(){
   textwidget_append2(mLog,0,"kill ... \n");
   killsearch = 1;
   return;
}

/**
 * @brief m_clear - clear screen
 */
void m_clear(){
   textwidget_append2(mLog,0,"clear \n");
   zsleep(100000000); //100ms
   gtk_text_buffer_set_text(textBuff,"", -1);
   return;
}

/**
 * @brief m_help - launch help file in new process
 */
void m_help(){
   showz_docfile(mWin,"userguide",0);
   return;
}

/**
 * @brief m_quit - exit application
 */
void m_quit()
{
   zdialog_inputs("save");                                                       //  1.6
   gtk_main_quit();
   return;
}

//  build and initiate search dialog

/***
          _________________________________________________________________
         |                                                                 |
         |  Find files containing:    Ignore files containing:             |
         |   (o) any search string     (o) any ignore string               |
         |   (o) all search strings    (o) all ignore strings              |
         |   (o) all in same record    (o) all in same record              |
         |                                                                 |
         |   [_] ignore file case     Ignore matching records with:        |
         |   [_] ignore string case    (o) any ignore string               |
         |                             (o) all ignore strings              |
         |                                                                 |
         |           search path  [______________________________________] |
         |        search file(s)  [______________________________________] |
         |      search string(s)  [______________________________________] |
         |        ignore file(s)  [______________________________________] |
         |      ignore string(s)  [______________________________________] |
         |     string delimiters  [______________________________________] |
         |                                                                 |
         |    date from  [_______]   to [_______]  -days or yyyy-mm-dd     |
         |                                                                 |
         |    [x] list matching records    with [__|-+] preceding records  |
         |                                 with [__|-+] following records  |
         |                                                                 |
         |    search criteria: [load file] [save file]                     |
         |                                                                 |
         |                          [search all] [search hits] [cancel]    |
         |_________________________________________________________________|

***/

void m_search()
{
   static int  ftf = 1;

   if (dialogbusy) return;
   dialogbusy = 1;

   zdialog     *zd;
   zd = zdialog_new("findwild - search criteria", mWin,
                        "search all", "search hits", "cancel", null);

   zdialog_add_widget(zd,"hbox","hbr1","dialog");

   zdialog_add_widget(zd,"hbox","space","hbr1",0,"space=5");
   zdialog_add_widget(zd,"vbox","vbr1","hbr1");
   zdialog_add_widget(zd,"hbox","hbr1a","vbr1",0,"space=3");
   zdialog_add_widget(zd,"label","rules1","hbr1a","Find files containing:");
   zdialog_add_widget(zd,"radio","ruleM1","vbr1","any search string");
   zdialog_add_widget(zd,"radio","ruleM2","vbr1","all search strings");
   zdialog_add_widget(zd,"radio","ruleM3","vbr1","all in same record");
   zdialog_add_widget(zd,"label","space","vbr1");
   zdialog_add_widget(zd,"check","FignorecaseF","vbr1","ignore file case");
   zdialog_add_widget(zd,"check","FignorecaseS","vbr1","ignore string case");

   zdialog_add_widget(zd,"hbox","space","hbr1",0,"space=15");
   zdialog_add_widget(zd,"vbox","vbr2","hbr1");
   zdialog_add_widget(zd,"hbox","hbr2a","vbr2",0,"space=3");
   zdialog_add_widget(zd,"label","rules2","hbr2a","Ignore files containing:");
   zdialog_add_widget(zd,"radio","ruleI1","vbr2","any ignore string");
   zdialog_add_widget(zd,"radio","ruleI2","vbr2","all ignore strings");
   zdialog_add_widget(zd,"radio","ruleI3","vbr2","all in same record");

   zdialog_add_widget(zd,"hbox","hbr2b","vbr2",0,"space=3");
   zdialog_add_widget(zd,"label","rules3","hbr2b","Ignore matching records with:");
   zdialog_add_widget(zd,"radio","ruleI4","vbr2","any ignore strings");
   zdialog_add_widget(zd,"radio","ruleI5","vbr2","all ignore strings");

   zdialog_add_widget(zd,"hbox","hbs1","dialog",0,"space=8");
   zdialog_add_widget(zd,"vbox","vbs11","hbs1",0,"homog|space=5");
   zdialog_add_widget(zd,"vbox","vbs12","hbs1",0,"homog|expand|space=3");
   zdialog_add_widget(zd,"label","lab_sr_path","vbs11","      search path");
   zdialog_add_widget(zd,"label","lab_sr_file","vbs11","     search file(s)");
   zdialog_add_widget(zd,"label","lab_sr_string","vbs11"," search string(s)");
   zdialog_add_widget(zd,"label","lab_ig_file","vbs11","     ignore file(s)");
   zdialog_add_widget(zd,"label","lab_ig_string","vbs11"," ignore string(s)");
   zdialog_add_widget(zd,"label","lab_delims","vbs11","string delimiters");
   zdialog_add_widget(zd,"entry","sr_path","vbs12",0,"expand");
   zdialog_add_widget(zd,"entry","sr_file","vbs12",0,"expand");
   zdialog_add_widget(zd,"entry","sr_string","vbs12",0,"expand");
   zdialog_add_widget(zd,"entry","ig_file","vbs12",0,"expand");
   zdialog_add_widget(zd,"entry","ig_string","vbs12",0,"expand");
   zdialog_add_widget(zd,"entry","delims","vbs12",0,"expand");

   zdialog_add_widget(zd,"hbox","hbd2","dialog",0,"space=3");
   zdialog_add_widget(zd,"label","lab_dt_from","hbd2","  date from");
   zdialog_add_widget(zd,"entry","dt_from","hbd2","-9999","size=10");
   zdialog_add_widget(zd,"label","lab_dt_to","hbd2","  to");
   zdialog_add_widget(zd,"entry","dt_to","hbd2","0","size=10");
   zdialog_add_widget(zd,"label","lab_dtx","hbd2","-days or yyyy-mm-dd","space=10");

   zdialog_add_widget(zd,"hbox","hblmr","dialog",0,"space=5");
   zdialog_add_widget(zd,"vbox","vblmr1","hblmr",0,"space=5");
   zdialog_add_widget(zd,"vbox","vblmr2","hblmr");

   zdialog_add_widget(zd,"check","list match","vblmr1","list matching records","space=2");

   zdialog_add_widget(zd,"hbox","hbt1","vblmr2");
   zdialog_add_widget(zd,"label","lab_lm1","hbt1","with","space=3");
   zdialog_add_widget(zd,"spin","prec","hbt1","0|99|1|0","space=3");
   zdialog_add_widget(zd,"label","lab_lm2","hbt1","preceding records");

   zdialog_add_widget(zd,"hbox","hbt2","vblmr2");
   zdialog_add_widget(zd,"label","lab_lm3","hbt2","with","space=3");
   zdialog_add_widget(zd,"spin","foll","hbt2","0|99|1|0","space=3");
   zdialog_add_widget(zd,"label","lab_lm4","hbt2","following records");

   zdialog_add_widget(zd,"hbox","hbf","dialog",0,"space=3");
   zdialog_add_widget(zd,"label","labfile","hbf","  search criteria:");
   zdialog_add_widget(zd,"button","load","hbf","load file","space=10");
   zdialog_add_widget(zd,"button","save","hbf","save file");

   search_dialog_stuff(zd);                                                      //  search criteria >> dialog_widgets
   zdialog_restore_inputs(zd);                                                   //  restore user inputs                1.6

   if (ftf) zdialog_stuff(zd,"delims",defaultdelims);                            //  if app startup, use default delimiters
   ftf = 0;                                                                      //    instead of last-used set         1.9

   zdialog_run(zd,search_dialog_event,"parent");                                 //  start dialog (non modal)
   return;
}

/**
 * @brief search_dialog_event - search dialog - widget event response function
 * @param zd
 * @param event
 * @return
 */
int search_dialog_event(zdialog *zd, cchar *event)
{
   if (zd->zstat)                                                                //  dialog complete
   {
      if (zd->zstat == 1) {                                                      //  search all
         zd->zstat = 0;
         event = "search";
         Fhits = 0;
      }

      else if (zd->zstat == 2) {                                                 //  search hits
         zd->zstat = 0;
         event = "search";
         Fhits = 1;
      }

      else {                                                                     //  cancel                             1.7
         zdialog_free(zd);
         dialogbusy = 0;
         return 1;
      }
   }

   if (strmatch(event,"search"))
   {                                                                             //  search button
      search_dialog_fetch(zd);                                                   //  dialog widgets >> search criteria
      filescan();                                                                //  start search
   }

   if (strmatch(event,"load")) load_file(zd);                                    //  1.2
   if (strmatch(event,"save")) save_file(zd);

   return 1;
}

/**
 * @brief break_criteria - break search criteria into 0-Smax search/ignore substrings
 * @param string
 * @param strings
 * @param count
 */
void break_criteria(char *string, char *strings[Smax], int &count)
{
   int         ii;
   cchar       *pp;

   count = 0;
   if (! strlen(string)) return;

   for (ii = 0; ii < Smax; ii++)
   {
      if (strings[ii]) zfree(strings[ii]);
      strings[ii] = 0;
   }

   for (ii = 0; ii < Smax; ii++)
   {
      pp = strtok(string," ");                                                   //  replace strField                   2.7
      if (! pp) break;                                                           //  (" chars. are retained)
      strings[ii] = zstrdup(pp);
      string = 0;
   }

   count = ii;
   return;
}

/**
 * @brief search_dialog_stuff - search criteria in memory >> dialog widgets
 * @param zd
 * @return
 */
int search_dialog_stuff(zdialog *zd)
{
   char  ruleMx[8] = "ruleMx", ruleIx[8] = "ruleIx";

   if (! matchrule) matchrule = 1;                                               //  defaults: match any, ignore any
   if (! ignorerule) ignorerule = 1;                                             //  1.5

   ruleMx[5] = matchrule + '0';
   ruleIx[5] = ignorerule + '0';
   zdialog_stuff(zd,ruleMx,1);
   zdialog_stuff(zd,ruleIx,1);
   zdialog_stuff(zd,"FignorecaseF",int(FignorecaseF));                                //  1.8
   zdialog_stuff(zd,"FignorecaseS",int(FignorecaseS));                                //  1.7
   zdialog_stuff(zd,"sr_path",sr_path);
   zdialog_stuff(zd,"sr_file",sr_file);
   zdialog_stuff(zd,"sr_string",sr_string);
   zdialog_stuff(zd,"ig_file",ig_file);
   zdialog_stuff(zd,"ig_string",ig_string);
   zdialog_stuff(zd,"delims",delims);
   zdialog_stuff(zd,"dt_from",date_from);
   zdialog_stuff(zd,"dt_to",date_to);

   return 0;
}

/**
 * @brief search_dialog_fetch - dialog widgets >> search criteria in memory
 * @param zd
 * @return
 */
int search_dialog_fetch(zdialog *zd)
{
   time_t  search_dialog_fetchdate(cchar *date);
   int      rule;

   zdialog_fetch(zd,"ruleM1",rule); if (rule) matchrule = 1;                     //  get match and ignore rules
   zdialog_fetch(zd,"ruleM2",rule); if (rule) matchrule = 2;
   zdialog_fetch(zd,"ruleM3",rule); if (rule) matchrule = 3;
   zdialog_fetch(zd,"ruleI1",rule); if (rule) ignorerule = 1;
   zdialog_fetch(zd,"ruleI2",rule); if (rule) ignorerule = 2;
   zdialog_fetch(zd,"ruleI3",rule); if (rule) ignorerule = 3;
   zdialog_fetch(zd,"ruleI4",rule); if (rule) ignorerule = 4;
   zdialog_fetch(zd,"ruleI5",rule); if (rule) ignorerule = 5;

   zdialog_fetch(zd,"FignorecaseF",FignorecaseF);                                //  1.8
   zdialog_fetch(zd,"FignorecaseS",FignorecaseS);                                //  1.7

   zdialog_fetch(zd,"sr_path",sr_path,Tmax);                                     //  get string entry fields
   zdialog_fetch(zd,"sr_file",sr_file,Tmax);
   zdialog_fetch(zd,"sr_string",sr_string,Tmax);
   zdialog_fetch(zd,"ig_file",ig_file,Tmax);
   zdialog_fetch(zd,"ig_string",ig_string,Tmax);
   zdialog_fetch(zd,"delims",delims,100);
   zdialog_fetch(zd,"dt_from",date_from,20);
   zdialog_fetch(zd,"dt_to",date_to,20);

   zdialog_fetch(zd,"list match",listmatch);                                     //  list matching records, yes/no
   zdialog_fetch(zd,"prec",listprec);                                            //  with preceding
   zdialog_fetch(zd,"foll",listfoll);                                            //  with following                     2.1

   dt_from = search_dialog_fetchdate(date_from);                                 //  get binary date range
   dt_to = search_dialog_fetchdate(date_to);

   return 0;
}

/**
 * @brief search_dialog_fetchdate - get a date from search dialog, format: -days  or  yyyy-mm-dd
 * @param date
 * @return
 */
time_t search_dialog_fetchdate(cchar *date)
{
   cchar       *pp;
   cchar       ddelims[] = "-./ ";
   int         err, days, day, month, year;
   time_t      now1;                                                             //  numeric time in seconds since base
   struct tm   now2;                                                             //  year/month/day/hour/min/sec

   now1 = time(0);                                                               //  get current date/time in both formats
   now2 = *localtime(&now1);

   err = convSI(date,days,-9999,0);                                              //  look for -9999 to 0 days ago
   if (err == 0) return now1 + days*24*3600;                                     //  OK, return NOW - seconds ago

   pp = strField(date,ddelims,1);                                                //  look for yyyy-mm-dd format
   if (! pp) return 0;
   err = convSI(pp,year,1970,now2.tm_year+1900);                                 //  year, 1970 to current
   if (err) return 0;
   pp = strField(date,ddelims,2);
   if (! pp) return 0;
   err = convSI(pp,month,1,12);                                                  //  month, 1-12
   if (err) return 0;
   pp = strField(date,ddelims,3);
   if (! pp) return 0;
   err = convSI(pp,day,1,31);                                                    //  day, 1-31
   if (err) return 0;
   now2.tm_year = year-1900;
   now2.tm_mon = month-1;
   now2.tm_mday = day;
   now1 = mktime(&now2);                                                         //  conv. to numeric time
   if (now1 < 0) return 0;
   return now1;
}

/**
 * @brief filescan - directory scan / file search function
 */
void filescan()
{
   int         ccp, ccf, ii, jj;
   int         fcount, lcount, ftf, err;
   char        ch;
   cchar       *pfile, *pname;
   struct tm   dfrom, dto;
   STATB       statf;
   FILE        *fid = null, *fid2;

   killsearch = 0;

   for (ii = 0; ii < (int) strlen(sr_string); ii++)
   {
      ch = sr_string[ii];
      if (ch != ' ' && ch != '*' && strchr(delims,ch)) {
         zmessageACK(mWin,"delimiter  %c  is contained in search string",ch);
         return;
      }
   }

   for (ii = 0; ii < (int) strlen(ig_string); ii++)
   {
      ch = ig_string[ii];
      if (ch != ' ' && ch != '*' && strchr(delims,ch)) {
         zmessageACK(mWin,"delimiter  %c  is contained in ignore string",ch);
         return;
      }
   }

   textwidget_append(mLog,0,"\n""begin search --------------------------- \n");  //  report all criteria used for search

   if (Fhits) textwidget_append(mLog,0," search hits (files from previous search results) \n");
   textwidget_append(mLog,0,"    find files with: %s \n",mstext[matchrule-1]);
   if (ignorerule < 4) textwidget_append(mLog,0,"  ignore files with: %s \n",igtext[ignorerule-1]);
   else textwidget_append(mLog,0,"  ignore match recs: %s \n",igtext[ignorerule-1]);

   textwidget_append(mLog,0,"        search path: %s \n",sr_path);
   textwidget_append(mLog,0,"        search file: %s \n",sr_file);
   textwidget_append(mLog,0,"   search string(s): %s \n",sr_string);
   textwidget_append(mLog,0,"     ignore file(s): %s \n",ig_file);
   textwidget_append(mLog,0,"   ignore string(s): %s \n",ig_string);
   textwidget_append(mLog,0,"  string delimiters: %s \n",delims);

   if(FignorecaseF){                                                             //  1.8
      textwidget_append(mLog,0," ignore file case: YES \n");
   }else{
      textwidget_append(mLog,0," ignore file case: NO \n");
   }
   if(FignorecaseS){                                                             //  1.7
      textwidget_append(mLog,0," ignore string case: YES \n");
   }else{
      textwidget_append(mLog,0," ignore string case: NO \n");
   }
   if (dt_from || dt_to) {                                                       //  report date range if defined
      dfrom = *localtime(&dt_from);
      dto = *localtime(&dt_to);
      textwidget_append(mLog,0,"     mod date from: %d-%02d-%02d  to: %d-%02d-%02d \n",
                     dfrom.tm_year+1900, dfrom.tm_mon+1, dfrom.tm_mday,
                     dto.tm_year+1900, dto.tm_mon+1, dto.tm_mday);
   }

   textwidget_append2(mLog,0,"\n");                                              //  scroll to end

   ccp = strlen(sr_path);
   ccf = strlen(sr_file);

   fcount = 0;

   if (! ccp) goto search_exit;                                                  //  sanity checks
   if (! ccf) { strcpy(sr_file,"*"); ccf = 1; }
   if (ccp + ccf > 998) goto search_exit;
   if (dt_from > dt_to) goto search_exit;
   if (dt_from > time(0)) goto search_exit;

   break_criteria(sr_file,srfiles,nsrf);                                         //  break search criteria into
   break_criteria(sr_string,srstrings,nsrs);                                     //    search/ignore substrings
   break_criteria(ig_file,igfiles,nigf);
   break_criteria(ig_string,igstrings,nigs);

   if (! Fhits)                                                                  //  normal search
   {
      fid = fopen(hitsFile,"w");                                                 //  open output file for search hits
      if (! fid) zappcrash("cannot open search_hits output file");

      for (ii = 0; ii < nsrf; ii++)                                              //  loop all search files
      {
         strcpy(workbuff,sr_path);                                               //  combine pathname/filename to search
         if (sr_path[ccp-1] == '*' && srfiles[ii][0] == '*')
               strcat(workbuff,srfiles[ii]+1);                                   //  avoid path/**file
         else  strcat(workbuff,srfiles[ii]);

         ftf = 1;                                                                //  new file search

         while (true)
         {
            if (FignorecaseF) {                                                  //  find matching files, ignoring case
               pfile = SearchWildCase(workbuff,ftf);                             //  1.8
               if (! pfile) break;
            }
            else {
               pfile = SearchWild(workbuff,ftf);                                 //  find matching files, case match
               if (! pfile) break;
            }

            pname = strrchr(pfile,'/') + 1;                                      //  file name part
            stbar_message(stbar,pfile);                                          //  progress tracking in status bar

            for (jj = 0; jj < nigf; jj++)
            {
               if (MatchWild(igfiles[jj],pname) == 0) break;                     //  file name part matches ignore file
               if (MatchWild(igfiles[jj],pfile) == 0) break;                     //  whole filespec matches ignore file
            }
            if (jj < nigf) continue;                                             //  ignore file

            if (dt_to > 0) {                                                     //  check mod date against range
               err = stat(pfile,&statf);
               if (err) continue;
               if (statf.st_mtime < dt_from ||                                   //  out of range, ignore file
                   statf.st_mtime > dt_to) continue;
            }

            lcount = filesearch(pfile);                                          //  search for match strings, list hits
            if (lcount) {
               fprintf(fid,"%s""\n",pfile);                                      //  write matching file to hits list
               fcount++;
            }

            if (killsearch) {
               ftf = 2;                                                          //  terminate search
               if (FignorecaseF)                                                 //  1.8
                  SearchWildCase(workbuff,ftf);
               else
                  SearchWild(workbuff,ftf);
               break;
            }

            zmainloop();                                                         //  keep GUI alive
         }

         if (killsearch) break;
      }

      fclose(fid);
   }

   if (Fhits)                                                                    //  search hits (previous files found)
   {
      strcpy(hitsFile2,hitsFile);                                                //  make copy of previous hits file
      strcat(hitsFile2,"_2");
      err = Qshell(0,0,"mv -f %s %s",hitsFile,hitsFile2);
      if (err) {
         zmessageACK(mWin,"no previous files to search");
         return;
      }

      fid2 = fopen(hitsFile2,"r");                                               //  open copy for reading previous hits
      if (! fid2) zappcrash("cannot open search_hits input file");

      fid = fopen(hitsFile,"w");                                                 //  open hits for output of new hits
      if (! fid) zappcrash("cannot open search_hits output file");

      while (true)
      {
         pfile = fgets_trim(workbuff,999,fid2);                                  //  get next file from hit list
         if (! pfile) break;

         pname = strrchr(pfile,'/') + 1;                                         //  file name part
         stbar_message(stbar,pfile);                                             //  progress tracking in status bar

         for (ii = 0; ii < nsrf; ii++)                                           //  check for match with search files
            if (MatchWild(srfiles[ii],pname) == 0) break;                        //  (match file name only)
         if (ii == nsrf) continue;                                               //  no match

         for (jj = 0; jj < nigf; jj++)
         {
            if (MatchWild(igfiles[jj],pname) == 0) break;                        //  file name part matches ignore file
            if (MatchWild(igfiles[jj],pfile) == 0) break;                        //  whole filespec matches ignore file
         }
         if (jj < nigf) continue;                                                //  ignore file

         if (dt_to > 0) {                                                        //  check mod date against range
            err = stat(pfile,&statf);
            if (err) continue;
            if (statf.st_mtime < dt_from ||                                      //  out of range, ignore file
                statf.st_mtime > dt_to) continue;
         }

         lcount = filesearch(pfile);                                             //  search for match strings, list hits
         if (lcount) {
            fprintf(fid,"%s""\n",pfile);                                         //  write matching file to hits list
            fcount++;
         }

         if (killsearch) break;
         zmainloop();                                                            //  keep GUI alive
      }

      fclose(fid);
      fclose(fid2);
   }

search_exit:
   if (killsearch) {
      textwidget_append2(mLog,0," *** search killed *** \n");
      killsearch = 0;
   }
   else {
      textwidget_append(mLog,0,"\n %d files found \n",fcount);
      textwidget_append2(mLog,0,"search completed ----------------------- \n");
   }

   return;
}


//  file search function - search all file records for search and ignore string(s)

int filesearch(cchar *filename)
{
   void recsearch(char *buff,                                                    //  record to search                   1.5
                  int Rmatch[], int nsrs,                                        //  search strings matched
                  int Rignore[], int nigs,                                       //  ignore strings matched
                  int &recmatch, int &recignore);                                //  returned total counts

   char * recsearch1(char* const record,                                         //  record to search                   2.5
                     char* const wildstr,                                        //  wildcard string to search for
                     char* const delims,                                         //  string delimiters in record
                     int &cc,                                                    //  length of returned match string
                     bool ignorecase);                                           //  ignore case flag

   int      Fmatch[Smax], Fignore[Smax];                                         //  search and ignore strings in file
   int      Rmatch[Smax], Rignore[Smax];                                         //  search and ignore strings in record
   int      filematch, recmatch, recignore;
   char     buff[1000], *pp, *pbuff[100];
   int      ii, Nline, Nlistfoll = 0, Fclearprec = 0;
   int      line, cc, pos;
   FILE     *fid;

   if (nsrs == 0 && nigs == 0) {                                                 //  no search or ignore strings (matches)
      textwidget_append2(mLog,0," %s \n",filename);                              //  output file name with no record counts
      return 1;
   }

   fid = fopen(filename,"r");                                                    //  open file
   if (! fid) return 0;

   for (ii = 0; ii < Smax; ii++) Fmatch[ii] = Fignore[ii] = 0;                   //  no strings found in file yet
   filematch = 0;

   while (true)
   {
      pp = fgets_trim(buff,999,fid,1);                                           //  read next record
      if (! pp) break;
      if (killsearch) break;

      recsearch(buff,Rmatch,nsrs,Rignore,nigs,recmatch,recignore);               //  search for match and ignore strings

      if (recmatch + recignore == 0) continue;                                   //  record has no matches, ignore

      if (nigs > 0) {
         if (ignorerule == ignore_any && recignore > 0) {
            filematch = 0;                                                       //  reject if any ignore string in record
            break;
         }

         if (ignorerule == ignore_rec_all && recignore == nigs) {
            filematch = 0;                                                       //  reject if all ignore strings in record
            break;
         }
      }

      if (recmatch > 0) {
         if (matchrule == match_rec_all && recmatch < nsrs) recmatch = 0;        //  ignore record without all match strings
         if (recignore > 0) {
            if (ignorerule == ignore_match_any) recmatch = 0;                    //  ignore record with any ignore string
            if (ignorerule == ignore_match_all)
                if (recignore == nigs) recmatch = 0;                             //  ignore record with all ignore strings
         }
      }

      if (recmatch > 0) {
         filematch += recmatch;                                                  //  find matches for entire file
         for (ii = 0; ii < nsrs; ii++) Fmatch[ii] += Rmatch[ii];
      }

      if (recignore > 0)                                                         //  ignore matches for entire file
         for (ii = 0; ii < nigs; ii++) Fignore[ii] += Rignore[ii];
   }

   fclose(fid);

   if (killsearch) return 0;

   if (filematch == 0) return 0;                                                 //  reject file with no match strings

   if (ignorerule == ignore_all && nigs > 0) {
      for (ii = 0; ii < nigs; ii++) if (Fignore[ii] == 0) break;                 //  reject files with all ignore strings
      if (ii == nigs) return 0;
   }

   if (matchrule == match_all) {
      for (ii = 0; ii < nsrs; ii++) if (Fmatch[ii] == 0) return 0;               //  reject files without all match strings
   }

   if (! listmatch) {
      textwidget_append2(mLog,0," %5d %s \n",filematch,filename);                //  output match count and file name
      return filematch;
   }

   //  file is selected and detail is wanted, read again to list matching records

   snprintf(buff,999,"\n %s \n",filename);                                       //  output file name in bold
   textwidget_append2(mLog,1,buff);

   fid = fopen(filename,"r");                                                    //  open file
   if (! fid) return 0;

   Nline = 0;                                                                    //  track line numbers                 2.0

   for (ii = 0; ii <= listprec; ii++)                                            //  number preceding recs. to list     1.5
      pbuff[ii] = 0;                                                             //  empty at first

   while (true)
   {
      if (killsearch) break;

      ii = listprec;                                                             //  discard oldest preceding record    1.5
      if (pbuff[ii]) zfree(pbuff[ii]);

      for (ii = listprec; ii > 0; ii--)                                          //  save 'listprec' preceding records
         pbuff[ii] = pbuff[ii-1];

      pbuff[0] = (char *) zmalloc(1000);                                         //  allocate new buffer
      pp = fgets_trim(pbuff[0],999,fid,1);                                       //  read next record
      if (! pp) break;

      Nline++;                                                                   //  track line numbers                 2.0

      recsearch(pbuff[0],Rmatch,nsrs,Rignore,nigs,recmatch,recignore);           //  search for match and ignore strings

      if (recmatch > 0) {
         if (matchrule == match_rec_all && recmatch < nsrs) recmatch = 0;        //  ignore record without all match strings
         if (recignore > 0) {
            if (ignorerule == ignore_match_any) recmatch = 0;                    //  ignore record with any ignore string
            if (ignorerule == ignore_match_all)
                if (recignore == nigs) recmatch = 0;                             //  ignore record with all ignore strings
         }
      }

      if (recmatch) {                                                            //  this record is a match
         filematch += recmatch;                                                  //  accumulate matches for entire file
         for (ii = 0; ii < nsrs; ii++) Fmatch[ii] += Rmatch[ii];
         for (ii = 0; ii < nigs; ii++) Fignore[ii] += Rignore[ii];
         Nlistfoll = listfoll;                                                   //  set following records to list      2.1
      }

      if (recmatch) {                                                            //  print preceding records            1.5
         for (ii = listprec; ii > 0; ii--)
            if (pbuff[ii])
               textwidget_append2(mLog,0,"%5d  %s \n",Nline-ii,pbuff[ii]);
         Fclearprec = 1;                                                         //  clear preceding records buffer
      }

      if (recmatch)
      {
         textwidget_append2(mLog,0,"%5d  %s \n",Nline,pbuff[0]);                 //  print matching record              2.5

         for (ii = 0; ii < nsrs; ii++) {                                         //  loop match strings
            pp = pbuff[0];
            while (true) {                                                       //  search record
               pp = recsearch1(pp,srstrings[ii],delims,cc,FignorecaseS);         //  get next matching string and length
               if (! pp) break;                                                  //  not found
               line = textwidget_linecount(mLog) - 2;                            //  text report line
               pos = pp-pbuff[0] + 7;                                            //  string position
               textwidget_bold_word(mLog,line,pos,cc);                           //  make string bold
               pp += cc;                                                         //  resume search from string end
            }
         }
      }

      if (recmatch && listprec > 0 && ! listfoll)                                //  add a spacer line if no following  2.1
         textwidget_append2(mLog,0,"\n");                                        //    records are to be listed

      if (! recmatch && Nlistfoll > 0) {
         textwidget_append2(mLog,0,"%5d  %s \n",Nline,pbuff[ii]);                //  list records following match       2.1
         Nlistfoll--;
         if (Nlistfoll == 0) textwidget_append2(mLog,0,"\n");                    //  add a spacer line after following  2.1
         Fclearprec = 1;                                                         //    records are listed
      }

      if (Fclearprec) {                                                          //  clear preceding records buffer     2.1
         for (ii = 0; ii <= listprec; ii++) {
            if (pbuff[ii]) zfree(pbuff[ii]);
            pbuff[ii] = 0;
         }
         Fclearprec = 0;
      }
   }

   fclose(fid);

   for (ii = 0; ii <= listprec; ii++)                                            //  clear preceding records buffer
      if (pbuff[ii]) zfree(pbuff[ii]);

   return filematch;
}

/**
 * @brief recsearch - search a single record for strings to match and strings not to match (ignore strings)
 * @param buff
 * @param Rmatch
 * @param nsrs
 * @param Rignore
 * @param nigs
 * @param recmatch
 * @param recignore
 */
void recsearch(char *buff,                                                       //  record to search
               int Rmatch[], int nsrs,                                           //  search strings matched
               int Rignore[], int nigs,                                          //  ignore strings matched
               int &recmatch, int &recignore)                                    //  returned total counts
{
   char     buff2[1000], *pp, *token;
   int      ii;

   strncpy0(buff2,buff,999);                                                     //  copy before destruction by strtok()

   for (ii = 0; ii < Smax; ii++)                                                 //  no strings found in record yet
      Rmatch[ii] = Rignore[ii] = 0;

   recmatch = recignore = 0;
   pp = buff2;

   if (FignorecaseS)                                                             //  ignore case option        1.7
   {
      while (true)
      {
         token = strtok(pp,delims);                                              //  get next string defined by delimiters
         if (! token) break;
         pp = 0;                                                                 //  for strtok() continuation calls

         for (ii = 0; ii < nsrs; ii++)
         {
            if (MatchWildIgnoreCase(srstrings[ii],token) == 0) {                 //  match with all search strings
               Rmatch[ii]++;
               if (Rmatch[ii] == 1) recmatch++;                                  //  search strings found, 0...nsrs
            }
         }

         if (nsrs == 0) recmatch++;                                              //  no search strings = match

         for (ii = 0; ii < nigs; ii++)
         {
            if (MatchWildIgnoreCase(igstrings[ii],token) == 0) {                 //  match with all ignore strings
               Rignore[ii]++;
               if (Rignore[ii] == 1) recignore++;                                //  ignore strings found, 0...nigs
            }
         }
      }
   }

   else
   {
      while (true)
      {
         token = strtok(pp,delims);                                              //  get next string defined by delimiters
         if (! token) break;
         pp = 0;                                                                 //  for strtok() continuation calls

         for (ii = 0; ii < nsrs; ii++)
         {
            if (MatchWild(srstrings[ii],token) == 0) {                           //  match with all search strings
               Rmatch[ii]++;
               if (Rmatch[ii] == 1) recmatch++;                                  //  search strings found, 0...nsrs
            }
         }

         if (nsrs == 0) recmatch++;                                              //  no search strings = match

         for (ii = 0; ii < nigs; ii++)
         {
            if (MatchWild(igstrings[ii],token) == 0) {                           //  match with all ignore strings
               Rignore[ii]++;
               if (Rignore[ii] == 1) recignore++;                                //  ignore strings found, 0...nigs
            }
         }
      }
   }

   return;
}


//
/**
 * @brief recsearch1 - Search a record for a string between delimiters which matches an input wildcard string.
 *                     Returns position of matching string within record, and its length in 'cc'.
 *                     Returns null if no match is found.
 *                     If ignorecase is not null, case is ignored in the string comparison.
 * @param record
 * @param wildstr
 * @param delims
 * @param cc
 * @param ignorecase
 * @return
 */
char* recsearch1(char* const record, char* const wildstr, char* const delims, int& cc, bool ignorecase){                               // 2.5
  char     *pp1, *pp2, delim;
  int      mm;
  pp1 = record;

  while (true){
    //  scan to next non-delimiter
    while ((*pp1=='\0') && strchr(delims,*pp1)){
      ++pp1;
    }
    if(*pp1=='\0'){
      pp1=nullptr;                                                             //  end of input, not found
      goto exit;
    }
    pp2 = pp1 + 1;                                                             //  pp1 = start of string in record
    while (*pp2 && ! strchr(delims,*pp2)) ++pp2;                               //  pp2 = next delimiter or null
    cc = pp2 - pp1;                                                            //  characters between pp1, pp2
    if (cc > 0) {                                                              //  length of string in record
       delim = *pp2;                                                           //  save delimiter at pp2
       *pp2 = 0;                                                               //  replace with null delimiter
       if(!ignorecase){
         mm = MatchWild(wildstr,pp1);                                          //  test for wildcard match
       }else{
         mm = MatchWildIgnoreCase(wildstr,pp1);                                //  test ignoring string case
       }
       *pp2 = delim;                                                           //  restore pp2 delimiter
       if(0==mm){
         goto exit;                                                            //  if match, return position, length
       }
    }
    pp1 = pp2;                                                                 //  pp1 = next delimiter or null
  }
exit:
  return pp1;
}

/**
 * @brief load_file - load search criteria from a file
 * @param zd
 */
void load_file(zdialog *zd){                                                    //  1.2
  char     *file;
  cchar    *dialogtitle = "load search criteria from file";
  int      err;

  file = zgetfile(dialogtitle,MWIN,"file",criteriaFile);                        //  get input file from user
  if (! file) return;

  err = load_file2(file);
  if (err) {
    zmessageACK(mWin,"error %s \n %s",strerror(err), file);
    zfree(file);
    return;
  }

  search_dialog_stuff(zd);                                                      //  stuff dialog with search criteria
  return;
}

/**
 * @brief load_file2 - load file function used by search dialog and initz. function
 * @param file
 * @return 0 if OK or 'errno' if not
 */
int load_file2(cchar *file){                                                     //  1.2
  FILE     *fid;
  char     *pp, buff[Tmax];
  int      err;

  fid = fopen(file,"r");                                                        //  open for read
  if (! fid) return errno;

  while (true)
  {
    pp = fgets_trim(buff,Tmax,fid,1);
    if (! pp) break;

    if (strmatchN(pp,"match rule ",11)) matchrule = atoi(pp+11);
    if (strmatchN(pp,"ignore rule ",12)) ignorerule = atoi(pp+12);
    if (strmatchN(pp,"search path ",12)) strcpy(sr_path,pp+12);
    if (strmatchN(pp,"search file ",12)) strcpy(sr_file,pp+12);
    if (strmatchN(pp,"search string ",14)) strcpy(sr_string,pp+14);
    if (strmatchN(pp,"ignore files ",13)) strcpy(ig_file,pp+13);
    if (strmatchN(pp,"ignore string ",14)) strcpy(ig_string,pp+14);
    if (strmatchN(pp,"delimiters ",11)) strncpy0(delims,pp+11,100);
    if (strmatchN(pp,"date from ",10)) strcpy(date_from,pp+10);
    if (strmatchN(pp,"date to ",8)) strcpy(date_to,pp+8);
  }

  err = fclose(fid);
  if (err) return errno;

  strcpy(criteriaFile,file);                                                    //  update current file
  return 0;
}

/**
 * @brief save_file - save search criteria to a file
 * @param zd
 */
void save_file(zdialog *zd){                                                    //  1.2

  FILE     *fid;
  char     *file;
  cchar    *dialogtitle = "save search criteria to a file";
  int      err;

  search_dialog_fetch(zd);                                                      //  get search criteria from dialog

  file = zgetfile(dialogtitle,MWIN,"save",criteriaFile);                        //  get output file from user
  if (! file) return;

  fid = fopen(file,"w");                                                        //  open for write
  if (! fid) {
    zmessageACK(mWin,"unable to open file %s",file);
    zfree(file);
    return;
  }

  fprintf(fid,"match rule %d \n",matchrule);                                    //  write search criteria to file
  fprintf(fid,"ignore rule %d \n",ignorerule);
  fprintf(fid,"search path %s \n",sr_path);
  fprintf(fid,"search file %s \n",sr_file);
  fprintf(fid,"search string %s \n",sr_string);
  fprintf(fid,"ignore files %s \n",ig_file);
  fprintf(fid,"ignore string %s \n",ig_string);
  fprintf(fid,"delimiters %s \n",delims);
  fprintf(fid,"date from %s \n",date_from);
  fprintf(fid,"date to %s \n",date_to);
  fprintf(fid,"\n");

  err = fclose(fid);
  if (err) {
    zmessageACK(mWin,"file I/O error %s",file);
    zfree(file);
    return;
  }

  strcpy(criteriaFile,file);                                                    //  update current file
  zfree(file);

  return;
  }

/**
 * @brief KBevent - supply unused zdialog callback function
 * @param event
 */
void KBevent(GdkEventKey *event){
  return;
}



