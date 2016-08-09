import unittest
import Acspy.Clients.SimpleClient as sc
import TYPES as tp

class TelescopeTestCase(unittest.TestCase):
	def setUp(self):
		client = sc.PySimpleClient()
		self.component = client.getComponent("TELESCOPE")
		
	def testObserve(self):
		pass

	def testMoveTo(self):
		pass

	def testGetCurrentPosition(self):
		p = tp.Position(90,50)
		self.component.moveTo(p)
		assert (p.az == 90 and p.az== 50), "Error"


if __name__ == "__main__":
	unittest.main()
