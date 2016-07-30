#!/usr/bin/env python
import unittest, logging
from TYPES import *
from SCHEDULER_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient
from SYSTEMErr import NoProposalExecutingEx

class SchedulerTest(unittest.TestCase):
    def setUp(self):
        self.client = PySimpleClient()
        self.retrieveResources()

    def tearDown(self):
        self.releaseResources()

    def retrieveResources(self):
        self.scheduler = self.client.getComponent('SCHEDULER')
        self.database = self.client.getComponent('DATABASE')
        self.telescope = self.client.getComponent('TELESCOPE')
        self.instrument = self.client.getComponent('INSTRUMENT')

    def releaseResources(self):
        self.client.releaseComponent(self.scheduler.name())
        self.client.releaseComponent(self.database.name())
        self.client.releaseComponent(self.telescope.name())
        self.client.releaseComponent(self.instrument.name())

    def getTargets(self):
        targets = TargetList()
        targets.length(2)
        tar = Target()
        tar.tid = 0
        pos = Position()
        pos.az = 100
        pos.el = 60
        tar.coordinates = pos
        tar.expTime = 5
        targets[0] = tar

        tar = Target()
        tar.tid = 1
        pos = Position()
        pos.az = 40
        pos.el = 65
        tar.coordinates = pos
        tar.expTime = 4
        targets[1] = tar
        return targets
        
    #storeProposal(TargetList) #Stores proposal, returns pid.
    #getProposalStatus(pid) #Retrieves proposal status, returns status.
    #removeProposal(pid) #Removes proposal.
    #getProposalObservations(pid) #Returns images of observed targets. Raises ProposalNotYetReadyEx.
    #getProposals() #Returns observations waiting to be observed.
    #setProposalStatus(pid, status) #Sets the status of a given proposal. Raises InvalidProposalStatusTransitionEx.
    #storeImage(pid, tid, image) #Stores image for a given <pid,tid>. Raises ImageAlreadyStoredEx.
    #clean() #Cleans the database.
    def testDatabase(self):
        #Test getting proposal status of an incorrect id.
	self.assertEqual(self.database.getProposalStatus(0),-1) #Not sure the expected behavior when the pid doesn't exist.
        props = self.database.getProposals()
        self.assertEqual(len(props),0)
        #Test storing proposal.
        pid = self.database.storeProposal(self.getTargets())
	self.assertNotEqual(self.database.getProposalStatus(pid),1)
        props = self.database.getProposals()
        self.assertEqual(len(props),1)
        self.assertEqual(props[0].pid,pid)
        #Test remove proposal.
        self.removeProposal(pid)
	self.assertEqual(self.database.getProposalStatus(0),-1) #Not sure the expected behavior when the pid doesn't exist.
        #Test store image.
        targets = self.getTargets()
        pid = self.database.storeProposal(targets)
        img0 = [0]
        img1 = [1]
        self.database.setProposalStatus(pid, 1)
        self.database.storeImage(pid, targets[0].tid, img)
        self.database.storeImage(pid, targets[1].tid, img)
        self.database.setProposalStatus(pid, 2)
        imgs = self.database.getProposalObservations(pid)
        self.assertEqual(imgs[0], img0)
        self.assertEqual(imgs[1], img1)
        props = self.database.getProposals()
        self.assertEqual(len(props),0)
        #Test clean
        self.database.clean()

    def testTelescope(self):
        pass
    def testScheduler(self):
        pass
    def testInstrument(self):
        pass
    def testProposal(self):
	self.assertRaises(NoProposalExecutingEx, self.comp.proposalUnderExecution)

if __name__ == "__main__":
    unittest.main()
