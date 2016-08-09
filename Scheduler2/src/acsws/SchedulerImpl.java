package acsws;

//import java.util.logging.Logger;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import acsws.SCHEDULER_MODULE.SchedulerOperations;

public class SchedulerImpl implements ComponentLifecycle, SchedulerOperations {

private ContainerServices m_containerServices;
//private Logger m_logger;

public void initialize (ContainerServices containerServices) {
	m_containerServices = containerServices;
	//m_logger = m_containerServices.getLogger();
}

public void execute() {
	// TODO Auto-generated method stub
	
}

public void cleanUp() {
	// TODO Auto-generated method stub
	
}

public void aboutToAbort() {
	// TODO Auto-generated method stub
	
}

public ComponentStates componentState() {
	return m_containerServices.getComponentStateManager().getCurrentState();
}

public String name() {
	return m_containerServices.getName();
}

public void start() {
	System.out.println("Scheduler started");
}

public void stop() {
	System.out.println("Scheduler stopped");
	
}

public int proposalUnderExecution(){
	System.out.println("Excecuting proposal");
	return 0;
}

}
