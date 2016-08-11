
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
   private String databaseName="DATABASE";
   private String instrumentName="INSTRUMENT";
   private String telescopeName="TELESCOPE";
   private boolean running = false;
   private Thread sLoop;
   private int proposalUnderExec; 

   /////////////////////////////////////////////////////////////
   // Implementation of ComponentLifecycle
   /////////////////////////////////////////////////////////////

   public void initialize(ContainerServices containerServices) {
     m_containerServices = containerServices;
     
    try {

      org.omg.CORBA.Object db = m_containerServices.getComponent(databaseName);
      database = DataBaseHelper.narrow(db);

    
      org.omg.CORBA.Object ins = m_containerServices.getComponent(instrumentName);
      instrument = InstrumentHelper.narrow(ins);


      org.omg.CORBA.Object telesc = m_containerServices.getComponent(telescopeName);
      telescope = TelescopeHelper.narrow(telesc);
    }
    catch (AcsJContainerServicesEx ex) {
      m_logger.warning(ex.toString());
    }


     
     m_logger = m_containerServices.getLogger();
     m_logger.info("initialize() called...");
     
   }
   public void execute() {
     m_logger.info("execute() called...");
   }
   public void cleanUp() {
     m_logger.info("cleanUp() called..., nothing to clean up.");
     m_containerServices.releaseComponent(databaseName);
     m_containerServices.releaseComponent(instrumentName);
     m_containerServices.releaseComponent(telescopeName);
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

				Proposal[] proposals = database.getProposals();
				byte[] image; 

				try {

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
					m_logger.warning(ex.toString());
				} catch(ProposalDoesNotExistEx ex){
					m_logger.warning(ex.toString());
				} catch (PositionOutOfLimitsEx ex) {
					m_logger.warning(ex.toString());
				} catch (ImageAlreadyStoredEx ex) {
					m_logger.warning(ex.toString());
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
	

