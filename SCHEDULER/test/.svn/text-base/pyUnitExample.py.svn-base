#!/usr/bin/env python
import unittest, logging
from SORTErr import *
from TYPES import *
from SCHEDULER_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient


class SchedulerTest(unittest.TestCase):
        
    def setUp(self):
        self.client = PySimpleClient()
        #self.comp = self.client.getComponent('SCHEDULER_SIM')
        self.comp = self.client.getComponent('SCHEDULER')

    def tearDown(self):
        #self.client.releaseComponent('SCHEDULER_SIM')
        self.client.releaseComponent('SCHEDULER')

    def testSchedulerSim(self):
        assert 1 == 1

    def testStart(self):
	try:
	    self.comp.start()
	except SchedulerAlreadyRunningEx, ex:
	    logging.info("Received expected start exception")    

    def testStop(self):
	try:
	    self.comp.stop()
	except SchedulerAlreadyStoppedEx, ex:
	    logging.info("Received expected stop exception")    

    def testProposalUnderExecution(self):
	try:
	    rv = self.comp.proposalUnderExecution()
	except NoProposalExecutingEx, ex:
	    logging.info("Received expected propUnder exception")    

if __name__ == "__main__":
    unittest.main()
