
#ifndef __LCD_H__
#define __LCD_H__

#include "color.h"
#include "config.h"

#define LCD_HEIGHT      240u
#define LCD_WIDTH       240u
#define ASPECT_RATIO    ((float)LCD_WIDTH / (float)LCD_HEIGHT)

void init_lcd();
void lcd_set_window(uint32_t x_start, uint32_t x_end, uint32_t y_start, uint32_t y_end);
void lcd_clear(color_t color);
void lcd_display(color_t* image);

#endif // __LCD_H__
