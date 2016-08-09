package acsws;

import java.util.logging.Logger;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import acsws.SCHEDULER_MODULE.SchedulerOperations;

public class SchedulerImpl implements ComponentLifecycle, SchedulerOperations {

private ContainerServices m_containerServices;
private Logger m_logger;

public void initialize (ContainerServices containerServices) {
	m_containerServices = containerServices;
	m_logger = m_containerServices.getLogger();
	m_logger.info("initialize() called...");
}

public void execute() {
	m_logger.info("execute() called...");
	
}

public void cleanUp() {
	m_logger.info("cleanUP() called... nothing to clean up.");
}

public void aboutToAbort() {
	m_logger.info("managed to abort");
}

public ComponentStates componentState() {
	return m_containerServices.getComponentStateManager().getCurrentState();
}

public String name() {
	return m_containerServices.getName();
}

public void start() {
	m_logger.info("Scheduler started");
}

public void stop() {
	m_logger.info("Scheduler stopped");
	
}

public int proposalUnderExecution(){
	m_logger.info("Excecuting proposal");
	return 0;
}

}
