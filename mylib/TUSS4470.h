#pragma once

#include "spi_hal.h"
#include "pulse_gen.h"
#include "hardware/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

// Register addresses
#define BPF_CONFIG_1_addr       0x10
#define BPF_CONFIG_2_addr       0x11
#define DEV_CTRL_1_addr         0x12
#define DEV_CTRL_2_addr         0x13
#define DEV_CTRL_3_addr         0x14
#define VDRV_CTRL_addr          0x16
#define ECHO_INT_CONFIG_addr    0x17
#define ZC_CONFIG_addr          0x18
#define BURST_PULSE_addr        0x1A
#define TOF_CONFIG_addr         0x1B
#define DEV_STAT_addr           0x1C
#define DEVICE_ID_addr          0x1D
#define REV_ID_addr             0x1E

// IO pins used by TUSS4470
#define IO1_PIN                 0
#define IO2_PIN                 7

/// TUSS4470 settings structure
typedef struct {
    SPI_Config TUSS4470_SPI_Config;  ///< SPI configuration
    PIO_settings sPIO;               ///< PIO settings for burst generation
    uint32_t freqHz;                 ///< Frequency for burst signal
    uint8_t burstPin;                ///< Pin used for burst output
    uint8_t nPulses;                 ///< Number of pulses in burst

    // Register settings
    unsigned char BPF_CONFIG_1;
    unsigned char BPF_CONFIG_2;
    unsigned char DEV_CTRL_1;
    unsigned char DEV_CTRL_2;
    unsigned char DEV_CTRL_3;
    unsigned char VDRV_CTRL;
    unsigned char ECHO_INT_CONFIG;
    unsigned char ZC_CONFIG;
    unsigned char BURST_PULSE;
    unsigned char TOF_CONFIG;
} TUSS4470_settings;

/// Write one byte to TUSS4470 register
/// \param sSettings Pointer to TUSS4470 settings structure
/// \param addr      Register address
/// \param data      Byte to write
/// \param tx_buff   Transmission buffer (must be at least 2 bytes)
void TUSS4470_write(TUSS4470_settings *sSettings, unsigned char addr, unsigned char data, unsigned char *tx_buff);

/// Read one byte from TUSS4470 register
/// \param sSettings Pointer to TUSS4470 settings structure
/// \param addr      Register address
/// \param tx_buff   Transmission buffer (at least 2 bytes)
/// \param rx_buff   Receive buffer (at least 2 bytes)
void TUSS4470_read(TUSS4470_settings *sSettings, unsigned char addr, unsigned char *tx_buff, unsigned char *rx_buff);

/// Initialize TUSS4470 device with predefined register values
/// \param sSettings Pointer to TUSS4470 settings structure
/// \param tx_buff   Transmission buffer (at least 2 bytes)
void TUSS4470_init(TUSS4470_settings *sSettings, unsigned char *tx_buff);

/// Trigger burst on TUSS4470 using configured PIO and burst parameters
/// \param sSettings Pointer to TUSS4470 settings structure
/// \param tx_buff   Transmission buffer (at least 2 bytes)
void TUSS4470_trigger(TUSS4470_settings *sSettings, uint8_t *tx_buff);

#ifdef __cplusplus
}
#endif
