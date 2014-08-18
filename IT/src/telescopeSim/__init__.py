import TYPES
import Acspy.Clients.SimpleClient
from Acspy.Common.Log import getLogger

def moveTo(params):
	logger = getLogger("Telescope Logger")
	logger.logTrace("Moving to Position...")
	return 3

def getCurrentPosition(params):
	logger = getLogger("Telescope Logger")
	logger.logTrace("Returning the current position...")
	return TYPES.Position(45.0,60.0)

# ___oOo___
