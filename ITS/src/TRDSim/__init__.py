from MARSCommon_idl import _0_MARS as TYPES
import Acspy.Clients.SimpleClient

def getRAB(type):
	print "Simulated getRAB()"
	return [TYPES.command_struct(0,0), TYPES.command_struct(1,1)]

def createReport(report):
        print "Simulated createReport()"
	return 0L

def getReportsList():
        print "Simulated getReportList()"
	return [0L, 1L]

def getReport(id):
        print "Simulated getReport()"
	return TYPES.report_struct(True,id,"errorMsg","rabType")

