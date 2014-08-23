import TYPES
import Acspy.Clients.SimpleClient
from Acspy.Common.Log import getLogger

def moveTo(params):
	logger = getLogger("Telescope Logger")
	logger.logTrace("Moving to Position...")
	return 3

def getCurrentPosition():
	logger = getLogger("Telescope Logger")
	logger.logTrace("Returning the current position...")
	return TYPES.Position(45.0,60.0)

def observe(position,exposureTime):
	logger = getLogger("Telescope Logger")
	logger.logTrace("Observing...")
	return [9,8,7,6,5,4,3,2,1,0]

# ___oOo___
