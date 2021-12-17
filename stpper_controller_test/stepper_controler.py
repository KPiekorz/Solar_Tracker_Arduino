
"""
    MS1 MS2 MS3 RESOLUTION
    0   0   0   FULL
    1   0   0   HALF
    0   1   0   QUATER
    1   1   0   EIGHT
    1   1   1   SIXTEENTH
"""

import os
import time
import RPi.GPIO as GPIO

HIGH = GPIO.HIGH
LOW = GPIO.LOW

#------------------- Used Pins asignation ---------------
PinMS1 = 2                  # Output, MS1 (Mode ), pin number 3
PinMS2 = 3                  # Output, MS2 (Mode ), pin number 5
PinMS3 = 14                 # Output, MS3 (Mode ), pin number 8
PinStep = 4                 # Output, Step (Pulses), pin number 7
PinDir = 15                 # Output, Dir (Direction), pin number 10
PinFdCWhite = 18                    # Input, LimitDetector 1 White, pin number 12
PinFdCYellow = 17                    # Input, LimitDetector 2 Yellow, pin number 11

step_wanted = 0

GPIO.setmode(GPIO.BCM)
GPIO.setup(PinMS1, GPIO.OUT)
GPIO.setup(PinMS2, GPIO.OUT)
GPIO.setup(PinMS3, GPIO.OUT)
GPIO.setup(PinDir, GPIO.OUT)
GPIO.setup(PinStep, GPIO.OUT)
GPIO.setup(PinFdCWhite, GPIO.IN)
GPIO.setup(PinFdCYellow, GPIO.IN)

pin_dir_state = LOW
pin_ms1_state = LOW
pin_ms2_state = LOW
pin_ms3_state = LOW

GPIO.output(PinMS1, pin_dir_state)    # Direction
GPIO.output(PinMS2, pin_ms1_state)
GPIO.output(PinMS3, pin_ms2_state)
GPIO.output(PinDir, pin_ms3_state)    # Mode Resolution

os.system('clear')

def ShowValues(): # present current configuration
    print("Current configuration: ")
    print("Direction: {}".format(pin_dir_state))
    print("MS1: {}".format(pin_ms1_state))
    print("MS2: {}".format(pin_ms2_state))
    print("MS3: {}".format(pin_ms3_state))

def ConfigModeSpeed(): # ask user to set microsep resolution
    print("Microstep resolutio:")
    print("1 - full")
    print("2 - half")
    print("3 - quater")
    r = input("Input resolutio:> ")
    if r == 1:
        pin_ms1_state = LOW
        pin_ms2_state = LOW
        pin_ms3_state = LOW

    if r == 2:
        pin_ms1_state = HIGH
        pin_ms2_state = LOW
        pin_ms3_state = LOW

    if r == 3:
        pin_ms1_state = LOW
        pin_ms2_state = HIGH
        pin_ms3_state = LOW

def SetModeSpeed(): # set microstep
    GPIO.output(PinMS2, pin_ms1_state)
    GPIO.output(PinMS3, pin_ms2_state)
    GPIO.output(PinDir, pin_ms3_state)    # Mode Resolution

def ConfigDirection(): # ask user to set direction
    print("Direction:")
    print("1 - right")
    print("2 - left")
    d = input("Input direction:> ")
    if d == 1:
        pin_dir_state = HIGH

    if d == 0:
        pin_dir_state = LOW

def SetDirection(): # set direction
    GPIO.output(PinMS1, pin_dir_state)    # Direction

def GoLimit(): # go to the limit with selected direction (check swihces all time)
    button_left_state = GPIO.input(PinFdCYellow)
    button_right_state = GPIO.input(PinFdCWhite)
    while button_left_state ==  True and button_right_state == True:
        time.sleep(0.001)
        GPIO.output(PinStep, GPIO.HIGH)
        time.sleep(0.001)
        GPIO.output(PinStep, GPIO.LOW)
        button_left_state = GPIO.input(PinFdCYellow)
        button_right_state = GPIO.input(PinFdCWhite)

def ConfigSteps(): # ask user to set step
    global step_wanted
    step_wanted = input("Input step wanted:> ")

def SetSteps(): # run motor set step (check swithces all time)
    global step_wanted
    while button_left_state ==  True and button_right_state == True and step_wanted >= 0:
        time.sleep(0.001)
        GPIO.output(PinStep, GPIO.HIGH)
        time.sleep(0.001)
        GPIO.output(PinStep, GPIO.LOW)
        button_left_state = GPIO.input(PinFdCYellow)
        button_right_state = GPIO.input(PinFdCWhite)
        step = step - 1

    step_wanted = 0

# ============================================================================
# ============================= MAIN =========================================
# ============================================================================

exit = False                           # Initial Values
vMS = 1                                 # value MS (1, 2, 4, 8, 16)
vDir = 1                                # value Dir: 1 (to White) , 2 (to Yellow)
vSteps = 0                              # value vSteps (1, ...)
GPIO.output(PinStep, GPIO.LOW)          # always initialitzed at '0'
SetModeSpeed()
SetDirection()
vLDWhite = GPIO.input(PinFdCWhite)
vLDYellow = GPIO.input(PinFdCYellow)
steps = 0                               # running steps

# ROUTINE 1-8:
ShowValues()
ConfigModeSpeed()
SetModeSpeed()
ConfigDirection()
SetDirection()
GoLimit()
ConfigSteps()
SetSteps()

while not exit:
    print ('Options:')
    print ('    E.- Exit program ')
    print ('    M.- Speed Mode ')
    print ('    D.- Direction ')
    print ('    L.- Go to Limit ')
    print ('    S.- Go to Steps ')
    print ("-------------------------------------------------------------------------")
    print
    ShowValues()
    print ("-------------------------------------------------------------------------")
    print
    op = input('> ')
    if (op == "E" or op == "e"):
        exit = True

    if (op == "M" or op== "m"):
        ConfigModeSpeed()
        SetModeSpeed()

    if (op == "D" or op== "d"):
        ConfigDirection()
        SetDirection()

    if (op == "L" or op == "l"):
        GoLimit()

    if (op == "S" or op == "s"):
        ConfigSteps()
        SetSteps()

# ---------------------------------------------------------------
GPIO.cleanup()
# ---------------------------------------------------------------