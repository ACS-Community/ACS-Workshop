#!/usr/bin/env python

import time
from TYPES import MotorState
from TYPES import SensorState
from Acspy.Clients.SimpleClient import PySimpleClient

simpleClient = PySimpleClient.getInstance()

av = 0
#devlegoAlpha = simpleClient.getComponent("LEGONXT_ALPHA")
devlegoBeta = simpleClient.getComponent("LEGONXT_BETA")
#devlegoGamma = simpleClient.getComponent("LEGONXT_GAMMA")

def setMotors(port, power, tachoLimit):
       # devlegoAlpha.setMotorState(MotorState(port, power, tachoLimit))
       # devlegoAlpha.setMotorState(MotorState(port + 2, power, tachoLimit))
        devlegoBeta.setMotorState(MotorState(port, power, tachoLimit))
        devlegoBeta.setMotorState(MotorState(port + 2, power, tachoLimit))
        #devlegoGamma.setMotorState(MotorState(port + 1, power, tachoLimit))
        #devlegoGamma.setMotorState(MotorState(port + 2, power, tachoLimit))

def getSensors(port):
        global av
       # devlegoAlpha.setSensorState(SensorState(0,5,0))
       # devlegoGamma.setSensorState(SensorState(0,5,0))
       # devlegoBeta.setSensorState(SensorState(0,5,0))
       # print "ALPHA: " + str(devlegoAlpha.getSensorState(0)) + "  GAMMA: " + str(devlegoGamma.getSensorState(0)) + " BETA: " + str(devlegoBeta.getSensorState(0))
	actual = devlegoBeta.getSensorState(port)
	print port, actual
	av = actual + av
        
devlegoBeta.setSensorState(SensorState(0,5,0))
devlegoBeta.setSensorState(SensorState(1,5,0))
i = 0
while i < 100:
        #setMotors(0, 80, 360)
        getSensors(0)
        getSensors(1)
        #opt = raw_input("Continue? ")
        i = i + 1
        #if opt != '1': break

print av/200.0
devlegoBeta.setSensorState(SensorState(0,0,0))
devlegoBeta.setSensorState(SensorState(1,0,0))
#simpleClient.releaseComponent("LEGONXT_ALPHA")
simpleClient.releaseComponent("LEGONXT_BETA")
#simpleClient.releaseComponent("LEGONXT_GAMMA")
simpleClient.disconnect()
