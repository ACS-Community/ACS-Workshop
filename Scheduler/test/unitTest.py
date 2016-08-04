#!/usr/bin/env python
import unittest, logging
from TYPES import *
from SCHEDULER_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient
from SYSTEMErr import NoProposalExecutingEx

class SchedulerTest(unittest.TestCase):
    def setUp(self):
        self.client = PySimpleClient()
        self.comp = self.client.getComponent('SCHEDULER')

    def tearDown(self):
        self.client.releaseComponent('SCHEDULER')

    def testProposal(self):
	self.assertRaises(NoProposalExecutingEx, self.comp.proposalUnderExecution)

if __name__ == "__main__":
    unittest.main()
