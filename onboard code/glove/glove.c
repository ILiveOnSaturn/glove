#include "pico/stdlib.h"

int main()
{
#ifndef PICO_DEFAULT_LED_PIN
    #warning blink example requires a board with regular LED
#else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    for (int i=0; i<30; i++){
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
#endif
    return 0;
}
