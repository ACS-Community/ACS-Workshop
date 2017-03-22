from Acspy.Clients.SimpleClient import PySimpleClient
import unittest
import SYSTEMErr

class TestScheduler(unittest.TestCase):

  def setUp(self):
    self.client = PySimpleClient.getInstance()
    self.scheduler = self.client.getComponent("Scheduler3_JAVA")
    self.scheduler.start()

  def tearDown(self):
    self.client.releaseComponent("Scheduler3_JAVA")

  def test_proposalCheck(self):
    try:
      self.assertTrue(isinstance(self.scheduler.proposalUnderExecution(), int))
    except SYSTEMErr.NoProposalExecutingEx:
      print "Scheduler expected 0"

  def test_startCheck(self):
    try:
      self.scheduler.start()
    except SYSTEMErr.SchedulerAlreadyRunningEx:
      print "Scheduler is already running"

  def test_stopCheck(self):
    try:
      self.scheduler.stop()
    except SYSTEMErr.SchedulerAlreadyStoppedEx:
      print "Scheduler is already stopped"


if __name__ == '__main__':
  unittest.main()
