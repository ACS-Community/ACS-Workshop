#!/usr/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient

client = PySimpleClient("Component Test Client")

##Test database
#database = client.getComponent("DATABASE") 
#proposals = database.getProposals()
#print proposals.pid
#client.releaseComponent("DATABASE")

#Test instrument
instrument = client.getComponent("INSTRUMENT") 
instrument.cameraOn()
instrument.cameraOff()
client.releaseComponent("INSTRUMENT")

#Test telescope
telescope = client.getComponent("TELESCOPE") 
pos = telescope.getCurrentPosition()
print pos.el
print pos.az
client.releaseComponent("TELESCOPE")

#Test scheduler
scheduler = client.getComponent("SCHEDULER") 
scheduler.start()
scheduler.stop()
client.releaseComponent("SCHEDULER")
