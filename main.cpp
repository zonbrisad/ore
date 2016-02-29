/*
 * File:   main.cpp
 * Author: pmg
 *
 * Created on den 16 oktober 2012, 18:43
 */

#include <QtCore/QCoreApplication>
#include "qextserialenumerator.h"
#include <QtCore/QList>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <argtable2.h>
#include <ncurses.h>
#include "OEditor.h"
#include "libtermkey-0.15b/termkey.h"
#include "libtermkey-0.15b/termkey-internal.h"

#define VERSION "0.1"

char *kalle = 
"#\n"
"#\n"
"# This is a small testfile\n"
"#\n"
"blaha\n"
"asdfaf  \n"
"aaaa\n"
"ddd\n";


char *test1 = 
"1234\n"
"2#\n"
"3##\n"
"4###\n"
"Next row is zero length\n"
"\n"
"10########\n"
"85########################################################################################"
"This is the last row with no trailing newline";



char *cc = 
"/**\n"
" *\n"
" * Testfile\n"
" */\n"
"\n\n"
"void function(int a) {\n"
"  char b;\n"
"  printf(\"Blaha\");\n"
"}\n"
"\n"
"void main() {\n"
"  int l;\n"
"  int a;\n"
"  for (l=0;l<100;l++) {\n"
"    a = l^2;           // a comment\n"
"  }\n"
"}\n";


WINDOW  *statusRow;
WINDOW  *messageRow;
WINDOW  *mainWin;
OEditor *a;


//#define TERMKEY(key, )

void settingsHandler() {
  QString oreDirPath(QDir::homePath().append("/.ore"));   // settings dir path
  QDir oreDir(oreDirPath);
  
  if (!oreDir.exists()) {             // create settings dir if not existant
    oreDir.mkdir(oreDirPath);
    printf("Settings dir does not exist\n");
  }
}

void initNcurses() {
  initscr();            // Start curses mode 		  
  refresh();            // Print it on to the real screen 
  //cbreak();             // turn off input buffering
  //noecho();             // turn off automatic echoing
  //keypad(stdscr, TRUE); // enables F1, F2 etc
  curs_set(2);          // make cursor visible
  //attron(COLOR_PAIR(2));newClass
  
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {	
  WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void updateStatusRow() {
  wclear(statusRow);
  wprintw(statusRow, "Col %3d  Line %3d  pos %4d  buf %d len %2d ch %c  %s\n", a->GetCurX(), a->Row(a->GetCursorPos()), a->GetCursorPos(), a->getBufPos(), a->RowLength(a->GetCursorPos()) , a->getChar(a->GetCursorPos()), a->GetFileName().toLocal8Bit().data());
  //wprintw(statusRow, "Col %3d  Line %3d  pos %4d  buf %d len %2d   %s\n", a->GetCurX(), 0, a->GetCursorPos(), a->getBufPos(), a->RowLength(a->GetCursorPos()) , a->GetFileName().toLocal8Bit().data());
  
  //wprintw(statusRow, "Col %3d  Line %3d  pos %4d  buf %d len %2d   %s\n", a->GetCurX());
  wrefresh(statusRow);
  a->updateCursor();
}

void updateMessageRow() {
  wclear(messageRow);
  //wprintw(messageRow, "Message Row");
  wprintw(messageRow, "%d", a->debugInt);
  
  wrefresh(messageRow);
  //a->updateCursor();
}

void OEditTest(OEditor *f) {
  int ch;
  int end;
  int row, col;
  TermKey       *tk;
  TermKeyKey    key;
  TermKeyResult ret;
  
  tk = termkey_new(2, TERMKEY_FLAG_SPACESYMBOL|TERMKEY_FLAG_CTRLC | TERMKEY_FLAG_NOTERMIOS );
  if (!tk) {
    printf("Could not allocate termkey data\n");
    return ;
  }
  
  printf("OEditTest\n");
  
  initNcurses();
  
  a = f;
 
  getmaxyx(stdscr, row, col);           // get the size of the textarea
 
  printf("Row %d col %d\n", row, col);
  
  // create the main view
  mainWin    = create_newwin(row-3,  col,  0,     0);
  statusRow  = create_newwin(1,      col,  row-2, 0);
  messageRow = create_newwin(1,      col,  row-1, 0);
  
  wprintw(statusRow, "Statusrow");
  wrefresh(statusRow);
  //printf("%d %d aaa\n", row,col);
  //a->Set(test1);
  a->Set(kalle);

  
  a ->SetWindow(mainWin);
  a->SetMessageWindow(messageRow);
  a->NCurPrint();
  updateStatusRow();
  //updateMessageRow();
  
  end = 1;
  while (end) {    
    ret = termkey_waitkey(tk, &key);
    
    if (key.type == TERMKEY_TYPE_KEYSYM) {
      switch (key.code.sym) {
        
        case TERMKEY_SYM_UP:        a->MoveCursor(CUR_UP);    break;
        case TERMKEY_SYM_DOWN:      a->MoveCursor(CUR_DOWN);  break;
        case TERMKEY_SYM_LEFT:      a->MoveCursor(CUR_LEFT);  break;
        case TERMKEY_SYM_RIGHT:     a->MoveCursor(CUR_RIGHT); break;
        case TERMKEY_SYM_PAGEDOWN: goto end; break;
      }
    }
    
    
    
    if (key.modifiers == TERMKEY_KEYMOD_CTRL) {     // handling ctrl keys
      if (key.type == TERMKEY_TYPE_UNICODE) {
        switch (key.utf8[0]) {
          case 'q': goto end; break;          
        }
      }
    }
    
//    ch = getch();
//
//    
//    switch (ch) {
//      case KEY_UP:        a->MoveCursor(CUR_UP);    break;
//      case KEY_DOWN:      a->MoveCursor(CUR_DOWN);  break;
//      case KEY_LEFT:      a->MoveCursor(CUR_LEFT);  break;
//      case KEY_RIGHT:     a->MoveCursor(CUR_RIGHT); break;
//      case KEY_END:       a->MoveCursor(CUR_END);   break;
//      case KEY_HOME:      a->MoveCursor(CUR_BEGIN); break;
//      case KEY_BACKSPACE: a->Delete(); break;
//      
//      default:
//        a->insert(ch);
//        break;  
//    }
    
    a->NCurPrint();
    updateStatusRow();
    //updateMessageRow();
  }
  
end:
  endwin();
  termkey_destroy(tk);
}

void termKeyTest() {
  TermKey       *tk;
  TermKeyKey    key;
  TermKeyResult ret;
  
  printf("TermkeyTest\n");
  
  tk = termkey_new(0, TERMKEY_FLAG_SPACESYMBOL|TERMKEY_FLAG_CTRLC);
  if (!tk) {
    printf("Could not allocate termkey data\n");
    return ;
  }
  while (1) {
    ret = termkey_waitkey(tk, &key);
    printf("Key received code %3d  type %3d  utf8 %3s  modifier %d\n", key.code, key.type, key.utf8, key.modifiers);
    if (key.utf8[0] == 'a')
      return;
  }
  
  termkey_destroy(tk);
  
}

int main(int argc, char *argv[]) {
  //struct arg_lit  *list     = arg_lit0("lL", NULL, "list files");
  //struct arg_lit  *recurse  = arg_lit0("R", NULL, "recurse through subdirectories");
  //struct arg_int  *repeat   = arg_int0("k", "scalar", NULL, "define scalar value k (default is 3)");
  //struct arg_str  *defines  = arg_strn("D", "define", "MACRO", 0, argc + 2, "macro definitions");
  //struct arg_file *outfile  = arg_file0("o", NULL, "<output>", "output file (default is \"-\")");
  //struct arg_lit  *verbose  = arg_lit0("v", "verbose,debug", "verbose messages");
  struct arg_lit  *help     = arg_lit0(NULL, "help", "print this help and exit");
  struct arg_lit  *version  = arg_lit0(NULL, "version", "print version information and exit");
  struct arg_file *infiles  = arg_filen(NULL, NULL, NULL, 1, argc + 2, "inputfile(s)");
  struct arg_end  *end      = arg_end(20);
  
  void* argtable[] = {help, version, infiles, end };
  
  const char* progname = "ore";
  int nerrors;
  int exitcode = 0;
  
  
  // verify the argtable[] entries were allocated sucessfully
  if (arg_nullcheck(argtable) != 0) {
    /* NULL entries were detected, some allocations must have failed */
    printf("%s: insufficient memory\n", progname);
    exitcode = 1;
    return EXIT_FAILURE;
  }
  
  // set any command line default values prior to parsing
  //repeat->ival[0] = 3;
  //outfile->filename[0] = "-";
  
  // Parse the command line as defined by argtable[]
  nerrors = arg_parse(argc, argv, argtable);
  
  /* special case: '--help' takes precedence over error reporting */
  if (help->count > 0) {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
//    printf("This program demonstrates the use of the argtable2 library\n");
//    printf("for parsing command line arguments. Argtable accepts integers\n");
//    printf("in decimal (123), hexadecimal (0xff), octal (0o123) and binary\n");
//    printf("(0b101101) formats. Suffixes KB, MB and GB are also accepted.\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    exitcode = 0;
    return EXIT_FAILURE;
  }
  
  /* special case: '--version' takes precedence error reporting */
  if (version->count > 0) {
    printf("ORE Version %s\n", VERSION);
    printf("Build %s %s\n", __DATE__, __TIME__);
    exitcode = 0;
    return EXIT_FAILURE;
  }
  
//  settingsHandler();
//  exit(0);
  
  OEditor *e;
  e = new OEditor();
  
  if (infiles->count>0) {
    printf("Fil %s\n",infiles->filename[0]);
    e->SetFile(infiles->filename[0]);
    e->OpenFile();
  }
  
  //termKeyTest();
  
  OEditTest(e);
  
  //if (infiles->count)main.cpp:46:1: warning: deprecated conversion from string constant to ‘char*’ [-Wwrite-strings]
  initNcurses();
  
  endwin();

  return EXIT_SUCCESS;
}
