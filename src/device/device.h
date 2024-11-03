
#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "pico/stdlib.h"

#define DEVICE_LOW  false
#define DEVICE_HIGH true

#define DEVICE_KEY_A          15u
#define DEVICE_KEY_B          17u
#define DEVICE_KEY_X          19u
#define DEVICE_KEY_Y          21u

#define DEVICE_KEY_FORWARD     2u
#define DEVICE_KEY_BACKWARD   18u
#define DEVICE_KEY_LEFT       16u
#define DEVICE_KEY_RIGHT      20u
#define DEVICE_KEY_CTRL        3u

#define DEVICE_SCREEN_HEIGHT 240u
#define DEVICE_SCREEN_WIDTH  240u

void device_init();
void device_display(uint16_t* image);
void device_set_button_irq_callback(gpio_irq_callback_t callback, uint32_t event_mask, bool enabled);

#endif // __DEVICE_H__
