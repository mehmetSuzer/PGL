
#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "pico/stdlib.h"

#define LOW  false
#define HIGH true

#define KEY_A          15u
#define KEY_B          17u
#define KEY_X          19u
#define KEY_Y          21u

#define KEY_FORWARD     2u
#define KEY_BACKWARD   18u
#define KEY_LEFT       16u
#define KEY_RIGHT      20u
#define KEY_CTRL        3u

#define SCREEN_HEIGHT 240u
#define SCREEN_WIDTH  240u

#define ASPECT_RATIO  ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)

void device_init();
void lcd_display(uint16_t* image);
void set_button_irq_callback(gpio_irq_callback_t callback, uint32_t event_mask, bool enabled);

#endif // __DEVICE_H__
