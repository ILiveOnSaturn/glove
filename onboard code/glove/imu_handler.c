#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "imu_handler.h"

void imu_init() {
    //stdio_init_all(); 
    i2c_init(I2C_PIN, 100);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C);
    reset();

}


//using the docs from https://www.pololu.com/file/0J563/L3GD20.pdf and https://www.mouser.com/datasheet/2/389/lsm303agr-954987.pdf
void reset_imu() {
    uint8_t ctrl_reg1_g = {0x20, 0x0F}
    uint8_t ctrl_reg1_a = {0x20, 0x57}
    uint8_t ctrl_reg4_a = {0x23, 0x90}
    uint8_t ctrl_reg5_a = {0x24, 0x0A}
    i2c_write_blocking(I2C_PIN, L3GD20_ADDRESS, ctrl_reg1_g, 2, false);
    i2c_write_blocking(I2C_PIN, LSM303_ADDRESS, ctrl_reg1_a, 2, false);
    i2c_write_blocking(I2C_PIN, LSM303_ADDRESS, ctrl_reg4_a, 2, false);
    i2c_write_blocking(I2C_PIN, LSM303_ADDRESS, ctrl_reg5_a, 2, false);
}

void read_imu(int16_t accel[3], int16_t gyro[3]){
    uint8_t buffer[6];

    uint8_t out_addr = 0x28;
    i2c_write_blocking(I2C_PIN, L3GD20_ADDRESS, &out_addr, 1, true);
    i2c_read_blocking(I2C_PIN, L3GD20_ADDRESS, buffer, 6, false);

    for (int i=0; i<3; ++i) {
        gyro[i] = (buffer[(i*2)+1] << 8 | buffer[i*2]);
    }

    i2c_write_blocking(I2C_PIN, LSM303D_ADDRESS, &out_addr, 1, true);
    i2c_read_blocking(I2C_PIN, LSM303D_ADDRESS, buffer, 6, false);

    for (int i=0; i<3; ++i) {
        accel[i] = (buffer[(i*2)+1] << 8 | buffer[i*2]);
    }
}
