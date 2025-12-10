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
#define WINDOW_HEIGHT 750
#define BUTTON_WIDTH 85
#define BUTTON_HEIGHT 65
#define DISPLAY_HEIGHT 100

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
char preview[100] = ""; /* shows ongoing operation e.g. "7 + 7" or "7 + 7 = 14" */
int preview_locked = 0; /* when set, don't recompute preview (used after '=' to show result) */

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
    /* Draw button shadow for depth */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(btn->x + 2, btn->y + 2, btn->x + btn->width + 2, btn->y + btn->height + 2);
    
    /* Draw main button with darker background */
    setfillstyle(SOLID_FILL, btn->color);
    bar(btn->x, btn->y, btn->x + btn->width, btn->y + btn->height);
    
    /* Draw button border */
    setcolor(BLACK);
    rectangle(btn->x, btn->y, btn->x + btn->width, btn->y + btn->height);
    rectangle(btn->x + 1, btn->y + 1, btn->x + btn->width - 1, btn->y + btn->height - 1);
    
    /* Draw button text - larger and darker for better visibility */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    setcolor(DARKGRAY);
    int text_x = btn->x + btn->width / 2 - 20;
    int text_y = btn->y + btn->height / 2 - 12;
    moveto(text_x, text_y);
    outtext(btn->label);
}

void drawDisplay() {
    /* Draw display background - dark gray for better contrast */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(20, 60, WINDOW_WIDTH - 20, 60 + DISPLAY_HEIGHT);
    
    /* Draw attractive border with darker green */
    setcolor(LIGHTGREEN);
    rectangle(20, 60, WINDOW_WIDTH - 20, 60 + DISPLAY_HEIGHT);
    
    /* Draw inner border for depth */
    setcolor(LIGHTCYAN);
    rectangle(25, 65, WINDOW_WIDTH - 25, 55 + DISPLAY_HEIGHT);

    /* Update preview unless explicitly locked (e.g., after '=' to show final result) */
    if (!preview_locked) {
        updatePreview();
    }

    /* Draw preview text (smaller) above the main display text */
    if (strlen(preview) > 0) {
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(LIGHTCYAN);
        int preview_x = 40;
        int preview_y = 72; /* slightly above main text */
        moveto(preview_x, preview_y);
        outtext(preview);
    }
    
    /* Draw large, dark text for maximum visibility */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    setcolor(BLACK);
    int text_x = 40;
    int text_y = 85;
    moveto(text_x, text_y);
    outtext(display);
}

void initializeButtons() {
    button_count = 0;
    
    int start_y = 180;
    int start_x = 20;
    int spacing = 8;
    
    // Create buttons layout with better spacing
    // Row 1: 7, 8, 9, /
    addButton(start_x, start_y, "7", LIGHTBLUE);
    addButton(start_x + BUTTON_WIDTH + spacing, start_y, "8", LIGHTBLUE);
    addButton(start_x + 2 * (BUTTON_WIDTH + spacing), start_y, "9", LIGHTBLUE);
    addButton(start_x + 3 * (BUTTON_WIDTH + spacing), start_y, "/", LIGHTBLUE);
    
    // Row 2: 4, 5, 6, *
    start_y += BUTTON_HEIGHT + spacing;
    addButton(start_x, start_y, "4", LIGHTBLUE);
    addButton(start_x + BUTTON_WIDTH + spacing, start_y, "5", LIGHTBLUE);
    addButton(start_x + 2 * (BUTTON_WIDTH + spacing), start_y, "6", LIGHTBLUE);
    addButton(start_x + 3 * (BUTTON_WIDTH + spacing), start_y, "*", LIGHTBLUE);
    
    // Row 3: 1, 2, 3, -
    start_y += BUTTON_HEIGHT + spacing;
    addButton(start_x, start_y, "1", LIGHTBLUE);
    addButton(start_x + BUTTON_WIDTH + spacing, start_y, "2", LIGHTBLUE);
    addButton(start_x + 2 * (BUTTON_WIDTH + spacing), start_y, "3", LIGHTBLUE);
    addButton(start_x + 3 * (BUTTON_WIDTH + spacing), start_y, "-", LIGHTBLUE);
    
    // Row 4: 0, ., =, +
    start_y += BUTTON_HEIGHT + spacing;
    addButton(start_x, start_y, "0", LIGHTCYAN);
    addButton(start_x + BUTTON_WIDTH + spacing, start_y, ".", LIGHTCYAN);
    addButton(start_x + 2 * (BUTTON_WIDTH + spacing), start_y, "=", LIGHTGREEN);
    addButton(start_x + 3 * (BUTTON_WIDTH + spacing), start_y, "+", LIGHTMAGENTA);
    
    // Row 5: Clear and Delete
    start_y += BUTTON_HEIGHT + spacing;
    addButton(start_x, start_y, "C", LIGHTRED);
    addButton(start_x + BUTTON_WIDTH + spacing, start_y, "Del", YELLOW);
}

void drawCalculator() {
    cleardevice();
    /* Use dark blue background for better contrast */
    setbkcolor(BLUE);
    
    /* Draw title with attractive style */
    setcolor(LIGHTGREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    moveto(110, 15);
    outtext("CALCULATOR");
    
    drawDisplay();
    
    /* Draw all buttons */
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
    preview_locked = 0; /* any manual display update should unlock preview */
    drawDisplay();
}

/* Build a preview string describing the current operation/inputs */
void updatePreview() {
    if (operation != '\0') {
        if (strlen(input) > 0) {
            snprintf(preview, sizeof(preview), "%g %c %s", result, operation, input);
        } else {
            snprintf(preview, sizeof(preview), "%g %c", result, operation);
        }
    } else {
        if (strlen(input) > 0) {
            snprintf(preview, sizeof(preview), "%s", input);
        } else {
            preview[0] = '\0';
        }
    }
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
        preview[0] = '\0';
        preview_locked = 0;
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
            /* capture operands for preview before calculation changes state */
            double left_val = result;
            char op_char = operation;
            double right_val = atof(input);

            performCalculation('\0');

            /* show a locked preview like "7 + 7 = 14" until next input */
            snprintf(preview, sizeof(preview), "%g %c %g = %g", left_val, op_char, right_val, result);
            preview_locked = 1;
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
            preview_locked = 0; /* allow preview to update showing "result op" */
        } else if (operation == '\0' && result != 0) {
            /* Just change the operation */
            operation = label[0];
            preview_locked = 0;
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
