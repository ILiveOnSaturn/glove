#include "hardware/i2c.h"
#define I2C_PIN i2c0
#define L3GD20_ADDRESS 0x6B
#define LSM303D_ADDRESS 0x19
#define SCL_PIN 17
#define SDA_PIN 16

#ifdef __cplusplus
extern "C" {
#endif

void imu_init();

void reset_imu();
void read_imu_raw(int16_t accel[3], int16_t gyro[3]);
void read_imu(float gyro[3], float accel[3]);

#ifdef __cplusplus
}
#endif