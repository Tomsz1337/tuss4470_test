#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"
#include "mylib/spi_hal.h"
#include "mylib/pulse_gen.h"
#include "hardware/timer.h"
#include "hardware/adc.h"
#include "hardware/uart.h"

#define LED_PIN 25
#define numSamples  1000
#define USER_BUTTON 24

void start_heartbeat(uint32_t interval_ms);

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

    //start_heartbeat(500);
    // ADC init
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);
    //adc_run(true);

    tx_buff[0] = 0x00;
    tx_buff[1] = 0x00;

    sSettings.TUSS4470_SPI_Config.baud_rate = 500000;
    sSettings.TUSS4470_SPI_Config.cpha = 1;
    sSettings.TUSS4470_SPI_Config.cpol = 0;
    sSettings.TUSS4470_SPI_Config.csbf = 0;
    sSettings.TUSS4470_SPI_Config.data_bits = 8;
    sSettings.TUSS4470_SPI_Config.spi = spi0;

    if(button_pressed)
    {
        sSettings.BPF_CONFIG_1 = 0x1E;
    }
    else
    {
        sSettings.BPF_CONFIG_1 = 0x00;     
    }
    sSettings.DEV_CTRL_2 = 0x00;
    sSettings.VDRV_CTRL = 0x0f;
    sSettings.BURST_PULSE = 0x08;
    sSettings.ECHO_INT_CONFIG = 0x19;

    TUSS4470_init(&sSettings, tx_buff);
    //sleep_ms(10000);
    while(1)
    {  
        sampleIndex = 0;
        if(button_pressed)
        {
           TUSS4470_trigger_200kHz(&sSettings, tx_buff);
           //printf("200kHz\n");
        }
        else
        {
            TUSS4470_trigger_40kHz(&sSettings, tx_buff);
            //printf("40kHz\n");
        }
        sleep_us(10);
        
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
