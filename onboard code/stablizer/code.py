import usb_cdc
import adafruit_l3gd20
import adafruit_lsm303_accel
#import adafruit_lsm303dlh_mag
import board
import busio
import math
from time import monotonic_ns

i2c = busio.I2C(board.GP21, board.GP20)
sensor_gyro = adafruit_l3gd20.L3GD20_I2C(i2c, rng=adafruit_l3gd20.L3DS20_RANGE_250DPS)
sensor_accel = adafruit_lsm303_accel.LSM303_Accel(i2c)
#sensor_mag = adafruit_lsm303dlh_mag.LSM303DLH_Mag(i2c)

data_serial = usb_cdc.data

GYRO_ACCEL_RATIO = 0.92

currtime = monotonic_ns()

roll = 0
pitch = 0
yaw = 0

def calibrate(num_of_calibration):
    gx_sum = 0
    gy_sum = 0
    gz_sum = 0
    ax_sum = 0
    ay_sum = 0
    for i in range(num_of_calibration):
        print(i/(num_of_calibration/100))
        x, y, z = sensor_gyro.gyro
        gx_sum += x
        gy_sum += y
        gz_sum += z
        x, y, z = sensor_accel.acceleration
        ax_sum += math.degrees(math.atan(y / math.sqrt(math.pow(x, 2) + math.pow(z, 2))))
        ay_sum += math.degrees(math.atan(-1 * x / math.sqrt(math.pow(y, 2) + math.pow(z, 2))))

    gx_sum /= num_of_calibration
    gy_sum /= num_of_calibration
    gz_sum /= num_of_calibration
    ax_sum /= num_of_calibration
    ay_sum /= num_of_calibration
    return gx_sum, gy_sum, gz_sum, ax_sum, ay_sum


gx_drift, gy_drift, gz_drift, ax_drift, ay_drift = calibrate(2000)

while True:
    ax, ay, az = sensor_accel.acceleration
    az += 2

    acc_angle_x = math.degrees(math.atan(ay / math.sqrt(math.pow(ax, 2) + math.pow(az, 2)))) - ax_drift
    acc_angle_y = math.degrees(math.atan(-1 * ax / math.sqrt(math.pow(ay, 2) + math.pow(az, 2)))) - ay_drift

#####################################

    prevtime = currtime
    currtime = monotonic_ns()
    elapsed = (currtime - prevtime)/1000000000
    print(elapsed)
    gx, gy, gz = sensor_gyro.gyro
    gx -= gx_drift
    gy -= gy_drift
    gz -= gz_drift
    gyro_angle_x = roll + math.degrees(gx * elapsed)
    gyro_angle_y = pitch + math.degrees(gy * elapsed)

    roll = gyro_angle_x * GYRO_ACCEL_RATIO + acc_angle_x * (1-GYRO_ACCEL_RATIO)
    pitch = gyro_angle_y * GYRO_ACCEL_RATIO + acc_angle_y * (1-GYRO_ACCEL_RATIO)
    yaw += math.degrees(gz * elapsed)

    data_serial.write(f"{roll},{pitch},{yaw}\n".encode())
