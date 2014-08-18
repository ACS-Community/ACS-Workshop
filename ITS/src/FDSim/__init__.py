#import MARSCommon_idl
from MARSCommon_idl import _0_MARS as TYPES
import Acspy.Clients.SimpleClient

def getSensorsList():
	print "Simulated getSensorsList()"      
	#return MARSCommon_idl.long_seq
	return [0, 1, 2]

def sensorStatus(id):
	print "Simulated sensorStatus()"
	return TYPES.sensor_status_struct(id, 0, TYPES.pos_struct(1, 2), True, True)
     
def resetSensor(id):
	print "Simulated resetSensor()"
