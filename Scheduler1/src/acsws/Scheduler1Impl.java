package acsws;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Logger;

import acsws.DATABASE_MODULE.DataBase;
import acsws.DATABASE_MODULE.DataBaseHelper;
import acsws.INSTRUMENT_MODULE.Instrument;
import acsws.INSTRUMENT_MODULE.InstrumentHelper;
import acsws.SCHEDULER_MODULE.SchedulerOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.NoProposalExecutingEx;
import acsws.SYSTEMErr.PositionOutOfLimitsEx;
import acsws.SYSTEMErr.ProposalDoesNotExistEx;
import acsws.SYSTEMErr.SchedulerAlreadyRunningEx;
import acsws.SYSTEMErr.SchedulerAlreadyStoppedEx;
import acsws.SYSTEMErr.wrappers.AcsJNoProposalExecutingEx;
import acsws.SYSTEMErr.wrappers.AcsJSchedulerAlreadyRunningEx;
import acsws.SYSTEMErr.wrappers.AcsJSchedulerAlreadyStoppedEx;
import acsws.TELESCOPE_MODULE.Telescope;
import acsws.TELESCOPE_MODULE.TelescopeHelper;
import acsws.TYPES.Position;
import acsws.TYPES.Proposal;
import acsws.TYPES.Target;

import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;

/**
 * A simple scheduler component.
 * 
 * @author Exequiel Fuentes
 * @author Juan Carlos Olivares
 * @author Pablo Valenzuela
 * 
 */
public class Scheduler1Impl implements ComponentLifecycle, SchedulerOperations {
	private ContainerServices m_containerServices;
	private Logger m_logger;
	
	// Constants for component names, these names have to match with the CDB definition
	private static final String DATABASE_COMPONENT_NAME = "IDL:acsws/DATABASE_MODULE/DataBase:1.0";
	private static final String INSTRUMENT_COMPONENT_NAME = "IDL:acsws/INSTRUMENT_MODULE/Instrument:1.0";
	private static final String TELESCOPE_COMPONENT_NAME = "IDL:acsws/TELESCOPE_MODULE/Telescope:1.0";
	
	private DataBase databaseComp = null;
	private Instrument intrumentComp = null;
	private Telescope telescopeComp = null;
	
	// A thread to avoid hang Corba with many proposals
	private Thread schedulerThread = null;
	
	// Flags for checking if start or stop methods were called
	private final AtomicBoolean isStarted = new AtomicBoolean(false);
	private final AtomicBoolean isStopped = new AtomicBoolean(false);
	
	// Store the Proposal under execution ID. -1 means no proposal under execution.
	private static final int NO_PROPOSAL_UNDER_EX = -1;
	private final AtomicInteger proposalUnderExID = new AtomicInteger(NO_PROPOSAL_UNDER_EX);
	
	/////////////////////////////////////////////////////////////
	// Implementation of ComponentLifecycle
	/////////////////////////////////////////////////////////////
	
	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		
		m_logger.info("initialize() called...");
		
		try {
			databaseComp = DataBaseHelper.narrow(m_containerServices.getDefaultComponent(DATABASE_COMPONENT_NAME));
			intrumentComp = InstrumentHelper.narrow(m_containerServices.getDefaultComponent(INSTRUMENT_COMPONENT_NAME));
			telescopeComp = TelescopeHelper.narrow(m_containerServices.getDefaultComponent(TELESCOPE_COMPONENT_NAME));
		} catch (AcsJContainerServicesEx e) {
			m_logger.severe("Error trying to get components " + e);
			m_logger.severe(getStackTraceFromException(e));
		}
	}
    
	/*
	 * (non-Javadoc)
	 * @see alma.acs.component.ComponentLifecycle#execute()
	 */
	public void execute() {
		m_logger.info("execute() called...");
	}
    
	/*
	 * (non-Javadoc)
	 * @see alma.acs.component.ComponentLifecycle#cleanUp()
	 */
	public void cleanUp() {
		m_logger.info("cleanUp() called...");
		
		// While until the custom thread stop the execution
		try {
			if(schedulerThread != null) {
				schedulerThread.join();
			}
		} catch (InterruptedException e) {
			m_logger.severe("Error trying to interrupt scheduler thread " + e);
		}
		
		m_containerServices.releaseComponent(databaseComp.name());
		m_containerServices.releaseComponent(intrumentComp.name());
		m_containerServices.releaseComponent(telescopeComp.name());
	}
    
	/*
	 * (non-Javadoc)
	 * @see alma.acs.component.ComponentLifecycle#aboutToAbort()
	 */
	public void aboutToAbort() {
		cleanUp();
		m_logger.info("managed to abort...");
        System.out.println("Scheduler component managed to abort... you should know this even if the logger did not flush correctly!");
	}
	
	/////////////////////////////////////////////////////////////
	// Implementation of ACSComponent
	/////////////////////////////////////////////////////////////
	
	public ComponentStates componentState() {
		return m_containerServices.getComponentStateManager().getCurrentState();
	}
	
	/*
	 * (non-Javadoc)
	 * @see alma.ACS.ACSComponentOperations#name()
	 */
	public String name() {
		return m_containerServices.getName();
	}
	
	/////////////////////////////////////////////////////////////
	// Implementation of SchedulerOperations
	/////////////////////////////////////////////////////////////
	
	/**
     * Start the scheduler.
     * The scheduler will loop through all available proposals,
     * either until all proposals are done or until the stop method is called.
     * Raises an exception if called twice.
     * 
     * @return None
     */
	public synchronized void start() throws SchedulerAlreadyRunningEx {
		m_logger.info("Scheduler1 inside start method");
		
		if(isStarted.get() == false) {
			Thread.UncaughtExceptionHandler threadExceptionHandler = new Thread.UncaughtExceptionHandler() {
			    public void uncaughtException(Thread th, Throwable e) {
			        m_logger.severe("Uncaught exception: " + e);
			    }
			};
			
			schedulerThread = new Thread(new Runnable() {
				public void run() {
					try {
						// Set flags for processing proposals
						isStarted.set(true);
						isStopped.set(false);
						
						Proposal[] proposalArr = databaseComp.getProposals();
						if(proposalArr == null) {
							throw new NullPointerException("Error: getProposals method returned a null response");
						}
						m_logger.info("Proposals length: " + proposalArr.length);
						
						//for(Proposal proposal: proposalArr) {//no valid for current java version
						for(int i=0; i<proposalArr.length; i++) {
							// Initialize proposal under execution id
							proposalUnderExID.set(NO_PROPOSAL_UNDER_EX);
							
							if(isStopped.get() == false) {
								m_logger.info("Proposal PID: " + proposalArr[i].pid);
								m_logger.info("Proposal status: " + proposalArr[i].status);
								
								if(proposalArr[i].status == 0) {
									try {
										// Storing current proposal under execution id
										proposalUnderExID.set(proposalArr[i].pid);
										
										// Setting proposal to running
										m_logger.info("Setting proposal to running...");
										databaseComp.setProposalStatus(proposalArr[i].pid, 1);
										
										// Turn on Camera
										m_logger.info("Turning on camera...");
										intrumentComp.cameraOn();
										
										// Now observe
										Target[] targetArr = proposalArr[i].targets;
										
										for(int j=0; j<targetArr.length; j++) {
											try {
												Target target = targetArr[j];
												
												m_logger.info("Target coordinates (AZ): " + target.coordinates.az);
												m_logger.info("Target coordinates (EL): " + target.coordinates.el);
												m_logger.info("Target expTime: " + target.expTime);
												
												// Call observe methodself.component.proposalUnderExecution from TELESCOPE Component
												m_logger.info("Calling observe method from TELESCOPE Component...");
												byte[] image = telescopeComp.observe((Position)target.coordinates, (int)target.expTime);
												
												// Store the image
												m_logger.info("Calling storeImage method from DATABASE Component...");
												databaseComp.storeImage(proposalArr[i].pid, target.tid, image);
											} catch (PositionOutOfLimitsEx pole) {
												m_logger.severe("Error trying to observe, position out of limit " + pole.getMessage());
												m_logger.severe(getStackTraceFromException(pole));
											} catch (ImageAlreadyStoredEx iase) {
												m_logger.severe("Error trying to store image, image already stored " + iase.getMessage());
												m_logger.severe(getStackTraceFromException(iase));
											} catch(ProposalDoesNotExistEx pdee) {
												m_logger.severe("Error trying to store image, proposal doesn't exist " + pdee.getMessage());
												m_logger.severe(getStackTraceFromException(pdee));
											}
										}
										
										// Turn off Camera
										m_logger.info("Turning off camera...");
										intrumentComp.cameraOff();
										
										// Setting proposal to ready
										m_logger.info("Setting proposal to ready...");
										databaseComp.setProposalStatus(proposalArr[i].pid, 2);
										
										// Initialize proposal under execution id
										proposalUnderExID.set(NO_PROPOSAL_UNDER_EX);
									} catch (InvalidProposalStatusTransitionEx e) {
										m_logger.severe("Error trying to set proposal status, invalid proposal status " + e.getMessage());
										m_logger.severe(getStackTraceFromException(e));
									} catch (ProposalDoesNotExistEx e) {
										m_logger.severe("Error trying to set proposal status, proposal doesn't exist " + e.getMessage());
										m_logger.severe(getStackTraceFromException(e));
									}
								}
							} else {
								m_logger.info("Stop method called, stopping...");
								isStarted.set(false);
								break;
							}
						}
						
						// Set flags for processing proposals
						isStarted.set(false);
						isStopped.set(true);
					} catch (Exception e) {
						m_logger.severe("A exception happening while trying to start proposals " + e);
						m_logger.severe(getStackTraceFromException(e));
					}
				}
			});
			
			m_logger.info("Starting scheduler thread...");
			schedulerThread.setUncaughtExceptionHandler(threadExceptionHandler);
			schedulerThread.start();
		} else {
			m_logger.severe("Error: Start method was already called");
			AcsJSchedulerAlreadyRunningEx ex = new AcsJSchedulerAlreadyRunningEx();
			ex.log(m_logger);
			throw ex.toSchedulerAlreadyRunningEx();
		}
	}
	
	/**
     * Stops the scheduler.
     * This will stop the scheduler from scheduling more proposals. 
     * 
     * It will not(!) break the ongoing observation, and will return only
     * when the running observation has finished.
     *
     * Raises an exception if called twice.
     * 
     * @return None
     */
	public synchronized void stop() throws SchedulerAlreadyStoppedEx {
		m_logger.info("Scheduler1 inside stop method");
		
		if(isStopped.get() == false) {
			// Set flags for stopping proposals
			isStopped.set(true);
		} else {
			m_logger.severe("Error: Stop method was already called");
			AcsJSchedulerAlreadyStoppedEx ex = new AcsJSchedulerAlreadyStoppedEx();
			ex.log(m_logger);
			throw ex.toSchedulerAlreadyStoppedEx();
		}
	}
	
	/**
     * Returns the pid of the proposal currently under execution
     *
     * Raises exception if no proposal is executing.
     * 
     * @return Proposal ID
     */
	public synchronized int proposalUnderExecution() throws NoProposalExecutingEx {
		m_logger.info("Scheduler1 inside proposalUnderExecution method");
		
		if(proposalUnderExID.get() != NO_PROPOSAL_UNDER_EX) {
			m_logger.info("Proposal under execution: " + proposalUnderExID.get());
			return proposalUnderExID.get();
		} else {
			m_logger.severe("Error: No proposal under execution");
			AcsJNoProposalExecutingEx ex = new AcsJNoProposalExecutingEx();
			ex.log(m_logger);
			throw ex.toNoProposalExecutingEx();
		}
	}
	
	/**
	 * A useful method for getting a string representation for stack trace.
	 * 
	 * @return A String representation of the stack trace.
	 */
	private String getStackTraceFromException(Exception e) {
		StringWriter sw = new StringWriter();
		e.printStackTrace(new PrintWriter(sw));
		return sw.toString();
	}
}