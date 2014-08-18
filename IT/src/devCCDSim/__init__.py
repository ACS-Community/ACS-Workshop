import TYPES
import Acspy.Clients.SimpleClient
from Acspy.Common.Log import getLogger

def on():
	logger = getLogger("DevCDD Logs")
	logger.logTrace( "Camera ON ...")

def off():
	logger = getLogger("DevCDD Logs")
	logger.logTrace( "Camera OFF...")

def lock():
	logger = getLogger("DevCDD Logs")
	logger.logTrace( "Camera LOCKED...")

def unlock():
	logger = getLogger("DevCDD Logs")
	logger.logTrace( "Camera UNLOCKED...")

def image(params):
	logger = getLogger("DevCCD Logs")
	logger.logTrace( "Taking image from Camera")
#	pass
#	print "Taking image..."
#	for i in range(0, 640):
#		for j in range(0, 480):
#			img[i][j] = 0
#	print img
	return [666, 111]
#
# ___oOo___
