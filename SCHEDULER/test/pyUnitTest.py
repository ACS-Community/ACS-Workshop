import unittest
import time
from SORTErr import *
from TYPES import *
from Acspy.Clients.SimpleClient import PySimpleClient


class SchedulerTest(unittest.TestCase):
    def setUp(self):
        '''
        TODO: Documentation string
        '''
        self.client = PySimpleClient()
        self.comp = self.client.getComponent("SCHEDULER")

    def testProposalUnderExecution(self):
        try:
            pos = self.comp.proposalUnderExecution()
        except NoProposalExecutingEx:
            assert True
            return
        assert False
        
    def testStart(self):
        try:
            self.comp.start()
        except SchedulerAlreadyRunningEx:
            assert False
            return
        assert True

    def testStop(self):
        try:
            self.comp.stop()
        except SchedulerAlreadyStoppedEx:
            assert True
            return
        assert False
        
    def testZ(self):
        try:
            self.comp.start()
            time.sleep(3)
            #pos = self.comp.proposalUnderExecution()
            self.comp.stop()
        except NoProposalExecutingEx:
            assert False
            return
        except SchedulerAlreadyRunningEx:
            assert False
            return
        except SchedulerAlreadyStoppedEx:
            assert False
            return
        #assert pos == 1
	assert True

    def tearDown(self):
        self.client.releaseComponent("SCHEDULER")

if __name__ == "__main__":
    unittest.main()
