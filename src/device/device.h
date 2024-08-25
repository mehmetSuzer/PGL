
#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "hardware/vreg.h"

// Use it to set the clock frequency of Pico
#define CLOCK_FREQUENCY_KHZ 300000u

#ifdef CLOCK_FREQUENCY_KHZ
    // spi baudrate when the cpu is overclocked
    #define SPI_BAUDRATE_HZ (CLOCK_FREQUENCY_KHZ * 250)
#else
    // default spi baudrate for default cpu clock frequency 
    #define SPI_BAUDRATE_HZ 62500000u
#endif

#define LOW     false
#define HIGH    true

#define SPI_PORT spi1

#define LCD_RST_PIN    12u
#define LCD_DC_PIN      8u
#define LCD_BL_PIN     13u
#define LCD_CS_PIN      9u
#define LCD_CLK_PIN    10u
#define LCD_MOSI_PIN   11u
#define LCD_SCL_PIN     7u
#define LCD_SDA_PIN     6u

#define KEY_A          15u
#define KEY_B          17u
#define KEY_X          19u
#define KEY_Y          21u

#define KEY_FORWARD     2u
#define KEY_BACKWARD   18u
#define KEY_LEFT       16u
#define KEY_RIGHT      20u
#define KEY_CTRL        3u

#define LCD_HEIGHT    240u
#define LCD_WIDTH     240u

#define ASPECT_RATIO  ((float)LCD_WIDTH / (float)LCD_HEIGHT)

void device_init();
void lcd_display(uint16_t* image);
void set_button_irq_callback(gpio_irq_callback_t callback, uint32_t event_mask, bool enabled);

#endif // __DEVICE_H__
