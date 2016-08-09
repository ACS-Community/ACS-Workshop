
package acsws.SCHEDULER_MODULE.SchedulerImpl;

import java.util.ArrayList;
import java.util.List;

import java.util.logging.Logger;

import acsws.SCHEDULER_MODULE.SchedulerOperations;
import acsws.TYPES.Proposal;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;

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
	
  public SchedulerImpl () {
  }
	
  @Override
  public void start (){
    System.out.println("Start Scheduler 3");
  }

  @Override
  public void stop (){
    System.out.println("Stop Scheduler 3");
  }

  @Override
  public int proposalUnderExecution(){
    System.out.println("Proposal Under Execution Scheduler 3");
    return 0;
  }	
	
}
