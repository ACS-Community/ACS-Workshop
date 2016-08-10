package acsws.DATABASE_MODULE.DataBaseImpl;
import java.util.ArrayList;
import java.util.logging.Logger;
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
	acsws.STORAGE_MODULE.Storage storage;
	
	
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
	}
	public ComponentStates componentState() {
		ComponentStates cs =  m_containerServices.getComponentStateManager().getCurrentState();
		return cs;
	}
	public int storeProposal (Target[] target){
		Proposal prop;
		int ind = propArray.size();
		prop = new Proposal(ind, target, 0);
		propArray.add(prop);
		//seeProp();
		return prop.pid;
	}
	public int getProposalStatus(int pid) throws ProposalDoesNotExistEx{
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
		if(propArray.get(pid).status != -1){
			m_logger.warning("Proposal number "+ pid + " doesn't exist");
		}else{
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
		byte[][] imagesStorage = storage.getObservation(pid);
		imagesStorage[tid] = image;
		storage.storeObservation(propArray.get(pid), imagesStorage);
	}
	public byte[][] getProposalObservations(int pid) throws ProposalNotYetReadyEx {
		byte[][] imagesStorage = storage.getObservation(pid);
		return imagesStorage;
	}
	
	
	public void execute() throws ComponentLifecycleException {
		
	}
	public void cleanUp() throws AcsJComponentCleanUpEx {
		
	}
	public void aboutToAbort() {
		
	}
	
	public String name() {
		return m_containerServices.getName();
	}
	
	
	


	
	public void clean() {
		
	}

}
