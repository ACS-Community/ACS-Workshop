import unittest
import SYSTEMErr
from Acspy.Clients.SimpleClient import PySimpleClient


class InstrumentTestCase(unittest.TestCase):
	def setUp(self):
		self.client = PySimpleClient()
		self.instrument = self.client.getComponent("INSTRUMENT_2")

class TakeImageOnTestCase(InstrumentTestCase):
	def runTest(self):
		self.instrument.cameraOn()
		self.instrument.takeImage(0)

class TakeImageOffTestCase(InstrumentTestCase):
	def runTest(self):
		self.instrument.cameraOff()
		try:
			self.instrument.takeImage(0)
		except SYSTEMErr.CameraIsOffEx:
			pass
		else:
			self.fail("CameraIsOff excepted.")

if __name__ == "__main__":
	unittest.main()

