#include "TUSS4470.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"


uint8_t SPI_oddParity(uint8_t hNibble, uint8_t lNibble)
{
	short SPIframe = (hNibble << 8) | lNibble;
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

void TUSS4470_write(TUSS4470_settings *sSettings, uint8_t addr, uint8_t register_data, uint8_t *tx_buff)
{
	tx_buff[0] = (addr & 0x3f) << 1;
	tx_buff[1] = register_data;
	tx_buff[0] |= SPI_oddParity(tx_buff[0], tx_buff[1]); 
	
	SPI_HAL_write(&sSettings->TUSS4470_SPI_Config, tx_buff, 2);
}

void TUSS4470_read(TUSS4470_settings *sSettings, uint8_t addr, uint8_t *tx_buff, uint8_t *rx_buff)
{
	tx_buff[0] = ((addr & 0x3f) << 1) + 0x80;
	tx_buff[1] = 0x00;
	tx_buff[0] |= SPI_oddParity(tx_buff[0], tx_buff[1]);

	SPI_HAL_read(&sSettings->TUSS4470_SPI_Config, tx_buff, rx_buff, 2);

}

void TUSS4470_init(TUSS4470_settings *sSettings, uint8_t *tx_buff)
{
	SPI_HAL_init(&sSettings->TUSS4470_SPI_Config);

	TUSS4470_write(sSettings, BPF_CONFIG_1_addr, sSettings->BPF_CONFIG_1, tx_buff);
	TUSS4470_write(sSettings, BPF_CONFIG_2_addr, sSettings->BPF_CONFIG_2, tx_buff);
	TUSS4470_write(sSettings, DEV_CTRL_1_addr, sSettings->DEV_CTRL_1, tx_buff);
	TUSS4470_write(sSettings, DEV_CTRL_2_addr, sSettings->DEV_CTRL_2, tx_buff);
	TUSS4470_write(sSettings, DEV_CTRL_3_addr, sSettings->DEV_CTRL_3, tx_buff);
	TUSS4470_write(sSettings, VDRV_CTRL_addr, sSettings->VDRV_CTRL, tx_buff);
	TUSS4470_write(sSettings, ECHO_INT_CONFIG_addr, sSettings->ECHO_INT_CONFIG, tx_buff);
	TUSS4470_write(sSettings, ZC_CONFIG_addr, sSettings->ZC_CONFIG, tx_buff);
	TUSS4470_write(sSettings, BURST_PULSE_addr, sSettings->BURST_PULSE, tx_buff);
	TUSS4470_write(sSettings, TOF_CONFIG_addr, sSettings->TOF_CONFIG, tx_buff);
}

void TUSS4470_trigger(TUSS4470_settings *sSettings, uint8_t *tx_buff)
{
	clock_stop(clk_gpout0);
	TUSS4470_write(sSettings, TOF_CONFIG_addr, 0x00, tx_buff);
	TUSS4470_write(sSettings, TOF_CONFIG_addr, 0x01, tx_buff);
	clock_gpio_init_int_frac8(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, 240, 0);
}