
package alma.UOS;

import java.util.logging.Logger;
import java.util.*;
import java.lang.*;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import alma.INSTRUMENT_MODULE.*;
import alma.TYPES.*;

public class InstrumentImpl 
		implements ComponentLifecycle, InstrumentOperations
{

private ContainerServices m_containerServices;
private Logger m_logger;

	/**
	 * Turns the Instrument camera on.
	 *
	 * @return None
	 */
	public void cameraOn (){};

	/**
	 * Turns the Instrument off.
	 * A NULL string as the target identifier indicates that no image file
	 * should be saved.
	 *
	 * @todo  Function should be refactored so that only one operation is performed.
	 *
	 * @return None
	 */
	public void cameraOff (){};

	/**
	 * Retrieve image from the Instrument. Raises an exception if the 
	 * camera is not on.
         *
         * @return array of longs containing the image pixels
	 */
	public	int[] takeImage( int exposureTime){
		int j[] = new int[2];
		return j;
	}
	//		raises(UOSErr::CameraIsOffEx);
        
        /* Camera settings */

        /** 
         * Set the RGB configuration of the camera 
         * @param rgbConfig the TYPES::RGB configuration
         */
	public void setRGB(RGB rgbConfig){};
	// raises(UOSErr::CameraIsOffEx);
        
        /** 
         * Set the pixel bias configuration of the camera 
         * @param bias the pixel bias configuration
         */
  	public void setPixelBias( int bias){};
	// raises(UOSErr::CameraIsOffEx);
        
        /** 
         * Set the reset level configuration of the camera 
         * @param resetLevel the reset level configuration
         */
       	public void setResetLevel( int  resetLevel){};
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
 	}

	public void cleanUp() {
 		m_logger.info("cleanUp() called..., nothing to clean up.");
 	}
 
	public void aboutToAbort() {
 		cleanUp();
 		m_logger.info("managed to abort...");
 	}

} //Class End

