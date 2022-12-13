import usb_hid
from adafruit_hid.mouse import Mouse


class MouseControl:
    def __init__(self, rot_multiplier, accel_multiplier):
        self.mouse = Mouse(usb_hid.devices)
        self.ROTATION_CONST = -rot_multiplier
        self.ACCEL_CONST = accel_multiplier
        self.__last_angle_x = 0
        self.__last_angle_z = 0

    def move_mouse(self, rot_x, rot_z, accel_x, accel_z):
        diff_x = (rot_x-self.__last_angle_x) * self.ROTATION_CONST
        diff_z = (rot_z-self.__last_angle_z) * self.ROTATION_CONST
        self.__last_angle_x = rot_x
        self.__last_angle_z = rot_z
        print(rot_x, rot_z, diff_x, diff_z)
        self.mouse.move(round(diff_z + (accel_x * self.ACCEL_CONST)), round(diff_x + (accel_z * self.ACCEL_CONST)))
#        self.mouse.move(round(rot_z + (accel_x * self.ACCEL_CONST)), round(rot_x + (accel_z * self.ACCEL_CONST)))
