import CONSOLE_MODULE__POA
from Acspy.Servants.ContainerServices import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent import ACSComponent
from Acspy.Clients.SimpleClient import PySimpleClient
from Acspy.Common.Log import getLogger

from SYSTEMErrImpl import SystemInAutoModeExImpl
from SYSTEMErrImpl import *

import logging
from TYPES import Position

class Console1_test(CONSOLE_MODULE__POA.Console,ACSComponent, ContainerServices, ComponentLifecycle):
	
	mode = False # FALSE = MANUAL TRUE = AUTOMATIC
	modeCamera = True
	cameraImage = b'jhashjashd'
	rgb = "255 255 255"
	pixelBias = 4
	resetLevel = 1
	telescopePosition = Position(0.0, 0.0)
	az = 45
	el = 360
	#telescopePosition.az = 0.0
	#telescopePosition.el = 0.0

	def __init__(self):
		ACSComponent.__init__(self)
		ContainerServices.__init__(self)
		self.logger = self.getLogger()

	def setMode(self, newMode):
		
		scheduler = self.getComponent("SCHEDULER")
		if self.mode == False:
			if(newMode == True):
		                self.mode = newMode
				scheduler.start()
				self.logger.log(logging.INFO, "Setting mode to AUTOMATIC")
		else:
			if(newMode == True):
				ex = AlreadyInAutomaticExImpl()
				ex.log(self.logger)
				raise ex
			else:
				self.mode = False
				scheduler.stop()
				self.logger.log(logging.INFO, "Setting mode to MANUAL")
			

	def getMode(self):
		if self.mode == True:
			self.logger.log(logging.INFO, "The actual mode is ACT:" + str(self.mode))
		else:
			self.logger.log(logging.INFO, "The actual mode is :" + str(self.mode))
		return self.mode

	def cameraOn(self):
		if self.mode == False: # This is the case for MANUAL mode
			instrument = self.getComponent("INSTRUMENT")
			instrument.cameraOn()
			#modeCamera = True
			self.logger.log(logging.INFO, "The camera is On")	
		else:
			self.logger.log(logging.WARNING, "Camera can not be turned on in AUTOMATIC mode")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex


	def cameraOff(self):
		if self.mode == False:
			instrument = self.getComponent("INSTRUMENT")
			instrument.cameraOff()
			#modeCamera = False	
			self.logger.log(logging.INFO, "The camera is Off")
		else:
			self.logger.log(logging.WARNING, "Camera can not be turned off in AUTOMATIC mode ")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex


	def moveTelescope(self, coordinates): # verificar formato de parametro "coordinates"
		self.az = coordinates.az
		self.el = coordinates.el
		if self.mode == False:
			telescope = self.getComponent("TELESCOPE")
			if(self.az < 45.0 and self.el < 360):
				telescope.moveTo(coordinates)
				self.logger.log(logging.INFO, "Moving telescope to : " + str(coordinates))
			else:
				ex = PositionOutOfLimitsExImpl()
				ex.log(self.logger)
				raise ex
		else:
			self.logger.log(logging.WARNING, "Telescope can not be moved in AUTOMATIC mode ")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex


	def getTelescopePosition(self):
		if self.mode == False:
			telescope = self.getComponent("TELESCOPE")
			self.telescopePosition = telescope.getCurrentPosition()
			self.logger.log(logging.INFO, "getTelescopePosition : " + str(self.telescopePosition))
			return self.telescopePosition
		else:
			self.logger.log(logging.WARNING, "Telescope position can not be obtained in AUTOMATIC mode ")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex
		
		

	def getCameraImage(self):
		if self.mode == False:
			if(self.modeCamera == True):
				self.logger.log(logging.INFO, "getCameraImage ...")
				instrument = self.getComponent("INSTRUMENT")
				self.cameraImage = instrument.takeImage(10)
				return self.cameraImage	
			else:
				return b'asd'
				ex = CameraIsOffExImpl()
				ex.log(self.logger)
				raise ex

		else:
			self.logger.log(logging.WARNING, "Camera can not be operated in AUTOMATIC mode ")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex
		

	def setRGB(self, rgbConfig):
		self.rgb = rgbConfig
		if self.mode == False:
			instrument = self.getComponent("INSTRUMENT")
			instrument.setRGB(rgbConfig)
			self.logger.log(logging.INFO, "Setting RGB to : " + str(rgbConfig))
		else:
			self.logger.log(logging.WARNING, "Camera can not be set (RGB) in AUTOMATIC mode ")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex
	
	def setPixelBias(self, bias):
		self.logger.log(logging.INFO, "Setting pixelsBias to : " + str(bias))
		self.pixelBias = bias
		if self.mode == False:
			if(self.modeCamera == True):
				instrument = self.getComponent("INSTRUMENT")
				instrument.setPixelBias(bias)
			else:
				ex = CameraIsOffExImpl()
				ex.log(self.logger)
				raise ex
				
		else:
			self.logger.log(logging.WARNING, "Camera can not be set (bias) in AUTOMATIC mode ")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex
		
	
	def setResetLevel(self, level):
		self.resetLevel = level
		if self.mode == False:
			if(self.modeCamera == True):
				instrument = self.getComponent("INSTRUMENT")
				instrument.setResetLevel(level)
				self.logger.log(logging.INFO, "Setting level to : " + str(level))
			else:
				ex = CameraIsOffExImpl()
				ex.log(self.logger)
				raise ex
			
		else:
			self.logger.log(logging.WARNING, "Camera can not be set (level) in AUTOMATIC mode ")
			ex = SystemInAutoModeExImpl()
                        ex.log(self.logger)
			raise ex
		
	
