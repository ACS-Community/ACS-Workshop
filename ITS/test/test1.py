#!/usr/bin/env python
import unittest, logging
from TYPES import *
from TELESCOPE_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient


class TelescopeTest(unittest.TestCase):
        
    def setUp(self):
        #self.client = PySimpleClient()
        #self.comp = self.client.getComponent('TELESCOPE')

    def tearDown(self):
        #self.client.releaseComponent('TELESCOPE')

    def testObserve(self):
	     #self.comp.observe(p, exposureTime)

if __name__ == "__main__":
    unittest.main()
