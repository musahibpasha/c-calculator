#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <windowsx.h>

/* External graphics window handle from libbgi.c */
extern HWND hWnd;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 700
#define BUTTON_WIDTH 70
#define BUTTON_HEIGHT 60
#define DISPLAY_HEIGHT 80

typedef struct {
    int x, y, width, height;
    char label[10];
    int color;
} Button;

char display[100] = "0";
char input[100] = "";
double result = 0;
int new_number = 1;
char operation = '\0';
int decimal_flag = 0;

Button buttons[20];
int button_count = 0;

void addButton(int x, int y, const char *label, int color) {
    buttons[button_count].x = x;
    buttons[button_count].y = y;
    buttons[button_count].width = BUTTON_WIDTH;
    buttons[button_count].height = BUTTON_HEIGHT;
    strcpy(buttons[button_count].label, label);
    buttons[button_count].color = color;
    button_count++;
}

void drawButton(Button *btn) {
    setfillstyle(SOLID_FILL, btn->color);
    bar(btn->x, btn->y, btn->x + btn->width, btn->y + btn->height);
    setcolor(BLACK);
    rectangle(btn->x, btn->y, btn->x + btn->width, btn->y + btn->height);
    
    int text_x = btn->x + btn->width / 2 - 10;
    int text_y = btn->y + btn->height / 2 - 5;
    moveto(text_x, text_y);
    outtext(btn->label);
}

void drawDisplay() {
    setfillstyle(SOLID_FILL, BLACK);
    bar(20, 20, WINDOW_WIDTH - 20, 20 + DISPLAY_HEIGHT);
    setcolor(YELLOW);
    rectangle(20, 20, WINDOW_WIDTH - 20, 20 + DISPLAY_HEIGHT);
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    int text_x = 40;
    int text_y = 45;
    moveto(text_x, text_y);
    outtext(display);
}

void initializeButtons() {
    button_count = 0;
    
    int start_y = 130;
    int start_x = 20;
    
    // Create buttons layout
    // Row 1: 7, 8, 9, /
    addButton(start_x, start_y, "7", LIGHTBLUE);
    addButton(start_x + 100, start_y, "8", LIGHTBLUE);
    addButton(start_x + 200, start_y, "9", LIGHTBLUE);
    addButton(start_x + 300, start_y, "/", LIGHTRED);
    
    // Row 2: 4, 5, 6, *
    start_y += 80;
    addButton(start_x, start_y, "4", LIGHTBLUE);
    addButton(start_x + 100, start_y, "5", LIGHTBLUE);
    addButton(start_x + 200, start_y, "6", LIGHTBLUE);
    addButton(start_x + 300, start_y, "*", LIGHTRED);
    
    // Row 3: 1, 2, 3, -
    start_y += 80;
    addButton(start_x, start_y, "1", LIGHTBLUE);
    addButton(start_x + 100, start_y, "2", LIGHTBLUE);
    addButton(start_x + 200, start_y, "3", LIGHTBLUE);
    addButton(start_x + 300, start_y, "-", LIGHTRED);
    
    // Row 4: 0, ., =, +
    start_y += 80;
    addButton(start_x, start_y, "0", LIGHTBLUE);
    addButton(start_x + 100, start_y, ".", LIGHTBLUE);
    addButton(start_x + 200, start_y, "=", LIGHTGREEN);
    addButton(start_x + 300, start_y, "+", LIGHTRED);
    
    // Row 5: Clear
    start_y += 80;
    addButton(start_x, start_y, "C", LIGHTRED);
    addButton(start_x + 100, start_y, "Del", YELLOW);
}

void drawCalculator() {
    cleardevice();
    setbkcolor(DARKGRAY);
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    moveto(150, 130);
    outtext("CALCULATOR");
    
    drawDisplay();
    
    // Draw all buttons
    for (int i = 0; i < button_count; i++) {
        drawButton(&buttons[i]);
    }
}

int isButtonClicked(Button *btn, int x, int y) {
    return (x >= btn->x && x <= btn->x + btn->width &&
            y >= btn->y && y <= btn->y + btn->height);
}

void updateDisplay() {
    strcpy(display, input);
    if (strlen(display) == 0) {
        strcpy(display, "0");
    }
    drawDisplay();
}

void performCalculation(char op) {
    if (strlen(input) == 0) return;
    
    double num = atof(input);
    
    if (operation != '\0') {
        switch (operation) {
            case '+':
                result = result + num;
                break;
            case '-':
                result = result - num;
                break;
            case '*':
                result = result * num;
                break;
            case '/':
                if (num != 0) {
                    result = result / num;
                } else {
                    strcpy(display, "Error");
                    drawDisplay();
                    strcpy(input, "");
                    return;
                }
                break;
        }
    } else {
        result = num;
    }
    
    operation = op;
    strcpy(input, "");
    decimal_flag = 0;
}

void handleButtonClick(const char *label) {
    if (strcmp(label, "C") == 0) {
        /* Clear all */
        strcpy(input, "");
        strcpy(display, "0");
        result = 0;
        operation = '\0';
        decimal_flag = 0;
        new_number = 1;
    } else if (strcmp(label, "Del") == 0) {
        /* Delete last character from input */
        if (strlen(input) > 0) {
            input[strlen(input) - 1] = '\0';
            if (strlen(input) == 0) {
                strcpy(display, "0");
            } else {
                strcpy(display, input);
                if (input[strlen(input) - 1] == '.') {
                    decimal_flag = 0;
                }
            }
        }
    } else if (strcmp(label, "=") == 0) {
        /* Execute calculation */
        if (operation != '\0' && strlen(input) > 0) {
            performCalculation('\0');
            /* Display the result */
            sprintf(display, "%.2f", result);
            int len = strlen(display);
            while (len > 0 && display[len - 1] == '0') {
                display[len - 1] = '\0';
                len--;
            }
            if (len > 0 && display[len - 1] == '.') {
                display[len - 1] = '\0';
            }
            strcpy(input, "");
            operation = '\0';
            new_number = 1;
            decimal_flag = 0;
        }
    } else if (strchr("+-*/", label[0]) && label[1] == '\0') {
        /* Operation button pressed */
        if (strlen(input) > 0) {
            /* We have a new number, perform any pending calculation */
            performCalculation(label[0]);
            /* Display current result */
            sprintf(display, "%.2f", result);
            int len = strlen(display);
            while (len > 0 && display[len - 1] == '0') {
                display[len - 1] = '\0';
                len--;
            }
            if (len > 0 && display[len - 1] == '.') {
                display[len - 1] = '\0';
            }
            new_number = 1;
        } else if (operation == '\0' && result != 0) {
            /* Just change the operation */
            operation = label[0];
        }
    } else if (strcmp(label, ".") == 0) {
        /* Decimal point */
        if (new_number) {
            strcpy(input, "0.");
            decimal_flag = 1;
            new_number = 0;
        } else if (decimal_flag == 0) {
            strcat(input, ".");
            decimal_flag = 1;
        }
        strcpy(display, input);
    } else if (isdigit(label[0])) {
        /* Number button pressed */
        if (new_number) {
            strcpy(input, label);
            new_number = 0;
            decimal_flag = 0;
        } else {
            if (strlen(input) < 20) {
                strcat(input, label);
            }
        }
        strcpy(display, input);
    }
    
    drawDisplay();
}

int main() {
    int gd = DETECT, gm;
    
    initgraph(&gd, &gm, "");
    
    if (graphresult() != grOk) {
        printf("Graphics initialization failed!\n");
        printf("Make sure graphics.h is properly installed.\n");
        exit(1);
    }
    
    initializeButtons();
    drawCalculator();
    swapbuffers();
    
    int running = 1;
    while (running) {
        /* Process window messages */
        MSG msg;
        while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        /* Check for mouse clicks from window procedure */
        if (mouse_clicked) {
            for (int i = 0; i < button_count; i++) {
                if (isButtonClicked(&buttons[i], mouse_x, mouse_y)) {
                    handleButtonClick(buttons[i].label);
                    drawCalculator();
                    swapbuffers();
                    Sleep(100);
                    break;
                }
            }
            mouse_clicked = 0;
        }
        
        Sleep(10);  /* Prevent CPU spinning */
    }
    
    closegraph();
    return 0;
}
