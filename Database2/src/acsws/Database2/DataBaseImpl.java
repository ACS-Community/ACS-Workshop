package acsws.Database2;

import java.util.ArrayList;
import java.util.Arrays;
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
import acsws.SYSTEMErr.wrappers.AcsJInvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.wrappers.AcsJProposalDoesNotExistEx;
import acsws.SYSTEMErr.wrappers.AcsJProposalNotYetReadyEx;
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
	int STATUS_INITIAL_PROPOSAL = 0;
	int STATUS_NO_SUCH_PROPOSAL = -999;
	List<Proposal> proposalList = new ArrayList<Proposal>();
	 
	HashMap<String, byte[]> map = new HashMap<String, byte[]>();
	
	
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
		Storage store = null;
		try { 
			store = StorageHelper.narrow( m_containerServices.getComponent("STORAGE") );
		} catch ( AcsJContainerServicesEx e) {
			m_logger.info("Excepcion de Java Container con Componente Storage");
			e.log(this.m_logger);
			return 0;
		}
		
		proposal = new Proposal();
		proposal.targets = targets;
		proposal.pid = store.getNextValidId(); 
		proposal.status = STATUS_INITIAL_PROPOSAL;
		proposalList.add(proposal);
		//imageList = new byte[targets.length][];
		for (Target tar : targets){
			map.put(Integer.toString(proposal.pid) + ":" + Integer.toString(tar.tid) , null);
		}
		
		m_logger.info("The proposal was created. PID: " + proposal.pid );
		return proposal.pid;
	}

	public int getProposalStatus(int pid) throws ProposalDoesNotExistEx{
		// TODO Auto-generated method stub

		if (proposalList.isEmpty()){
		   m_logger.info("The proposal list is empty");
		   AcsJProposalDoesNotExistEx impl = new AcsJProposalDoesNotExistEx();
		   throw impl.toProposalDoesNotExistEx();
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
		long statusActual = -1;
		byte[][] imageList = null ;
		int target_cnt = 0;
		
		try { 
			store = StorageHelper.narrow( m_containerServices.getComponent("STORAGE") );
		} catch ( AcsJContainerServicesEx e) {
			m_logger.info("Excepcion de Java Container con Componente Storage");
			e.log(this.m_logger);
			return imageList;
		}
		
		try {
			statusActual = getProposalStatus(pid);
			// Si la proposal no ha sido completada
			if (statusActual != 2){
				m_logger.info("ProposalNotYetReadyEx");
				ProposalNotYetReadyEx e = new ProposalNotYetReadyEx();
				throw e;
			}
		} catch (ProposalDoesNotExistEx e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
			m_logger.info("ProposalDoesNotExist in memory, but may exist in storage");
		}
		

		// Se cuentan los target en targetList
		for (String key_pid : map.keySet()){
			if(key_pid.startsWith(Integer.toString(pid) + ":") ) {
				target_cnt++;
			}
		}		
		imageList = new byte[target_cnt][];
		
		imageList = store.getObservation(pid);
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
		int image_cnt = 0, bool_cnt = 0;
		List<String> pids_list = new ArrayList<String>();
                byte[][] imageList = null ;
		Storage store = null;
                
		try {
                        store = StorageHelper.narrow(m_containerServices.getComponent("STORAGE"));
                } catch ( AcsJContainerServicesEx e) {
                        m_logger.info("Excepcion de Java Container con Componente Storage");
                }


		try {
			statusActual = getProposalStatus(pid);
		} catch (ProposalDoesNotExistEx e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
			m_logger.info("ProposalDoesNotExistEx");
			AcsJProposalDoesNotExistEx e2 = new AcsJProposalDoesNotExistEx(e1);
			throw e2.toProposalDoesNotExistEx();
		}

		if (statusActual + 1 != status ){
			m_logger.info("InvalidProposalStatusTransitionEx");
			AcsJInvalidProposalStatusTransitionEx e = new AcsJInvalidProposalStatusTransitionEx();
			throw e.toInvalidProposalStatusTransitionEx();
			//throw e;
			 //  AcsJProposalDoesNotExistEx impl = new AcsJProposalDoesNotExistEx();
			  // throw impl.toProposalDoesNotExistEx();
		}
		for (Proposal pro : proposalList ) {
			if (pro.pid == pid){
				pro.status = status;
				m_logger.info("Proposal Status Changed Successfully");

				if (status == 2){
				    for (String key_pid : map.keySet()){
                                        if(key_pid.startsWith(Integer.toString(pid) + ":") ) {
                                            image_cnt++;
					    if ( map.get(key_pid) != null ) {
                                	        bool_cnt++;
                                                pids_list.add(key_pid);
					    }
					}
				     }

				    //if(image_cnt == bool_cnt){
                                    imageList = new byte[image_cnt][];
				    for( int l = 0; l < pids_list.size() ; l++){
			 	        // Guardar aun cuando no estan todas las imagenes
			 	        imageList[l] = map.get( pids_list.get(l) );
					if (map.get( pids_list.get(l) ) == null ) {
					    m_logger.info("Saving incomplete proposal");
					    Arrays.fill( imageList[l], (byte) 0 );
   				        }					  
                                    }
				    store.storeObservation(pro,  imageList);
                                    // Eliminar imagenes 
                                    for( String image_delete : pids_list){
				    map.replace(image_delete, null);
                                    }
				    
				}
			}	
		}		
	}

	public void storeImage(int pid, int tid, byte[] image)
			throws ImageAlreadyStoredEx, ProposalDoesNotExistEx{
		// TODO Auto-generated method stub
		boolean Exists = false;
		
		if (proposalList.isEmpty()){
			   m_logger.info("The proposal list is empty");
			   AcsJProposalDoesNotExistEx impl = new AcsJProposalDoesNotExistEx();
			   throw impl.toProposalDoesNotExistEx();
		}

		for ( Proposal pro : proposalList) {			
			if (pro.status == 1 && pro.pid == pid){
				Exists = true;
				// Revisar si imagen fue guardada previamente
				if ( map.get(Integer.toString(proposal.pid) + ":" + Integer.toString(tid)) != null  ){
					m_logger.info("ImageAlreadyStoredEx");
					throw new ImageAlreadyStoredEx();		
				}
				map.put(Integer.toString(proposal.pid) + ":" + Integer.toString(tid), image);
			}
		}
		if (Exists == false){
			m_logger.info("ProposalDoesNotExistEx");
			AcsJProposalDoesNotExistEx impl = new AcsJProposalDoesNotExistEx();
			throw impl.toProposalDoesNotExistEx();
		}

	}


	public void clean() {
		// TODO Auto-generated method stub
		proposalList.clear();
		m_logger.info("All Proposals cleared");
		Storage store = null;
                try {
                        store = StorageHelper.narrow( m_containerServices.getComponent("STORAGE") );
                } catch ( AcsJContainerServicesEx e) {
                        m_logger.info("Excepcion de Java Container con Componente Storage");
                        e.log(this.m_logger);
                }
		store.clearAllData();
		m_logger.info("Storage cleared");
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
