#!/usr/bin/env python

import unittest
from Acspy.Clients.SimpleClient import PySimpleClient
import TYPES

class TestDBMethods(unittest.TestCase):

    def test_full(self):
        self.client = PySimpleClient()
        self.database = self.client.getComponent("DATABASE_MODULE_JAVA");

        self.database.clean()

        targets = []
	targets2 = []
        images = []
        for i in range(10):
	    targets.append(TYPES.Target(i, TYPES.Position(10.0, 45.0), 2))
            image = bytearray()
            for j in range(1000000):
                image.append(j%256)
            images.append(bytes(image))

        pid = self.database.storeProposal(targets)
	print "pid Asignado", pid 


	'''
	setear status 1 para Proposal 0
	'''
	self.database.setProposalStatus(pid,i)

	'''
	for each tid in proposal 0
	'''
	for i in range(10):
		self.database.storeImage(0,i,image[i])

	ProposalList = self.database.getProposals()
	print ProposalList


	'''
	Camino para borrar proposal
	'''
        pid2 = self.database.storeProposal(targets)
        print "pid Asignado a proposal a borrar ", pid2

	self.database.removeProposal(pid2)

	'''
	getProposalObs
	'''
	imageList = self.database.getProposalObservation(pid)
	print imageList


        self.database.clean()

        self.client.releaseComponent(self.database._get_name())

if __name__ == '__main__':
    unittest.main()

