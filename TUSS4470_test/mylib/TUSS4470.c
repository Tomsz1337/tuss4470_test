#include "TUSS4470.h"
#include "hardware/spi.h" 
#include "hardware/gpio.h"
#include "stdint.h"

#define DATA_BITS		8
#define SPI_X			spi0
#define SPI_SCK_PIN 	2
#define SPI_TX_PIN 		3
#define SPI_RX_PIN 		4
#define SPI_CSn_PIN 	5

#define BPF_CONFIG_1_addr		0x10
#define BPF_CONFIG_2_addr 		0x11
#define DEV_CTRL_1_addr 		0x12  
#define DEV_CTRL_2_addr 		0x13
#define DEV_CTRL_3_addr 		0x14
#define VDRV_CTRL_addr 			0x16
#define ECHO_INT_CONFIG_addr	0x17
#define ZC_CONFIG_addr 			0x18
#define BURST_PULSE_addr 		0x1A
#define TOF_CONFIG_addr 		0x1B
#define DEV_STAT_addr 			0x1C
#define DEVICE_ID_addr 			0x1D
#define REV_ID_addr 			0x1E


void SPI_config(struct SPI_FrameParams *sParams)
{
	gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
	gpio_init(SPI_CSn_PIN);
	gpio_set_dir(SPI_CSn_PIN, true);
	gpio_put(SPI_CSn_PIN, true);

	spi_set_slave(SPI_X, false);
	spi_init(SPI_X, sParams->BAUD_RATE);
	spi_set_format(SPI_X, DATA_BITS, sParams->ucCpol, sParams->ucCpha, sParams->ucClsbf);	
}

uint8_t SPI_oddParity(uint8_t hNibble, uint8_t lNibble)
{
	uint16_t SPIframe = (hNibble << 8) | lNibble;
	uint8_t ones = 0;
	for(int i = 0; i < 16; i++)
	{
		if((SPIframe >> i) & 1)
		{
			ones++;
		}
	}
	return (ones + 1) % 2;
}

void TUSS4470_write(uint8_t addr, uint8_t data, uint8_t *pucTXbuff)
{
	pucTXbuff[0] = (addr & 0x3f) << 1;
	pucTXbuff[1] = data;
	pucTXbuff[0] |= SPI_oddParity(pucTXbuff[0], pucTXbuff[1]); 

	gpio_put(SPI_CSn_PIN, false);
	spi_write_blocking(SPI_X, pucTXbuff, sizeof(pucTXbuff));
	gpio_put(SPI_CSn_PIN, true);
}

void TUSS4470_read(uint8_t addr, uint8_t *pucTXbuff, uint8_t *pucRXbuff)
{
	pucTXbuff[0] = ((addr & 0x3f) << 1) + 0x80;
	pucTXbuff[1] = 0x00;
	pucTXbuff[0] |= SPI_oddParity(pucTXbuff[0], pucTXbuff[1]);

	gpio_put(SPI_CSn_PIN, false);
	spi_write_read_blocking(SPI_X, pucTXbuff, pucRXbuff, (sizeof(pucTXbuff) + sizeof(pucRXbuff)));
	gpio_put(SPI_CSn_PIN, true);

}

void TUSS4470_settings(struct TUSS4470_settings *sSettings, uint8_t *pucTXbuff)
{
	TUSS4470_write(BPF_CONFIG_1_addr, sSettings->BPF_CONFIG_1, pucTXbuff);
	TUSS4470_write(BPF_CONFIG_2_addr, sSettings->BPF_CONFIG_2, pucTXbuff);
	TUSS4470_write(DEV_CTRL_1_addr, sSettings->DEV_CTRL_1, pucTXbuff);
	TUSS4470_write(DEV_CTRL_2_addr, sSettings->DEV_CTRL_2, pucTXbuff);
	TUSS4470_write(DEV_CTRL_3_addr, sSettings->DEV_CTRL_3, pucTXbuff);
	TUSS4470_write(VDRV_CTRL_addr, sSettings->VDRV_CTRL, pucTXbuff);
	TUSS4470_write(ECHO_INT_CONFIG_addr, sSettings->ECHO_INT_CONFIG, pucTXbuff);
	TUSS4470_write(ZC_CONFIG_addr, sSettings->ZC_CONFIG, pucTXbuff);
	TUSS4470_write(BURST_PULSE_addr, sSettings->BURST_PULSE, pucTXbuff);
	TUSS4470_write(TOF_CONFIG_addr, sSettings->TOF_CONFIG, pucTXbuff);
}

void TUSS4470_status(uint8_t *pucTXbuff, uint8_t *pucRXbuff)
{
	TUSS4470_read(DEV_STAT_addr, pucTXbuff, pucRXbuff);
}
