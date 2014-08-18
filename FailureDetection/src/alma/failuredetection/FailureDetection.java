package alma.failuredetection;
import java.util.logging.Logger;
import java.util.HashSet;
import java.util.Vector;
import cern.laser.source.alarmsysteminterface.FaultState;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.acs.logging.AcsLogLevel;
import alma.acs.nc.Consumer;
import alma.alarmsystem.clients.SourceClient;
import alma.alarmsystem.clients.source.SourceListener;
import alma.MARS.SCBaseHelper;
import alma.MARS.TSBaseHelper;
import alma.MARS.sensor_status_struct;
import alma.MARS.action_type;
import alma.MARS.FDBaseOperations;
import alma.MARS.SCBase;
import alma.MARS.TSBase;

public class FailureDetection implements ComponentLifecycle,FDBaseOperations, SourceListener {
	private ContainerServices m_containerServices;
	private Logger m_logger;
	private String name[];
	private Vector<SCBase> sensorVector;
	private SourceClient client;
	private TSBase task_scheduler;
	// Implementation of  ComponentLifecycle

	public void initialize(ContainerServices containerServices){
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger(); 
		m_logger.info("initialize() called...");
	}

	public void execute() throws ComponentLifecycleException {
		
		m_logger.info("execute() called...");
		try {
			name = m_containerServices.findComponents(null, "IDL:alma/MARS/SCBase:1.0");
			sensorVector = new Vector<SCBase>();
			for(int i=0; i < name.length;i++) {
				sensorVector.add(SCBaseHelper.narrow(m_containerServices.getComponent( name[i])) );	
			}

			client = new SourceClient(m_containerServices);
			client.addAlarmListener(this);
			client.connect();
			m_logger.info("connected to alarm channel");
			// TODO use getDefaultComponent once the single comp instance is marked with Default=true in the CDB
			String TS_name[] = m_containerServices.findComponents(null, "IDL:alma/MARS/TSBase:1.0");
			if(TS_name.length > 0) {
				task_scheduler = TSBaseHelper.narrow(m_containerServices.getComponent(TS_name[0]));
			} else {
				throw new ComponentLifecycleException("no components TSBase found");  
			}
		}
		catch (ComponentLifecycleException e1) {
			throw e1;
		}
		catch(Exception e) {
			 m_logger.log(AcsLogLevel.DEBUG, "got exception on initialization.", e);
			 throw new ComponentLifecycleException(e);
		}
		
	}

	public void cleanUp() {
		m_logger.info("cleanUp() called...");
		// TODO : to be more robust, cache the comp names before and use these strings here
		// instead of making remote calls to name()
		for (SCBase sensorComp : sensorVector) {
			m_containerServices.releaseComponent(sensorComp.name());
		}
		m_containerServices.releaseComponent(task_scheduler.name());
		client.close();
	}

	public void aboutToAbort() {
		cleanUp();
		m_logger.info("aboutToAbort() called...");
	}

	// Implementation of ACSComponent

	public ComponentStates componentState() {
		return m_containerServices.getComponentStateManager().getCurrentState();
	}

	public String name() {
		return m_containerServices.getName();
	}

    public int[] getSensorsList() {
    	int array[] = new int[sensorVector.size()];
    	for(int i=0; i < sensorVector.size(); i++) {
    		array[i] = ( (SCBase)sensorVector.elementAt(i) ).status().id;
    	}
		return array;
	}
        	
    public sensor_status_struct sensorStatus(int id) {
    	for(int i=0; i < sensorVector.size(); i++) {
    		if( ( (SCBase)sensorVector.elementAt(i) ).status().id == id ) {
    			return ( (SCBase)sensorVector.elementAt(i) ).status();
    		}
    	}
	return null;
    }

    public void resetSensor(int id) {
    	for(int i=0; i < sensorVector.size(); i++) {
    		if( ( (SCBase)sensorVector.elementAt(i) ).status().id == id ) {
    			( (SCBase)sensorVector.elementAt(i) ).reset();
    		}
    	}
	}
    public void sourceXMLMsgReceived(String some) {
    	m_logger.info("receive xml alarm type :"+some);
    }
    
    public void faultStateReceived(FaultState faultst) {
    	long type = 0;
    	switch( faultst.getCode()){
    	case SCBase.alarmTypeVacuum:
    		type = alma.MARS.action_type.vacuum.value();
    		break;
    	case SCBase.alarmTypeFire:
    		type = alma.MARS.action_type.fire.value();
    		break;
    	case SCBase.alarmTypePiping:
    		type = alma.MARS.action_type.piping.value();
    		break;
    	}
    	//m_logger.info("receive alarm type :"+type);
    	String member = faultst.getMember();
    	String pippo=member.substring(member.length()-1, member.length());
    	int id = Integer.parseInt(pippo);
    	id = id -1;
    	//m_logger.info("receive alarm ids:"+id);
    	//m_logger.info("receive alarm vectorSize:"+sensorVector.size());
    	for(int i=0; i < sensorVector.size(); i++) {
    		//m_logger.info("vector id: " + ( sensorVector.elementAt(i) ).status().id + "index: " + i);
    		if( ( sensorVector.elementAt(i) ).status().id == id ) {
    			m_logger.info("Get a sensor id "+id);
    		   	task_scheduler.runAutoRAB((int)type, (sensorVector.elementAt(i) ).position(), id);
    		}
    	}
 	
    		
 
    }
}
