from Acspy.Clients.SimpleClient import PySimpleClient
from TYPES import Position
import unittest
import random

class generalTest(unittest.TestCase):
    
    def setUp(self):
        self._componentname = "TELESCOPECPP"
        self._instrumentname = "INSTRUMENT"
        self._client = PySimpleClient()
        self._telescope = self._client.getComponent(self._componentname)
        self._instrument = self._client.getComponent(self._instrumentname)
    def tearDown(self):
        self._client.releaseComponent(self._componentname)
        self._client.releaseComponent(self._instrumentname)
        self._client.disconnect()
        
    def test_move(self):
        testcases = 10
        for i in range(testcases):
            print "Testing moveTo: {0} of {1}".format(i, testcases)
            az = random.randint(0,360)
            el = random.randint(0,90)
            posini = Position(az,el)
            self._telescope.moveTo(posini)
            pos = self._telescope.getCurrentPosition()
            self.assertEqual(posini.az, pos.az, 'Failed to set Position. posini: {0}, getpos: {1}'.format(posini, pos))
            self.assertEqual(posini.el, pos.el, 'Failed to set Position. posini: {0}, getpos: {1}'.format(posini, pos))
        ## Check bad positions raises exception
        for i in range(testcases):
            print "Testing moveTo with bad arguments: {0} of {1}".format(i, testcases)
            az = random.randint(361,390)
            el = random.randint(91,190)
            posini = Position(az,el)
            self.assertRaises(Exception, self._telescope.moveTo, (posini,))
            
    
    def test_observe(self):
        '''
        '''
        testcases = 2
        self._instrument.cameraOn()
        for i in range(testcases):
            print "Testing observe with camera on: {0} of {1}".format(i, testcases)
            az = random.randint(0,360)
            el = random.randint(0,90)
            timeexp = 1#random.randint(1,1)
            posini = Position(az,el)
            img = self._telescope.observe(posini, timeexp)
        for i in range(testcases):
            print "Testing observe with camera on and bad position: {0} of {1}".format(i, testcases)
            az = random.randint(361,390)
            el = random.randint(91,190)
            timeexp = 1#random.randint(1,1)
            posini = Position(az,el)
            self.assertRaises(Exception, self._telescope.observe, (posini, timeexp))
        self._instrument.cameraOff()
        for i in range(testcases):
            print "Testing observe with camera off: {0} of {1}".format(i, testcases)
            az = random.randint(0,360)
            el = random.randint(0,90)
            timeexp = 1#random.randint(1,1)
            posini = Position(az,el)
            self.assertRaises(Exception, self._telescope.observe, (posini, timeexp))
            
            
            
        
        
    
if __name__ == '__main__':
    unittest.main()