#include "spi_hal.h"
#include <stdint.h>


void SPI_HAL_init(SPI_Config *sSPI_Config)
{
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);

    gpio_init(SPI_CSn_PIN);
    gpio_set_dir(SPI_CSn_PIN, true);
    gpio_put(SPI_CSn_PIN, true);

    spi_init(sSPI_Config->spi, sSPI_Config->baud_rate);

    spi_set_format(sSPI_Config->spi, sSPI_Config->data_bits, sSPI_Config->cpol, sSPI_Config->cpha, sSPI_Config->csbf);

} 

void SPI_HAL_write(SPI_Config *sSPI_Config, uint8_t *tx_data, uint32_t length)
{
    gpio_put(SPI_CSn_PIN, false);
    spi_write_blocking(sSPI_Config->spi, tx_data, length);
    gpio_put(SPI_CSn_PIN, true);
}

void SPI_HAL_read(SPI_Config *sSPI_Config, uint8_t *tx_data,uint8_t *rx_data, uint32_t length)
{
    gpio_put(SPI_CSn_PIN, false);
    spi_write_read_blocking(sSPI_Config->spi, tx_data, rx_data, length);
    gpio_put(SPI_CSn_PIN, true);    
}

