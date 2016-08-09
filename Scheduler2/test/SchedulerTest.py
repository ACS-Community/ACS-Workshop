import Acspy.Clients.SimpleClient
import unittest

class SimpleTestCase(unittest.TestCase):


	@classmethod
	def setUpClass(cls):
		cls.client = Acspy.Clients.SimpleClient.PySimpleClient()
		cls.component = cls.client.getComponent('Scheduler2')

	@classmethod
	def tearDownClass(cls):
		pass
		#cls.component.disconnect()

	def test_start(self):
		self.component.start()

	def test_stop(self):
		self.component.stop()

	def test_proposal(self):
		print self.component.proposalUnderExecution()
		
if __name__ == '__main__':
	unittest.main()

