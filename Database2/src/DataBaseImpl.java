import acsws.DATABASE_MODULE.DataBaseOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.ProposalNotYetReadyEx;
import acsws.TYPES.Proposal;
import acsws.TYPES.Target;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.acs.logging.AcsLogger;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;


public class DataBaseImpl implements ComponentLifecycle, DataBaseOperations {

	
	// Implementation of ACSComponent 
	
	private AcsLogger m_logger;
	private ContainerServices m_containerServices;

	public String name() {
		// TODO Auto-generated method stub
		System.out.print("Hello World");
		return null;
	}

	public ComponentStates componentState() {
		// TODO Auto-generated method stub
		return null;
	}

	// Implementation of DataBaseOperations
	
	public int storeProposal(Target[] targets) {
		// TODO Auto-generated method stub
		return 0;
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
		
	}

	public void cleanUp() throws AcsJComponentCleanUpEx {
		// TODO Auto-generated method stub
		
	}

	public void aboutToAbort() {
		// TODO Auto-generated method stub
		
	}

}
