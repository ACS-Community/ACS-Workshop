package acsws;

import java.util.concurrent.atomic.AtomicBoolean;
import java.util.logging.Logger;

import acsws.SCHEDULER_MODULE.SchedulerOperations;
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
			m_logger.severe("Error trying to interrupt scheduler thread " + e.getMessage());
			e.printStackTrace();
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
			        m_logger.severe("Uncaught exception: " + e.getMessage());
			        e.printStackTrace();
			    }
			};
			
			//schedulerThread = new Thread(new Runnable() {
				//public void run() {
					try {
						// Set flags for processing proposals
						isStarted.set(true);
						isStopped.set(false);
						
						DataBase databaseComp = DataBaseHelper.narrow(m_containerServices.getComponent(DATABASE_COMPONENT_NAME));
						Instrument intrumentComp = InstrumentHelper.narrow(m_containerServices.getComponent(INSTRUMENT_COMPONENT_NAME));
						Telescope telescopeComp = TelescopeHelper.narrow(m_containerServices.getComponent(TELESCOPE_COMPONENT_NAME));
						
						Proposal[] proposalArr = databaseComp.getProposals();
						if(proposalArr == null) {
							throw new NullPointerException("getProposals method returned null response");
						}
						m_logger.info("Proposals length: " + proposalArr.length);
						
						// TODO Doing all status to 0 (remover esta cochinada!!)
						for(int i=0; i<proposalArr.length; i++) {
							proposalArr[i].status = 0;
						}
						
						for(int i=0; i<proposalArr.length; i++) {
							if(isStopped.get() == false) {
								m_logger.info("Proposal PID: " + proposalArr[i].pid);
								m_logger.info("Proposal status: " + proposalArr[i].status);
								
								if(proposalArr[i].status == 0) {
									try {
										// Setting proposal to running
										databaseComp.setProposalStatus(proposalArr[i].pid, 1);
										
										// Turn on Camera
										intrumentComp.cameraOn();
										
										// Now observe
										Target[] targetArr = proposalArr[i].targets;
										
										for(int j=0; j<targetArr.length; j++) {
											try {
												Target target = targetArr[j];
												
												m_logger.info("Target coordinates (AZ): " + target.coordinates.az);
												m_logger.info("Target coordinates (EL): " + target.coordinates.el);
												m_logger.info("Target expTime: " + target.expTime);
												
												byte[] image = telescopeComp.observe((Position)target.coordinates, (int)target.expTime);
												
											} catch (PositionOutOfLimitsEx e) {
												m_logger.severe("Error trying to observe, position out of limit " + e.getMessage());
												e.printStackTrace();
											}
										}
									} catch (InvalidProposalStatusTransitionEx e) {
										m_logger.severe("Error trying to set proposal status, invalid proposal status " + e.getMessage());
										e.printStackTrace();
									} catch (ProposalDoesNotExistEx e) {
										m_logger.severe("Error trying to set proposal status, proposal doesn't exist " + e.getMessage());
										e.printStackTrace();
									}
								}
							} else {
								m_logger.info("Stop method called, stopping...");
								isStarted.set(false);
								break;
							}
						}
					} catch (AcsJContainerServicesEx ae) {
						m_logger.severe("Error in start method while getting proposals " + ae.getMessage());
						ae.printStackTrace();
					} catch (Exception e) {
						m_logger.severe("A exception happening while trying to start proposals " + e.getMessage());
						e.printStackTrace();
					}
				//}
			//});
			
			//m_logger.info("Starting scheduler thread...");
			//schedulerThread.setUncaughtExceptionHandler(threadExceptionHandler);
			//schedulerThread.start();
		} else {
			m_logger.severe("Start method was already called");
			throw new SchedulerAlreadyRunningEx("Start method was already called", null);
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
			m_logger.severe("Stop method was already called");
			throw new SchedulerAlreadyStoppedEx("Stop method was already called", null);
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
		
		return 0;
	}
}