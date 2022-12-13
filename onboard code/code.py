from hiddevice.hid import MouseControl
from stablizer.stabilizer import Stabilizer
import board
from time import monotonic_ns

SCL = board.GP21
SDA = board.GP20

stb = Stabilizer(SCL, SDA, 0.96)
m = MouseControl(100, 0)

stb.calibrate(2000)
print("done calibration")

curr_time = monotonic_ns()

vx = 0
vz = 0

while True:
    prev_time = curr_time
    curr_time = monotonic_ns()
    elapsed = (curr_time - prev_time) / 1000000000
    stb.tick(elapsed)

    #ax, ay, az = stb.get_acceleration()
    #vx += ax
    #vz += az

    rotation = stb.get_rotation()
    m.move_mouse(rotation['roll'], rotation['yaw'], 0, 0)

    #gx, gy, gz = stb.get_rotation()
    #m.move_mouse(gx, gz, 0, 0)

