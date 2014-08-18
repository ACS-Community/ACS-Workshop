#!/usr/bin/env python

import time
from Acspy.Clients.SimpleClient import PySimpleClient

simpleClient = PySimpleClient.getInstance()

nxtAlpha = simpleClient.getComponent("ALPHA")
nxtAlpha.associate("LEGONXT_ALPHA")

while True:
        nxtAlpha.runVehicle()
        opt = raw_input("Continue? ")
        if opt != '1': break

simpleClient.releaseComponent("ALPHA")
simpleClient.disconnect()
