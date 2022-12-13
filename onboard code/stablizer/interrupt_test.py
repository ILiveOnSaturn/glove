import adafruit_l3gd20
import adafruit_lsm303_accel
import adafruit_lsm303dlh_mag
import busio
import board
import digitalio

i2c = busio.I2C(board.GP21, board.GP20)
sensor_gyro = adafruit_l3gd20.L3GD20_I2C(i2c)
sensor_accel = adafruit_lsm303_accel.LSM303_Accel(i2c)
sensor_mag = adafruit_lsm303dlh_mag.LSM303DLH_Mag(i2c)

pin = digitalio.DigitalInOut(board.GP21)

while True:
    ax, ay, az = sensor_accel.acceleration
    gx, gy, gz = sensor_gyro.gyro
    mx, my, mz = sensor_mag.magnetic

    print(ax, ay, az, "|", gx, gy, gz, "|", mx, my, mz, "|", pin.value)
