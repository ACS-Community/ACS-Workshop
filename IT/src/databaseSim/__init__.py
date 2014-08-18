import TYPES
import Acspy.Clients.SimpleClient
from Acspy.Common.Log import getLogger
from Acssim.Goodies import getGlobalData,setGlobalData

def storeProposal(params):
	logger = getLogger("DataBase Logger")
	logger.logTrace("Storing Proposal...")
	return 3

def getProposalStatus(params):
	logger = getLogger("DataBase Logger")
	logger.logTrace("Returning Proposal Status...")
	return 1

def getProposals():
	logger = getLogger("DataBase Logger")
	logger.logTrace("Creating Proposals...")
	#Position = TYPES.Position( getGlobalData("currentAz"), getGlobalData("currentEl"))
	Position = TYPES.Position( 45.0, 65.0 )
	Target = TYPES.Target(1, Position, 10)
	TargetList = [Target]
	Proposal = TYPES.Proposal(1, TargetList, 0 )
	ProposalList = [Proposal]
	logger.logTrace("Returning Proposals...")
	return ProposalList


# ___oOo___
