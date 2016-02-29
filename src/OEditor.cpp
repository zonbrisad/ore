/** 
 * File:   OEditor.cpp
 * Author: pmg
 * 
 * Created on den 17 november 2012, 11:25
 */

#include <QByteArray>
#include <QTextStream>
#include <stdio.h>
#include <ncurses.h>
#include "OEditor.h"

OEditor::OEditor() {
  this->Clear();
  maxx      = 0;
  maxy      = 0;
  curx      = 0;
  cury      = 0;
  bufpos    = 0;
  CursorPos = 0;
  preferedXPos = 0;
  //printf("Constructor\n");
  debugFile.setFileName("debugfile.txt");
  debugFile.open(QIODevice::ReadWrite | QIODevice::Text);
  
}

OEditor::~OEditor() {
}

void OEditor::Clear() {
  buf.clear();
}

void OEditor::Set(char *str) {
  buf.clear();
  buf.append(str);
}

void OEditor::SetRows(int rows) {
  this->maxy = rows;
}

void OEditor::SetColumns(int cols) {
  this->maxx = cols;
}
  
int OEditor::Rows() {
  return buf.count('\n');
}

int OEditor::Row(int pos) {
  int row;
  int trav;
  trav = -1;
  row  = 0;
  
  while (trav<pos) {
    trav = buf.indexOf("\n", trav+1);   // find next newline
    
    if (trav>pos)                       // first row
      goto end;
    
    if (trav==-1) {                     // abort when no more newline is found
      goto end;
    }
    row++;
  }
end:  
  return row+1;
}

int OEditor::RowEnd(int pos) {
  int rowEnd;
  rowEnd = buf.indexOf('\n',pos); // find next newline
  
  if (rowEnd==-1) {               // if on last row
    rowEnd = buf.length()-1;      // return last char in buf
  }
  
  return rowEnd;
  
}


void OEditor::Buf(int c) {
  int n;
  n = CursorPos + c;
  
  if (n<0)
    return;
  
  if (n >= (buf.length()))
    return;
    
  CursorPos = n;        
}

int OEditor::RowLength(int pos) {
  int s, e;
  int sPos;
  int ePos;
  
  //return 0;
  if (pos==0)
    return 1;
  
  sPos = pos;
  
//  if (buf.at(pos)=='\n') 
//    sPos++;
  
  if (pos>0) {
    if (buf.at(pos-1)=='\n') {
      sPos--;
    }
  }
  s = buf.lastIndexOf('\n', sPos);
  e = buf.indexOf('\n', pos);
  return e-s-1;
}

void OEditor::ScrollRows(int r) {
  if (r>0) {
    bufpos = buf.indexOf('\n', bufpos) + 1;
  }
  if (r<0) {
    bufpos = buf.lastIndexOf('\n', bufpos);
    if (bufpos<0)
      bufpos = 0;
    else
      bufpos = buf.lastIndexOf('\n', bufpos-1);
  }
  if (bufpos<0) {
    bufpos = 0;
  }
}

void OEditor::CursorIndent() {
  int rl;
  return;
  rl = RowLength(CursorPos);
  if (rl < preferedXPos) {
    SetPos(CursorPos + rl);    // end of row
  } else {
    SetPos(CursorPos + preferedXPos); // prefered pos
  }
}  

void OEditor::MoveCursor(int dir) {
  QString x;
  int i;
  LastCursorPos = CursorPos;
  switch (dir) {
    case CUR_UP:
      if (CursorPos==0)       // check if at first char in buf
        break;
      
      if (cury == 0) {        // if at first row scroll up
        ScrollRows(-1);
      }
      
      i = buf.lastIndexOf('\n', CursorPos);
      i = buf.lastIndexOf('\n', i-1);
      if (i==-1)
        i = 0;
      //wprintw(win, "%d", i);
      //printf("i %d\n", i);
      SetPos(i);
      CursorIndent();
      break;
    case CUR_DOWN:
      
      if (cury >= maxy) {
        ScrollRows(1);
      }   
      i = buf.indexOf('\n', CursorPos+1);
      SetPos(i);
      CursorIndent();
      break;
    case CUR_LEFT:
      SetPos(CursorPos-1);
      preferedXPos--;
      break;
    case CUR_RIGHT:
      SetPos(CursorPos+1);
      preferedXPos++;
      break;
    case CUR_NEXT_TOKEN: break;
    case CUR_PREV_TOKEN: break;
    case CUR_END:
      if (buf.at(CursorPos)=='\n') {
        CursorPos = buf.indexOf('\n', CursorPos+1) - 1;
      } else 
        CursorPos = buf.indexOf('\n', CursorPos) - 1;
      break;
    case CUR_BEGIN: 
      CursorPos = buf.lastIndexOf('\n', CursorPos);
      
      break;
  }
  
  this->UpdateCursor();     // calculate new cursor position 
  
  this->NCurPrint();        // update screen
}

void OEditor::Print() {
  printf("%s", buf.data());
}

void OEditor::PrintInfo() {
  printf("Rows: %d\n", this->Rows());
}

void OEditor::NCurPrint() {
  int i, nl, l, lcol;
  char ch;
  wmove(win, 0, 0);
  wclear(win);
  nl = 0;
  
  //printf("Bufpos %d\n", bufpos);
  lcol = 0;
  for (i = bufpos ; i < buf.size(); i++) {
    
    // handle lines that are longer than the window is wide
    if (lcol>maxx) {
      nl++;           
      lcol = 0;
      i = this->RowEnd(i) + 1;
    }
    ch = buf.at(i);
    if (ch == '\n') {
      wprintw(win, "$");
    }
      
    wprintw(win, "%c", buf.at(i));
    lcol++;
    
    if (buf.at(i) == '\n') {
      nl++;
      lcol = 0;
    };
    
    
    if (nl >=this->maxy)
      break;
  }
  //wprintw(win,buf.data());
  //printf("%d  %d\n", curx, cury);
  
  wmove(win, cury,curx);  
  wrefresh(win);
}

void OEditor::updateCursor() {
  wmove(win, cury,curx);  
  wrefresh(win);
}


void OEditor::SetWindow(WINDOW *win) {
  int row, col;
  this->win = win;
  getmaxyx(win, row, col);           // get the size of the textarea
  maxx = col;
  maxy = row;
  //printf("Row %d  Col %d\n", row, col);
}

void OEditor::UpdateCursor() {
  int i;
  int nls;  // newlines
  int l;
  
//   find the number of newlines
  nls = 0;
  for (i=bufpos; i<=CursorPos; i++) {
    //if (i>0) 
      if (getChar(i) == '\n') {
        nls++;
      }
  }
  cury = nls;
  
  l = buf.lastIndexOf('\n', CursorPos); // search backwards
  DebugInt(l);
  if (l==-1) {
    curx = CursorPos;     // we are on first row
  } else {
    //curx = CursorPos - buf.lastIndexOf('\n', CursorPos) ;
    curx = CursorPos - l;
  }
}

int OEditor::GetCurX() {
  return curx;
}

int OEditor::GetCurY() {
  return cury;
}

int OEditor::GetCursorPos() {
  return CursorPos;
}

void OEditor::SetPos(int pos) {
  if ((pos>=0) && (pos<buf.size())) 
    CursorPos = pos;
}

void OEditor::ParenthesisMatching() {
  //if (buf.at)
}

void OEditor::Update() {
  ParenthesisMatching();
  UpdateCursor();     // calculate new cursor position
  NCurPrint();        // update screen
}

void OEditor::ParenthesisCompletion(char ch) {
  if (ch =='(')
    buf.insert(CursorPos+1, ')');
}

void OEditor::insert(char ch) {
  buf.insert(CursorPos+1, ch);
  SetPos(CursorPos+1);
  ParenthesisCompletion(ch);
  Update();
}

void OEditor::Delete() {
  buf.remove(CursorPos,1);
  SetPos(CursorPos-1);
  Update();
}

void OEditor::SetFile(const char *fileName) {
  file.setFileName(QString(fileName));
}

void OEditor::OpenFile() {
  if (file.exists()) {
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    buf = file.readAll();
    
    file.close();
    CursorPos = 0;
  }
}

QString OEditor::GetFileName() {
  return file.fileName();
}

void OEditor::SendMessage(char *msg) {
  wclear(msgWin);
  wprintw(msgWin, "%s", msg);
  wrefresh(msgWin);
}

void OEditor::SetMessageWindow(WINDOW *win) {
  msgWin = win;
}

int OEditor::getBufPos() {
  return bufpos;
}

unsigned char OEditor::getChar(unsigned int pos) {
  return buf.at(pos);
}

//unsigned char OEditor::getCharV(unsigned int pos) {
//  if (buf.at(pos) =='\n') {
//    return '&';
//  } else {
//    return buf.at(pos);
//  }
//}

void OEditor::DebugInt(int dbg) {
  QTextStream debugStream(&debugFile);
  debugStream << dbg << "\n";
  
}