#include "pico/stdlib.h"
#include "tusb.h"
#include "bsp/board.h"

#include "usb_descriptors.h"

int main()
{
#ifndef PICO_DEFAULT_LED_PIN
#warning blink has no led
#else
    tusb_init();
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
#endif
}
