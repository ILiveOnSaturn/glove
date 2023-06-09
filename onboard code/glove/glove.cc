#include "pico/stdlib.h"
#include "tusb.h"
#include "pico/bootrom.h"
#include "imu_handler.h"
#include "nn_functions.h"
#include "constants.h"
#include "tusb_config.h"
#include "usb_descriptors.h"

#define BUTTON_PIN 13
#define MAX_BUFFER 10

void hid_task();
uint8_t get_keycode(int num);

uint8_t char_buffer[MAX_BUFFER] = {0};
int char_buffer_size = 0;
bool is_key_pressed = false;

int main()
{
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    sleep_ms(50); //wait for the button to load
    if (!gpio_get(BUTTON_PIN)) { //go into the bootsel if needed.
        reset_usb_boot(0, 0);
    }

    imu_init();
    tusb_init();
    setup_nn();

    float imu_buffer[max_timestamp * 6] = {0};
    int cnt;
    float* output;
    while (true) {
        tud_task();
        if (!gpio_get(BUTTON_PIN)) {
            cnt = 0;
            while (!gpio_get(BUTTON_PIN) && cnt < max_timestamp) {
                //read imu input adn put it in buffer in the corresponding place.
                read_imu(imu_buffer + cnt * 6, imu_buffer + cnt * 6 + 3);
                ++cnt;
                sleep_ms(50);
            }
            if (cnt == max_timestamp) {
                continue;
            }
            if (cnt == 1) { //if the click is short then add the backspace button to the buffer.
                if (char_buffer_size < MAX_BUFFER) {
                    char_buffer[char_buffer_size] = get_keycode(-1);
                    char_buffer_size++;
                } else {
                    printf("buffer too big\n");
                }
                continue;
            }
            output = get_nn_output(imu_buffer, max_timestamp * 6);
            printf("output:\n");
            int max_n = 0;
            for (int i=0; i<28; ++i) { //softmax output, get the corresponding output.
                if (output[max_n] < output[i]) {
                    max_n = i;
                }
                printf("%f\n", output[i]);
            }
            printf("buffer size: %d\n", cnt);
            uint8_t keycode = get_keycode(max_n);
            printf("keycode: %d\n", keycode);
            if (char_buffer_size < MAX_BUFFER) {
                //add keycode to char buffer.
                char_buffer[char_buffer_size] = keycode;
                char_buffer_size++;
            } else {
                printf("buffer too big\n");
            }
            for (int i=0; i<6*max_timestamp; i++) { //clear buffer
                imu_buffer[i] = 0;
            }
        }
        hid_task();
    }
}

uint8_t get_keycode(int num) { //turn special keys into their keycodes.
    if (num == 0) {
        return HID_KEY_SPACE;
    }
    else if (num == 27) {
        return HID_KEY_PERIOD;
    }
    else if (num == -1) {
        return HID_KEY_BACKSPACE;
    } else {
        return num+3;
    }
}

void send_hid_report(uint8_t key) {
    if (!tud_hid_ready()) { return;} //check if hid is ready
    uint8_t keycode[6] = {0};
    keycode[0] = key;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode); //click key
    if (key == HID_KEY_NONE) {
        is_key_pressed = false;
    } else {
        //clear pressed key from buffer and move elements down one.
        for (int i=0; i<char_buffer_size; i++) {
            if (i+1 >= MAX_BUFFER) {
                char_buffer[i] = 0;
            } else {
                char_buffer[i] = char_buffer[i + 1];
            }
        }
        char_buffer_size--;
        is_key_pressed = true;
    }
}

void hid_task() {
    if (is_key_pressed) { //clear key
        send_hid_report(HID_KEY_NONE);
        return;
    }
    if (char_buffer_size == 0) {return;} //buffer empty
    if (tud_suspended()) {
        //wake up device if host has remote wakeup enabled.
        tud_remote_wakeup();
    } else {
        send_hid_report(char_buffer[char_buffer_size-1]);
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
