
#ifndef __LCD_H__
#define __LCD_H__

#include "color.h"
#include "config.h"

#define LCD_HEIGHT  240u
#define LCD_WIDTH   240u

void init_lcd();
void lcd_set_window(uint x_start, uint x_end, uint y_start, uint y_end);
void lcd_clear(color_t color);
void lcd_display(color_t* image);

#endif // __LCD_H__
