#include "spi_hal.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <stdint.h>

SPI_error SPI_HAL_init(SPI_Config *sSPI_Config)
{
    if(sSPI_Config->spi_sck_pin == 0 || sSPI_Config->spi_miso_pin == 0 || sSPI_Config->spi_mosi_pin == 0 || sSPI_Config->spi_CSn_pin == 0)
    {
        return SPI_ERR_PIN_CONFIG;
    }

    gpio_set_function(sSPI_Config->spi_sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(sSPI_Config->spi_miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(sSPI_Config->spi_mosi_pin, GPIO_FUNC_SPI);

    gpio_init(sSPI_Config->spi_CSn_pin);
    gpio_set_dir(sSPI_Config->spi_CSn_pin, true);
    gpio_put(sSPI_Config->spi_CSn_pin, true);
    
    if(sSPI_Config->spi == NULL)
    {
        return SPI_ERR_INIT; 
    }

    spi_init(sSPI_Config->spi, sSPI_Config->baud_rate);

    spi_set_format(sSPI_Config->spi, sSPI_Config->data_bits, sSPI_Config->cpha, sSPI_Config->cpol, sSPI_Config->csbf);

    return SPI_OK;
}

SPI_error SPI_HAL_write(SPI_Config *sSPI_Config, uint8_t *tx_data, uint32_t length)
{
    gpio_put(sSPI_Config->spi_CSn_pin, false);

    if(spi_write_blocking(sSPI_Config->spi, tx_data, length) == 0)
    {
        gpio_put(sSPI_Config->spi_CSn_pin, true);
        return SPI_ERR_WRITE;
    }
    gpio_put(sSPI_Config->spi_CSn_pin, true);

    return SPI_OK;
}

SPI_error SPI_HAL_read(SPI_Config *sSPI_Config, uint8_t *tx_data,uint8_t *rx_data, uint32_t length)
{
    gpio_put(sSPI_Config->spi_CSn_pin, false);
    if(spi_write_read_blocking(sSPI_Config->spi, tx_data, rx_data, length) == 0)
    {
        gpio_put(sSPI_Config->spi_CSn_pin, true);
        return SPI_ERR_READ;
    }
    gpio_put(sSPI_Config->spi_CSn_pin, true);    

    return SPI_OK;
}

