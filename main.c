#include <stdint.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
#include "mylib/spi_hal.h"
#include "mylib/pulse_gen.h"

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

    gpio_init(12);
    gpio_set_dir(12, GPIO_OUT);
    gpio_put(12, 1);

    sSettings.TUSS4470_SPI_Config.baud_rate = 500000;
    sSettings.TUSS4470_SPI_Config.cpha = 1;
    sSettings.TUSS4470_SPI_Config.cpol = 0;
    sSettings.TUSS4470_SPI_Config.csbf = 0;
    sSettings.TUSS4470_SPI_Config.data_bits = 8;
    sSettings.TUSS4470_SPI_Config.spi = spi0;
    //SPI_HAL_init(&sSettings.TUSS4470_SPI_Config);

    sSettings.BPF_CONFIG_1 = 0x1E;      // BFP factory trirm | BFP - on | BFP center frequency = 206.05 kHz |
    sSettings.DEV_CTRL_3 = 0x80;        // IO_MODE_0
    sSettings.VDRV_CTRL = 0x10;         // VDRV - on | VDRV = 5V | charging current = 20mA
    sSettings.BURST_PULSE = 0xC0;       // HALF_BRIDGE_MODE - on | PRE_DRIVER - on | continous burst mode
    sSettings.TOF_CONFIG = 0x03;        // enable burst


    TUSS4470_init(&sSettings, tx_buff);
    while(1)
    {  
        gpio_put(25, 0);
        //TUSS4470_trigger(&sSettings, tx_buff);
        TUSS4470_read(&sSettings, DEV_STAT_addr, tx_buff, rx_buff);
        sleep_ms(500);
        //TUSS4470_read(&sSettings, DEV_STAT_addr, tx_buff, rx_buff);
        gpio_put(25, 1);
        sleep_ms(500);
    }
    return 1;
}
