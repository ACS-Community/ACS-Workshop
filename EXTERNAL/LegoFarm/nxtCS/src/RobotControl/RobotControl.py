#--REGULAR IMPORTS-------------------------------------------------------------

#--CORBA STUBS-----------------------------------------------------------------
import ROBOTCONTROL_MODULE__POA
import ROBOTCONTROL_MODULE
from TYPES import *
#--ACS Imports-----------------------------------------------------------------
from Acspy.Servants.ContainerServices  import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent       import ACSComponent
#--GLOBALS---------------------------------------------------------------------

#------------------------------------------------------------------------------
class RobotControl(ROBOTCONTROL_MODULE__POA.RobotControl,  #CORBA stubs for IDL interface
                ACSComponent,  #Base IDL interface
                ContainerServices,  #Developer niceties
                ComponentLifecycle):  #HLA stuff
    def __init__(self):
        ACSComponent.__init__(self)
        ContainerServices.__init__(self)
	self.coord = None
        self.positions = None
        return
    #------------------------------------------------------------------------------
    #--Override ComponentLifecycle methods-----------------------------------------
    #------------------------------------------------------------------------------
    def initialize(self):
        self.getLogger().logInfo("called...")
	#self.coord = self.getComponent("LEGOFARM")
        #self.getLogger().logInfo("Get component: LEGOFARM")
        # Default configuration
	self.positions = Positions([ [0, 0, 0, 0], [5, 0, 5, 0], [0, 5, 0, 5], [5, 5, 5, 5] ])

    def cleanUp(self):
	#self.releaseComponent("LEGOFARM")
	self.coord = None
        self.getLogger().logInfo("called...") 
    #------------------------------------------------------------------------------
    #--Implementation of IDL methods-----------------------------------------------
    #------------------------------------------------------------------------------
    def move(self, robotName, x, y):
        if robotName == "Assembly":
            #self.coord.instantiate("ALPHA")
            self.getLogger().logInfo("Assembly robot called") 
            print "ASSEMBLY - ALPHA"
	    print self.positions.coord[0][0], self.positions.coord[0][1]
            self.positions.coord[0][2] = x
            self.positions.coord[0][3] = y
            print x, y
            #self.coord.setObjectivesCaller(self.positions, "ALPHA") 
            #self.coord.runVehiclesCaller("ALPHA")
            self.positions.coord[0][0] = x
            self.positions.coord[0][1] = y

        elif robotName == "Welding":
            #self.coord.instantiate("BETA")
            self.getLogger().logInfo("Welding robot called") 
            print "WELDING - BETA"
	    print self.positions.coord[1][0], self.positions.coord[1][1]
            print x, y
            self.positions.coord[1][2] = x
            self.positions.coord[1][3] = y
            #self.coord.setObjectivesCaller(self.positions, "BETA") 
            #self.coord.runVehiclesCaller("BETA")
            self.positions.coord[1][0] = x
            self.positions.coord[1][1] = y

        elif robotName == "Debris":
            #self.coord.instantiate("GAMMA")
            self.getLogger().logInfo("Debris robot called") 
            print "DEBRIS - GAMMA"
	    print self.positions.coord[2][0], self.positions.coord[2][1]
            print x, y
            self.positions.coord[2][2] = x
            self.positions.coord[2][3] = y
            #self.coord.setObjectivesCaller(self.positions, "GAMMA") 
            #self.coord.runVehiclesCaller("GAMMA")
            self.positions.coord[2][0] = x
            self.positions.coord[2][1] = y

        elif robotName == "Transport":
            #self.coord.instantiate("DELTA")
            self.getLogger().logInfo("Transport robot called") 
            print "TRANSPORT - DELTA"
	    print self.positions.coord[3][0], self.positions.coord[3][1]
            print x, y
            self.positions.coord[3][2] = x
            self.positions.coord[3][3] = y
            #self.coord.setObjectivesCaller(self.positions, "DELTA") 
            #self.coord.runVehiclesCaller("DELTA")
            self.positions.coord[3][0] = x
            self.positions.coord[3][1] = y

        else:
            self.getLogger().logError("Wrong Robot Name")

#------------------------------------------------------------------------------
#--Main defined only for generic testing---------------------------------------
#------------------------------------------------------------------------------
if __name__ == "__main__":
    print "Creating an object"
    g = RobotControl()
    print "Done..."
