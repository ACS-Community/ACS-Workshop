from MARSCommon_idl import _0_MARS as TYPES
import Acspy.Clients.SimpleClient

def reset():
	print "Simulated reset()"

def status():
	print "Simulated status()"
	return TYPES.sensor_status_struct(0, 0, TYPES.pos_struct(1,2), True, False)

def position():
	print "Simulated position()"
	return TYPES.pos_struct(1,2)

def sensorType():
	print "Simulated sensorType()"
	return 0L
