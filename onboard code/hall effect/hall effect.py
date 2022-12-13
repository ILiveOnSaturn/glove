import analogio
import board

pin = analogio.AnalogIn(board.A0)


try:
    while True:
        print(pin.value)
except KeyboardInterrupt:
    pin.deinit()
    quit()
