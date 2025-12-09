/* BGI Graphics Header for Windows (WinBGIm) - C Compatible
   Minimal implementation for graphics calculator
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Graphics result codes */
#define grOk                0
#define grNoInitGraph       -1
#define grNotDetected       -2
#define grFileNotFound      -3
#define grInvalidDriver     -4
#define grNoLoadMem         -5
#define grNoScanMem         -6
#define grInvalidMode       -7
#define grError             -8
#define grIOerror           -9
#define grInvalidFont       -10
#define grInvalidFontNum    -11
#define grInvalidDeviceNum  -12
#define grInvalidVersion    -13

/* Graphics modes */
#define DETECT 0

/* Colors */
#define BLACK       0
#define BLUE        1
#define GREEN       2
#define CYAN        3
#define RED         4
#define MAGENTA     5
#define BROWN       6
#define LIGHTGRAY   7
#define DARKGRAY    8
#define LIGHTBLUE   9
#define LIGHTGREEN  10
#define LIGHTCYAN   11
#define LIGHTRED    12
#define LIGHTMAGENTA 13
#define YELLOW      14
#define WHITE       15

/* Fill patterns */
#define EMPTY_FILL      0
#define SOLID_FILL      1
#define LINE_FILL       2
#define LTSLASH_FILL    3
#define SLASH_FILL      4
#define BKSLASH_FILL    5
#define LTBKSLASH_FILL  6
#define HATCH_FILL      7
#define XHATCH_FILL     8
#define INTERLEAVE_FILL 9
#define WIDE_DOT_FILL   10
#define CLOSE_DOT_FILL  11
#define USER_FILL       12

/* Line styles */
#define SOLID_LINE   0
#define DOTTED_LINE  1
#define CENTER_LINE  2
#define DASHED_LINE  3
#define USERBIT_LINE 4

/* Text directions */
#define HORIZ_DIR 0
#define VERT_DIR  1

/* Text justification */
#define LEFT_TEXT   0
#define CENTER_TEXT 1
#define RIGHT_TEXT  2
#define BOTTOM_TEXT 0
#define VCENTER_TEXT 1
#define TOP_TEXT    2

/* Fonts */
#define DEFAULT_FONT    0
#define TRIPLEX_FONT    1
#define SMALL_FONT      2
#define SANS_SERIF_FONT 3
#define GOTHIC_FONT     4

/* Global graphics state */
extern int gd, gm;
extern HWND hWnd;
extern HDC hdc, hdcBuf;
extern HBRUSH hbrush;
extern HPEN hpen;
extern HFONT hfont;
extern int currentcolor;
extern int currentx, currenty;

/* Mouse input */
extern int mouse_x;
extern int mouse_y;
extern int mouse_clicked;

/* Function declarations */
void initgraph(int *gd, int *gm, char *path);
void closegraph(void);
void setcolor(int color);
int getcolor(void);
void setfillstyle(int pattern, int color);
void bar(int left, int top, int right, int bottom);
void rectangle(int left, int top, int right, int bottom);
void circle(int x, int y, int radius);
void line(int x1, int y1, int x2, int y2);
void lineto(int x, int y);
void moveto(int x, int y);
void putpixel(int x, int y, int color);
int getpixel(int x, int y);
void outtextxy(int x, int y, char *textstring);
void outtext(char *textstring);
void settextstyle(int font, int direction, int charsize);
int textwidth(char *textstring);
int textheight(char *textstring);
void cleardevice(void);
int getmaxx(void);
int getmaxy(void);
void delay(unsigned milliseconds);
void setbkcolor(int color);
int getbkcolor(void);
void floodfill(int x, int y, int border);
void fillellipse(int x, int y, int xradius, int yradius);
void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);
void arc(int x, int y, int stangle, int endangle, int radius);
void drawpoly(int numpoints, int *polypoints);
void fillpoly(int numpoints, int *polypoints);
void setlinewidth(int width);
int graphresult(void);
void swapbuffers(void);

#ifdef __cplusplus
}
#endif

#endif /* GRAPHICS_H */
