from MARSCommon_idl import _0_MARS as TYPES
import Acspy.Clients.SimpleClient

def executeRAB(type, pos):
 	print "Simulated executeRAB()"
 
def getReportsList():
	print "Simulated getReportsList()"
	return [1, 2, 3]
     
def getReport(id):
	print "Simulated getReport()"
	return TYPES.report_struct(True, id, "Done", "transporter")

def getSensorsList():
	print "Simulated getSensorsList()"
	return [2, 3, 4]

def getSensorStatus(id):
	print "Simulated getSensorStatus()"
	return TYPES.sensor_status_struct(id, 0L , TYPES.pos_struct(1,2), True, False)

def getRobotsList():
	print "Simulated getRobotsList()"
	return [0,1,2]

def getRobotStatus(id):
	print "Simulated getRobotStatus()"
	return TYPES.robot_status_struct(id, 0L, TYPES.pos_struct(1,3), True, True)
