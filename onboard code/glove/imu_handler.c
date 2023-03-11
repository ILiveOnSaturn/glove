#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "imu_handler.h"

void imu_init() {
    //stdio_init_all();
    i2c_init(I2C_PIN, 100);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    //bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));
    reset_imu();

}


//using the docs from https://www.pololu.com/file/0J563/L3GD20.pdf and https://cdn-shop.adafruit.com/datasheets/LSM303DLHC.PDF
void reset_imu() {
    uint8_t ctrl_reg1_g[] = {0x20, 0x0F};
    uint8_t ctrl_reg4_g[] = {0x23, 0x00};
    uint8_t ctrl_reg1_a[] = {0x20, 0x57};
    uint8_t ctrl_reg4_a[] = {0x23, 0x18};
    //uint8_t ctrl_reg5_a[] = {0x24, 0x0A};
    i2c_write_blocking(I2C_PIN, L3GD20_ADDRESS, ctrl_reg1_g, 2, false);
    i2c_write_blocking(I2C_PIN, L3GD20_ADDRESS, ctrl_reg4_g, 2, false);
    i2c_write_blocking(I2C_PIN, LSM303D_ADDRESS, ctrl_reg1_a, 2, false);
    i2c_write_blocking(I2C_PIN, LSM303D_ADDRESS, ctrl_reg4_a, 2, false);
    //i2c_write_blocking(I2C_PIN, LSM303D_ADDRESS, ctrl_reg5_a, 2, false);
}

void read_imu_raw(int16_t accel[3], int16_t gyro[3]){
    uint8_t buffer[6];

    uint8_t out_addr = 0xA8;
    i2c_write_blocking(I2C_PIN, L3GD20_ADDRESS, &out_addr, 1, true);
    i2c_read_blocking(I2C_PIN, L3GD20_ADDRESS, buffer, 6, false);

    for (int i=0; i<3; ++i) {
        gyro[i] = (buffer[(i*2)+1] << 8 | buffer[i*2]);
    }

    out_addr = 0xA8;
    i2c_write_blocking(I2C_PIN, LSM303D_ADDRESS, &out_addr, 1, true);
    i2c_read_blocking(I2C_PIN, LSM303D_ADDRESS, buffer, 6, false);

    for (int i=0; i<3; ++i) {
        accel[i] = (buffer[(i*2)+1] << 8 | buffer[i*2]);
    }
}

void read_imu(float gyro[3], float accel[3]) {
    int16_t accel_raw[3];
    int16_t gyro_raw[3];
    read_imu_raw(accel_raw, gyro_raw);
    for (int i=0; i<3; i++) {
        gyro[i] = gyro_raw[i]*(float)0.00875;
        accel[i] = (accel_raw[i]/(float)32767)*4; //maybe?? https://github.com/pimoroni/lsm303d-python/blob/master/library/lsm303d/__init__.py
    }
}
