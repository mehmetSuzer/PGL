
#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include "device.h"
#include "pglm.h"

extern uint16_t screen[LCD_HEIGHT][LCD_WIDTH];

void clear_screen(uint16_t color);
void plot_wired_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
void plot_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);

#endif // __RASTERIZER_H__
