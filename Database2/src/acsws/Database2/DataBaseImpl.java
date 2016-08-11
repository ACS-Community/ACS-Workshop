package acsws.Database2;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.hibernate.type.ImageType;

import acsws.DATABASE_MODULE.DataBase;
import acsws.DATABASE_MODULE.DataBaseHelper;
import acsws.DATABASE_MODULE.DataBaseOperations;
import acsws.STORAGE_MODULE.Storage;
import acsws.STORAGE_MODULE.StorageHelper;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.ProposalNotYetReadyEx;
import acsws.SYSTEMErr.ProposalDoesNotExistEx;
import acsws.TYPES.ImageListHelper;
import acsws.TYPES.Proposal;
import acsws.TYPES.Target;
import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.acs.logging.AcsLogger;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;


public class DataBaseImpl implements ComponentLifecycle, DataBaseOperations {

	
	// Implementation of ACSComponent 
	
	private AcsLogger m_logger;
	private ContainerServices m_containerServices;
	private Proposal proposal;
	private static int pid_cnt ;
	int STATUS_INITIAL_PROPOSAL = 0;
	int STATUS_NO_SUCH_PROPOSAL = -999;
	List<Proposal> proposalList = new ArrayList<Proposal>();
	byte[][] imageList; 
	int[][][] map;
	int image_cnt = 0;
	
	
	public String name() {
		// TODO Auto-generated method stub
		return "DATABASE_MODULE_JAVA";
	}

	public ComponentStates componentState() {
		// TODO Auto-generated method stub
		System.out.print("We are Defunct ;D");
		return ComponentStates.COMPSTATE_DEFUNCT;
	}

	// Implementation of DataBaseOperations
	
	public int storeProposal(Target[] targets) {
		// TODO Auto-generated method stub
		pid_cnt++;
		proposal = new Proposal();
		proposal.targets = targets;
		proposal.pid = pid_cnt; 
		proposal.status = STATUS_INITIAL_PROPOSAL;
		proposalList.add(proposal);
		imageList = new byte[targets.length][];
		map = new int[targets.length][targets.length][targets.length];
		
		return proposal.pid;
	}

	public int getProposalStatus(int pid) throws ProposalDoesNotExistEx{
		// TODO Auto-generated method stub
		if (proposalList == null){
		   m_logger.info("The proposal list is empty");
		   throw new ProposalDoesNotExistEx();
		}
		for (Proposal pro : proposalList ) {
			if (pro.pid == pid){
				return pro.status;
			}
		}
		m_logger.info("The proposal does not exists");
		throw new ProposalDoesNotExistEx();
	}

	public void removeProposal(int pid) {
		// TODO Auto-generated method stub
		m_logger.info("Deleting proposal");
		for (int i = 0; i < proposalList.size() ; i++) {
			if (proposalList.get(i).pid == pid){
				proposalList.remove(i);
				break;
			}
		}
	}

	public byte[][] getProposalObservations(int pid)
			throws ProposalNotYetReadyEx {
		// TODO Auto-generated method stub
		Storage store = null;
		byte[][] imageList = new byte[0][0];
		try { 
			store = StorageHelper.narrow( m_containerServices.getComponent("STORAGE") );
		} catch ( AcsJContainerServicesEx e) {
			m_logger.info("Excepcion de Java Container con Componente Storage");
			return imageList;
		}
		
		imageList = store.getObservation(pid);
		
		if (imageList.length == 0){
			m_logger.info("ProposalNotYetReadyEx");
			ProposalNotYetReadyEx e = new ProposalNotYetReadyEx();
			throw e;
		}
		return imageList;
	}

	public Proposal[] getProposals() {
		// Returns stored proposals which have not been executed yet.
		// @return Proposals with queued status. If there are no pending proposals returns an empty list
		
		Proposal[] list = new Proposal[proposalList.size()];
		for (int i = 0; i < proposalList.size() ; i++) {
			list[i] = proposalList.get(i);
			}
		return list;
	}

	public void setProposalStatus(int pid, int status)
			throws InvalidProposalStatusTransitionEx, ProposalDoesNotExistEx {
		long statusActual = -1;
		try {
			statusActual = getProposalStatus(pid);
		} catch (ProposalDoesNotExistEx e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
			m_logger.info("ProposalDoesNotExistEx");
			throw e1;
		}
		if (statusActual + 1 != status){
			m_logger.info("InvalidProposalStatusTransitionEx");
			InvalidProposalStatusTransitionEx e = new InvalidProposalStatusTransitionEx();
			throw e;
		}
		for (Proposal pro : proposalList ) {
			if (pro.pid == pid){
				pro.status = status;
				m_logger.info("Proposal Status Changed Successfully");
				break;
			}
		}		
	}

	public void storeImage(int pid, int tid, byte[] image)
			throws ImageAlreadyStoredEx, ProposalDoesNotExistEx{
		// TODO Auto-generated method stub
		Storage store = null;
		try { 
			store = StorageHelper.narrow( m_containerServices.getComponent("STORAGE") );
		} catch ( AcsJContainerServicesEx e) {
			m_logger.info("Excepcion de Java Container con Componente Storage");
		}
		
		for (Proposal pro : proposalList ) {
			if (pro.pid == pid){
				if (pro.status == 1){
					for (Target targetList : pro.targets ) {
						if(targetList.tid == tid){
							// Revisar si tid ya tiene imagen
							for (int j = 0; j < pro.targets.length ; j++ ){
								if (map[pid][tid][j] == 0 ) {
									// Store locally or in memory
									imageList[image_cnt] = new byte[image.length];
									image_cnt++;
									map[pid][tid][j] = 1;
									break;
								}
							}
							m_logger.info("ImageAlreadyStoredEx");
							throw new ImageAlreadyStoredEx();							
						}
					}
				}
					
					//m_logger.warning("");
			}
			m_logger.info("ProposalDoesNotExistEx");
			throw new ProposalDoesNotExistEx();
		}
	}
		
			

	public void clean() {
		// TODO Auto-generated method stub
		proposalList.clear();
		m_logger.info("All Proposals cleared");
	}

	// Implementation of LifeCycle
	
	public void initialize(ContainerServices containerServices)
			throws ComponentLifecycleException {
		m_containerServices = containerServices;
		m_logger = containerServices.getLogger();
		m_logger.info("Initialize DB");
		// TODO Auto-generated method stub
		
	}

	public void execute() throws ComponentLifecycleException {
		// TODO Auto-generated method stub
		m_logger.info("Executing DB");
		pid_cnt = 0;
	}

	public void cleanUp() throws AcsJComponentCleanUpEx {
		// TODO Auto-generated method stub
		m_logger.info("DB cleanup");
	}

	public void aboutToAbort() {
		// TODO Auto-generated method stub
		m_logger.info("DB aboutToAbort");
	}

}
