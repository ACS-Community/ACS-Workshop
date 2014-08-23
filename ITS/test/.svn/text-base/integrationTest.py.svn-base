#!/usr/bin/env python
import unittest
import time, logging
from SORTErr import *
import TYPES
from Acspy.Clients.SimpleClient import PySimpleClient


class ITS(unittest.TestCase):
    '''
    ITS test cases.
    '''

    prop_id = -1  # Created proposal ID

    def setUp(self):
        '''
        Environment setup.
        '''
        self.client = PySimpleClient()
        self.console = self.client.getComponent("CONSOLE")
        self.database = self.client.getComponent("DATABASE")

    def tearDown(self):
        '''
        Environment cleanup.
        '''
        self.client.releaseComponent("CONSOLE")
        self.client.releaseComponent("DATABASE")

#    def testE2E_1_MANUAL(self):
#        '''
#        End-to-end manual observation use case.
#        '''
#        try:
#        #self.console.setMode(False)
#
#            # Target 1
#            self.console.moveTelescope( TYPES.Position(50.0, 51.0) )
#            self.console.cameraOn()
#            image = self.console.getCameraImage()
#            self.console.cameraOff()
#            assert image is not None
#
#            # Target 2
#            self.console.moveTelescope( TYPES.Position(60.0, 61.0) )
#            self.console.cameraOn()
#            image = self.console.getCameraImage()
#            self.console.cameraOff()
#            assert image is not None
#
#            # Target 3
#            self.console.moveTelescope( TYPES.Position(70.0, 71.0) )
#            self.console.cameraOn()
#            image = self.console.getCameraImage()
#            self.console.cameraOff()
#            assert image is not None
#        except:
#            assert False
#
#        assert True


    def testE2E_3_AUTOMATIC(self):
        '''
        End-to-end automatic observation use case.
        '''
        try:
            target = TYPES.Target(1, TYPES.Position(55.0, 75.0), 10)
            self.prop_id = self.database.storeProposal([target])
            #TODO: look for the stored proposal?
            #target_list = self.database.getProposals()
        except Exception, ex:
            logging.exception("Got an exception!")
            assert False

        try:
            # Start automatic observation
            self.console.setMode(True)
            time.sleep(5)
            # Stop automatic observation
            self.console.setMode(False)
        except Exception, ex:
            logging.exception("Got an exception!")
            assert False

        try:
            images = self.database.getProposalObservations(self.prop_id)
            assert images is not None
        except Exception, ex:
            logging.exception("Got and exception!")
            assert False

        assert True

if __name__ == "__main__":
    unittest.main()

