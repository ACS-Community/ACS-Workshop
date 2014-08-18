#!/usr/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient
import SORTErr
import TYPES

def help(args):
	print ""
	print "*******************************************************************************"
	print "The following methods are available"
	print ""
	print "\033[91m getMode \033[0m: Get the current mode, default mode is set to Manual(False)"
	print "\033[91m setMode # \033[0m: Set mode, where # is 1:Automatic and 0:Manual"
	print "\033[91m cameraOn \033[0m: Turn on Camera"
	print "\033[91m cameraOff \033[0m: Turn off Camera"
	print "\033[91m getCameraImage \033[0m: Get an image from the camera"
	print "\033[91m setPixelBias # \033[0m: Set the pixel bias configuration of the camera, where # is a bias configuration"
	print "\033[91m setRGB R G B \033[0m: Set the RGB configuration of the camera, eg. setRGB 10 20 30"
	print "\033[91m setResetLevel # \033[0m: Set the reset level configuration of the camera, ex. setResetLevelwhere # is a reset level configuration"
	print "\033[91m moveTelescope Az El \033[0m: Move telescope in synchronous mode, where # is coordinates, eg. moveTelescope 10 20"
	print "\033[91m getTelescopePosition \033[0m: Current telescope position"
	print "\033[91m help \033[0m: print this help"
	print "\033[91m quit \033[0m"
	print "*******************************************************************************"	
		

def setMode(args):	
	arg1 = (int(args[0]) != 0)
	print arg1
	comp.setMode(arg1)

def getMode(args):
	print comp.getMode()
	
def cameraOn(args):
	try:
		comp.cameraOn()
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"
	
def cameraOff(args):
	try:
		comp.cameraOff()
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"
		
def setRGB(args):
	try:
		rgb = TYPES.RGB(int(args[0]), int(args[1]), int(args[2]))
		comp.setRGB(rgb)
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"

def getCameraImage(args):
	try:
		comp.getCameraImage()
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"
	except SORTErr.CameraIsOffEx:
		print "The camera is off"

def setPixelBias(args):
	try:
		comp.setPixelBias(int(args[0]))
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"
	except SORTErr.CameraIsOffEx:
		print "The camera is off"

def setResetLevel(args):
	try:
		comp.setResetLevel(int(args[0]))
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"
	except SORTErr.CameraIsOffEx:
		print "The camera is off"

def moveTelescope(args):
	try:
		pos=TYPES.Position(int(args[0]),int(args[1]))	
		comp.moveTelescope(pos)
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"
		
def getTelescopePosition(args):
	try:
		print comp.getTelescopePosition()
	except SORTErr.SystemInAutoModeEx:
		print "The system is in automatic mode"
	
		
commands = {
	"setMode": setMode,
	"getMode": getMode,
	"cameraOn": cameraOn,
	"cameraOff": cameraOff,
	"getCameraImage": getCameraImage,
	"setRGB": setRGB,
	"setPixelBias": setPixelBias,
	"setResetLevel": setResetLevel,
	"moveTelescope": moveTelescope,
	"getTelescopePosition": getTelescopePosition,
	"help": help,
	"quit": exit
}

if __name__ == "__main__":
	client = PySimpleClient()
	comp = client.getComponent("CONSOLE")
	help("")
	
	while True:
		str = raw_input("#> ")
		try:
			words = str.split(" ")		
			command = commands[words[0]]
			if command:
				command(words[1:])
		except Exception as ex:
			print ex