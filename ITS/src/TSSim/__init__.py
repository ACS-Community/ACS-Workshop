from MARSCommon_idl import _0_MARS as TYPES
import Acspy.Clients.SimpleClient

def runManualRAB(type, pos):
	print "Simulated runManualRAB()"      

def runAutoRAB(type, pos):
	print "Simulated runAutoRAB()"
     
def getRobotsList():
	print "Simulated getRobotsList()"
	return [1, 2, 3]

def getRobotStatus(id):
	print "Simulated getRobotStatus()"
	return TYPES.robot_status_struct(id, 1, TYPES.pos_struct(1, 2), True, True)
