import adafruit_l3gd20
import adafruit_lsm303_accel
import board
import busio
import digitalio
import analogio
import usb_cdc
from adafruit_debouncer import Debouncer

SCL = board.GP20
SDA = board.GP21

i2c = busio.I2C(SCL, SDA)
gyro = adafruit_l3gd20.L3GD20_I2C(i2c)
accel = adafruit_lsm303_accel.LSM303_Accel(i2c)

_pin = digitalio.DigitalInOut(board.G12)
_pin.direction = digitalio.Direction.INPUT
_pin.pull = digitalio.Pull.DOWN
button = Debouncer(_pin)
finger = analogio.AnalogIn(board.A2)

interrupted = False
running = False

data_serial = usb_cdc.data

while not interrupted:
    try:
        button.update()
        if button.rose:
            if running:
                raise Exception("Unexpected Logic: running is true but button was down")
            running = True
            data_serial.write("<")
        if running:
            gx, gy, gz = gyro.gyro
            ax, ay, az = accel.acceleration
            data_serial.write(f"{gx},{gy},{gz},{ax},{ay},{az},{finger.value}|")
        if button.fell:
            running = False
            data_serial.write(">")
    except KeyboardInterrupt:
        interrupted = True

finger.deinit()
data_serial.write("<->")
