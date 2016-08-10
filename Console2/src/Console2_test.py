#!/alma/ACS-2016.6/Python/bin/python

import CONSOLE_MODULE__POA

import sys
import os
from sys import stdout
import TYPES
from Acspy.Servants.ContainerServices import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent import ACSComponent

class Console2_test(CONSOLE_MODULE__POA.Console, ContainerServices, ComponentLifecycle, ACSComponent):

	def __init__(self):
		ACSComponent.__init__(self)
		ContainerServices.__init__(self)
		pass

	def initialize(self):
		'''
		Override this method inherited from ComponentLifecycle
		'''
		self.getLogger().logInfo("CONSOLE 2 INIT ACCESS")
		'''
		try:
		    import acsexmplLamp
		    lamp = self.getComponent("LAMP1")
		    self.brightness = lamp._get_brightness()
		except Exception, e:
		    print "LAMP1 unavailable"
		    print e		
		'''

	def cleanUp(self):
		'''
		Override this method inherited from ComponentLifecycle
		'''
		self.getLogger().logInfo("CONSOLE 2 CLEANUP ACCESS")
		#self.releaseComponent("LAMP1")

	def setMode(self, *args):
		self.getLogger().logDebug("CONSOLE 2 SET_MODE ACCESS")
		pass
		
	def getMode(self, *args):
		self.getLogger().logDebug("CONSOLE 2 GET_MODE ACCESS")		
		return True
		

	def cameraOn(self, *args):
		self.getLogger().logInfo("CONSOLE 2 CAMERA ON METHOD ACCESS")		
		pass
	
	def cameraOff(self, *args):
		self.getLogger().logInfo("CONSOLE 2 CAMERA OFF METHOD ACCESS")	
		pass

	def moveTelescope(self, *args):
		self.getLogger().logInfo("CONSOLE 2 MOVE TELESCOPE ACCESS")	
		pass

	def getTelescopePosition(self, *args):
		a=TYPES.Position(0.0,0.0)	
		self.getLogger().logInfo("CONSOLE 2 GET TELESCOPE POSITION ACCESS")			
		return a

	def getCameraImage(self, *args):
		self.getLogger().logCritical("CONSOLE 2 GET CAMERA IMAGE ACCESS")	
		return b'sjkhddkjhfsalk'

	def setRGB(self, *args):
		self.getLogger().logInfo("CONSOLE 2 SET RGB ACCESS")	
		pass

	def setPixelBias(self, *args):
		self.getLogger().logError("CONSOLE 2 SET PIXEL BIAS ACCESS")	
		pass

	def setResetLevel(self, *args):
		self.getLogger().logInfo("CONSOLE 2 RESET LEVEL ACCESS")	
		pass

if __name__ == "__main__":
	con2=Console2()
	con2.initialize()
	con2.sayHello()
	con2.cleanUp
