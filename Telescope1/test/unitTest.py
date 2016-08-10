#!/usr/bin/env python
import unittest, logging
from TYPES import *
from TELESCOPE_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient
from SYSTEMErr import PositionOutOfLimitsEx
import time

class Telescope1Test(unittest.TestCase):
    def setUp(self):
        self.client = PySimpleClient()
        self.retrieveResources()

    def tearDown(self):
        self.releaseResources()

    def retrieveResources(self):
        self.telescope = self.client.getComponent('T1_TEST')

    def releaseResources(self):
        self.client.releaseComponent(self.telescope.name())

    def testTelescope(self):
        pos = Position(90,15)
        self.telescope.moveTo(pos)
        self.assertEquals(self.telescope.getCurrentPosition().az,90)
        self.assertEquals(self.telescope.getCurrentPosition().el,15)
        img = self.telescope.observe(pos, 3)
        self.assertIsNotNone(img)
        pos.az = 45
        pos.el = 95
        self.assertRaises(PositionOutOfLimitsEx, self.telescope.moveTo, pos)
        self.assertRaises(PositionOutOfLimitsEx, self.telescope.observe, pos, 3)
        self.assertEquals(self.telescope.getCurrentPosition().az,90)
        self.assertEquals(self.telescope.getCurrentPosition().el,15)
        pos.az = 45
        pos.el = 45
        self.assertEquals(self.telescope.getCurrentPosition().az,45)
        self.assertEquals(self.telescope.getCurrentPosition().el,45)

if __name__ == "__main__":
    unittest.main()
