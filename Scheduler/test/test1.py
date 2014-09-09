#!/usr/bin/env python
import unittest, logging
from TYPES import *
from SCHEDULER_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient


class SchedulerTest(unittest.TestCase):

    def setUp(self):
        self.client = PySimpleClient()
        self.comp = self.client.getComponent('SCHEDULER')

    def tearDown(self):
        self.client.releaseComponent('SCHEDULER')

    def testProposal(self):
	self.assertEqual(self.comp.proposalUnderExecution(),0)

if __name__ == "__main__":
    unittest.main()
