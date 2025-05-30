#pragma once

#include "hardware/spi.h"
#include "hardware/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_SCK_PIN     2
#define SPI_MOSI_PIN    3
#define SPI_MISO_PIN    4
#define SPI_CSn_PIN     5

typedef void *SPI_Type;

/// Configuration structure for SPI interface
typedef struct {
    SPI_Type spi;         ///< SPI instance (e.g., spi0 or spi1)
    uint8_t cpha;         ///< Clock Phase
    uint8_t cpol;         ///< Clock Polarity
    uint8_t csbf;         ///< Chip Select active level (1 = active high, 0 = active low)
    uint32_t data_bits;   ///< Number of bits per SPI frame
    uint32_t baud_rate;   ///< SPI clock speed in Hz
} SPI_Config;

/// Initialize the SPI interface
/// \param sSPI_Config Pointer to SPI_Config structure
void SPI_HAL_init(SPI_Config *sSPI_Config);

/// Write data over SPI
/// \param sSPI_Config Pointer to SPI_Config structure
/// \param tx_data     Pointer to transmit buffer
/// \param length      Number of bytes to transmit
void SPI_HAL_write(SPI_Config *sSPI_Config, uint8_t *tx_data, uint32_t length);

/// Write and read data over SPI (full-duplex)
/// \param sSPI_Config Pointer to SPI_Config structure
/// \param tx_data     Pointer to transmit buffer
/// \param rx_data     Pointer to receive buffer
/// \param length      Number of bytes to transmit/receive
void SPI_HAL_read(SPI_Config *sSPI_Config, uint8_t *tx_data, uint8_t *rx_data, uint32_t length);

#ifdef __cplusplus
}
#endif
