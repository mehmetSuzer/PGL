
#include "device.h"

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

static void lcd_reset() {
    gpio_put(LCD_RST_PIN, HIGH);
    sleep_ms(100);
    gpio_put(LCD_RST_PIN, LOW);
    sleep_ms(100);
    gpio_put(LCD_RST_PIN, HIGH);
    sleep_ms(100);
}

static void lcd_command(uint8_t reg) {
    gpio_put(LCD_DC_PIN, LOW);
    gpio_put(LCD_CS_PIN, LOW);
    spi_write_blocking(SPI_PORT, &reg, 1);
    gpio_put(LCD_CS_PIN, HIGH);
}

static void lcd_write_8bit_data(uint8_t data) {
    gpio_put(LCD_DC_PIN, HIGH);
    gpio_put(LCD_CS_PIN, LOW);
    spi_write_blocking(SPI_PORT, &data, 1);
    gpio_put(LCD_CS_PIN, HIGH);
}

static void lcd_init() {
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

static void gpio_set(uint pin, bool mode) {
    gpio_init(pin);
    gpio_set_dir(pin, mode);
}

static void lcd_set_pins() {
    gpio_set(LCD_RST_PIN, GPIO_OUT);
    gpio_set(LCD_DC_PIN, GPIO_OUT);
    gpio_set(LCD_CS_PIN, GPIO_OUT);
    gpio_set(LCD_BL_PIN, GPIO_OUT);
    gpio_set(LCD_CS_PIN, GPIO_OUT);
    gpio_set(LCD_BL_PIN, GPIO_OUT);

    gpio_put(LCD_CS_PIN, HIGH);
    gpio_put(LCD_DC_PIN, LOW);
    gpio_put(LCD_BL_PIN, HIGH);
}

static void buttons_init() {
    gpio_set(KEY_A, GPIO_IN);
    gpio_pull_up(KEY_A);
    gpio_set(KEY_B, GPIO_IN);
    gpio_pull_up(KEY_B);
    gpio_set(KEY_X, GPIO_IN);
    gpio_pull_up(KEY_X);
    gpio_set(KEY_Y, GPIO_IN);
    gpio_pull_up(KEY_Y);

    gpio_set(KEY_FORWARD, GPIO_IN);
    gpio_pull_up(KEY_FORWARD);
    gpio_set(KEY_BACKWARD, GPIO_IN);
    gpio_pull_up(KEY_BACKWARD);
    gpio_set(KEY_LEFT, GPIO_IN);
    gpio_pull_up(KEY_LEFT);
    gpio_set(KEY_RIGHT, GPIO_IN);
    gpio_pull_up(KEY_RIGHT);
    gpio_set(KEY_CTRL, GPIO_IN);
    gpio_pull_up(KEY_CTRL);
}

void lcd_display(uint16_t* screen) {
    for (uint32_t i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++) {
        uint16_t color = screen[i];
        screen[i] = (color << 8) | ((color & 0xFF00) >> 8);
    }

    // Set the Xstart and Xend coordinates of the LCD
    lcd_command(0x2A);
    lcd_write_8bit_data(0x00);
    lcd_write_8bit_data(0); // Xstart
	lcd_write_8bit_data(0x00);
    lcd_write_8bit_data(SCREEN_WIDTH-1); // Xend

    // Set the Ystart and Yend coordinates of the LCD
    lcd_command(0x2B);
    lcd_write_8bit_data(0x00);
	lcd_write_8bit_data(0); // Ystart
	lcd_write_8bit_data(0x00);
    lcd_write_8bit_data(SCREEN_HEIGHT-1); // Yend

    lcd_command(0x2C);

    gpio_put(LCD_DC_PIN, HIGH);
    gpio_put(LCD_CS_PIN, LOW);
    spi_write_blocking(SPI_PORT, (uint8_t*)screen, 2*SCREEN_HEIGHT*SCREEN_WIDTH);
    gpio_put(LCD_CS_PIN, HIGH);
    lcd_command(0x29);
}

void device_init() {
    #ifdef CLOCK_FREQUENCY_KHZ
        vreg_set_voltage(VREG_VOLTAGE_MAX);
    #endif

    stdio_init_all();  

    #ifdef CLOCK_FREQUENCY_KHZ
        set_sys_clock_khz(CLOCK_FREQUENCY_KHZ, true);
        setup_default_uart();

        // Wait and do it again to clear up some UART issues
        sleep_ms(100);
        set_sys_clock_khz(CLOCK_FREQUENCY_KHZ, true);
        setup_default_uart();

        // Get the processor sys_clk frequency in Hz
        uint32_t freq = clock_get_hz(clk_sys);

        // clk_peri does not have a divider, so input and output frequencies will be the same
        clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, freq, freq);
    #endif 

    lcd_set_pins();
    buttons_init();

    spi_init(SPI_PORT, SPI_BAUDRATE_HZ);
    // (SPI_CPOL_1, SPI_CPHA_1) (0.5 clock cycles is wasted) is faster than (SPI_CPOL_0, SPI_CPHA_0) (1.5 clock cycles is wasted).
    spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);

    gpio_set_function(LCD_BL_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LCD_BL_PIN);
    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 60);
    pwm_set_clkdiv(slice_num, 50.0f);
    pwm_set_enabled(slice_num, true);

    lcd_init();
}

void set_button_irq_callback(gpio_irq_callback_t callback, uint32_t event_mask, bool enabled) {
    gpio_set_irq_enabled_with_callback(KEY_FORWARD,  event_mask, enabled, callback);
    gpio_set_irq_enabled(KEY_BACKWARD, event_mask, enabled);
    gpio_set_irq_enabled(KEY_LEFT,     event_mask, enabled);
    gpio_set_irq_enabled(KEY_RIGHT,    event_mask, enabled);
    gpio_set_irq_enabled(KEY_CTRL,     event_mask, enabled);
    gpio_set_irq_enabled(KEY_A,        event_mask, enabled);
    gpio_set_irq_enabled(KEY_B,        event_mask, enabled);
    gpio_set_irq_enabled(KEY_X,        event_mask, enabled);
    gpio_set_irq_enabled(KEY_Y,        event_mask, enabled);
}
