
package acsws.SCHEDULER_MODULE.SchedulerImpl;

import acsws.DATABASE_MODULE.*;
import acsws.INSTRUMENT_MODULE.*;
import acsws.TELESCOPE_MODULE.*;
import acsws.SYSTEMErr.*;
import acsws.SYSTEMErr.wrappers.*;
import acsws.TYPES.*;
import java.util.ArrayList;
import java.util.List;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import java.lang.Thread;
import java.lang.InterruptedException;

import java.util.logging.Logger;

import acsws.SCHEDULER_MODULE.SchedulerOperations;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;

public class SchedulerImpl implements ComponentLifecycle, SchedulerOperations {

   private ContainerServices m_containerServices;
   private DataBase database;
   private Telescope telescope;
   private Instrument instrument;
   private Logger m_logger;
   private String IDLdatabaseName="IDL:acsws/DATABASE_MODULE/DataBase:1.0";
   private String IDLinstrumentName="IDL:acsws/INSTRUMENT_MODULE/Instrument:1.0";
   private String IDLtelescopeName="IDL:acsws/TELESCOPE_MODULE/Telescope:1.0";
   private boolean running = false;
   private Thread sLoop;
   private int proposalUnderExec; 

   /////////////////////////////////////////////////////////////
   // Implementation of ComponentLifecycle
   /////////////////////////////////////////////////////////////

   public void initialize(ContainerServices containerServices) {
     m_containerServices = containerServices;
     
    try {

      org.omg.CORBA.Object db = m_containerServices.getDefaultComponent(IDLdatabaseName);
      database = DataBaseHelper.narrow(db);

    
      org.omg.CORBA.Object ins = m_containerServices.getDefaultComponent(IDLinstrumentName);
      instrument = InstrumentHelper.narrow(ins);


      org.omg.CORBA.Object telesc = m_containerServices.getDefaultComponent(IDLtelescopeName);
      telescope = TelescopeHelper.narrow(telesc);
    }
    catch (AcsJContainerServicesEx ex) {

     AcsJContainerServicesEx ex3 = new AcsJContainerServicesEx(ex);
     m_logger.warning(ex3.toString());
	


    }


     
     m_logger = m_containerServices.getLogger();
     m_logger.info("initialize() called...");
     
   }
   public void execute() {
     m_logger.info("execute() called...");
   }
   public void cleanUp() {
     m_logger.info("cleanUp() called..., nothing to clean up.");
     m_containerServices.releaseComponent(IDLdatabaseName);
     m_containerServices.releaseComponent(IDLinstrumentName);
     m_containerServices.releaseComponent(IDLtelescopeName);
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
	  public void start () throws SchedulerAlreadyRunningEx {


			if (running) {
				AcsJSchedulerAlreadyRunningEx ex = new AcsJSchedulerAlreadyRunningEx();
				ex.log(m_logger);
				throw ex.toSchedulerAlreadyRunningEx();
			
			}else {
				running = true;
				m_logger.info("Running...");
				m_logger.info("Start Scheduler 3"); 
				startLoop();
			}

	  }

	public void startLoop() {
		sLoop = new Thread(){
			public void run(){

			
				byte[] image; 

				try {
			
				Proposal[] proposals = database.getProposals();


					for (Proposal p: proposals){
					
						if (!running)
							break;

						proposalUnderExec=p.pid;

						database.setProposalStatus(p.pid, 1);
						instrument.cameraOn();

						for (Target t: p.targets){
							image = telescope.observe(t.coordinates, t.expTime);

							database.storeImage(p.pid,t.tid,image);

						}

						instrument.cameraOff();
						database.setProposalStatus(p.pid, 2);
					
					}
				} catch (InvalidProposalStatusTransitionEx ex) {
                                        AcsJInvalidProposalStatusTransitionEx ex2 = new AcsJInvalidProposalStatusTransitionEx(ex);
					ex2.log(m_logger);
				} catch(ProposalDoesNotExistEx ex){
					AcsJProposalDoesNotExistEx ex2 = new AcsJProposalDoesNotExistEx(ex);
					ex2.log(m_logger);
				} catch (PositionOutOfLimitsEx ex) {
					AcsJPositionOutOfLimitsEx ex2 = new AcsJPositionOutOfLimitsEx(ex);
					ex2.log(m_logger);
				} catch (ImageAlreadyStoredEx ex) {
					AcsJImageAlreadyStoredEx ex2 = new AcsJImageAlreadyStoredEx(ex);
					ex2.log(m_logger);
				}

				running = false;
			

			m_logger.info("Not Running...");

			}
		};
		sLoop.start();
	}


  @Override
  public void stop () throws SchedulerAlreadyStoppedEx {
	m_logger.info("Stop Scheduler 3");

	if (running){
		running = false;
	}else {
		AcsJSchedulerAlreadyStoppedEx ex = new AcsJSchedulerAlreadyStoppedEx();
		ex.log(m_logger);
		throw ex.toSchedulerAlreadyStoppedEx();
	}
  
  }

  @Override
  public int proposalUnderExecution() throws NoProposalExecutingEx {
	m_logger.info("Proposal Under Execution Scheduler  3");
 
	if (running){
		return proposalUnderExec;
	}else {
		AcsJNoProposalExecutingEx ex = new AcsJNoProposalExecutingEx();
		ex.log(m_logger);
		throw ex.toNoProposalExecutingEx();
	}
   
  }
	
}
	

