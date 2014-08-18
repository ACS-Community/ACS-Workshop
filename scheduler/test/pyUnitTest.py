import unittest
from Acspy.Clients.SimpleClient import PySimpleClient


class HelloDemoTest(unittest.TestCase):
    def setUp(self):
        self.client = PySimpleClient()
        self.comp = client.getComponent("COMPONENT_NAME")

    def testSayHello(self):
        hello = self.comp.sayHello()
        assert hello is not None

    def tearDown(self):
        self.client.releaseComponent("COMPONENT_NAME")


if __name__ == "__main__":
    unittest.main()
