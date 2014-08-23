package sort.SchedulerImpl;
import java.util.logging.Logger;

import sort.DATABASE_MODULE.DataBase;
import sort.INSTRUMENT_MODULE.Instrument;
import sort.SORTErr.ImageAlreadyStoredEx;
import sort.SORTErr.InvalidProposalStatusTransitionEx;
import sort.SORTErr.PositionOutOfLimitsEx;
import sort.TELESCOPE_MODULE.Telescope;
import sort.TYPES.Proposal;
import sort.TYPES.Target;


public class RunProposals extends Thread {

	private Logger m_logger;
	private Proposal[] m_proplist;
	private boolean m_okToRun;
	private DataBase m_database;
	private Telescope m_telescope;
	private Instrument m_instrument;
	private int currPid;
	
	public RunProposals (Logger logger, Proposal[] proplist, 
			DataBase db, Telescope tele, Instrument inst){
		this.m_logger = logger;
		this.m_proplist = proplist;
		this.m_database = db;
		this.m_telescope = tele;
		this.m_instrument = inst;
		m_okToRun = true;
		currPid = -1;
	}
	
	@Override
	public void run() {
		for (int i=0; m_okToRun && i<m_proplist.length; i++)
		{
			//2. Set proposal status in database
			m_logger.info("Starting proposal " + i + " of " + m_proplist.length);
			currPid = m_proplist[i].pid;
//			try {
//				m_database.setProposalStatus(currPid, 0); //queued
//			} catch (InvalidProposalStatusTransitionEx e) {
//				e.printStackTrace();
//			}
				
			//3. Camera on
			m_logger.fine("Turning camera on");
			m_instrument.cameraOn();
			
			//4. Observe: this call blocks until observation complete
			try {
				m_database.setProposalStatus(currPid, 1); //running
			} catch (InvalidProposalStatusTransitionEx e) {
				m_logger.fine("Unable to set proposal pid " + currPid + " to 1");
			}
			Observe(m_proplist[i],m_database, m_telescope, m_logger);
				
			//5. Camera off
			m_logger.fine("Turning camera off");
			m_instrument.cameraOff();
				
			//6. Set proposal status to finished/ready
			try {
				m_database.setProposalStatus(currPid, 2); //ready
			} catch (InvalidProposalStatusTransitionEx e) {
				m_logger.fine("Unable to set proposal pid " + currPid + " to 2");
			}	
			m_logger.info("Finished proposal " + i + " of " + m_proplist.length);							
		} 	
		currPid = -1;
	}

	public void Observe(Proposal proposal, DataBase db, Telescope tele,
			Logger log){
		int[] image = null;
		log.fine("Starting proposal " + proposal.pid);
		for (int i=0; i<proposal.targets.length; i++)
		{
			Target tg = proposal.targets[i];
			int tid = tg.tid;
			//take image
			try {
				log.fine("Starting Target " + tid);
				image = m_telescope.observe(tg.coordinates, tg.expTime);
			} catch (PositionOutOfLimitsEx e) {
				e.printStackTrace();
			}		
			//store image
			try {
				db.storeImage(proposal.pid, tg.tid, image);
			} catch (ImageAlreadyStoredEx e) {
				e.printStackTrace();
			}
			log.fine("Target " + tid + " image stored in database");
		}
	}
	
	public void FinishObserving(){
		m_okToRun = false;
	}
	
	public int GetProposalPidUnderExecution(){
		return currPid;
	}
	
}
