#!/usr/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient

print "Creating client"
client = PySimpleClient("Component Test Client")
print "Client created"
print ""

#Test scheduler
print "Obtaining scheduler component"
scheduler = client.getComponent("SCHEDULER") 
print "Retrieved scheduler component. Testing component."
print "Testing start()"
scheduler.start()
print "Testing stop()"
scheduler.stop()
print "Testing proposalUnderExecution(): ",scheduler.proposalUnderExecution()
print "Releasing scheduler component"
client.releaseComponent("SCHEDULER")
print "Released scheduler component"
print ""
