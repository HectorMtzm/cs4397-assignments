from __future__ import print_function # use python 3 syntax but make it compatible with python 2
from __future__ import division       #                           ''

import time     # import the time library for the sleep function
import brickpi3 # import the BrickPi3 drivers

def main():
    #Initialize components
    BP = brickpi3.BrickPi3()
    BP.reset_all()

    BP.set_sensor_type(BP.PORT_1, BP.SENSOR_TYPE.NXT_ULTRASONIC)
    BP.offset_motor_encoder(BP.PORT_A, BP.get_motor_encoder(BP.PORT_A))
    BP.offset_motor_encoder(BP.PORT_D, BP.get_motor_encoder(BP.PORT_D))

    speedAdvanceToGetOverTheRailsOfTheTrain = -60
    speedStop = 0

    time.sleep(10)

    def getDistance():
        try:
            value = BP.get_sensor(BP.PORT_1)
            return value                            # print the distance in CM
        except brickpi3.SensorError as error:
            print(error)

    try:
        while(True):
            print ("waiting for train to cross")
            if(getDistance() < 12):
                while(True):
                    print ("Train crossing")
                    if(getDistance() > 40):
                        print("train crossed")

                        print("Motors should be running")
                        time.sleep(.05)
                        BP.set_motor_power(BP.PORT_A + BP.PORT_D, speedAdvanceToGetOverTheRailsOfTheTrain)
                        time.sleep(.05)
                        while(True):
                            print ("waiting to stop")
                            if(getDistance() < 10):
                                BP.set_motor_power(BP.PORT_A + BP.PORT_D, speedStop)
                                return
                            time.sleep(0.02)
                    time.sleep(0.02)       
            time.sleep(0.02)    

    except KeyboardInterrupt: # except the program gets interrupted by Ctrl+C on the keyboard.
        BP.reset_all()        # Unconfigure the sensors, disable the motors, and restore the LED to the control of the BrickPi3 firmware.
            
   
main()
