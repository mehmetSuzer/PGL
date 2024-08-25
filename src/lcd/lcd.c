
#include "lcd.h"

// Resets the LCD
static void lcd_reset() {
    gpio_put(LCD_RST_PIN, HIGH);
    sleep_ms(100);
    gpio_put(LCD_RST_PIN, LOW);
    sleep_ms(100);
    gpio_put(LCD_RST_PIN, HIGH);
    sleep_ms(100);
}

// Executes a command in the LCD
static void lcd_command(uint8_t reg) {
    gpio_put(LCD_DC_PIN, LOW);
    gpio_put(LCD_CS_PIN, LOW);
    spi_write_blocking(SPI_PORT, &reg, 1);
    gpio_put(LCD_CS_PIN, HIGH);
}

// Writes 8-bit data to the LCD
static void lcd_write_8bit_data(uint8_t data) {
    gpio_put(LCD_DC_PIN, HIGH);
    gpio_put(LCD_CS_PIN, LOW);
    spi_write_blocking(SPI_PORT, &data, 1);
    gpio_put(LCD_CS_PIN, HIGH);
}

// Writes 16-bit data to the LCD
static void lcd_write_16bit_data(uint16_t data) {
    gpio_put(LCD_DC_PIN, HIGH);
    gpio_put(LCD_CS_PIN, LOW);
    uint8_t data_arr[2] = {(data >> 8) & 0xFF, data & 0xFF};
    spi_write_blocking(SPI_PORT, data_arr, 2);
    gpio_put(LCD_CS_PIN, HIGH);
}

void init_lcd() {
    lcd_reset();
    
    lcd_command(0x36);
    lcd_write_8bit_data(0X70);
    
    lcd_command(0x3A);
    lcd_write_8bit_data(0x05);

    lcd_command(0xB2);
    lcd_write_8bit_data(0x0C);
    lcd_write_8bit_data(0x0C);
    lcd_write_8bit_data(0x00);
    lcd_write_8bit_data(0x33);
    lcd_write_8bit_data(0x33);

    lcd_command(0xB7);          //Gate Control
    lcd_write_8bit_data(0x35);

    lcd_command(0xBB);          //VCOM Setting
    lcd_write_8bit_data(0x19);

    lcd_command(0xC0);          //LCM Control     
    lcd_write_8bit_data(0x2C);

    lcd_command(0xC2);          //VDV and VRH Command Enable
    lcd_write_8bit_data(0x01);

    lcd_command(0xC3);          //VRH Set
    lcd_write_8bit_data(0x12);
    
    lcd_command(0xC4);          //VDV Set
    lcd_write_8bit_data(0x20);

    lcd_command(0xC6);          //Frame Rate Control in Normal Mode
    lcd_write_8bit_data(0x0F);
    
    lcd_command(0xD0);          // Power Control 1
    lcd_write_8bit_data(0xA4);
    lcd_write_8bit_data(0xA1);

    lcd_command(0xE0);          //Positive Voltage Gamma Control
    lcd_write_8bit_data(0xD0);
    lcd_write_8bit_data(0x04);
    lcd_write_8bit_data(0x0D);
    lcd_write_8bit_data(0x11);
    lcd_write_8bit_data(0x13);
    lcd_write_8bit_data(0x2B);
    lcd_write_8bit_data(0x3F);
    lcd_write_8bit_data(0x54);
    lcd_write_8bit_data(0x4C);
    lcd_write_8bit_data(0x18);
    lcd_write_8bit_data(0x0D);
    lcd_write_8bit_data(0x0B);
    lcd_write_8bit_data(0x1F);
    lcd_write_8bit_data(0x23);

    lcd_command(0xE1);           //Negative Voltage Gamma Control
    lcd_write_8bit_data(0xD0);
    lcd_write_8bit_data(0x04);
    lcd_write_8bit_data(0x0C);
    lcd_write_8bit_data(0x11);
    lcd_write_8bit_data(0x13);
    lcd_write_8bit_data(0x2C);
    lcd_write_8bit_data(0x3F);
    lcd_write_8bit_data(0x44);
    lcd_write_8bit_data(0x51);
    lcd_write_8bit_data(0x2F);
    lcd_write_8bit_data(0x1F);
    lcd_write_8bit_data(0x1F);
    lcd_write_8bit_data(0x20);
    lcd_write_8bit_data(0x23);

    lcd_command(0x21);          //Display Inversion On

    lcd_command(0x11);          //Sleep Out

    lcd_command(0x29);          //Display On
}

void lcd_set_window(uint32_t x_start, uint32_t x_end, uint32_t y_start, uint32_t y_end) {
    //set the X coordinates
    lcd_command(0x2A);
    lcd_write_8bit_data(0x00);
    lcd_write_8bit_data(x_start);
	lcd_write_8bit_data(0x00);
    lcd_write_8bit_data(x_end-1);

    //set the Y coordinates
    lcd_command(0x2B);
    lcd_write_8bit_data(0x00);
	lcd_write_8bit_data(y_start);
	lcd_write_8bit_data(0x00);
    lcd_write_8bit_data(y_end-1);

    lcd_command(0X2C);
}

void lcd_clear(color_t color) {
    const uint16_t swapped = (color << 8) | ((color & 0xFF00) >> 8);
    color_t image[LCD_WIDTH];
    for (uint32_t i = 0; i < LCD_WIDTH; i++) {
        image[i] = swapped;
    }
    
    lcd_set_window(0, LCD_WIDTH, 0, LCD_HEIGHT);
    gpio_put(LCD_DC_PIN, HIGH);
    gpio_put(LCD_CS_PIN, LOW);
    for (uint32_t i = 0; i < LCD_HEIGHT; i++) {
        spi_write_blocking(SPI_PORT, (uint8_t*)image, LCD_WIDTH << 1);
    }
    gpio_put(LCD_CS_PIN, HIGH);
}

void lcd_display(color_t* screen) {
    for (uint32_t i = 0; i < (LCD_HEIGHT*LCD_WIDTH); i++) {
        color_t color = screen[i];
        screen[i] = (color << 8) | ((color & 0xFF00) >> 8);
    }

    lcd_set_window(0, LCD_WIDTH, 0, LCD_HEIGHT);
    gpio_put(LCD_DC_PIN, HIGH);
    gpio_put(LCD_CS_PIN, LOW);
    spi_write_blocking(SPI_PORT, (uint8_t*)screen, (LCD_HEIGHT*LCD_WIDTH) << 1);
    // spi_write16_blocking(SPI_PORT, screen, LCD_HEIGHT*LCD_WIDTH);
    gpio_put(LCD_CS_PIN, HIGH);
    lcd_command(0x29);
}
