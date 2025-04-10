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
    uint8_t cpha;
	uint8_t cpol;
	uint8_t csbf;
    uint32_t data_bits;
	uint32_t baud_rate;
}SPI_Config;

void SPI_HAL_init(SPI_Config *sSPI_Config);
void SPI_HAL_write(SPI_Config *sSPI_Config, uint8_t *tx_data, uint32_t length);
void SPI_HAL_read(SPI_Config *sSPI_Config, uint8_t *tx_data,uint8_t *rx_data, uint32_t length);

#endif