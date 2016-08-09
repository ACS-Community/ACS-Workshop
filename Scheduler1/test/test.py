#

import Acspy.Clients.SimpleClient
import unittest

class TestScheduler(unittest.TestCase):
  def setUp(self):
    client = Acspy.Clients.SimpleClient.PySimpleClient()
    self.component = client.getComponent('SCHEDULER1')
  
  def tearDown(self):
    pass

  def test_start(self):
    #self.assert
    print "Invoking start method"
    self.component.start()

if __name__ == '__main__':
    unittest.main()

