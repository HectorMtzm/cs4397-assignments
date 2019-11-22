from __future__ import print_function # use python 3 syntax but make it compatible with python 2
from __future__ import division       #                           ''

import time     # import the time library for the sleep function
import brickpi3 # import the BrickPi3 drivers

main():
    #Initialize components
    BP = brickpi3.BrickPi3()
    BP.set_sensor_type(BP.PORT_1, BP.SENSOR_TYPE.NXT_ULTRASONIC)

    speedAdvanceToGetOverTheRailsOfTheTrain = 60
    speedStop = 0

    try:
        while(True):
            if(getDistance() < 15):
                while(True):
                    if(getDistance() > 30):
                        BP.set_motor_power(BP.PORT_A + BP.PORT_D, speedAdvanceToGetOverTheRailsOfTheTrain)
                        while(True):
                            if(getDistance() > 3)
                                BP.set_motor_power(BP.PORT_A + BP.PORT_D, speedStop)
                            time.sleep(0.02)
                    time.sleep(0.02)       
            time.sleep(0.02)    

    except KeyboardInterrupt: # except the program gets interrupted by Ctrl+C on the keyboard.
        BP.reset_all()        # Unconfigure the sensors, disable the motors, and restore the LED to the control of the BrickPi3 firmware.


def: getDistance():
    try:
        value = BP.get_sensor(BP.PORT_1)
        return value                            # print the distance in CM
    except brickpi3.SensorError as error:
        print(error)
		
main()