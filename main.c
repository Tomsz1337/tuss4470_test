#include <stdint.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
#include "mylib/spi_hal.h"
#include "mylib/pulse_gen.h"
<<<<<<< HEAD
#include "mylib/pulse_pwm.h"
=======
#include "hardware/timer.h"

#define LED_PIN 25

void start_heartbeat(uint32_t interval_ms);
>>>>>>> c01e83a868e35074f85847fa591f88c4f8b807e3

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
<<<<<<< HEAD
    
=======
>>>>>>> c01e83a868e35074f85847fa591f88c4f8b807e3

    sSettings.BPF_CONFIG_1 = 0x1E;      // BFP factory trirm | BFP - on | BFP center frequency = 206.05 kHz |
    sSettings.DEV_CTRL_3 = 0x80;        // IO_MODE_0
    sSettings.VDRV_CTRL = 0x0f;         // VDRV - on | VDRV = 25V | charging current = 10mA
    sSettings.BURST_PULSE = 0x08;       // HALF_BRIDGE_MODE - off | PRE_DRIVER - off | continous burst mode
    sSettings.TOF_CONFIG = 0x03;        // enable burst

<<<<<<< HEAD

    //TUSS4470_init(&sSettings, tx_buff);
    SPI_HAL_init(&sSettings.TUSS4470_SPI_Config);

    TUSS4470_write(&sSettings, 0x10, 0x00, tx_buff);
    TUSS4470_write(&sSettings, 0x13, 0x00, tx_buff);
    TUSS4470_write(&sSettings, 0x16, 0x0f, tx_buff);
    TUSS4470_write(&sSettings, 0x1A, 0x08, tx_buff);
    TUSS4470_write(&sSettings, 0x17, 0x19, tx_buff);

    while(1)
    {  
        //gpio_put(25, 0);
        //TUSS4470_trigger(&sSettings, tx_buff);
        TUSS4470_write(&sSettings, TOF_CONFIG_addr, 0x01, tx_buff);
        pulse_gen_start_pwm();
        TUSS4470_write(&sSettings, TOF_CONFIG_addr, 0x00, tx_buff);
        //TUSS4470_read(&sSettings, DEV_STAT_addr, tx_buff, rx_buff);
        sleep_ms(150);
        //TUSS4470_read(&sSettings, DEV_STAT_addr, tx_buff, rx_buff);
        //gpio_put(25, 1);
        //sleep_ms(500);
=======
    TUSS4470_init(&sSettings, tx_buff);
    while(1)
    {  
        TUSS4470_trigger(&sSettings, tx_buff);
        gpio_put(25, 0);
>>>>>>> c01e83a868e35074f85847fa591f88c4f8b807e3
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
