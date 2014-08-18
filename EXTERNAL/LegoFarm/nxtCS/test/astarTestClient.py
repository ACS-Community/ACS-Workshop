#!/usr/bin/env python

import time
from TYPES import Grid
from TYPES import Solution
from Acspy.Clients.SimpleClient import PySimpleClient

simpleClient = PySimpleClient.getInstance()

astarSolver = simpleClient.getComponent("ASTAR_SOLVER")

a = Grid([1])

astarSolver.solveAlgorithm(Grid())

simpleClient.releaseComponent("ASTAR_SOLVER")
simpleClient.disconnect()
