package alma.SCHEDULER_MODULE.SchedulerImpl;

import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import java.util.logging.Logger;
import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.SCHEDULER_MODULE.SchedulerOperations;
import alma.DATABASE_MODULE.DataBase;
import alma.INSTRUMENT_MODULE.Instrument;
import alma.TELESCOPE_MODULE.Telescope;
import alma.TYPES.Proposal;
import alma.TYPES.Target;




public class SchedulerImpl implements ComponentLifecycle, SchedulerOperations {
	
	
	private ContainerServices m_containerServices;
    private Logger m_logger;
    static private int PROPOSAL_QUEUED=0;
    static private int PROPOSAL_RUNNING=1;
    static private int PROPOSAL_READY=2;
    private TheLoop loop;
    boolean loopValidator = true;
    boolean startValidator = false;
    int currentpid = -1;
    
    
    private class TheLoop extends Thread {
    	
    	
    	public void run() {
    		startValidator = true;
    		
    		Proposal prToRun;
    		org.omg.CORBA.Object compObj;
    		try {
    			compObj = m_containerServices.getComponent("DATABASE");
    			//assertNotNull(compObj);
    			DataBase dbComp = alma.DATABASE_MODULE.DataBaseHelper.narrow(compObj);
    			
    			compObj = m_containerServices.getComponent("INSTRUMENT");
    			//assertNotNull(compObj);
    			Instrument instrComp = alma.INSTRUMENT_MODULE.InstrumentHelper.narrow(compObj);
    			
    			compObj = m_containerServices.getComponent("TELESCOPE");
    			//assertNotNull(compObj);
    			Telescope telComp = alma.TELESCOPE_MODULE.TelescopeHelper.narrow(compObj);
    			
    			while(loopValidator){
    				m_logger.info("getProposals");
    				Proposal[] proposals = dbComp.getProposals();
    				
    				prToRun = null;
    				for (Proposal proposal : proposals) {	
    					if(proposal.status == PROPOSAL_QUEUED){
    						prToRun = proposal;
    						break;
    					}
    				}
    				
    				if(prToRun != null){
    					
    					currentpid = prToRun.pid;
    				
    					m_logger.info("setProposalStatus running");
    					dbComp.setProposalStatus(prToRun.pid,PROPOSAL_RUNNING);
    					m_logger.info("CameraOn");
    					instrComp.cameraOn();
    					Target[] targets = prToRun.targets;
    					for (Target target : targets) {
    						m_logger.info("storeImage");
    						dbComp.storeImage(prToRun.pid, target.tid, telComp.observe(target.coordinates, target.expTime));
    						}
    					m_logger.info("CameraOff");
    					instrComp.cameraOff();
    					m_logger.info("setProposalStatus ready");
    					dbComp.setProposalStatus(prToRun.pid,PROPOSAL_READY);
    				} else {
    					try {
    						m_logger.info("sleeping for 2 seconds...");
							Thread.sleep(2000);
							currentpid = -1;
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
    				}
    			}
    			if(loopValidator == false){
    				startValidator = false;
    				currentpid = -1;
    			}
    			
    			
    		} catch (AcsJContainerServicesEx e) {
    			// TODO Auto-generated catch block
    			e.printStackTrace();
    		}
    		
    	}
  
    }
    
    
    
    
/**
 * Implementation of ComponentLifecycle
 */
    
	public void initialize(ContainerServices containerServices) throws ComponentLifecycleException {
		// TODO Auto-generated method stub
		m_containerServices = containerServices;
        m_logger = m_containerServices.getLogger();
        m_logger.info("initialize() called...");

	}
    
	public void execute() throws ComponentLifecycleException {
		// TODO Auto-generated method stub
		m_logger.info("execute() called...");

	}
	
	public void cleanUp() {
		// TODO Auto-generated method stub
		m_logger.info("cleanUp() called..., nothing to clean up.");

	}
	
	public void aboutToAbort() {
		// TODO Auto-generated method stub
		
	}
	
   /**
    * Implementation of ACSComponent
    */

	
	public ComponentStates componentState() {
		// TODO Auto-generated method stub
		return m_containerServices.getComponentStateManager().getCurrentState();
	}

	public String name() {
		return m_containerServices.getName();
	}

	
	
    /**
     * Implementation of SchedulerOperations
     */

	
	/*
     * Start the scheduler.
     * The scheduler will loop through all available proposals,
     * either until all proposals are done or until the stop method is called.
     * Raises an exception if called twice.
     * @return None
     */
    public void start (){
    	m_logger.info("Initialized ScheduleImpl.start()");
    	
    	
    	if(startValidator == false){
    		loopValidator = true;
        	loop = new TheLoop();
        	loop.start();
    	}
    	else{
    		m_logger.warning("Start() method already in use! :P");
    	}

    }
    //          raises(UOSErr::SchedulerAlreadyRunningEx);

    /*
     * Stop the scheduler.
     * This will stop the scheduler from scheduling more proposals,
     * it will not(!) break the ongoing observation.
     *
     * Raises an exception if called twice.
     * @return None
     */
    public void stop (){
    	m_logger.info("Initialized ScheduleImpl.stop()");
    	loopValidator = false;
    	if(startValidator){
    		try {
    			loop.join();
    		} catch (InterruptedException e) {
    			// TODO Auto-generated catch block
    			e.printStackTrace();
    		}
    	}else{
		m_logger.warning("Start() method must be used first.");
    	}
    	
    }
    //          raises(UOSErr::SchedulerAlreadyStoppedEx);

    /**
     * Returns the pid of the proposal currently under execution
     *
     * Raises exception if no proposal is executing.
     * @retun Proposal ID
     */
    public int proposalUnderExecution(){
    	m_logger.info("current pid = " + currentpid);
		return currentpid;
    }
//          raises(UOSErr::NoProposalExecutingEx);









	
}
