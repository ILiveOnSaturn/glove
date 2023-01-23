import usb_cdc
import threading
from random import random, randint
term = usb_cdc.data

run = False
stop = False


def start_stop():
    global run, stop
    run = not run
    print("running:", run)
    if input("[ENTER FOR SWITCH, Q FOR QUIT]") == 'Q':
        stop = True


threading.Thread(target=start_stop()).start()
while True:
    if run:
        term.write('<')
        while run:
            term.write(f"{random()*100},{random()*100},{random()*100},{random()*100},{random()*100},{random()*100},{randint(30, 60)}|")
        term.write('>')
    if stop:
        term.write('<->')

