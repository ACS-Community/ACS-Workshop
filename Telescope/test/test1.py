#!/usr/bin/env python
import unittest, logging
from TYPES import *
from TELESCOPE_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient

class TelescopeTest(unittest.TestCase):
    def setUp(self):
        self.client    = PySimpleClient()
        self.telescope = self.client.getComponent('TELESCOPE')

    def tearDown(self):
        self.client.releaseComponent('TELESCOPE')

    #def testObserve(self):
	     #self.comp.observe(p, exposureTime)

    def testTelescope(self):
        curpos  = self.telescope.getCurrentPosition()
        testpos = curpos 
        # Set new coordinates
        testpos.el = 45.0
        testpos.az = 90.0
        # Move the telescope
        self.telescope.moveTo(testpos)
        # Get Position
        curpos = self.telescope.getCurrentPosition()
        # Assert the test
        self.assertEqual(curpos.el, testpos.el)
        self.assertEqual(curpos.az, tespos.az )
        

if __name__ == "__main__":
    unittest.main()