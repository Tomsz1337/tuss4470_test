#include <stdint.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
#include "mylib/spi_hal.h"
#include "mylib/pulse_gen.h"
#include "hardware/timer.h"

#define LED_PIN 25

void start_heartbeat(uint32_t interval_ms);

TUSS4470_settings sSettings;
uint8_t tx_buff[2];
uint8_t rx_buff[2];

int main()
{
    //start_heartbeat(500);

    tx_buff[0] = 0x00;
    tx_buff[1] = 0x00;

    gpio_set_function(0, UART_FUNCSEL_NUM(uart0, 0));
    gpio_set_function(1, UART_FUNCSEL_NUM(uart0, 1));

    uart_init(uart0, 115200);
    uart_puts(uart0, "TUSS4470_debug.\n");
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    gpio_init(12);
    gpio_set_dir(12, GPIO_OUT);
    gpio_put(12, 1);

    sSettings.TUSS4470_SPI_Config.baud_rate = 500000;
    sSettings.TUSS4470_SPI_Config.cpha = 1;
    sSettings.TUSS4470_SPI_Config.cpol = 0;
    sSettings.TUSS4470_SPI_Config.csbf = 0;
    sSettings.TUSS4470_SPI_Config.data_bits = 8;
    sSettings.TUSS4470_SPI_Config.spi = spi0;

    sSettings.BPF_CONFIG_1 = 0x1E;      // BFP factory trirm | BFP - on | BFP center frequency = 206.05 kHz |
    sSettings.DEV_CTRL_3 = 0x80;        // IO_MODE_0
    sSettings.VDRV_CTRL = 0x10;         // VDRV - on | VDRV = 5V | charging current = 20mA
    sSettings.BURST_PULSE = 0xC0;       // HALF_BRIDGE_MODE - on | PRE_DRIVER - on | continous burst mode
    sSettings.TOF_CONFIG = 0x03;        // enable burst

    TUSS4470_init(&sSettings, tx_buff);
    while(1)
    {  
        TUSS4470_trigger(&sSettings, tx_buff);
        gpio_put(25, 0);
    }
    return 1;
}


static bool heartbeat_callback(struct repeating_timer *t) {
    static bool led_on = false;
    led_on = !led_on;
    gpio_put(LED_PIN, led_on);
    return true;
}

void start_heartbeat(uint32_t interval_ms) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    static struct repeating_timer timer;
    add_repeating_timer_ms(interval_ms, heartbeat_callback, NULL, &timer);
}
