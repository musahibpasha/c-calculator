/* BGI Graphics Library - C Implementation for Windows
   Improved WinBGIm compatible library
*/

#include "graphics.h"
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdio.h>

/* Global variables */
int gd = 0, gm = 0;
HWND hWnd = NULL;
HDC hdc = NULL, hdcBuf = NULL;
HBRUSH hbrush = NULL;
HPEN hpen = NULL;
HFONT hfont = NULL;
HBITMAP hdcBufBitmap = NULL;
int screen_width = 600;
int screen_height = 700;
int currentcolor = WHITE;
int currentx = 0, currenty = 0;
int currentbgcolor = BLACK;
int currentlinewidth = 1;
int currentlinestyle = SOLID_LINE;
int currentfillpattern = EMPTY_FILL;
int currentfillcolor = WHITE;
int graphicsInitialized = 0;
int graphicsErrorCode = 0;

/* Mouse click tracking */
int mouse_x = 0;
int mouse_y = 0;
int mouse_clicked = 0;

/* Window class and messages */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc_paint = BeginPaint(hwnd, &ps);
            if (hdcBuf && hdc) {
                BitBlt(hdc_paint, 0, 0, screen_width, screen_height, hdcBuf, 0, 0, SRCCOPY);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            mouse_x = GET_X_LPARAM(lParam);
            mouse_y = GET_Y_LPARAM(lParam);
            mouse_clicked = 1;
            return 0;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

void initgraph(int *gd, int *gm, char *path) {
    const char *class_name = "BGI_Window_Class";
    WNDCLASS wc = {0};
    
    /* Register window class */
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = class_name;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.style = CS_VREDRAW | CS_HREDRAW;
    
    if (!RegisterClass(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        *gd = -1;
        graphicsErrorCode = grNoInitGraph;
        return;
    }
    
    /* Create window */
    hWnd = CreateWindowEx(
        0,
        class_name,
        "Graphics Calculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        screen_width + 16, screen_height + 39,
        NULL, NULL,
        GetModuleHandle(NULL),
        NULL
    );
    
    if (!hWnd) {
        *gd = -1;
        graphicsErrorCode = grNoInitGraph;
        return;
    }
    
    /* Get device context */
    hdc = GetDC(hWnd);
    if (!hdc) {
        *gd = -1;
        graphicsErrorCode = grNoInitGraph;
        return;
    }
    
    /* Create compatible DC and bitmap for double buffering */
    hdcBuf = CreateCompatibleDC(hdc);
    hdcBufBitmap = CreateCompatibleBitmap(hdc, screen_width, screen_height);
    SelectObject(hdcBuf, hdcBufBitmap);
    
    /* Fill with background color */
    HBRUSH bg_brush = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = {0, 0, screen_width, screen_height};
    FillRect(hdcBuf, &rect, bg_brush);
    DeleteObject(bg_brush);
    
    /* Show window */
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    
    graphicsInitialized = 1;
    graphicsErrorCode = grOk;
    *gd = 0;
    *gm = 0;
}

void closegraph(void) {
    if (hdcBufBitmap) {
        DeleteObject(hdcBufBitmap);
        hdcBufBitmap = NULL;
    }
    if (hdcBuf) {
        DeleteDC(hdcBuf);
        hdcBuf = NULL;
    }
    if (hdc && hWnd) {
        ReleaseDC(hWnd, hdc);
        hdc = NULL;
    }
    if (hWnd) {
        DestroyWindow(hWnd);
        hWnd = NULL;
    }
}

void setcolor(int color) {
    currentcolor = color;
    if (hpen) DeleteObject(hpen);
    
    int r = 0, g = 0, b = 0;
    switch (color) {
        case BLACK: r = 0; g = 0; b = 0; break;
        case BLUE: r = 0; g = 0; b = 255; break;
        case GREEN: r = 0; g = 128; b = 0; break;
        case CYAN: r = 0; g = 255; b = 255; break;
        case RED: r = 255; g = 0; b = 0; break;
        case MAGENTA: r = 255; g = 0; b = 255; break;
        case BROWN: r = 165; g = 82; b = 0; break;
        case LIGHTGRAY: r = 192; g = 192; b = 192; break;
        case DARKGRAY: r = 64; g = 64; b = 64; break;
        case LIGHTBLUE: r = 173; g = 216; b = 230; break;
        case LIGHTGREEN: r = 144; g = 238; b = 144; break;
        case LIGHTCYAN: r = 224; g = 255; b = 255; break;
        case LIGHTRED: r = 255; g = 128; b = 128; break;
        case LIGHTMAGENTA: r = 255; g = 192; b = 203; break;
        case YELLOW: r = 255; g = 255; b = 0; break;
        case WHITE: r = 255; g = 255; b = 255; break;
    }
    
    hpen = CreatePen(PS_SOLID, currentlinewidth, RGB(r, g, b));
    SelectObject(hdcBuf, hpen);
}

int getcolor(void) {
    return currentcolor;
}

void setfillstyle(int pattern, int color) {
    currentfillpattern = pattern;
    currentfillcolor = color;
    
    if (hbrush) DeleteObject(hbrush);
    
    int r = 0, g = 0, b = 0;
    switch (color) {
        case BLACK: r = 0; g = 0; b = 0; break;
        case BLUE: r = 0; g = 0; b = 255; break;
        case GREEN: r = 0; g = 128; b = 0; break;
        case CYAN: r = 0; g = 255; b = 255; break;
        case RED: r = 255; g = 0; b = 0; break;
        case MAGENTA: r = 255; g = 0; b = 255; break;
        case BROWN: r = 165; g = 82; b = 0; break;
        case LIGHTGRAY: r = 192; g = 192; b = 192; break;
        case DARKGRAY: r = 64; g = 64; b = 64; break;
        case LIGHTBLUE: r = 173; g = 216; b = 230; break;
        case LIGHTGREEN: r = 144; g = 238; b = 144; break;
        case LIGHTCYAN: r = 224; g = 255; b = 255; break;
        case LIGHTRED: r = 255; g = 128; b = 128; break;
        case LIGHTMAGENTA: r = 255; g = 192; b = 203; break;
        case YELLOW: r = 255; g = 255; b = 0; break;
        case WHITE: r = 255; g = 255; b = 255; break;
    }
    
    if (pattern == SOLID_FILL) {
        hbrush = CreateSolidBrush(RGB(r, g, b));
    } else {
        hbrush = CreateSolidBrush(RGB(r, g, b));
    }
    SelectObject(hdcBuf, hbrush);
}

void bar(int left, int top, int right, int bottom) {
    if (!hdcBuf) return;
    RECT rect = {left, top, right, bottom};
    FillRect(hdcBuf, &rect, hbrush);
}

void rectangle(int left, int top, int right, int bottom) {
    if (!hdcBuf) return;
    MoveToEx(hdcBuf, left, top, NULL);
    LineTo(hdcBuf, right, top);
    LineTo(hdcBuf, right, bottom);
    LineTo(hdcBuf, left, bottom);
    LineTo(hdcBuf, left, top);
}

void circle(int x, int y, int radius) {
    if (!hdcBuf) return;
    Ellipse(hdcBuf, x - radius, y - radius, x + radius, y + radius);
}

void line(int x1, int y1, int x2, int y2) {
    if (!hdcBuf) return;
    MoveToEx(hdcBuf, x1, y1, NULL);
    LineTo(hdcBuf, x2, y2);
}

void lineto(int x, int y) {
    if (!hdcBuf) return;
    LineTo(hdcBuf, x, y);
}

void moveto(int x, int y) {
    if (!hdcBuf) return;
    MoveToEx(hdcBuf, x, y, NULL);
    currentx = x;
    currenty = y;
}

void putpixel(int x, int y, int color) {
    if (!hdcBuf) return;
    setcolor(color);
    SetPixel(hdcBuf, x, y, RGB(255, 255, 255));
}

int getpixel(int x, int y) {
    if (!hdcBuf) return 0;
    COLORREF col = GetPixel(hdcBuf, x, y);
    return (int)col;
}

void outtextxy(int x, int y, char *textstring) {
    if (!hdcBuf || !textstring) return;
    
    SetTextColor(hdcBuf, RGB(255, 255, 255));
    SetBkMode(hdcBuf, TRANSPARENT);
    
    TextOutA(hdcBuf, x, y, textstring, strlen(textstring));
}

void outtext(char *textstring) {
    if (!hdcBuf || !textstring) return;
    
    SetTextColor(hdcBuf, RGB(255, 255, 255));
    SetBkMode(hdcBuf, TRANSPARENT);
    
    SIZE size;
    GetTextExtentPoint32A(hdcBuf, textstring, strlen(textstring), &size);
    
    TextOutA(hdcBuf, currentx, currenty, textstring, strlen(textstring));
    currentx += size.cx;
}

void settextstyle(int font, int direction, int charsize) {
    if (hfont) DeleteObject(hfont);
    
    int height = charsize * 8;
    hfont = CreateFontA(height, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    SelectObject(hdcBuf, hfont);
}

int textwidth(char *textstring) {
    if (!hdcBuf || !textstring) return 0;
    SIZE size;
    GetTextExtentPoint32A(hdcBuf, textstring, strlen(textstring), &size);
    return size.cx;
}

int textheight(char *textstring) {
    if (!hdcBuf || !textstring) return 0;
    SIZE size;
    GetTextExtentPoint32A(hdcBuf, textstring, strlen(textstring), &size);
    return size.cy;
}

void cleardevice(void) {
    if (!hdcBuf) return;
    RECT rect = {0, 0, screen_width, screen_height};
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdcBuf, &rect, brush);
    DeleteObject(brush);
}

int getmaxx(void) {
    return screen_width - 1;
}

int getmaxy(void) {
    return screen_height - 1;
}

void delay(unsigned milliseconds) {
    Sleep(milliseconds);
}

void setbkcolor(int color) {
    currentbgcolor = color;
}

int getbkcolor(void) {
    return currentbgcolor;
}

void floodfill(int x, int y, int border) {
    /* Simplified implementation */
}

void fillellipse(int x, int y, int xradius, int yradius) {
    if (!hdcBuf) return;
    Ellipse(hdcBuf, x - xradius, y - yradius, x + xradius, y + yradius);
}

void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius) {
    if (!hdcBuf) return;
    Arc(hdcBuf, x - xradius, y - yradius, x + xradius, y + yradius,
        x + xradius, y, x + xradius, y);
}

void arc(int x, int y, int stangle, int endangle, int radius) {
    if (!hdcBuf) return;
    Arc(hdcBuf, x - radius, y - radius, x + radius, y + radius,
        x + radius, y, x + radius, y);
}

void drawpoly(int numpoints, int *polypoints) {
    if (!hdcBuf) return;
    Polyline(hdcBuf, (POINT *)polypoints, numpoints);
}

void fillpoly(int numpoints, int *polypoints) {
    if (!hdcBuf) return;
    Polygon(hdcBuf, (POINT *)polypoints, numpoints);
}

void setlinewidth(int width) {
    currentlinewidth = width;
    if (hpen) DeleteObject(hpen);
    hpen = CreatePen(PS_SOLID, width, RGB(255, 255, 255));
    SelectObject(hdcBuf, hpen);
}

int graphresult(void) {
    return graphicsErrorCode;
}

void swapbuffers(void) {
    if (!hWnd || !hdc || !hdcBuf) return;
    
    BitBlt(hdc, 0, 0, screen_width, screen_height, hdcBuf, 0, 0, SRCCOPY);
    
    /* Process Windows messages to allow interaction */
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
