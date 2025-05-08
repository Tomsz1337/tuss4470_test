#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
#include "mylib/spi_hal.h"
#include "mylib/pulse_gen.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

#define LED_PIN 25
#define numSamples  850
void start_heartbeat(uint32_t interval_ms);

TUSS4470_settings sSettings;
uint8_t tx_buff[2];
uint8_t rx_buff[2];


int analogValues[numSamples];

volatile int pulseCount = 0;
volatile int sampleIndex = 0;

int main()
{
    stdio_init_all();

    //start_heartbeat(500);
     // ADC init
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

    //sSettings.BPF_CONFIG_1 = 0x1E;      // BFP factory trirm | BFP - on | BFP center frequency = 206.05 kHz |
    sSettings.BPF_CONFIG_1 = 0x00;      // BFP factory trirm | BFP - on | BFP center frequency = 40 kHz |
    sSettings.DEV_CTRL_2 = 0x02;
    sSettings.VDRV_CTRL = 0x0f;
    sSettings.BURST_PULSE = 0x0f;
    sSettings.ECHO_INT_CONFIG = 0x19;

    TUSS4470_init(&sSettings, tx_buff);
    while(1)
    {  
        sampleIndex = 0;
        TUSS4470_trigger(&sSettings, tx_buff);

        sleep_us(100);
        
        for (sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) 
        {
            analogValues[sampleIndex] = adc_read();
        }
        for (int i = 0; i < numSamples; i++) 
        {
            printf("%d", analogValues[i]);
            printf(i < numSamples - 1 ? "," : "\n");
        }
        sleep_ms(200);

    }
    return 1;
}


static bool heartbeat_callback(struct repeating_timer *t) {
    static bool led_on = false;
    led_on = !led_on;
    gpio_put(LED_PIN, led_on);
    return true;
}

void start_heartbeat(uint32_t interval_ms) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    static struct repeating_timer timer;
    add_repeating_timer_ms(interval_ms, heartbeat_callback, NULL, &timer);
}
