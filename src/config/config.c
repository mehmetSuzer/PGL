
#include "config.h"

static void gpio_set(uint pin, bool mode) {
    gpio_init(pin);
    gpio_set_dir(pin, mode);
}

static void set_lcd_pins() {
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

static void init_buttons() {
    gpio_set(KEY_A, GPIO_IN);
    gpio_pull_up(KEY_A);
    gpio_set(KEY_B, GPIO_IN);
    gpio_pull_up(KEY_B);
    gpio_set(KEY_X, GPIO_IN);
    gpio_pull_up(KEY_X);
    gpio_set(KEY_Y, GPIO_IN);
    gpio_pull_up(KEY_Y);

    gpio_set(KEY_UP, GPIO_IN);
    gpio_pull_up(KEY_UP);
    gpio_set(KEY_DOWN, GPIO_IN);
    gpio_pull_up(KEY_DOWN);
    gpio_set(KEY_LEFT, GPIO_IN);
    gpio_pull_up(KEY_LEFT);
    gpio_set(KEY_RIGHT, GPIO_IN);
    gpio_pull_up(KEY_RIGHT);
    gpio_set(KEY_CTRL, GPIO_IN);
    gpio_pull_up(KEY_CTRL);
}

void init_device() {
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

    // Initialize LCD and button GPIO
    set_lcd_pins();
    init_buttons();

    // Initialize SPI
    spi_init(SPI_PORT, SPI_BAUDRATE_HZ);
    // (SPI_CPOL_1, SPI_CPHA_1) (0.5 clock cycles is wasted) is faster than (SPI_CPOL_0, SPI_CPHA_0) (1.5 clock cycles is wasted).
    spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);

    // Initialize PWM
    gpio_set_function(LCD_BL_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LCD_BL_PIN);
    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 60);
    pwm_set_clkdiv(slice_num, 50.0f);
    pwm_set_enabled(slice_num, true);
}
