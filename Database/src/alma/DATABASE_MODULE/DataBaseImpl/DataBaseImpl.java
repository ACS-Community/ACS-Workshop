/**
 * 
 */
package alma.DATABASE_MODULE.DataBaseImpl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;

import alma.DATABASE_MODULE.DataBase;
import alma.DATABASE_MODULE.DataBaseOperations;
import alma.SORTErr.ImageAlreadyStoredEx;
import alma.SORTErr.InvalidProposalIdEx;
import alma.SORTErr.InvalidProposalStatusTransitionEx;
import alma.SORTErr.InvalidTargetIdEx;
import alma.SORTErr.ProposalNotYetReadyEx;
import alma.SORTErr.wrappers.AcsJImageAlreadyStoredEx;
import alma.SORTErr.wrappers.AcsJInvalidProposalIdEx;
import alma.SORTErr.wrappers.AcsJInvalidProposalStatusTransitionEx;
import alma.SORTErr.wrappers.AcsJInvalidTargetIdEx;
import alma.SORTErr.wrappers.AcsJProposalNotYetReadyEx;
import alma.TYPES.Proposal;
import alma.TYPES.Target;
import alma.acs.component.ComponentImplBase;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.acs.logging.AcsLogLevel;

/**
 * @author almadev
 * 
 */
public class DataBaseImpl extends ComponentImplBase implements
		DataBaseOperations {

	// we only allow this one instance in the system, but it cannot be enforced from here...
	static public final String DATABASE_INSTANCE_NAME = "DATABASE";
	
	static final String DBFILE_DIR = "/tmp";
	String DBFILE_SUFFIX; // to be set to the actual instance name = "DBFILE.bin";



	static class Entry implements Serializable {
		private static final long serialVersionUID = 1L;
		Proposal proposal;
		int[][] observations;
	}

	/**
	 * 
	 */
	private int lastProposalId;
	
	private Map<Integer, Entry> proposals;

	// ///////////////////////////////////////
	// / Functional methods
	// ///////////////////////////////////////

	@Override
	public synchronized int[][] getProposalObservations(int pid)
			throws ProposalNotYetReadyEx {
		Entry data = proposals.get(pid);
		for (int i = 0; i < data.observations.length; i++) {
			if (data.observations[i] == null) {
				AcsJProposalNotYetReadyEx jEx = new AcsJProposalNotYetReadyEx(); // plain JDK style exception
				jEx.setProposalId(pid);
				throw jEx.toProposalNotYetReadyEx(); // need to transform to Corba type when hitting IDL interface
			}
		}
		return data.observations;
	}

	@Override
	public synchronized void setProposalStatus(int pid, int status)
			throws InvalidProposalStatusTransitionEx {
		Entry data = proposals.get(pid);
		if (status != DataBase.STATUS_QUEUED && status != DataBase.STATUS_RUNNING && status != DataBase.STATUS_READY) {
			throw new AcsJInvalidProposalStatusTransitionEx().toInvalidProposalStatusTransitionEx();
		}
		if (data == null) {
			// m_logger.log(AcsLogLevel.DEBUG, "pid doesn't exists");
			throw new AcsJInvalidProposalStatusTransitionEx().toInvalidProposalStatusTransitionEx();
		} else {
			data.proposal.status = status;
		}
	}

	@Override
	public synchronized void storeImage(int pid, int tid, int[] image)
			throws ImageAlreadyStoredEx, InvalidProposalIdEx, InvalidTargetIdEx {
		
		if (m_logger.isLoggable(Level.FINEST)) { // just to show how this optimization can be used...
			m_logger.finest("storeImage called with pid=" + pid + " tid=" + tid);
		}
		
		Entry data = proposals.get(pid);
		if (data == null) {
			m_logger.log(AcsLogLevel.ALERT, "pid not found");
			AcsJInvalidProposalIdEx ex = new AcsJInvalidProposalIdEx();
			ex.setProposalId(pid);
			throw ex.toInvalidProposalIdEx();
		}
		for (int i = 0; i < data.proposal.targets.length; i++) {
			if (tid == data.proposal.targets[i].tid) {
				if (data.observations[i] == null) {
					data.observations[i] = image;
					return;
				} else {
					// ugly but legal:
					// ImageAlreadyStoredEx ex2 = new ImageAlreadyStoredEx();
					// ex2.errorTrace = new ErrorTrace();
					// throw ex2;

					// better
					throw (new AcsJImageAlreadyStoredEx()).toImageAlreadyStoredEx();
				}
			}
		}
		m_logger.log(AcsLogLevel.ALERT, "tid not found, will throw InvalidTargetIdEx");
		AcsJInvalidTargetIdEx ex = new AcsJInvalidTargetIdEx();
		ex.setProposalId(pid);
		ex.setTargetId(tid);
		throw ex.toInvalidTargetIdEx();
	}

	@Override
	public synchronized Proposal[] getProposals() {
		ArrayList<Proposal> readyProposals = new ArrayList<Proposal>();
		for (Entry x : proposals.values()) {
			if (x.proposal.status == DataBaseOperations.STATUS_QUEUED) {
				readyProposals.add(x.proposal);
			}
		}
		return readyProposals.toArray(new Proposal[readyProposals.size()]);
	}

	public synchronized int storeProposal(Target[] targets) {
		Proposal prop = new Proposal();
		lastProposalId++;
		prop.pid = lastProposalId;
		prop.targets = targets;
		prop.status = DataBaseOperations.STATUS_QUEUED;

		Entry entry = new Entry();
		entry.proposal = prop;
		entry.observations = new int[prop.targets.length][];

		assert proposals != null; // do not use assert in real alma code
		proposals.put(entry.proposal.pid, entry);
		return entry.proposal.pid;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see alma.DATABASE_MODULE.DataBaseOperations#getProposalStatus(int)
	 */
	@Override
	public synchronized int getProposalStatus(int pid) {
		if (!proposals.containsKey(pid)) {
			return STATUS_NO_SUCH_PROPOSAL;
		} else {
			return proposals.get(pid).proposal.status;
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see alma.DATABASE_MODULE.DataBaseOperations#removeProposal(int)
	 */
	@Override
	public synchronized void removeProposal(int pid) {
		if (!proposals.containsKey(pid)) {
			m_logger.log(AcsLogLevel.ALERT, "pid does not exists!");
		} else {
			proposals.remove(pid);
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see alma.DATABASE_MODULE.DataBaseOperations#clean()
	 */
	@Override
	public synchronized void clean() {
		proposals.clear();
	}

	private String getDBPath() {
		String path = System.getenv("DBPATH");
		if (path == null) {
			path = DBFILE_DIR;
		}
		if (path.endsWith("/")) {
			path = path.substring(path.length() - 1);
		}
		return path + "/" + DBFILE_SUFFIX;
	}


	// ///////////////////////////////////////
	// / Framework methods
	// ///////////////////////////////////////

	@Override
	public synchronized void initialize(ContainerServices containerServices)
			throws ComponentLifecycleException {
		super.initialize(containerServices);
		DBFILE_SUFFIX = name(); // to have a per-instance DB file, just in case multiple instances are used.
		lastProposalId = 0;
		Map<Integer, Entry> values = null;
		try {
			FileInputStream fis = new FileInputStream(getDBPath());
			try {
				ObjectInputStream ois = new ObjectInputStream(fis);
				try {
					Integer lastPid = (Integer) ois.readObject();
					lastProposalId = lastPid;
					values = (Map<Integer, Entry>) ois.readObject();
				} finally {
					ois.close();
				}
			} finally {
				fis.close();
			}
		} catch (Exception e) {
			m_logger.log(AcsLogLevel.ALERT, "Couldnt read DBFILE", e);
		}
		if (values == null) {
			values = new HashMap<Integer, DataBaseImpl.Entry>();
		}
		proposals = values;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see alma.acs.component.ComponentLifecycle#cleanUp()
	 */
	@Override
	public synchronized void cleanUp() {
		File outFile = new File(getDBPath());
		try {
			m_logger.info("Starting to write proposal data into file " + outFile.getAbsolutePath() + 
					( outFile.exists() ? " overwriting the previous file." : " creating it from scratch.") );
			FileOutputStream fos = new FileOutputStream(getDBPath());
			try {
				ObjectOutputStream oos = new ObjectOutputStream(fos);
				try {
					oos.writeObject(lastProposalId);
					oos.writeObject(proposals);
					m_logger.info("Done writing" + outFile.getAbsolutePath());
				} finally {
					oos.close();
				}
			} finally {
				fos.close();
			}
		} catch (Exception e) {
			m_logger.log(AcsLogLevel.ALERT, "Failed write data to " + outFile.getAbsolutePath(), e);
		}
	}
}
