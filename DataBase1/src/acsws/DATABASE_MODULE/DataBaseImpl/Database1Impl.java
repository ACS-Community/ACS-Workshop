package acsws.DATABASE_MODULE.DataBaseImpl;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.logging.Logger;

import javax.swing.JOptionPane;

import org.eclipse.uml2.uml.RaiseExceptionAction;

import acsws.DATABASE_MODULE.DataBaseOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.ProposalDoesNotExistEx;
import acsws.SYSTEMErr.ProposalNotYetReadyEx;
//import acsws.TYPES.Proposal;
import acsws.TYPES.*;
import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;



public class Database1Impl implements DataBaseOperations, ComponentLifecycle {
	private ContainerServices m_containerServices;
	private Logger m_logger;
	int ind = STATUS_INITIAL_PROPOSAL;
	public ArrayList<Proposal> propArray = new ArrayList<Proposal>();
	public ArrayList<byte[][]> imagArray = new ArrayList<byte[][]>();
	acsws.STORAGE_MODULE.Storage storage;
	int firstPid;
	
	
	//public Database1Impl() {	
	//}
	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.info("initialize() called...");

		try {
			storage = acsws.STORAGE_MODULE.StorageHelper.narrow(m_containerServices.getComponent("STORAGE"));
		} catch (AcsJContainerServicesEx e) {
			m_logger.warning(e.toString());
		}
		storage.clearAllData();
	}
	public ComponentStates componentState() {
		ComponentStates cs =  m_containerServices.getComponentStateManager().getCurrentState();
		return cs;
	}
	public int storeProposal (Target[] target){
		firstPid = storage.getNextValidId();
		Proposal prop;
		int ind = propArray.size();
		prop = new Proposal(ind, target, 0);
		propArray.add(prop);
		byte[][] emptyImage = new byte[target.length][];
		imagArray.add(emptyImage);
		return prop.pid;
	}
	public int getProposalStatus(int pid) throws ProposalDoesNotExistEx{
		if (pid > propArray.size()){
			throw new ProposalDoesNotExistEx();
		}
		if (propArray.get(pid).status == -1){
			throw new ProposalDoesNotExistEx();
		}
		return propArray.get(pid).status;
	}
	public void removeProposal(int pid) {
		if(propArray.isEmpty()==true ){
			System.out.println("ERROR, empty array");
		}else{
			if(propArray.get(pid).status == -1){
				
				System.out.println("Already remove");
				m_logger.warning("PID" + pid +" already remove");
				
			}else{
				propArray.get(pid).status = -1;
				m_logger.warning("The"+ pid +"was removed");
			}
		}
	}
	public Proposal[] getProposals() {
		
		int sizeProp = propArray.size();
		int tempInd = 0;
		for(int i=0; i < sizeProp; i++){
			if(propArray.get(i).status == 0){
				tempInd++;
			}
		}
		Proposal[] tempPropArray = new Proposal[tempInd];
		int j = 0;
		for(int i=0; i < sizeProp; i++){
			if(propArray.get(i).status == 0){
				tempPropArray[j] = propArray.get(i);
				j++;
			}
		}
		return tempPropArray;
	}
	public void setProposalStatus(int pid, int status) throws InvalidProposalStatusTransitionEx,ProposalDoesNotExistEx {
		if (pid > propArray.size()){
			throw new ProposalDoesNotExistEx();
		}
		if (propArray.get(pid).status == -1){
			throw new ProposalDoesNotExistEx();
		}
		if(propArray.get(pid).status == -1){
			m_logger.warning("Proposal number "+ pid + " doesn't exist");
		}else{
			int currentStat = propArray.get(pid).status;
			if (currentStat == 0 && status != 1){
				throw new InvalidProposalStatusTransitionEx();
			}
			if (currentStat == 1 && status != 2){
				throw new InvalidProposalStatusTransitionEx();
			}
			propArray.get(pid).status = status;
			String status_str = "";
			switch(status){
			case 0:
				status_str = "queque";
				break;
			case 1:
				status_str = "running";
				break;
			case 2:
				status_str = "ready";
				break;
			}
			m_logger.info("The status for "+ pid + " was change to " + status_str);
		}
	}
	public void storeImage(int pid, int tid, byte[] image) throws ImageAlreadyStoredEx,ProposalDoesNotExistEx {
		if (pid > propArray.size()){
			throw new ProposalDoesNotExistEx();
		}
		if (propArray.get(pid).status == -1){
			throw new ProposalDoesNotExistEx();
		}
		int tarLen = propArray.get(pid).targets.length;
		byte[][] element = imagArray.get(pid);
		if (element[tid] == null){
			element[tid] = image;
		} else{
			throw new ImageAlreadyStoredEx();
		}
		imagArray.add(pid, element);
		boolean complete = true;
		for (int i = 0; i<tarLen; i++){
			if (element[i] == null){
				complete = false;
				break;
			}
		}
		if (complete == true){
			storage.storeObservation(propArray.get(pid), element);
		}
	}
	public byte[][] getProposalObservations(int pid) throws ProposalNotYetReadyEx {
		byte[][] element = imagArray.get(pid);
		int tarLen = propArray.get(pid).targets.length;
		boolean exception = false;
		for (int i = 0; i<tarLen; i++){
			if (element[i] == null){
				exception = true;
				break;
			}
		}
		if (exception == true){
			throw new ProposalNotYetReadyEx();
		}
		byte[][] imagesStorage = storage.getObservation(pid);
		return imagesStorage;
	}
	
	
	public void execute() throws ComponentLifecycleException {
		m_logger.info("execute() called...");
	}
	public void cleanUp() throws AcsJComponentCleanUpEx {
//		clean();
//		Called after the last functional call to the component has finished. 
//		The component should then orderly release resources etc.
		m_containerServices.releaseComponent("STORAGE");
	}
	public void aboutToAbort() {
		
	}
	
	public String name() {
		return m_containerServices.getName();
	}
	
	public void clean() {
		if(propArray.isEmpty()== true){
			m_logger.warning("Proposals already clean");
		}else{
			for(int i=0;i<propArray.size();i++){
				propArray.remove(i);
			}
			m_logger.info("All proposals clean");
		}
		
		
	}

}
