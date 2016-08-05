#!/usr/bin/env python

import unittest
from Acspy.Clients.SimpleClient import PySimpleClient
import TYPES

class TestStorageMethods(unittest.TestCase):
    
    def test_full(self):
        self.client = PySimpleClient()
        self.storage = self.client.getComponent("STORAGE");
        
        self.storage.clearAllData()
        
        targets = []
        images = []
        for i in range(10):
            targets.append(TYPES.Target(i, TYPES.Position(10.0, 45.0), 2))
            image = bytearray()
            for j in range(1000000):
                image.append(j%256)
            images.append(bytes(image))
        proposal = TYPES.Proposal(0, targets, 0)
        self.storage.storeObservation(proposal, images)
        
        id = self.storage.getNextValidId()
        print "ID", id
        self.assertEqual(1, id, "Checking ID")
        
        result = self.storage.getObservation(0)
        self.assertEqual(10, len(result), "Number of observations")
        
        self.storage.clearAllData()
        
        self.client.releaseComponent(self.storage._get_name())
    
if __name__ == '__main__':
    unittest.main()