package acsws;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Logger;

import acsws.SCHEDULER_MODULE.SchedulerOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.ProposalDoesNotExistEx;
import acsws.SYSTEMErr.NoProposalExecutingEx;
import acsws.SYSTEMErr.PositionOutOfLimitsEx;
import acsws.SYSTEMErr.SchedulerAlreadyRunningEx;
import acsws.SYSTEMErr.SchedulerAlreadyStoppedEx;

import acsws.DATABASE_MODULE.DataBase;
import acsws.DATABASE_MODULE.DataBaseHelper;

import acsws.INSTRUMENT_MODULE.Instrument;
import acsws.INSTRUMENT_MODULE.InstrumentHelper;

import acsws.TELESCOPE_MODULE.Telescope;
import acsws.TELESCOPE_MODULE.TelescopeHelper;

import acsws.TYPES.*;

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
	
	// A thread to avoid hang Corba with many proposals
	private Thread schedulerThread = null;
	
	// Constants for component names, these names have to match with the CDB definition
	private static final String DATABASE_COMPONENT_NAME = "DATABASE";
	private static final String INSTRUMENT_COMPONENT_NAME = "INSTRUMENT";
	private static final String TELESCOPE_COMPONENT_NAME = "TELESCOPE";
	
	// Flags for checking if start ot stop methods were called
	private final AtomicBoolean isStarted = new AtomicBoolean(false);
	private final AtomicBoolean isStopped = new AtomicBoolean(false);
	
	// Store the Proposal under execution ID
	private final AtomicInteger proposalUnderExID = new AtomicInteger(-1);
	
	/////////////////////////////////////////////////////////////
	// Implementation of ComponentLifecycle
	/////////////////////////////////////////////////////////////
	
	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		
		m_logger.info("initialize() called...");
	}
    
	public void execute() {
		m_logger.info("execute() called...");
	}
    
	public void cleanUp() {
		m_logger.info("cleanUp() called..., nothing to clean up.");
		
		// While until the custom thread stop the execution
		try {
			if(schedulerThread != null) {
				schedulerThread.join();
			}
		} catch (InterruptedException e) {
			m_logger.severe("Error trying to interrupt scheduler thread " + e);
		}
	}
    
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
		System.out.println("Scheduler1 inside start method");
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
						
						DataBase databaseComp = DataBaseHelper.narrow(m_containerServices.getComponent(DATABASE_COMPONENT_NAME));
						Instrument intrumentComp = InstrumentHelper.narrow(m_containerServices.getComponent(INSTRUMENT_COMPONENT_NAME));
						Telescope telescopeComp = TelescopeHelper.narrow(m_containerServices.getComponent(TELESCOPE_COMPONENT_NAME));
						
						Proposal[] proposalArr = databaseComp.getProposals();
						if(proposalArr == null) {
							throw new NullPointerException("Error: getProposals method returned null response");
						}
						m_logger.info("Proposals length: " + proposalArr.length);
						
						for(int i=0; i<proposalArr.length; i++) {
							// Initialize proposal under execution id
							proposalUnderExID.set(-1);
							
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
												
												// Call observe method from TELESCOPE Component
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
										proposalUnderExID.set(-1);
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
					} catch (AcsJContainerServicesEx ae) {
						m_logger.severe("Error in start method while getting proposals " + ae);
						m_logger.severe(getStackTraceFromException(ae));
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
			throw new SchedulerAlreadyRunningEx();
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
		System.out.println("Scheduler1 inside stop method");
		m_logger.info("Scheduler1 inside stop method");
		
		if(isStopped.get() == false) {
			// Set flags for stopping proposals
			isStopped.set(true);
		} else {
			m_logger.severe("Error: Stop method was already called");
			throw new SchedulerAlreadyStoppedEx();
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
		System.out.println("Scheduler1 inside proposalUnderExecution method");
		m_logger.info("Scheduler1 inside proposalUnderExecution method");
		
		if(proposalUnderExID.get() != -1) {
			m_logger.info("Proposal under execution: " + proposalUnderExID.get());
			return proposalUnderExID.get();
		} else {
			m_logger.severe("Error: No proposal under execution");
			throw new NoProposalExecutingEx();
		}
	}
	
	/**
	 * A useful method for getting a string representation for the stacktrace
	 * 
	 * @return A String representation of the stacktrace
	 */
	private String getStackTraceFromException(Exception e) {
		StringWriter sw = new StringWriter();
		e.printStackTrace(new PrintWriter(sw));
		return sw.toString();
	}
}