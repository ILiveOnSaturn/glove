#include <stdio.h>
#include "pico/stdlib.h"
#include "imu_handler.h"
#include "pico/bootrom.h"

#define BUTTON_PIN 13

int main() {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    sleep_ms(50);
    if (!gpio_get(BUTTON_PIN)) {
        reset_usb_boot(0, 0);
    }

    imu_init();


    double accel[3];
    double gyro[3];
    while (1) {
        if (!gpio_get(BUTTON_PIN)) {
            printf("<");
            while (!gpio_get(BUTTON_PIN)) {
                read_imu(accel, gyro);
                printf("%f,%f,%f,%f,%f,%f|", gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2]);
                sleep_ms(50);
            }
            printf(">");
        }
    }
    return 0;
}
