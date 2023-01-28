import usb_cdc
from random import random, randint

term = usb_cdc.data
#print(type(term))

try:
    while True:
        input("waiting for button press")
        term.write(b'<')
        for i in range(randint(1,17)):
            term.write(f"{random()*100},{random()*100},{random()*100},{random()*100},{random()*100},{random()*100},{randint(30, 60)}|".encode())
        term.write(b'>')
except (KeyboardInterrupt, EOFError):
    term.write(b'<->')

