package acsws;

import java.util.logging.Logger;

import acsws.SCHEDULER_MODULE.SchedulerOperations;
import acsws.SYSTEMErr.NoProposalExecutingEx;
import acsws.SYSTEMErr.SchedulerAlreadyRunningEx;
import acsws.SYSTEMErr.SchedulerAlreadyStoppedEx;
import acsws.DATABASE_MODULE.DataBase;
import acsws.DATABASE_MODULE.DataBaseHelper;
import acsws.DATABASE_MODULE.DataBaseOperations;
import acsws.TYPES.*;

import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;

/**
 * A simple scheduler component.
 * 
 * @author Juan Carlos Olivares
 * @author Pablo Valenzuela
 * @author Exequiel Fuentes
 * 
 */
public class SchedulerImpl implements ComponentLifecycle, SchedulerOperations {
	private ContainerServices m_containerServices;
	private Logger m_logger;
	
	private boolean isStarted = false;
	private boolean isStopped = false;
	
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
		if(this.isStarted == false) {
			this.isStarted = true;
			
			System.out.println("Scheduler1 inside start method");
			m_logger.info("Scheduler1 inside start method");
			
			/*
			try {
				DataBase databaseComp = DataBaseHelper.narrow(m_containerServices.getComponent("Database1"));
				
				databaseComp.getProposals();
			} catch (AcsJContainerServicesEx e) {
				m_logger.severe("Error in start method while getting proposals" + e.getMessage());
				e.printStackTrace();
			}
			*/
			
			int[] proposals = new int[2];
			proposals[0] = 0;
			proposals[1] = 1;
			
			for(int i=0; i<proposals.length; i++){
				if(this.isStopped == false) {
					m_logger.info("Running proposal");
					System.out.println("Running proposal");
				} else {
					m_logger.info("Stop method invoked");
					System.out.println("Stop method invoked");
					break;
				}
			}
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
		if(this.isStopped == true) {
			System.out.println("Scheduler1 inside stop method");
			m_logger.info("Scheduler1 inside stop method");
			
			this.isStopped = false;
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