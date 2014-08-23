import TYPES
import Acspy.Clients.SimpleClient
from Acspy.Common.Log import getLogger

def cameraOn():
	logger = getLogger("Instrument Logger")
	logger.logTrace("Turning Camera On...")

def cameraOff():
	logger = getLogger("Instrument Logger")
	logger.logTrace("Turning Camera Off...")

def takeImage(params):
        logger = getLogger("Instrument Logger")
        logger.logTrace( "Taking image from devCCD")
#       pass
#       print "Taking image..."
#       for i in range(0, 640):
#               for j in range(0, 480):
#                       img[i][j] = 0
#       print img
        return [666, 111]

# ___oOo___
