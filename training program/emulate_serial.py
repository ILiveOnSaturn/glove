import usb_cdc
from random import random, randint
from time import sleep

term = usb_cdc.data
#print(type(term))

try:
    while True:
        sleep(5)
        term.write('<'.encode())
        temp = ""
        for i in range(randint(1,17)):
            temp += f"{random()*100},{random()*100},{random()*100},{random()*100},{random()*100},{random()*100},{randint(30, 60)}|"
        term.write((temp+'>').encode())
except (KeyboardInterrupt, EOFError):
    term.write('<->'.encode())

