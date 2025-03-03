#include <stdint.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"

SPI_FrameParams sParams = {0,0,0,1000000};

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

uint8_t tx[2];

int main()
{
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    SPI_config(&sParams);
    tx[0] = 0x00;
    tx[1] = 0x00;
    while (true) {
        pico_set_led(true);
        TUSS4470_write(BPF_CONFIG_1_addr,0x12,tx);
        sleep_ms(500);
        pico_set_led(false);
        sleep_ms(500);
    }
}
