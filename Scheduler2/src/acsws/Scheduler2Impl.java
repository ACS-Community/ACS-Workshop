package acsws;

import java.sql.Timestamp;
import java.util.logging.Logger;
import cern.laser.source.alarmsysteminterface.ASIException;
import cern.laser.source.alarmsysteminterface.AlarmSystemInterface;
import cern.laser.source.alarmsysteminterface.AlarmSystemInterfaceFactory;
import cern.laser.source.alarmsysteminterface.FaultState;
import acsws.DATABASE_MODULE.DataBase;
import acsws.DATABASE_MODULE.DataBaseHelper;
import acsws.INSTRUMENT_MODULE.Instrument;
import acsws.INSTRUMENT_MODULE.InstrumentHelper;
import acsws.SCHEDULER_MODULE.SchedulerOperations;
import acsws.SYSTEMErr.ImageAlreadyStoredEx;
import acsws.SYSTEMErr.InvalidProposalStatusTransitionEx;
import acsws.SYSTEMErr.NoProposalExecutingEx;
import acsws.SYSTEMErr.PositionOutOfLimitsEx;
import acsws.SYSTEMErr.ProposalDoesNotExistEx;
import acsws.SYSTEMErr.SchedulerAlreadyRunningEx;
import acsws.SYSTEMErr.SchedulerAlreadyStoppedEx;
import acsws.TELESCOPE_MODULE.Telescope;
import acsws.TELESCOPE_MODULE.TelescopeHelper;
import acsws.TYPES.Proposal;
import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;

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
String faultFamily = "Scheduler";
AlarmSystemInterface alarmSource;
FaultState fs;

private void launchAlarm(String faultMember, int faultCode) {
	try {
		alarmSource = AlarmSystemInterfaceFactory.createSource(this.name());
		fs = AlarmSystemInterfaceFactory.createFaultState(faultFamily, faultMember, faultCode);
		fs.setDescriptor(FaultState.ACTIVE);
		fs.setUserTimestamp(new Timestamp(System.currentTimeMillis()));
		alarmSource.push(fs);
	} catch (ASIException e1) {
		// TODO Auto-generated catch block
		e1.printStackTrace();
	}
}

public void initialize (ContainerServices containerServices) {
	m_containerServices = containerServices;
	m_logger = m_containerServices.getLogger();
	m_logger.info("initialize() called...");
	thisProposal = null;
	
	try {
		org.omg.CORBA.Object databaseObj = m_containerServices.getDefaultComponent("IDL:acsws/DATABASE_MODULE/DataBase:1.0");
		databaseComponente = DataBaseHelper.narrow(databaseObj);
	} catch (AcsJContainerServicesEx e) {
		launchAlarm("DATABASE_ERROR_CONNECTION", 1);
		m_logger.severe("Reference to database could not be obtained.");
		e.printStackTrace();
	}
		
	try {
		org.omg.CORBA.Object instrumentObj = m_containerServices.getDefaultComponent("IDL:acsws/INSTRUMENT_MODULE/Instrument:1.0");
		instrumentComponente = InstrumentHelper.narrow(instrumentObj);
	} catch (AcsJContainerServicesEx e) {
		launchAlarm("INSTRUMENT_ERROR_CONNECTION", 1);
		m_logger.severe("Reference to instrument could not be obtained.");
		e.printStackTrace();
	}
	

	try {
		org.omg.CORBA.Object telescopeObj = m_containerServices.getDefaultComponent("IDL:acsws/TELESCOPE_MODULE/Telescope:1.0");
		telescopeComponente = TelescopeHelper.narrow(telescopeObj);
	} catch (AcsJContainerServicesEx e) {
		launchAlarm("TELESCOPE_ERROR_CONNECTION", 1);
		m_logger.severe("Reference to telescope could not be obtained.");
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

public void start() throws SchedulerAlreadyRunningEx {
	m_logger.info("Scheduler started");
	if (thread == null) {
		run = true;
		thread = new Thread (this, "Scheduler");
		thread.start();
	} else {
		m_logger.severe("Scheduler already running.");
		throw new SchedulerAlreadyRunningEx();
	}
}

public void stop() throws SchedulerAlreadyStoppedEx {
	
	if (thread != null) {
		m_logger.info("Scheduler stopped");
		run = false;
	} else {
		m_logger.severe("Scheduler already stopped.");
		throw new SchedulerAlreadyStoppedEx();
	}
	
}

public int proposalUnderExecution() throws NoProposalExecutingEx{
	
	if (thisProposal==null || !run) {
		m_logger.severe("No proposal executing.");
		throw new NoProposalExecutingEx();
	}
	m_logger.info("Excecuting proposal");
	return thisProposal.pid;
}

public void run() {
	byte[] image;
	m_logger.info("Scheduler running");
	proposals = databaseComponente.getProposals();
	for(int i=0; i<proposals.length; i++){
		m_logger.info("Running proposal");
		thisProposal = proposals[i];
		if (thisProposal.status==0){
			try {
		
			databaseComponente.setProposalStatus(thisProposal.pid, 1);
			instrumentComponente.cameraOn();
			for(int j=0; j< thisProposal.targets.length; j++){
				image = telescopeComponente.observe(thisProposal.targets[j].coordinates, thisProposal.targets[j].expTime);
				//FileOutputStream stream = new FileOutputStream("/home/almaproc/Desktop/image" + i + j + ".jpg");
				//stream.write(image);
				//stream.close();
				databaseComponente.storeImage(thisProposal.pid, thisProposal.targets[j].tid, image);
				if(!run){
					instrumentComponente.cameraOff();
					databaseComponente.setProposalStatus(thisProposal.pid, 0);
					thisProposal=null;
					break;
				}
			}
			instrumentComponente.cameraOff();
			databaseComponente.setProposalStatus(thisProposal.pid, 2);
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
			run = false;
		}/* catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			run = false;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			run = false;
		}*/
			
		m_logger.info("Proposal finished");
		}
		
		
			
	}
	thisProposal=null;
	m_logger.info("Scheduler finished");
	thread = null;
}

}