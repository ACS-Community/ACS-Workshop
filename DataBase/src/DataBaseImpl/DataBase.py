# vim: ts=4

#--CORBA STUBS-----------------------------------------------------------------
import DATABASE_MODULE__POA
#--ACS Imports-----------------------------------------------------------------
from Acspy.Servants.ACSComponent		import ACSComponent
from Acspy.Servants.ContainerServices	import ContainerServices
from Acspy.Servants.ComponentLifecycle	import ComponentLifecycle
from Acspy.Common.Log					import getLogger
#------------------------------------------------------------------------------
import TYPES
#------------------------------------------------------------------------------
# import sqlite3  ¡¡bug!!
# ACS python share a broken sqlite :(
# - Download pysqlite2 from:
# http://oss.itsystementwicklung.de/trac/pysqlite/wiki/WikiStart#Downloads
# - Install sqlite-devel (python-sqlite and sqlite as dependence) in the system.
# - Untar pysqlite-2.5.tgz and _enter_ to the generated directory
# - Execute 'python setup.py install'
# - Add the path to the compiled module to $PYTHONPATH
#   Example: $HOME/pysqlite-2.5.1/build/lib.linux-i686-2.5
# - Enjoy! :D

from pysqlite2 import dbapi2 as sqlite
import os
import marshal


## Class DataBase Implementation
#
class DataBase(DATABASE_MODULE__POA.DataBase,
	ACSComponent,
	ContainerServices,
	ComponentLifecycle):
	#------------------------------------------------------------------------------
	'''
	Class Database Implementation
	'''
	#------------------------------------------------------------------------------

	def __init__(self):
		'''
		Just call superclass constructors here.
		'''
		self.dbname="DataBase.sqlite"
		self.dbpath=os.getenv("HOME")+"/"+self.dbname

		ACSComponent.__init__(self)
		ContainerServices.__init__(self)

		self.logger = getLogger("DataBaseImpl")
		
		self.logger.logTrace("Attempting DataBase Connection...")
		
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		query=cursor.execute("SELECT name FROM sqlite_master WHERE name='proposal';")
		if len(query.fetchall()) == 0:
			cursor.execute('''
--
-- @author Mauricio Cardenas
-- @author Renato Covarrubias
-- @author Exequiel Fuentes

--
-- Create UOS DataBase SQLite
--

-- Create proposal table
CREATE TABLE proposal (
--      every table as a ROWID. Foreach row is uniq.
        status INTEGER NOT NULL
);
''')
			self.logger.logDebug("Table 'proposal' Created")

		query = cursor.execute("SELECT name FROM sqlite_master WHERE name='target';")
		if len(query.fetchall()) == 0:
			cursor.execute('''
-- Create target table
CREATE TABLE target (
--      every table as a ROWID. Foreach row is uniq.
        proposal INTEGER NOT NULL,
		target_id INTEGER NOT NULL,
        az REAL NOT NULL,
        el REAL NOT NULL,
        exp_time INTEGER NOT NULL,
		image TEXT NULL
);
''')
			self.logger.logDebug("Table 'target' Created")

		db.close()

		self.logger.logInfo("DataBase Object initialized")
		return
	#------------------------------------------------------------------------------
#  Will it fail?
#
#	def  initialize(self):
#		pass
#	#------------------------------------------------------------------------------


	## Stores a new Proposal.
	# 
	# @param targets Target list composing this proposal.
	# @return Assigned proposal ID (pid).

	def storeProposal (self, targets):
		# Creating proposal ID, so we have create a new proposal
		# status (0 - queued, 1 - running, 2 - ready), default is 0
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		self.logger.logTrace("Creating proposal ID...")
		cursor.execute('INSERT INTO proposal(status) VALUES (0)')
		db.commit()
		self.logger.logDebug("Proposal ID created!")
		
		# Getting the last PKid from proposal table
		self.logger.logTrace("Getting the last Proposal ID...")
		query = cursor.execute('SELECT MAX(ROWID) FROM proposal')
		proposal_id = query.fetchone()[0]
		self.logger.logDebug("The last Proposal ID get: " + str(proposal_id))
		
		# Now, we are going to insert the targets
		self.logger.logTrace("Inserting Targets...")
		sql = 'INSERT INTO target(proposal,target_id,az,el,exp_time) VALUES (?,?,?,?,?)'
		for i in targets:
			cursor.execute(sql, (proposal_id, i.tid, i.coordinates.az,
				i.coordinates.el, i.expTime))
			db.commit()
			self.logger.logDebug("Target "+str(i.tid)+" inserted")
		self.logger.logInfo("Targets inserted!")
	
		db.close()
	
		l = long(proposal_id)
		return l
	#------------------------------------------------------------------------------
	
	## Get the current proposal status for the given proposal.
	#
	# @param pid Proposal ID
	# @return status
	def getProposalStatus(self, pid):
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		self.logger.logTrace("Getting Proposal Status...")
		query = cursor.execute("SELECT status FROM proposal WHERE ROWID=?", (pid,))
		result = query.fetchall()
		if len(result) == 0:
			self.logger.logError("Proposal ID "+str(pid)+" does NOT exist.")
			r = long(-1)
		else:
			r = long(result[0][0])
			self.logger.logInfo("Proposal ID "+str(pid)+" is "+str(r))
	
		db.close()
	
		return r
	#------------------------------------------------------------------------------
	
	## Remove proposal.
	# 
	# @param pid Proposal ID
	def removeProposal(self, pid):
		self.logger.logTrace("Removing Proposal...")
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		cursor.execute("DELETE FROM target WHERE proposal=?", (pid,))
		cursor.execute("DELETE FROM proposal WHERE ROWID=?", (pid,))
		db.commit()
		self.logger.logInfo("Proposal "+str(pid)+" Deleted")
		db.close()
	#------------------------------------------------------------------------------

	## Retuns all images for a given proposal. 
	# Raises an exception if proposal has not been 
	# executed yet. (raises(UOSErr::ProposalNotYetReadyEx);)
	#
	# @param pid Proposal ID
	# @return Image list that belongs to this proposal
	def getProposalObservations(self, pid):
		imageList=[]
		self.logger.logTrace("Getting Proposal Observations...")
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		query = cursor.execute("SELECT image FROM target WHERE proposal=?", (pid,))
		images = query.fetchall()
		for image in images:
			imageList.append([long(i) for i in img[0].split(";")])
		self.logger.logInfo("Proposals Observations Get!")
		return imageList
	#------------------------------------------------------------------------------

	## Returns stored proposals which have not been executed yet.
	#
	# @return Proposals with queued status. If there are no 
	# pending proposals returns an empty list

	def getProposals(self):
		proposalList = []
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		self.logger.logTrace("Getting proposals...")
		query = cursor.execute("SELECT ROWID,status FROM proposal ORDER BY ROWID")
		props = query.fetchall()
		for prop in props:
			targetList = []
			query = cursor.execute("SELECT ROWID,az,el,exp_time FROM target WHERE proposal=?", (prop[0],))
			targets = query.fetchall()
			for targ in targets:
				positionType = TYPES.Position(targ[1], targ[2])
				targetType = TYPES.Target(targ[0], positionType, targ[3])
				targetList.append(targetType)

			pro = TYPES.Proposal(prop[0], targetList, prop[1])
			proposalList.append(pro)
		
		self.logger.logInfo("Proposals Get!")
		
		return proposalList
	#------------------------------------------------------------------------------

	## Set the proposal status. Raises an exception if the change is not from
	# queued(0) to running(1) or from running(1) to ready(2).
	#
	# @param pid Proposal ID
	# @param status New Status
	# @return None

	def setProposalStatus(self, pid, status):
		# raises(UOSErr::InvalidProposalStatusTransitionEx);
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		self.logger.logTrace("Getting Proposal Status...")
		query = cursor.execute("SELECT status FROM proposal WHERE ROWID=?", (pid,))
		statusActual = query.fetchone()
		if statusActual[0] == 0:
			if status == 1:
				query = cursor.execute("UPDATE proposal SET status=1 WHERE ROWID=?", (pid,))
				db.commit()
				self.logger.logInfo("Proposal "+str(pid)+" updated. Now 1")
			else:
				self.logger.logError("Proposal Transaction Invalid. 0->"+str(status))
		elif statusActual[0] == 1:
			if status == 2:
				query = cursor.execute("UPDATE proposal SET status=2 WHERE ROWID=?", (pid,))
				db.commit()
				self.logger.logInfo("Proposal "+str(pid)+" updated. Now 2")
			else:
				self.logger.logError("Proposal Transaction Invalid. 1->"+str(status))
		elif statusActual[0] == 2:
			self.logger.logError("Proposal Transaction Invalid. Proposal is ready.")
		else:
			self.logger.logError("Proposal Original Status Unknown ("+str(statusActual[0])+")")
		db.close()
	#------------------------------------------------------------------------------

	## Stores an image for a given proposal and target. Raises an exception if
	# an image has already been stored for the given tid and pid.
	# (raises(UOSErr::ImageAlreadyStoredEx);)
	#
	# @param pid Proposal ID
	# @param tid target ID
	# @param image ImageType
	# @return None

	def storeImage(self, pid, tid, image):
		self.logger.logTrace("Storing image...")
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		query = cursor.execute("SELECT image FROM target WHERE proposal=? AND target_id=?", (pid,tid,))
		imageObj = query.fetchone()
		if imageObj[0] == None:
			image_str=";".join([str(i) for i in image])
			
			cursor.execute('UPDATE target SET image=? WHERE proposal=? AND target_id=?', (image_str,pid,tid,))
			db.commit()
			self.logger.logInfo("Image stored for proposal "+str(pid)+" and target_id "+str(tid))
		else:
			self.logger.logError("Image already exists!")
		db.close()
#		# Feature for rtobar
#		filename = os.getenv("HOME")+"/img/"+str(pid)+"-"+str(tid)+".raw"
#		f = open(filename, "w")
#		f.write(image)
#		f.close()
	#------------------------------------------------------------------------------

	## Clean all the proposals
	#
	def clean(self):
		self.logger.logTrace("Cleaning all proposals...")
		db = sqlite.connect(self.dbpath)
		cursor = db.cursor()
		query = cursor.execute("DELETE FROM target")
		query = cursor.execute("DELETE FROM proposal")
		db.commit()
		db.close()
		self.logger.logInfo("All proposals cleaned")
	#------------------------------------------------------------------------------

#
# ___oOo___
