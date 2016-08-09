package acsws.DATABASE_MODULE.DataBaseImpl;
import java.util.ArrayList;
import java.util.logging.Logger;


import com.google.inject.util.Types;

import acsws.DATABASE_MODULE.DataBaseOperations;
import acsws.STORAGE_MODULE.StorageOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.ProposalNotYetReadyEx;
//import acsws.TYPES.Proposal;
import acsws.TYPES.*;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;



public class Database1Impl implements DataBaseOperations, ComponentLifecycle, StorageOperations {
	private ContainerServices m_containerServices;
	private Logger m_logger;
	//int ind = getNextValidId();
	int ind = STATUS_INITIAL_PROPOSAL;
	public ArrayList propArray = new ArrayList();
	
	public Database1Impl() {
		
	}
	
	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.info("initialize() called...");
	}

	public int storeProposal (Target[] target){
		Proposal prop;
		prop = new Proposal(ind, target, 0);
		propArray.add(prop);
		ind++;
		return ind;
	}
	public long getProposalStatus(long pid){
		return 2;
	}
	public String name() {
		// TODO Auto-generated method stub
		return null;
	}
	public ComponentStates componentState() {
		// TODO Auto-generated method stub
		return null;
	}
	public int getProposalStatus(int pid) {
		// TODO Auto-generated method stub
		return 0;
	}
	public void removeProposal(int pid) {
		// TODO Auto-generated method stub
		
	}
	public byte[][] getProposalObservations(int pid)
			throws ProposalNotYetReadyEx {
		// TODO Auto-generated method stub
		return null;
	}
	public Proposal[] getProposals() {
		// TODO Auto-generated method stub
		return null;
	}
	public void setProposalStatus(int pid, int status)
			throws InvalidProposalStatusTransitionEx {
		// TODO Auto-generated method stub
		
	}
	public void storeImage(int pid, int tid, byte[] image)
			throws ImageAlreadyStoredEx {
		// TODO Auto-generated method stub
		
	}
	public void clean() {
		// TODO Auto-generated method stub
		
	}
	public void execute() throws ComponentLifecycleException {
		// TODO Auto-generated method stub
		
	}
	public void cleanUp() throws AcsJComponentCleanUpEx {
		// TODO Auto-generated method stub
		
	}
	public void aboutToAbort() {
		// TODO Auto-generated method stub
		
	}
	public int getNextValidId() {
		// TODO Auto-generated method stub
		return 0;
	}
	public void storeObservation(Proposal prop, byte[][] images) {
		// TODO Auto-generated method stub
		
	}
	public void clearAllData() {
		// TODO Auto-generated method stub
		
	}
	public byte[][] getObservation(int pid) {
		// TODO Auto-generated method stub
		return null;
	}

}
