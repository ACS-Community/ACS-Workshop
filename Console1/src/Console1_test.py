import CONSOLE_MODULE__POA
from Acspy.Servants.ContainerServices import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent import ACSComponent

class Console1_test(CONSOLE_MODULE__POA.Console,ACSComponent, ContainerServices, ComponentLifecycle):
	def __init__(self):
		ACSComponent.__init__(self)
		ContainerServices.__init__(self)
		self.logger = self.getLogger()
		
		print "Ejecutando Console 1"
		return


	def setMode(self, mode):
		print "setting mode to:", mode

	def getMode(self):
		return True

	def cameraOn(self):
		print "Camera is On.."

	def cameraOff(self):
		print "Camera is Off.."

	def moveTelescope(self, coordinates): # verificar formato de parametro "coordinates"
		print "Moving telescope to", coordinates

	def getTelescopePosition(self):
		return [1,2,3]

	def getCameraImage(self):
		return [1,1,1,0,0,0]

	def setRGB(self, rgbConfig):
		print "Setting RGB :", rgbConfig # verificar formato de parametro "regConfig"
	
	def setPixelBias(self, bias):
		print "Setting pixelsBias:", bias
	
	def setResetLevel(self, resetLevel):
		print "Reset level:", resetLevel
		
	
