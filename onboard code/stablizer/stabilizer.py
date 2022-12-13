import adafruit_l3gd20
import adafruit_lsm303_accel
import busio
from math import degrees, atan, sqrt, pow, cos


class Stabilizer:
    def __init__(self, SCL, SDA, GYRO_ACCEL_RATIO=0.92):
        i2c = busio.I2C(SCL, SDA)
        self.sensor_gyro = adafruit_l3gd20.L3GD20_I2C(i2c, rng=adafruit_l3gd20.L3DS20_RANGE_2000DPS,
                                                      rate=adafruit_l3gd20.L3DS20_RATE_400HZ)
        self.sensor_accel = adafruit_lsm303_accel.LSM303_Accel(i2c)
        self.GYRO_ACCEL_RATIO = GYRO_ACCEL_RATIO
        self.rotation_drift = {'gx': 0, 'gy': 0, 'gz': 0, 'ax': 0, 'ay': 0}
        self.acceleration_drift = {'ax': 0, 'ay': 0, 'az': 0}
        self.rotation = {'roll': 0.0, 'pitch': 0.0, 'yaw': 0.0}

    def calibrate(self, iterations):
        for i in self.rotation_drift:
            self.rotation_drift[i] = 0
        for i in self.acceleration_drift:
            self.acceleration_drift[i] = 0
        for i in range(iterations):
            x, y, z = self.sensor_gyro.gyro
            self.rotation_drift['gx'] += x
            self.rotation_drift['gy'] += y
            self.rotation_drift['gz'] += z
            x, y, z = self.sensor_accel.acceleration
            self.rotation_drift['ax'] += degrees(atan(y / sqrt(pow(x, 2) + pow(z, 2))))
            self.rotation_drift['ay'] += degrees(atan(-1 * x / sqrt(pow(y, 2) + pow(z, 2))))
            self.acceleration_drift['ax'] += x
            self.acceleration_drift['ay'] += y
            self.acceleration_drift['az'] += (z + 9.807)
        for i in self.rotation_drift:
            self.rotation_drift[i] /= iterations
        for i in self.acceleration_drift:
            self.acceleration_drift[i] /= iterations

    def tick(self, elapsed):
        ax, ay, az = self.sensor_accel.acceleration
        acc_angle_x = degrees(atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))) - self.rotation_drift['ax']
        acc_angle_y = degrees(atan(-1 * ax / sqrt(pow(ay, 2) + pow(az, 2)))) - self.rotation_drift['ay']
        gx, gy, gz = self.sensor_gyro.gyro
        gyro_angle_x = self.rotation['roll'] + degrees((gx - self.rotation_drift['gx']) * elapsed)
        gyro_angle_y = self.rotation['pitch'] + degrees((gy - self.rotation_drift['gy']) * elapsed)

        self.rotation['roll'] = (gyro_angle_x * self.GYRO_ACCEL_RATIO) + (acc_angle_x * (1 - self.GYRO_ACCEL_RATIO))
        self.rotation['pitch'] = (gyro_angle_y * self.GYRO_ACCEL_RATIO) + (acc_angle_y * (1 - self.GYRO_ACCEL_RATIO))
        self.rotation['yaw'] += degrees((gz - self.rotation_drift['gz']) * elapsed)

    def get_rotation(self):
        return self.rotation

    def get_acceleration(self):
        # TODO fix this
        ax, ay, az = self.sensor_accel.acceleration
        ax -= self.acceleration_drift['ax'] + (9.807 * cos(self.rotation['roll']))
        ay -= self.acceleration_drift['ay'] + (9.807 * cos(self.rotation['pitch']))
        az -= self.acceleration_drift['az'] + (9.807 * cos(self.rotation['yaw']))
        return ax, ay, az

    def get_raw_rotation(self):
        return self.sensor_gyro.gyro

    def get_raw_acceleration(self):
        return self.sensor_accel.acceleration

    def get_raw(self):
        ax, ay, az = self.sensor_accel.acceleration
        gx, gy, gz = self.sensor_gyro.gyro
        return gx, gy, gz, ax, ay, az
