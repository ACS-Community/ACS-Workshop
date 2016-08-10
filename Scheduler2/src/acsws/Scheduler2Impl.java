package acsws;

import java.util.logging.Logger;
import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import acsws.SCHEDULER_MODULE.SchedulerOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.NoProposalExecutingEx;
import acsws.SYSTEMErr.PositionOutOfLimitsEx;
import acsws.SYSTEMErr.ProposalDoesNotExistEx;
import acsws.TYPES.Proposal;
import acsws.DATABASE_MODULE.DataBase;
import acsws.DATABASE_MODULE.DataBaseHelper;
import acsws.INSTRUMENT_MODULE.Instrument;
import acsws.INSTRUMENT_MODULE.InstrumentHelper;
import acsws.TELESCOPE_MODULE.Telescope;
import acsws.TELESCOPE_MODULE.TelescopeHelper;

public class Scheduler2Impl implements ComponentLifecycle, SchedulerOperations, Runnable {

private ContainerServices m_containerServices;
private Logger m_logger;
private DataBase databaseComponente;
private Instrument instrumentComponente;
private Telescope telescopeComponente;
private Proposal[] proposals;
private boolean run;
private Thread thread;
private Proposal thisProposal;

public void initialize (ContainerServices containerServices) {
	m_containerServices = containerServices;
	m_logger = m_containerServices.getLogger();
	m_logger.info("initialize() called...");
	thisProposal = null;
	
	try {
		org.omg.CORBA.Object databaseObj = m_containerServices.getDefaultComponent("IDL:acsws/DATABASE_MODULE/DataBase:1.0");
		//org.omg.CORBA.Object databaseObj = m_containerServices.getComponent("DATABASE");
		databaseComponente = DataBaseHelper.narrow(databaseObj);
		org.omg.CORBA.Object instrumentObj = m_containerServices.getDefaultComponent("IDL:acsws/INSTRUMENT_MODULE/Instrument:1.0");
		//org.omg.CORBA.Object instrumentObj = m_containerServices.getComponent("INSTRUMENT");
		instrumentComponente = InstrumentHelper.narrow(instrumentObj);
		org.omg.CORBA.Object telescopeObj = m_containerServices.getDefaultComponent("IDL:acsws/TELESCOPE_MODULE/Telescope:1.0");
		//org.omg.CORBA.Object telescopeObj = m_containerServices.getComponent("TELESCOPE");
		telescopeComponente = TelescopeHelper.narrow(telescopeObj);
	} catch (AcsJContainerServicesEx e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
	
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
	if (thread == null) {
		run = true;
		thread = new Thread (this, "Scheduler");
		thread.start();
	}
}

public void stop() {
	m_logger.info("Scheduler stopped");
	if (thread != null) {
		run = false;
	}
	this.thread = null;
}

public int proposalUnderExecution() throws NoProposalExecutingEx{
	m_logger.info("Excecuting proposal");
	if (!run) {
		throw new NoProposalExecutingEx();
	}
	if (thisProposal==null){
		throw new NoProposalExecutingEx();
	}
	return thisProposal.pid;
}

public void run() {
	byte[] image;
	m_logger.info("Scheduler running");
	proposals = databaseComponente.getProposals();
	
	for(int i=0; i<proposals.length; i++){
		m_logger.info("Running proposal");
		thisProposal = proposals[i];
		try {
			databaseComponente.setProposalStatus(thisProposal.pid, 1);
			instrumentComponente.cameraOn();
			for(int j=0; j< thisProposal.targets.length; j++){
				image = telescopeComponente.observe(thisProposal.targets[j].coordinates, thisProposal.targets[j].expTime);
				databaseComponente.storeImage(thisProposal.pid, thisProposal.targets[j].tid, image);
			}
			instrumentComponente.cameraOff();
			
		} catch (InvalidProposalStatusTransitionEx e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			run = false;
		} catch (PositionOutOfLimitsEx e) {
			// TODO Auto-generated catch block
			run = false;
			e.printStackTrace();
		} catch (ImageAlreadyStoredEx e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			run = false;
		} catch (ProposalDoesNotExistEx e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		m_logger.info("Proposal finished");
		if(!run)
			break;
	}
	m_logger.info("Scheduler finished");
}

}
