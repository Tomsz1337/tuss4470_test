#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
#include "mylib/spi_hal.h"
#include "hardware/timer.h"
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "pulse_gen.h"

#define numSamples  1000
#define USER_BUTTON 24

TUSS4470_settings sSettings;
uint8_t tx_buff[2];
uint8_t rx_buff[2];

uint16_t analogValues[numSamples];

volatile int pulseCount = 0;
volatile int sampleIndex = 0;

int main()
{
    stdio_init_all();

    //stdio_uart_init_full(uart0, 921600, 0, 1);
    gpio_init(USER_BUTTON);
    gpio_pull_up(USER_BUTTON);  
    sleep_ms(10);                
    bool button_pressed = !gpio_get(USER_BUTTON);

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    tx_buff[0] = 0x00;
    tx_buff[1] = 0x00;

    sSettings.TUSS4470_SPI_Config.baud_rate = 500000;
    sSettings.TUSS4470_SPI_Config.cpha = 1;
    sSettings.TUSS4470_SPI_Config.cpol = 0;
    sSettings.TUSS4470_SPI_Config.csbf = 0;
    sSettings.TUSS4470_SPI_Config.data_bits = 8;
    sSettings.TUSS4470_SPI_Config.spi = spi0;

    sSettings.burstPin = 15;
    sSettings.nPulses = 8;

    if(button_pressed)
    {
        sSettings.BPF_CONFIG_1 = 0x1E;
        sSettings.freqHz = 200000;
    }
    else
    {
        sSettings.BPF_CONFIG_1 = 0x04;
        sSettings.freqHz = 40000;     
    }
    sSettings.DEV_CTRL_2 = 0x00;
    sSettings.VDRV_CTRL = 0x0f;
    sSettings.BURST_PULSE = 0x08;
    sSettings.ECHO_INT_CONFIG = 0x19;

    TUSS4470_init(&sSettings, tx_buff);

    while(1)
    {  
        sampleIndex = 0;
        
        TUSS4470_trigger(&sSettings, tx_buff);
        
        sleep_us(20);
        
        for (sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) 
        {
            analogValues[sampleIndex] = adc_read();
            sleep_us(20);
        }
        printf("sp\n"); 
        for (int i = 0; i < numSamples; i++) 
        {
            printf("%d", analogValues[i]);
            printf(i < numSamples - 1 ? "," : "\n");
        }
        sleep_ms(100);

    }
    return 0;
}
