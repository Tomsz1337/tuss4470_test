#include <stdint.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
//#include "hardware/spi.h"

TUSS4470_settings sSettings;
uint8_t tx_buff[2];
uint8_t rx_buff[2];

int main()
{
    sSettings.TUSS4470_SPI_Config->baud_rate = 1000000;
    sSettings.TUSS4470_SPI_Config->cpha = 1;
    sSettings.TUSS4470_SPI_Config->cpol = 0;
    sSettings.TUSS4470_SPI_Config->csbf = 0;
    sSettings.TUSS4470_SPI_Config->data_bits = 8;
    sSettings.TUSS4470_SPI_Config->spi = spi0;
    sSettings.TUSS4470_SPI_Config->spi_CSn_pin;
    sSettings.TUSS4470_SPI_Config->spi_miso_pin;
    sSettings.TUSS4470_SPI_Config->spi_mosi_pin;
    sSettings.TUSS4470_SPI_Config->spi_sck_pin;

    sSettings.BPF_CONFIG_1 = 0x1F;
    sSettings.VDRV_CTRL = (1<<5);
    sSettings.BURST_PULSE = 0x08 | (1 << 7) | (1 << 6);

    TUSS4470_init(&sSettings, tx_buff);
    
}
