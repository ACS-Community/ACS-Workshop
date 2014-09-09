#!/usr/bin/env python
import unittest, logging
from TYPES import *
from INSTRUMENT_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient

class InstrumentTest(unittest.TestCase):
    def setUp(self):
        self.client    = PySimpleClient()
        self.instrument = self.client.getComponent('INSTRUMENT')
        self.camera = self.client.getComponent('CAMERA')

    def tearDown(self):
        self.client.releaseComponent('INSTRUMENT')
        self.client.releaseComponent('CAMERA')

    def testInstrument(self):
        self.instrument.cameraOn()
        #Observe
        #CameraOff
        #Observe
        #if self.instrument.checkCameraState() == 1:
        print "CAMERA ON"
        #else:
        #    print "ERROR"     

    def testCamera(self):
        mylist=self.camera.getFrame(100)
        print mylist
 

if __name__ == "__main__":
    unittest.main()
