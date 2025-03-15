#include <stdint.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
#include "mylib/spi_hal.h"
#include "hardware/spi.h"

TUSS4470_settings sSettings;
uint8_t tx_buff[2];
uint8_t rx_buff[2];



int main()
{
    tx_buff[0] = 0x00;
    tx_buff[1] = 0x00;

    gpio_set_function(0, UART_FUNCSEL_NUM(uart0, 0));
    gpio_set_function(1, UART_FUNCSEL_NUM(uart0, 1));

    uart_init(uart0, 115200);
    uart_puts(uart0, "TUSS4470_debug.\n");
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    sSettings.TUSS4470_SPI_Config.baud_rate = 1000000;
    sSettings.TUSS4470_SPI_Config.cpha = 0;
    sSettings.TUSS4470_SPI_Config.cpol = 0;
    sSettings.TUSS4470_SPI_Config.csbf = 0;
    sSettings.TUSS4470_SPI_Config.data_bits = 8;
    sSettings.TUSS4470_SPI_Config.spi = spi0;

    sSettings.BPF_CONFIG_1 = 0xaa;

    TUSS4470_init(&sSettings, tx_buff);

    while(1)
    {  
        gpio_put(25, 0);
        TUSS4470_write(&sSettings, BPF_CONFIG_1_addr, sSettings.BPF_CONFIG_1, tx_buff);
        sleep_ms(500);
        gpio_put(25, 1);
        sleep_ms(500);
    }
    return 1;
}
