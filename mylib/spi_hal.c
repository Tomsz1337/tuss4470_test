#include "spi_hal.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <stdint.h>

void SPI_HAL_init(SPI_Config *sSPI_Config)
{
    gpio_set_function(sSPI_Config->spi_sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(sSPI_Config->spi_miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(sSPI_Config->spi_mosi_pin, GPIO_FUNC_SPI);

    gpio_init(sSPI_Config->spi_CSn_pin);
    gpio_set_dir(sSPI_Config->spi_CSn_pin, true);
    gpio_put(sSPI_Config->spi_CSn_pin, true);
    

    spi_init(sSPI_Config->spi, sSPI_Config->baud_rate);

    spi_set_format(sSPI_Config->spi, sSPI_Config->data_bits, sSPI_Config->cpha, sSPI_Config->cpol, sSPI_Config->csbf);
}

void SPI_HAL_write(SPI_Config *sSPI_Config, uint8_t *tx_data, uint32_t length)
{
    gpio_put(sSPI_Config->spi_CSn_pin, false);
    spi_write_blocking(sSPI_Config->spi, tx_data, length);
    gpio_put(sSPI_Config->spi_CSn_pin, true);
}

void SPI_HAL_read(SPI_Config *sSPI_Config, uint8_t *tx_data,uint8_t *rx_data, uint32_t length)
{
    gpio_put(sSPI_Config->spi_CSn_pin, false);
    spi_write_read_blocking(sSPI_Config->spi, tx_data, rx_data, length);
    gpio_put(sSPI_Config->spi_CSn_pin, true);    
}

