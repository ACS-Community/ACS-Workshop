package alma.demo;

import acsws.TYPES.RGB;
import acsws.SYSTEMErr.CameraIsOffEx;
import alma.ACS.ComponentStates;

import org.omg.PortableServer.Servant;
import alma.ACS.ACSComponentOperations;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;
import alma.acs.container.ContainerServices;
import acsws.INSTRUMENT_MODULE.InstrumentOperations;
import acsws.INSTRUMENT_MODULE.InstrumentPOATie;

import java.util.logging.Logger;
//import acsws.INSTRUMENT_MODULE.InstrumentImpl.InstrumentImpl;

public class instrument_java 
 implements ComponentLifecycle,
 acsws.INSTRUMENT_MODULE.InstrumentOperations{
	 

	@Override
	public void cameraOn() {
		// TODO Auto-generated method stub
		System.out.println("Camera is on");
	}

	@Override
	public void cameraOff() {
		// TODO Auto-generated method stub
		System.out.println("Camera is off");
	}

	@Override
	public int[] takeImage(int exposureTime) throws CameraIsOffEx {
		// TODO Auto-generated method stub
		System.out.println("image taken with " + exposureTime );
		
		return null;
		
		
	}

	@Override
	public void setRGB(RGB rgbConfig) throws CameraIsOffEx {
		// TODO Auto-generated method stub
		System.out.println("RGB set");
		
		
	}

	@Override
	public void setPixelBias(int bias) throws CameraIsOffEx {
		// TODO Auto-generated method stub
		System.out.println("Pixel bias set");
	}

	@Override
	public void setResetLevel(int resetLevel) throws CameraIsOffEx {
		// TODO Auto-generated method stub
		System.out.println("reset level set");
		
	}

	@Override
	public String name() {
		// TODO Auto-generated method stub
		System.out.println("name requested");
		return null;
	}

	@Override
	public ComponentStates componentState() {
		// TODO Auto-generated method stub
		System.out.println("component state");
		return null;
	}
	
	
	@Override
	public void aboutToAbort(){
		System.out.println("about to abort");
	}
	
	
	@Override
	public void cleanUp(){
		System.out.println("cleanup");
	}
	
	@Override
	public void execute(){
		System.out.println("execute");
	}
	
	@Override
	public void initialize(ContainerServices cs){
		System.out.println("container services");
	}
	
//	 /**
//     * Constructor
//     * @param containerLogger logger used only by the parent class.
//     */
//    public instrument_java(Logger containerLogger)
//    {
//            super(containerLogger);
//    }
//
//	/**
//     * @see alma.acs.container.ComponentHelper#_createComponentImpl()
//     */
//     protected ComponentLifecycle _createComponentImpl()
//     {
//             return new instrument_java();
//     }
//
//     /**
//     * @see alma.acs.container.ComponentHelper#_getPOATieClass()
//     */
//     protected Class<? extends Servant> _getPOATieClass()
//     {
//             return InstrumentPOATie.class;
//     }
//
//     /**
//     * @see alma.acs.container.ComponentHelper#getOperationsInterface()
//     */
//     protected Class<? extends ACSComponentOperations> _getOperationsInterface()
//     {
//             return InstrumentOperations.class;
//     }
//	

}
