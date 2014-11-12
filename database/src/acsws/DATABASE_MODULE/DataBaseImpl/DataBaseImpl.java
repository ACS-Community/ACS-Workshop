/**
 * 
 */
package acsws.DATABASE_MODULE.DataBaseImpl;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

import acsws.DATABASE_MODULE.DataBaseOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.ProposalNotYetReadyEx;
import acsws.SYSTEMErr.wrappers.AcsJImageAlreadyStoredEx;
import acsws.SYSTEMErr.wrappers.AcsJInvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.wrappers.AcsJProposalNotYetReadyEx;
import acsws.TYPES.Proposal;
import acsws.TYPES.Target;
import alma.acs.component.ComponentImplBase;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;

/**
 * @author almadev8
 */
public class DataBaseImpl extends ComponentImplBase implements
		DataBaseOperations {

	private AtomicInteger lastPid = new AtomicInteger(0);

	private final Map<Integer, Proposal> proposalStore;
	private final HashMap<Integer, HashMap<Integer, int[]>> proposalImages;

	public DataBaseImpl() {
		proposalStore = new HashMap<Integer, Proposal>();
		proposalImages = new HashMap<Integer, HashMap<Integer, int[] >>();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see alma.acs.component.ComponentLifecycle#initialize(alma.acs.container.
	 * ContainerServices)
	 */
	@Override
	public void initialize(ContainerServices containerServices)
			throws ComponentLifecycleException {
		super.initialize(containerServices);
		m_logger.info("Starting Database");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see alma.acs.component.ComponentLifecycle#cleanUp()
	 */
	@Override
	public void cleanUp() throws AcsJComponentCleanUpEx {
		// TODO Auto-generated method stub

	}

	@Override
	public int storeProposal(Target[] targets) {
		int pid = getNextId();
		int status = 0;
		Proposal proposal = new Proposal(pid, targets, status);
		HashMap<Integer, int[]> map = new HashMap<Integer, int[]>();

		proposalStore.put(pid, proposal);
		proposalImages.put(pid, map);
		m_logger.info("Added proposal " + pid + " to database");
		return pid;
	}

	private int getNextId() {
		return lastPid.getAndIncrement();
	}

	@Override
	public int getProposalStatus(int pid) {
		if (pid > lastPid.get()) {
			m_logger.warning("Proposal " + pid
					+ " does not exist. Returning status -1.");
			return -1;
		}
		Proposal prop = proposalStore.get(pid);
		m_logger.info("Returning status of proposal " + pid + " (status="
				+ prop.status + ")");
		return prop.status;
	}

	/**
	 * 
	 */
	@Override
	public void removeProposal(int pid) {
		if (pid > lastPid.get()) {
			m_logger.warning("Proposal " + pid + " does not exist.");
		} else {

			proposalStore.remove(pid);
			proposalImages.remove(pid);
			m_logger.info("Removed proposal " + pid);
		}
	}

	@Override
	public int[][] getProposalObservations(int pid)
			throws ProposalNotYetReadyEx {
		if (pid > lastPid.get()){	
			m_logger.warning("Proposal " + pid + " does not exist.");
			AcsJProposalNotYetReadyEx jex = new AcsJProposalNotYetReadyEx();
			throw jex.toProposalNotYetReadyEx();
		}
		else if (proposalStore.get(pid).status != 2){
			m_logger.warning("Proposal " + pid + " not yet executed.");
			AcsJProposalNotYetReadyEx jex = new AcsJProposalNotYetReadyEx();
			throw jex.toProposalNotYetReadyEx();
		}
		else 
		{
			int[][] obs = proposalImages.get(pid).values().toArray(new int[0][0]);//.values().toArray(new int[0][0]);
			m_logger.info("Found " + obs.length + " images for proposal "+pid);
			return obs;
		}
	}

	@Override
	public Proposal[] getProposals() {
		Proposal[] proposals = proposalStore.values().toArray(new Proposal[0]);

		m_logger.info("Found " + proposals.length + " proposals.");
		return proposals;
	}

	@Override
	public void setProposalStatus(int pid, int status)
			throws InvalidProposalStatusTransitionEx {
		if (pid > lastPid.get()) {
			m_logger.warning("Proposal " + pid
					+ " does not exist. Cannot set status.");
		} else {
			int currentStatus = proposalStore.get(pid).status;
			if (status != currentStatus+1) {
				m_logger.warning("Status of proposal " + pid
						+ " was not incremented by 1.");
				AcsJInvalidProposalStatusTransitionEx jex = new AcsJInvalidProposalStatusTransitionEx();
				throw jex.toInvalidProposalStatusTransitionEx();
			}
			
			proposalStore.get(pid).status = status;
			m_logger.info("Proposal " + pid + ": status set to " + status);
		}
	}

	@Override
	public void storeImage(int pid, int tid, int[] image)
		throws ImageAlreadyStoredEx {
	
			if(pid>lastPid.get()-1)
			{
				m_logger.warning("Proposal "+pid+" does not exist. Cannot store image.");
				return;
			}
			
			Proposal prop = proposalStore.get(pid);
			boolean hasTid = false;
			for(int i=0;i<prop.targets.length;i++)
			{
				if (tid == prop.targets[i].tid)
				{
					hasTid = true;
					break;
				}
			}
			if (!hasTid)
			{
				m_logger.warning("Target ID "+tid+" does not exist in proposal "+pid);
				return;
			}
			else
			{
				if(proposalImages.get(pid).containsKey(tid) )
				{
					m_logger.warning("Image for target ID "+tid+" already taken in proposal "+pid);
					AcsJImageAlreadyStoredEx jex = new AcsJImageAlreadyStoredEx();
					throw jex.toImageAlreadyStoredEx();
				}
			}
			
			proposalImages.get(pid).put(tid,image);
			m_logger.info("Stored image "+tid+" for proposal "+pid+". Number of images: "+proposalImages.get(pid).size());
	}

	@Override
	public void clean() {
		proposalStore.clear();
		proposalImages.clear();
		lastPid.set(0);
		m_logger.info("Cleaning database");
	}

}
