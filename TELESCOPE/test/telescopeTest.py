#!/usr/bin/env python
import unittest, logging
from SORTErr import *
from TYPES import *
from TELESCOPE_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient


class TelescopeTest(unittest.TestCase):
        
    def setUp(self):
        self.client = PySimpleClient()
        self.comp = self.client.getComponent('TELESCOPE')

    def tearDown(self):
        self.client.releaseComponent('TELESCOPE')

    def testObserve(self):

	exposureTime = 1
        p = Position(0.0, 91.0)

        try:
	    self.comp.observe(p, exposureTime)
        #    self.fail("Expected failure")
        except PositionOutOfLimitsEx, ex:
            logging.info("Exception Expected! :D")

if __name__ == "__main__":
    unittest.main()
