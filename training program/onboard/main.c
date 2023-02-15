#include <stdio.h>
#include "pico/stdlib.h"
#include "imu_handler.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#define BUTTON_PIN 13

int main() {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    adc_init();
    imu_init();

    adc_gpio_init(26);
    adc_select_input(0);

    if (!gpio_get(BUTTON_PIN)) {
        reset_usb_boot(0, 0);
    }

    double accel[3];
    double gyro[3];
    uint finger;
    while (1) {
        if (!gpio_get(BUTTON_PIN)) {
            printf("<");
            while (!gpio_get(BUTTON_PIN)) {
                read_imu(accel, gyro);
                finger = adc_read();
                printf("%f,%f,%f,%f,%f,%f,%d|", gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], finger);
                sleep_ms(100);
            }
            printf(">");
        }
    }
    return 0;
}
