import MARS__POA
import MARS		#Added because it is in an example

from MARSCommon_idl import _0_MARS as TYPES

#ACS imports
from Acspy.Servants.ACSComponent  import ACSComponent
from Acspy.Servants.ContainerServices import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle

class OCimpl (MARS__POA.OCBase, ACSComponent, ContainerServices, ComponentLifecycle):
    def __init__(self):
        ACSComponent.__init__(self)
        ContainerServices.__init__(self)

    def initialize (self):
        self.theLogger=self.getLogger()
        self.theLogger.logInfo("OCImpl.py: Entering OCBase:initialize")
        #   Getting Task and Report Database component from Container Services
        try:
           self.Trd = self.getComponent("TRD")
           self.isTrdEnabled = True
           self.theLogger.logInfo("Got TRD component.")
        except Exception,e:
           print e
           self.isTrdEnabled = False
           self.theLogger.logError("ERROR: failed to get TRD component")

        #   Getting Task Scheduler component from Container Services
        try:
           self.Ts = self.getComponent("TS")
           self.isTsEnabled = True
           self.theLogger.logInfo("Got TS component")
        except Exception,e:
           print e
           self.isTsdEnabled = False
           self.theLogger.logInfo("ERROR: failed to get TS component")

        #   Getting Failure Detection component from Container Services
        try:
           self.Fd = self.getComponent("FD")
           self.isFdEnabled = True
           self.theLogger.logInfo("Got FD component")
        except Exception,e:
           print e
           self.isFdEnabled = False
           self.theLogger.logError("ERROR: failed to get FD component")
        self.theLogger.logInfo("OCImpl.py: Exiting OCBase:initialize")

    def executeRAB(self, longtype, pos_structpos):
        self.theLogger.logInfo("Entering executeRAB")
        try:
            self.Ts.runManualRAB(longtype, pos_structpos)
        except Exception,e:
            self.theLogger.logError("ERROR: Exception from TS")
            print e
        self.theLogger.logInfo("Exiting executeRAB")

    def getReportsList(self):
        # If Tr is enabled, get the report list
        self.theLogger.logInfo("Entering OCBase:getReportsList")
        if self.isTrdEnabled:
           self.theLogger.logInfo("Got TRD")
           self.reportList =[]
           self.reportIDList = []
           for reportID in self.Trd.getReportsList():
             self.reportList += [self.Trd.getReport(reportID)]
             self.reportIDList += [reportID]
        else:
           self.theLogger.logError("ERROR: TRD is not enabled")
           self.reportList = []
           self.reportIDList = []
        self.theLogger.logInfo("Exiting OCBase:getReportsList")
        return self.reportIDList

    def getReport(self, longID):
        self.getReportsList()
        self.theLogger.logInfo("Entering getReport")
        notFound=True
        for indexID in self.reportIDList:
           if indexID == longID:
               notFound=False
               self.currentReport = self.reportList[longID]
               self.theLogger.logDebug("Found requested report.")
        # If longId is not in the report list, set current report to None
        if notFound:
            self.theLogger.logInfo("Requested report ID not found.")
          #TODO: return a blank status strucs so that things don't break.
            self.currentReport = None
        self.theLogger.logInfo("Exiting getReport")
	return self.currentReport

    def getSensorsList(self):
        self.theLogger.logInfo("Entering getSensorsList")
        # If Fd is enabled, get the sensors list
        if self.isFdEnabled:
            try:
                self.sensorsList = self.Fd.getSensorsList()
            except Exception,e:
                self.theLogger.logError("ERROR: exception from FD")
                print e

        return self.sensorsList
        self.theLogger.logInfo("Exiting getSensorsList")

    def getSensorStatus(self, longid):
        self.getSensorsList()
        self.theLogger.logInfo("Entering getSensorStatus")
        notFound=True
        for indexID in self.sensorsList:
            if indexID == longid:
                self.theLogger.logInfo("Found Requested sensor %d" % (longid))
                notFound=False
                self.currentSensor = self.sensorsList[longid]
        if notFound:
            # If longid is not in the sensors list, set current sensor to None
            #TODO: return a blank status strucs so that things don't break.
            self.currentReport = None
            self.theLogger.logError("ERROR: Requested report not found.")
        self.theLogger.logInfo("Exiting getSensorStatus")
	return self.Fd.sensorStatus(self.currentSensor)

    def getRobotsList(self):
        self.theLogger.logInfo("Entering getRobotsList")
        # If Ts is enabled, get the robots list
        if self.isTsEnabled:
           try:
               self.robotsList = self.Ts.getRobotsList()
           except Exception,e:
               self.theLogger.logError("Exception from Ts.")
               print e
        self.theLogger.logInfo("Exiting getRobotsList")
        return self.robotsList

    def getRobotStatus(self, longid):
        self.theLogger.logInfo("Entering getRobotStatus")
        self.getRobotsList()
        notFound=True
        for indexid in self.robotsList:
           if indexid == longid:
               self.theLogger.logInfo("Found robot in robot list :)")
               notFound=False
               self.currentRobot = self.robotsList[longid]
        if notFound:
            # If longid is not in the robots list, set current robot to None
            #TODO: return a blank status strucs so that things don't break.
            self.currentRobot = None
            self.theLogger.logError("ERROR: unable to find given robot.")
        self.theLogger.logInfo("Exiting getRobotsStatus")
	return self.Ts.getRobotStatus(self.currentRobot)
