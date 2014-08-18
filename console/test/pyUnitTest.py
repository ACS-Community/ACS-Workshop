import unittest
from SORTErrImpl import *
from TYPES import *
from Acspy.Clients.SimpleClient import PySimpleClient



class ConsoleTest(unittest.TestCase):
    def setUp(self):
        self.client = PySimpleClient()
        self.comp = self.client.getComponent("CONSOLE")
        self.comp.setMode(False)

    def testmoveTelescopeOutOfRange(self):
        try:
            pos = Position(10000, 111155)
            self.comp.moveTelescope(pos)
        except PositionOutOfLimitsExImpl:
            assert True
            return 
        assert False

    def testmoveTelescopeInRange(self):
        try:
            pos = Position(10, 10)
            self.comp.moveTelescope(pos)
        except PositionOutOfLimitsExImpl:
            assert False
            return 
        assert True

    def testgetTelescopePosition(self):
        pos = self.comp.getTelescopePosition()
        assert pos is not None

    def testgetCameraImage(self):
        imgType = self.comp.getCameraImage()
        assert imgType is not None

    def tearDown(self):
        self.client.releaseComponent("CONSOLE")

    def testMode(self):
        try:
            self.comp.setMode(True)
            bol = self.comp.getMode()
            if(bol):
                assert True
            else:
                assert False
        except SystemInAutoModeExImpl:
            self.comp.setMode(False)
            bol = self.comp.getMode()
            if(bol):
                assert False
            else:
                assert True

if __name__ == "__main__":
    unittest.main()
