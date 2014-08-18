#!/usr/bin/env python

from TYPES import *
from Acspy.Clients.SimpleClient import PySimpleClient

simpleClient = PySimpleClient.getInstance()

legofarm = simpleClient.getComponent("LEGOFARM")

#legofarm.instantiate("ALPHA")
legofarm.instantiate("BETA")
#legofarm.instantiate("GAMMA")
#legofarm.instantiate("DELTA")
#legofarm.instantiate("EPSILON")

#coord = ([ [0, 0, 1, 1], [2, 2, 3, 3], [4, 4, 5, 5], [0, 1, 3, 2], [4, 3, 2, 4] ])
#space = SpacePosition(180, 180, 13)
#pos = Positions(coord, space)
pos = Positions([ [0, 0, 1, 1], [2, 2, 3, 3], [4, 4, 5, 5], [0, 1, 3, 2], [4, 3, 2, 4] ])

#legofarm.setObjectivesCaller(pos, "ALPHA")
legofarm.setObjectivesCaller(pos, "BETA")
#legofarm.setObjectivesCaller(pos, "GAMMA")
#legofarm.setObjectivesCaller(pos, "DELTA")
#legofarm.setObjectivesCaller(pos, "EPSILON")
#
#legofarm.runVehiclesCaller("ALPHA")
legofarm.runVehiclesCaller("BETA")
#legofarm.runVehiclesCaller("GAMMA")
#legofarm.runVehiclesCaller("DELTA")
#legofarm.runVehiclesCaller("EPSILON")

#legofarm.runAntennasCaller("ALPHA")
#legofarm.runAntennasCaller("BETA")
#legofarm.runAntennasCaller("GAMMA")
#legofarm.runAntennasCaller("DELTA")
#legofarm.runAntennasCaller("EPSILON")

simpleClient.releaseComponent("LEGOFARM")
simpleClient.disconnect()
