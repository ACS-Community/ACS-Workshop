#!/usr/bin/env python
# 
# A simple unit tests for Scheduler 1
# 
# @author Exequiel Fuentes
# @author Juan Carlos Olivares
# @author Pablo Valenzuela

import Acspy.Clients.SimpleClient
import unittest

class TestScheduler(unittest.TestCase):
  COMPONENT_NAME = "SCHEDULER1"
  
  def setUp(self):
    self.client = Acspy.Clients.SimpleClient.PySimpleClient()
    self.component = self.client.getComponent(self.COMPONENT_NAME)
  
  def tearDown(self):
    self.client.releaseComponent(self.COMPONENT_NAME)
    self.client.disconnect()

  def test_startOK(self):
    print "Testing start method"
    self.component.start()
  
  def test_stopFail(self):
    print "Testing stop method"
    # It should fail because start method have no been invoked yet.
    #with self.assertRaises(Exception):
    self.component.stop()
  
  def test_proposal_under_execution(self):
    print "Testing stop method"
    proposal = self.component.proposalUnderExecution()
    self.assertEqual(proposal, 0)

if __name__ == '__main__':
    unittest.main()

