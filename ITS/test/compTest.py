#!/usr/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient

print "Creating client"
client = PySimpleClient("Component Test Client")
print "Client created"
print ""

#Test database
print "Obtaining instrument component"
database = client.getComponent("DATABASE") 
print "Retrieved instrument component. Testing component."
proposals = database.getProposals()
print proposals.pid
print "Releasing instrument component"
client.releaseComponent("DATABASE")
print "Released instrument component"
print ""

#Test instrument
print "Obtaining instrument component"
instrument = client.getComponent("INSTRUMENT") 
print "Retrieved instrument component. Testing component."
instrument.cameraOn()
instrument.cameraOff()
print "Releasing instrument component"
client.releaseComponent("INSTRUMENT")
print "Released instrument component"
print ""

#Test telescope
print "Obtaining telescope component"
telescope = client.getComponent("TELESCOPE") 
print "Retrieved telescope component. Testing component."
pos = telescope.getCurrentPosition()
print pos.el
print pos.az
print "Releasing telescope component"
client.releaseComponent("TELESCOPE")
print "Released telescope component"
print ""

#Test scheduler
print "Obtaining scheduler component"
scheduler = client.getComponent("SCHEDULER") 
print "Retrieved scheduler component. Testing component."
scheduler.start()
scheduler.stop()
print "Releasing scheduler component"
client.releaseComponent("SCHEDULER")
print "Released scheduler component"
print ""
