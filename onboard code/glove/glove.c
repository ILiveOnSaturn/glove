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
        tud_task();
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
#endif
}

void tud_mount_cb(void) {//invoked when device is mounted
}

void tud_umount_cb(void) {//invoked when device is unmounted
}

void tud_suspend_cb(bool remote_wakeup_en) { //invoked when usb bus is suspended. within 7ms device must draw and average current of less than 2.5mA from bus.
    (void) remote_wakeup_en; //if host allows remote wake up
    
}

void tud_resume_cb(void) { //invoked when usb bus is resumed.
    
}
