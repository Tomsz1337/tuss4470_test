#ifndef SPI_HAL_H
#define SPI_HAL_H
#include <stdint.h>

typedef void *SPI_Type;
typedef struct{
    SPI_Type spi;
    uint8_t cpha;
	uint8_t cpol;
	uint8_t csbf;
    uint8_t spi_sck_pin;
    uint8_t spi_miso_pin;
    uint8_t spi_mosi_pin;
    uint8_t spi_CSn_pin;
    uint8_t data_bits;
	uint32_t baud_rate;
}SPI_Config;

void SPI_HAL_init(SPI_Config *sSPI_Config);
void SPI_HAL_write(SPI_Config *sSPI_Config, uint8_t *tx_data, uint32_t length);
void SPI_HAL_read(SPI_Config *sSPI_Config, uint8_t *tx_data,uint8_t *rx_data, uint32_t length);

#endif