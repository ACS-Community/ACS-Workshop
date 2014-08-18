from Acspy.Clients.SimpleClient import PySimpleClient
from Acspy.Common.Log import getLogger
import TYPES
import sys

# ----------------------------------------
logger = getLogger("DataQueryMaker")

logger.logInfo("TEST: Initializing DataQueryMaker...")

# Get DataBase component
sc = PySimpleClient()
dbimpl = sc.getComponent("DATABASE")

if (0 == float(sys.argv[1]) and 0 == float(sys.argv[2]) and 0 == int(sys.argv[3])):
	dbimpl.clean()
	logger.logInfo("TEST: DB cleaned")
	sys.exit()

logger.logInfo("TEST: Adding Proposal: Az:" + sys.argv[1] + " - El:"+ sys.argv[2] + " - T:" + sys.argv[3] + " ...")

# Create a proposals:

pos_one = TYPES.Position(float(sys.argv[1]), float(sys.argv[2]))

targ_one = TYPES.Target(1,pos_one,int(sys.argv[3]))

# Store proposals:
tl_one = [targ_one]
prop_id_one = dbimpl.storeProposal(tl_one)
stat_one = dbimpl.getProposalStatus(prop_id_one)
logger.logInfo("TEST: Proposal with ID: " + str(prop_id_one) + " stored. Current Status: " + str(stat_one) )

# Release DataBase
sc.releaseComponent("DATABASE")


