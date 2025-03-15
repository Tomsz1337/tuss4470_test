#ifndef SPI_HAL_H
#define SPI_HAL_H
#include "hardware/spi.h"
#include "hardware/gpio.h"

#define SPI_SCK_PIN     2
#define SPI_MOSI_PIN    3
#define SPI_MISO_PIN    4
#define SPI_CSn_PIN     5

typedef void *SPI_Type;
typedef struct SPI_Config{
    SPI_Type spi;
    spi_cpha_t cpha;
	spi_cpol_t cpol;
	spi_order_t csbf;
    uint data_bits;
	uint baud_rate;
}SPI_Config;

void SPI_HAL_init(SPI_Config *sSPI_Config);
void SPI_HAL_write(SPI_Config *sSPI_Config, uint8_t *tx_data, uint32_t length);
void SPI_HAL_read(SPI_Config *sSPI_Config, uint8_t *tx_data,uint8_t *rx_data, uint32_t length);

#endif