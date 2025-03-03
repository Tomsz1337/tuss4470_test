#include <stdint.h>
#include "pico/stdlib.h"
#include "mylib/TUSS4470.h"

uint8_t tx[2];

int main()
{
    
    while (true) {
        TUSS4470_write(0x00,0x01,tx);
        sleep_ms(1000);
    }
}
