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

  # Should call start method without issues
  def test_startOK(self):
    self.component.start()
  
  # Should raise an exception if try to call start twice
  def test_start_twice(self):
    with self.assertRaises(Exception):
      self.component.start()
      self.component.start()
  
  # Should call stop method without issues
  def test_stopOK(self):
    self.component.stop()
  
  # Should raise an exception if try to call stop twice
  def test_stop_twice(self):
    with self.assertRaises(Exception):
      self.component.stop()
      self.component.stop()
  
  # Should raise an exception when call proposalUnderExecution without 
  # proposal under execution
  def test_proposalUnderExecution_no_proposal(self):
    with self.assertRaises(Exception):
      proposal = self.component.proposalUnderExecution()
      #self.assertEqual(proposal, -1)
  
  # Should call start method and call stop method
  def test_start_stop(self):
    self.component.start()
    # Just in case thread are so fast
    try:
      print self.component.proposalUnderExecution()
    except:
      pass
    self.component.stop()

if __name__ == '__main__':
    unittest.main()

