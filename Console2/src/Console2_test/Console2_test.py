#!/alma/ACS-2016.6/Python/bin/python

import CONSOLE_MODULE__POA

from Acspy.Servants.ContainerServices import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent import ACSComponent

class Console2(CONSOLE_MODULE__POA.Console, ContainerServices, ComponentLifecycle, ACSComponent):

	def __init__(self):
		ACSComponent.__init__(self)
		ContainerServices.__init__(self)
		pass

	def initialize(self):
		'''
		Override this method inherited from ComponentLifecycle
		'''
		self.getLogger().logInfo("called...")
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
		self.getLogger().logInfo("called...") 
		#self.releaseComponent("LAMP1")

	def sayHello(self):
		'''
		Python implementation of IDL method.
		string sayHello();
		'''
		self.getLogger().logInfo("called...") 
		return "hello from Console2"

if __name__ == "__main__":
	con2=Console2()
	con2.initialize()
	con2.sayHello()
	con2.cleanUp