
package alma.INSTRUMENT_MODULE.InstrumentImpl;

import alma.DEVCCD_MODULE.DevCCD;
import java.util.logging.Logger;
import java.util.*;
import java.lang.*;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import alma.INSTRUMENT_MODULE.*;
import alma.TYPES.*;
import org.omg.CORBA.DATA_CONVERSION;

public class InstrumentImpl 
		implements ComponentLifecycle, InstrumentOperations
{

private ContainerServices m_containerServices;
private Logger m_logger;
private org.omg.CORBA.Object webCam;	
private DevCCD myCam;

	private DevCCD getCamera(){
	
		try{
		webCam = m_containerServices.getComponent("DEVCCD");
		m_logger.info( " Camera ready");
		return alma.DEVCCD_MODULE.DevCCDHelper.narrow(webCam);
		}
		catch (Exception e){
		m_logger.warning( "Can't reference DEVCCD!: "+ e.getMessage() );
		return null;
		}
	}

	/**
	 * Turns the Instrument camera on.
	 *
	 * @return None
	 */
	public void cameraOn (){
	DevCCD myCam = getCamera();
	myCam.on();
	m_logger.info(" Camera on");
	};

	/**
	 * Turns the Instrument off.
	 * A NULL string as the target identifier indicates that no image file
	 * should be saved.
	 *
	 * @todo  Function should be refactored so that only one operation is performed.
	 *
	 * @return None
	 */
	public void cameraOff (){
	DevCCD myCam = getCamera();
	myCam.off();
	m_logger.info(" Camera off");
	};

	/**
	 * Retrieve image from the Instrument. Raises an exception if the 
	 * camera is not on.
         *
         * @return array of longs containing the image pixels
	 */
	public	int[] takeImage( int exposureTime){
		int j[] = myCam.image(exposureTime);
		m_logger.info(" image taken");
		return j;
	}
	//		raises(UOSErr::CameraIsOffEx);
        
        /* Camera settings */

        /** 
         * Set the RGB configuration of the camera 
         * @param rgbConfig the TYPES::RGB configuration
         */
	public void setRGB(RGB rgbConfig){
		myCam.red().set_sync(rgbConfig.red);
	        myCam.blue().set_sync(rgbConfig.blue);
		myCam.green().set_sync(rgbConfig.green);
		m_logger.info(" RGB set");
	};
	// raises(UOSErr::CameraIsOffEx);
        
        /** 
         * Set the pixel bias configuration of the camera 
         * @param bias the pixel bias configuration
         */
  	public void setPixelBias( int bias){
	myCam.pixelBias().set_sync(bias);
	m_logger.info(" pixel bias set");

	};
	// raises(UOSErr::CameraIsOffEx);
        
        /** 
         * Set the reset level configuration of the camera 
         * @param resetLevel the reset level configuration
         */
       	public void setResetLevel( int  resetLevel){

	myCam.resetLevel().set_sync(resetLevel);
	m_logger.info(" reset level set");
	}
	// raises(UOSErr::CameraIsOffEx);
	


//ACSComponent

	public ComponentStates componentState(){
		return m_containerServices.getComponentStateManager().getCurrentState();
	}

	public String name(){
		return m_containerServices.getName();
	}

	public void initialize(ContainerServices containerServices) {
 		m_containerServices = containerServices;
 		m_logger = m_containerServices.getLogger();
 		m_logger.info("initialize() called...");
 	}
 
	public void execute() {
 		m_logger.info("execute() called...");
 		myCam = getCamera();
		
	}

	public void cleanUp() {
 		m_logger.info("cleanUp() called..., nothing to clean up.");
		m_containerServices.releaseComponent("DEVCCD"); 
	}
 
	public void aboutToAbort() {
 		cleanUp();
 		m_logger.info("managed to abort...");
 	}

} //Class End

