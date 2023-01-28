import serial

term = serial.Serial("/dev/ttyACM1", 115200)
while True:
    term.read_until(b'<')
    print("read")
    print(term.read_until(b'>'))
