from time import sleep
from A10Lime import *

#initialize module
init()

#set pin as output
if getcfg(PH2) == INPUT:
    setcfg(PH2, OUTPUT)

while True:
    sleep(0.5)
    output(PH2, HIGH)
    sleep(0.5)
    output(PH2, LOW)
    
