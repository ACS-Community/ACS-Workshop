package alma.CONSOLE_MODULE.ConsoleImpl;
import java.util.logging.Logger;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import alma.TYPES.*;
import org.omg.CORBA.Object;
import alma.CONSOLE_MODULE.ConsoleOperations;


public class ConsoleImpl implements ComponentLifecycle, ConsoleOperations 
{
	private ContainerServices m_containerServices;
	private Logger m_logger;
	private alma.SCHEDULER_MODULE.Scheduler scheduler;
	private alma.INSTRUMENT_MODULE.Instrument instrument;
	private alma.TELESCOPE_MODULE.Telescope telescope;
	
	private boolean mode=true; // true automatic mode
	

	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
 		m_logger = m_containerServices.getLogger();
 		m_logger.info("initialize() called...");
 	}

	public void execute() {
 		m_logger.info("execute() called...");
		
		try{
		
			org.omg.CORBA.Object comptScheduler = m_containerServices.getComponent("SCHEDULER");
			scheduler = alma.SCHEDULER_MODULE.SchedulerHelper.narrow(comptScheduler);
			
			org.omg.CORBA.Object comptInstrument = m_containerServices.getComponent("INSTRUMENT");
			instrument = alma.INSTRUMENT_MODULE.InstrumentHelper.narrow(comptInstrument);
			
			org.omg.CORBA.Object comptTelescope = m_containerServices.getComponent("TELESCOPE");
			telescope = alma.TELESCOPE_MODULE.TelescopeHelper.narrow(comptTelescope);
			
		}catch(Exception e){m_logger.info("Error components instances");}
 	}

 	public void cleanUp() {
 		m_logger.info("cleanUp() called..., nothing to clean up.");
 	}

 	public void aboutToAbort() {
 		cleanUp();
 		m_logger.info("managed to abort...");
	}

	public ComponentStates componentState() {
 		return m_containerServices.getComponentStateManager().getCurrentState();
	}

	public String name() {
 		return "hola";//m_containerServices.getComponentInstanceName();
	};
	

	public void setMode(boolean mode)
	{	
		if(mode!=getMode()){
			if(!mode){
				try{
				this.mode=mode;
				m_logger.info("Manual mode");
				scheduler.stop();
				}catch(Exception e){}
			}
			else{
				try{
				this.mode=mode;
				m_logger.info("Automatic mode");
				scheduler.start();
				}catch(Exception e){}
			}
		}
		
		
	}

	public boolean getMode()
	{
		return mode;
	}

	public void cameraOn(){
		if(!getMode()){
			try{
				m_logger.info("Camera on");
				instrument.cameraOn();
			}catch(Exception e){}
		}
	}

	public void cameraOff(){
		if(!getMode()){
			try{
				m_logger.info("Camera off");
				instrument.cameraOff();
			}catch(Exception e){}
		}
	}
	
	public void setRGB(RGB rgb){
		if(!getMode()){
			try{
				m_logger.info("Set RGB");
				instrument.setRGB(rgb);
			}catch(Exception e){}
		}
	}

	public void setPixelBias(int bias){
		if(!getMode()){
			try{
				m_logger.info("Set Pixel bias");
				instrument.setPixelBias(bias);
			}catch(Exception e){}
		}
	}

	public int[] getCameraImage(){ 
		int[] image;
		try{
			m_logger.info("Get Camera Image");
			image=instrument.takeImage(1);
			return image;
		}
		catch(Exception e){
		m_logger.info("Get Camera Image Exception");
		int[] i =new int[1];
		i[0]=-1;
		return i;
		}
		
	}
					
	public void setResetLevel(int resetlevel){
		if(!getMode()){
			try{
				m_logger.info("Set reset level");
				instrument.setResetLevel(resetlevel);
			}catch(Exception e){}
		}
	}


	public void moveTelescope(Position coordinates){
		if(!getMode()){
			try{
				m_logger.info("Move telescope");
				telescope.moveTo(coordinates);
			}catch(Exception e){}
		}	
	}

	public Position getTelescopePosition(){
		Position pos=new Position();
		try{
			m_logger.info("get telescope position");
			pos=telescope.getCurrentPosition();
			return pos;
		}catch(Exception e){
			pos.el=-1;
			pos.az=-1;
			return pos;
		}
						
	}
	
				
}
