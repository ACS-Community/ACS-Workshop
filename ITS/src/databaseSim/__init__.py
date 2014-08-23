import TYPES
import Acspy.Clients.SimpleClient
from Acspy.Common.Log import getLogger
from Acssim.Goodies import getGlobalData,setGlobalData

propStatus = [0,0]

def storeProposal(params):
	logger = getLogger("DataBase Logger")
	logger.logTrace("Storing Proposal...")
	return 3

def setProposalStatus(pid, status):
	logger = getLogger("DataBase Logger")
	logger.logTrace("Setting Proposal Status...")
        propStatus[pid-1] = status

def getProposalStatus(pid):
	logger = getLogger("DataBase Logger")
	logger.logTrace("Returning Proposal Status...")
	return propStatus[pid-1]

def getProposals():
	logger = getLogger("DataBase Logger")
	logger.logTrace("Creating Proposals...")
	#Position = TYPES.Position( getGlobalData("currentAz"), getGlobalData("currentEl"))
	Position1 = TYPES.Position( 55.0, 65.0 )
	Position2 = TYPES.Position( 75.0, 66.0 )
	Target1 = TYPES.Target(1, Position1, 10)
	Target2 = TYPES.Target(2, Position2, 30)
	TargetList1 = [Target1, Target2]
	TargetList2 = [Target2]
	Proposal1 = TYPES.Proposal(1, TargetList1, propStatus[0] )
	Proposal2 = TYPES.Proposal(2, TargetList2, propStatus[1] )
	ProposalList = []
	if (propStatus[0] == 0):
		ProposalList.append(Proposal1)
	if (propStatus[1] == 0):
		ProposalList.append(Proposal2)
	logger.logTrace("Returning Proposals...")
	return ProposalList

# ___oOo___
