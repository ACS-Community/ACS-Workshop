#!/usr/bin/env python
import unittest, logging
from SORTErr import *
from TYPES import *
from DATABASE_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient


class DatabaseTest(unittest.TestCase):
        
    def setUp(self):
        self.client = PySimpleClient()
        self.comp = self.client.getComponent('DATABASE')
        self.comp.clean()

    def tearDown(self):
        self.comp.clean()
        self.client.releaseComponent('DATABASE')

    def testStoreProposal(self):
        p = Position(0.0, 90.0)
        t = Target(0, p, 1)
        targets = [];
        targets.append(t);
        
        pid = self.comp.storeProposal(targets)
        
        proposals = self.comp.getProposals()
        
        assert len(proposals) == 1
        assert DataBase.STATUS_INITIAL_PROPOSAL == self.comp.getProposalStatus(pid)
        assert len(proposals[0].targets) == 1
        
    def testException(self):
        p = Position(0.0, 90.0)
        t = Target(0, p, 1)
        targets = [];
        targets.append(t);
        
        pid = self.comp.storeProposal(targets)
        
        try:
            self.comp.setProposalStatus(pid, 2)
        except InvalidProposalStatusTransitionEx, ex:
            logging.info("Exception Expected! :D")
        
if __name__ == "__main__":
    unittest.main()
