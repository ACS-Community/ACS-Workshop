package acsws;

import java.util.logging.Logger;

import acsws.SCHEDULER_MODULE.SchedulerOperations;

import alma.ACS.ComponentStates;
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
        System.out.println("HelloDemo component managed to abort... you should know this even if the logger did not flush correctly!");
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
	// Implementation of HelloDemoOperations
	/////////////////////////////////////////////////////////////
	
	public void start() {
		System.out.println("Scheduler1 inside start method");
		m_logger.info("Scheduler1 inside start method");
	}
	
	public void stop() {
		System.out.println("Scheduler1 inside stop method");
		m_logger.info("Scheduler1 inside stop method");
	}
	
	public int proposalUnderExecution() {
		System.out.println("Scheduler1 inside proposalUnderExecution method");
		m_logger.info("Scheduler1 inside proposalUnderExecution method");
		return 0;
	}
}