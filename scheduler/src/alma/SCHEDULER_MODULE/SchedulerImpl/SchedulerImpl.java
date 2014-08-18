/**
 * 
 */
package alma.SCHEDULER_MODULE.SchedulerImpl;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.logging.Level;

import alma.DATABASE_MODULE.DataBase;
import alma.DATABASE_MODULE.DataBaseHelper;
import alma.INSTRUMENT_MODULE.Instrument;
import alma.INSTRUMENT_MODULE.InstrumentHelper;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.SCHEDULER_MODULE.SchedulerOperations;
import alma.SORTErr.ImageAlreadyStoredEx;
import alma.SORTErr.InvalidProposalIdEx;
import alma.SORTErr.InvalidProposalStatusTransitionEx;
import alma.SORTErr.InvalidTargetIdEx;
import alma.SORTErr.NoProposalExecutingEx;
import alma.SORTErr.PositionOutOfLimitsEx;
import alma.SORTErr.SchedulerAlreadyRunningEx;
import alma.SORTErr.SchedulerAlreadyStoppedEx;
import alma.SORTErr.wrappers.AcsJSchedulerAlreadyRunningEx;
import alma.TELESCOPE_MODULE.Telescope;
import alma.TELESCOPE_MODULE.TelescopeHelper;
import alma.TYPES.Proposal;
import alma.TYPES.Target;
import alma.acs.component.ComponentImplBase;
import alma.acs.logging.AcsLogLevel;

/**
 * @author almadev
 *
 */
public class SchedulerImpl extends ComponentImplBase implements SchedulerOperations {

	private DataBase databaseComp;
	private Telescope telescopeComp;
	private Instrument instrumentComp;
	private volatile int proposalUnderExecution = -1; 
	private volatile boolean stopSchedulerRequest = false;
	
	private CountDownLatch telescopeFinishedSync;
	private final AtomicBoolean isRunning = new AtomicBoolean(false);
	
	/*private Thread proposalStatusThread = new Thread(new Runnable() {
		public void run()
		{
			
		}
	},"This thread gets the proposal status to send it to console");*/
	
	
	private Runnable executeProposalRun = new Runnable() {
		public void run()
		{
			try {
				telescopeFinishedSync = new CountDownLatch(1);
				
				/* Get proposal list */
				Proposal[] proposals = databaseComp.getProposals();
				m_logger.info("Retrieved " + proposals.length + " proposals from the database.");
				
				// iterating the proposals
				for (int i=0; i<proposals.length; i++) {
					if (!stopSchedulerRequest) {
						int pid = proposals[i].pid;
						proposalUnderExecution = pid;
						// int status = proposals[i].status;
						/* queued(0) to running(1) or from running(1) to ready(2). */
						try {
							databaseComp.setProposalStatus(pid, 1);
						} catch (InvalidProposalStatusTransitionEx e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						
						// turn on the camera
						m_logger.fine("Will turn on the camera now...");
						instrumentComp.cameraOn();
						
						// iterating the targets
						Target[] targets = proposals[i].targets;
						m_logger.info("Retrieved " + targets.length + " targets from the proposal number " + pid);
						for (int j=0; j<targets.length; j++) {
							/* TODO put the tid in the log */
							int targetId = targets[j].tid;				
							// calling the observe method in the Telescope component
							
							int[] image = null;
							
							try {
								image = telescopeComp.observe(targets[j].coordinates, targets[j].expTime);
							} 
							catch (PositionOutOfLimitsEx ex) {
								m_logger.log(AcsLogLevel.CRITICAL, "Failed to observe target of proposal with pid=" + pid + " . Will continue with next target.", ex);
								continue; // try next target
							}
							catch (org.omg.CORBA.SystemException ex) {
								// This could be TIMEOUT, or any other network problem etc
								m_logger.log(AcsLogLevel.CRITICAL, "Got a corba runtime exception from call to Telescope#observe. Will continue with next target.", ex);
								continue;
							}
		 				
							
							// storing the new image in the database
							try {
								if (image != null) {
									databaseComp.storeImage(pid, targetId, image);
								}
							} 
							catch (ImageAlreadyStoredEx e) {
								m_logger.log(AcsLogLevel.CRITICAL, "Failed to store image for proposal with pid=" + pid + " in the database. Will continue with next proposal.", e);
								continue; // try next target
							}
							catch (InvalidProposalIdEx e ) {
								m_logger.log(AcsLogLevel.CRITICAL, "Failed to store image for proposal with pid=" + pid + " in the database. " +
										" because the proposal ID is not valid. Will continue with next proposal.", e);
								continue;
								
							}
							catch (InvalidTargetIdEx e ) {
								m_logger.log(AcsLogLevel.CRITICAL, "Failed to store image for proposal with pid=" + pid + " in the database. " +
										" because the target ID=" + targetId + " is not valid. Will continue with next proposal.", e);
								continue;
							}
							catch (Throwable th) {
								m_logger.log(AcsLogLevel.CRITICAL, "Got a corba runtime exception from call to Database#storeImage. Will continue with next target.", th);
								continue;
							}
						}
						
						// turn off the camera
						instrumentComp.cameraOff();
						
						// finally we update the state of the proposal to ready
						try {
							databaseComp.setProposalStatus(pid, 2);
						} catch (InvalidProposalStatusTransitionEx e) {
							m_logger.log(Level.INFO, "Failed to set proposal status. Will continue nonetheless.", e);
						}
					}
					else {
						m_logger.info("Someone called stop()... skipping the remaining " + (proposals.length - i) + " proposals.");
						stopSchedulerRequest = false;
						break;
					}
				}		
				proposalUnderExecution = -1;
				
				// if the thread that called stop() waits on telescopeFinishedSync, then we release it now.
				telescopeFinishedSync.countDown();
				telescopeFinishedSync = null;
				
				m_containerServices.releaseComponent("DATABASE");
				m_containerServices.releaseComponent("INSTRUMENT");
				m_containerServices.releaseComponent("TELESCOPE");
				m_logger.info("*** Done with all Scheduling. Call start() again for another round of observations. ***");
			}
			finally {
				isRunning.set(false);
			}
		}
	};
	
		
	
	////////////////// functional methods /////////////////////
	
	
	/* (non-Javadoc)
	 * @see alma.SCHEDULER_MODULE.SchedulerOperations#proposalUnderExecution()
	 */
	@Override
	public int proposalUnderExecution() throws NoProposalExecutingEx {
		return proposalUnderExecution;
	}

	/* (non-Javadoc)
	 * @see alma.SCHEDULER_MODULE.SchedulerOperations#start()
	 */
	@Override
	public void start() throws SchedulerAlreadyRunningEx {
		
		try {
			
			// we set this isRunning flag here, and clear it when the thread terminates
			if (isRunning.getAndSet(true)) {
				m_logger.warning("The Scheduler is already running. Will ignore this request and throw SchedulerAlreadyRunningEx.");
				throw new AcsJSchedulerAlreadyRunningEx().toSchedulerAlreadyRunningEx();
			}
			
			databaseComp = DataBaseHelper.narrow(m_containerServices.getComponent("DATABASE"));
			instrumentComp = InstrumentHelper.narrow(m_containerServices.getComponent("INSTRUMENT")); //to be confirmed
			telescopeComp = TelescopeHelper.narrow(m_containerServices.getComponent("TELESCOPE"));

			// it is better to create the thread from the thread factory provided by container services, 
			// instead of writing "new Thread(runnable)".  
			m_containerServices.getThreadFactory().newThread(executeProposalRun).start();
						
			// or alternatively: m_containerServices.getDefaultComponent("IDL:alma/.../Database:1.0");
		} catch (AcsJContainerServicesEx e) {
			e.printStackTrace();
			// TODO add a new exception to IDL and throw it here
			throw new AcsJSchedulerAlreadyRunningEx().toSchedulerAlreadyRunningEx();
		} 
	}

	/* (non-Javadoc)
	 * @see alma.SCHEDULER_MODULE.SchedulerOperations#stop()
	 */
	@Override
	public void stop() throws SchedulerAlreadyStoppedEx {
		/* first, we set the private variable stopSchedulerRequest as true.
		 * this will stop the proposal's loop in the SchedulerAlreadyRunningEx Trhead.
		 */
		
		stopSchedulerRequest = true;
		
		// The IDL description says we must wait until the current observation has finished
		if (telescopeFinishedSync != null) {
			m_logger.info("Will wait for current observation to finish (at most 60 s)");
			try {
				telescopeFinishedSync.await(60, TimeUnit.SECONDS);
				m_logger.info("Observation has finished, will return from stop() method.");
			} catch (InterruptedException e) {
				m_logger.log(Level.INFO, "Got interrupted while waiting for telescope to finish current observation", e);
			}
		}
		else {
			m_logger.info("No need to wait for current observation, just returning from stop()");
		}
		isRunning.set(false);
	}

	
	
	////////////////// framework methods /////////////////////

	/* (non-Javadoc)
	 * @see alma.acs.component.ComponentLifecycle#cleanUp()
	 */
	@Override
	public void cleanUp() {
		// TODO Auto-generated method stub
	}


}
