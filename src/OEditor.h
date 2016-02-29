/* 
 * File:   OEditor.h
 * Author: pmg
 *
 * Created on den 17 november 2012, 11:25
 */

#ifndef OEDITOR_H
#define	OEDITOR_H

#include <ncurses.h>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QVector>

#define CUR_UP          0
#define CUR_DOWN        1
#define CUR_LEFT        2
#define CUR_RIGHT       3
#define CUR_NEXT_TOKEN  4
#define CUR_PREV_TOKEN  5
#define CUR_BEGIN       6
#define CUR_END         7

class Token {
public:
private:
  QString token;
  int     position;
  int     type;
};


class OEditor {
public:
  OEditor();
  virtual ~OEditor();
  
  /**
   * Clear buffer.
   */
  void Clear();
  
  void Set(char *str);
  
  unsigned char getChar(unsigned int pos);
  
  void SetRows(int rows);
  
  void SetColumns(int cols);
  
  /**
   * Set the dimensions of the output window in characters.
   * 
   * @param rows
   * @param cols
   */
  void SetWindowDimensions(int rows , int cols);
  
  void SetWindow(WINDOW *win);
  
  void SetMessageWindow(WINDOW *win);
  
  void ScrollRows(int r);
  
  void Print();
  
  void PrintInfo();
  
  void ParenthesisCompletion(char ch);
  
  void ParenthesisMatching();
  
  void ColorEncode();
  
  /**
   * The number of rows that the buffer contains
   * @return rows
   */
  int Rows();             
  
  /**
   * Witch row is a certain position in the buffer on.
   * 
   * @param pos
   * @return 
   */
  int Row(int pos);
  
  /**
   * The length of a row that pos is on.
   * 
   * @param pos
   * @return 
   */
  int RowLength(int pos);

  /**
   * The start of the row that pos in on 
   * @param pos
   * @return 
   */
  int RowStart(int pos);
  
  /**
   * The end of the row that pos is on.
   * 
   * @param pos
   * @return 
   */
  int RowEnd(int pos);

  /**
   * Print buffer to ncurses window
   */
  void NCurPrint();       

  void MoveCursor(int dir);
  
  void Buf(int c);
  
  void UpdateCursor();
  
  /**
   * Write file to disk.
   */
  void SaveFile();
  
  /**
   * Set file to open.
   * 
   * @param fileName
   */
  void SetFile(const char *fileName);
  
  /**
   * Open file and read into buffer.
   */
  void OpenFile();
  
  /**
   * Close the file.
   */
  void CloseFile();
  
  QString GetFileName();
  
  /**
   * Return the number of rows in buffer.
   * @return nr of rowsSetPo
   */
  int nrOfRows();
  
  /**
   * Returns nr of characters in buffer.
   * @return 
   */
  int nrOfChars();
  
  int GetCurX();
  
  int GetCurY();
  
  int GetCursorPos();
  
  void SetPos(int pos);
  
  //int RowLength(int pos);
  
  void insert(char ch);
  
  void Update();
  
  void Delete();
  
  void updateCursor();
  
  int getBufPos();
  
  void DebugInt(int dbg);
  
  void CursorIndent();
  
  
  
  QFile debugFile;
  //QTextStream debugStream();
  int debugInt;
  
private:
  
  WINDOW     *win;        // window to print output to
  WINDOW     *statusRow;  // statusrow
  WINDOW     *msgWin;     // window for messages
  QByteArray buf;         // main buffer holding data
  int        CursorPos;   // current position of 
  int        LastCursorPos;
  QString    bufName;
  QFile      file;
  int        curx;        // x position of cursor
  int        cury;        // y position of cursor
  int        maxx;        // maximal x position
  int        maxy;        // maximal y   position
  int        bufpos;      // buffer possition
  int        blockStart;  // start of block
  int        blockEnd;    // end of block
  
  int        preferedXPos; 
  
//  QTime      time;
  QString    separators;  // string with separators for tokenizer
  QVector<QString> reservedWords;

  int        FileType;
  //  QVector<uint32_t> metadata;

  void SendMessage(char *msg);

};

#endif	/* OEDITOR_H */

