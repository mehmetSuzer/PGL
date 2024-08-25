
#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include "lcd.h"

extern color_t screen[LCD_HEIGHT][LCD_WIDTH];

void plot_horizontal_line(int x0, int x1, int y, color_t color);
void plot_vertical_line(int y0, int y1, int x, color_t color);

// Draws a line where x0 < x1 and the slope is in [-1, 1]
void plot_line_low(int x0, int y0, int x1, int y1, color_t color);

// Draws a line where x0 < x1 and the slope is in [-inf, -1] U [1, inf]
void plot_line_high(int x0, int y0, int x1, int y1, color_t color);

// Draws a line and handles all cases
void plot_line(int x0, int y0, int x1, int y1, color_t color);

void plot_wired_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void plot_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);

#endif // __RASTERIZER_H__
