# Sensor Simulator - For ACS Workshop 2009  UTFSM
#
#  Created by Camilo Menay - cmenay[at]csrg.inf.utfsm.cl
#
#
#  Nov 8 2009  ... Created


import SS__POA
from Acspy.Servants.ContainerServices import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent import ACSComponent


class SensorSimulator(SS__POA.SensorSimulator,
            ACSComponent,
            ContainerServices,
            ComponentLifecycle):

    def __init__(self):
        ACSComponent.__init__(self)
        ContainerServices.__init__(self)

    #lifecycle methods
    def initialize(self):
        self.sensorArray = [False, False, False]

    #implementation of own methods (simulation methods)
    def resetSensor(self, sensorID):
        self.sensorArray[sensorID] = False

    def getStatus(self, sensorID):
        return self.sensorArray[sensorID]

    def setStatus(self, sensorID, status):
        self.sensorArray[sensorID] = status
        

