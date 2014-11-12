import random
import time
from Acspy.Clients.SimpleClient import PySimpleClient
from TYPES import RGB, Position, Target, ImageType

client = PySimpleClient()
m=client.getComponent("DATABASE")
pos = Position(1,2)
target = Target(1,pos,3)
def createProposal():
	pos = Position(random.randint(1,10),random.randint(1,10))
	target = Target(random.randint(1,10),pos,random.randint(1,10))
	return [target] 
m.storeProposal(createProposal())



m=client.getComponent("INSTRUMENT")
m.cameraOn()
m.cameraOff()
m.takeImage(1)
m.setPixelBias(1)
myRGB=RGB(3,4,5)
m.setRGB(myRGB)
m.setResetLevel(1)

print ("tests of INSTRUMENT are finished")
time.sleep(1)

m=client.getComponent("TELESCOPE")
m.getCurrentPosition()
m.moveTo(Position(1,2))
m.observe(Position(1,2),3)
print ("tests of TELESCOPE are finished")
time.sleep(1)

m=client.getComponent("SCHEDULER")
m.start()
m.stop()
m.proposalUnderExecution()

m=client.getComponent("DATABASE")
for i in range (1, 10):
	m.storeProposal(createProposal())
pos = Position(1,2)
target = Target(1,pos,3)
m.storeProposal([target])
m.getProposals()
m.getProposalStatus(0)

#m.getProposalObservations(1)
img=[1,2,3]
m.storeImage(1,1,img)
m.removeProposal(1)


print ("tests of DATABASE are finished")

