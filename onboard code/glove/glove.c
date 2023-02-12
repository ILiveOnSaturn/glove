#include "pico/stdlib.h"
#include "tusb.h"
#include "bsp/board.h"
#include "imu_handler.h"

#include "usb_descriptors.h"

#define BUTTON_PIN 13

int main()
{
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    if (!gpio_get(BUTTON_PIN)) {
        reset_usb_boot(0, 0);
    }
    tusb_init();
    stdio_init_all();
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        tud_task();
        printf("working\n");
    }
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


uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) { //returns the buffer size to the host
    (void) instance; //TODO
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) { //gets buffer from host. can be used to check key presses. don't really need it rn.
    (void) instance;
    (void) report_id;
    (void) report_type;
    tud_hid_report(0, buffer, bufsize);//echo back everything we get
}
