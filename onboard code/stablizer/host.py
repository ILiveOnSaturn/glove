import ursina
import serial

comnum = input("Enter COM number: ")
pico_data = serial.Serial('COM' + comnum, 115200)
app = ursina.Ursina()

base = ursina.Entity(model="sphere", scale=1, color=ursina.color.black)
x_arr = ursina.Entity(model='cube', scale=(3, 0.25, 0.25), position=(-3 / 2, 0, 0), color=ursina.color.red, parent=base)
y_arr = ursina.Entity(model='cube', scale=(0.25, 0.25, 3), position=(0, 0, -3 / 2), color=ursina.color.green,
                      parent=base)
z_arr = ursina.Entity(model='cube', scale=(0.25, 3, 0.25), position=(0, -3 / 2, 0), color=ursina.color.blue,
                      parent=base)
text = ursina.Text(scale=1, y=-.4, x=-0.5, font='Courier New')


def get_data():
    data = pico_data.read_until('\n'.encode())
    data = data.decode()[0:-1].split(',')
    data = [float(i) for i in data]
    return data


def update():
    roll, pitch, yaw = get_data()
    base.rotation_x = roll
    base.rotation_y = -yaw
    base.rotation_z = -pitch
    text.text = f"{'':<15}{'x:':<15}{'y:':<15}{'z:':<15}\n{'gyro:':<15}{roll:<15}{pitch:<15}{yaw:<15}"


app.run()
