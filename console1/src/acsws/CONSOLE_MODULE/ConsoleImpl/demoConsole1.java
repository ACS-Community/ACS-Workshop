package acsws.CONSOLE_MODULE.ConsoleImpl;

import java.util.logging.Logger;

import org.omg.CORBA.DoubleHolder;
import org.omg.CORBA.IntHolder;

import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import acsws.CONSOLE_MODULE.*;



public class demoConsole1 implements ComponentLifecycle, ConsoleOperations{

	private ContainerServices m_containerServices;
	private Logger m_logger;

	/////////////////////////////////////////////////////////////
	// Implementation of ComponentLifecycle
	/////////////////////////////////////////////////////////////
	
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
        System.out.println("HelloDemo component managed to abort... you should know this even if the logger did not flush correctly!");
	}

public void setMode()

{

	System.out.println("hi setMode");


}


	public boolean getMode()
{

	System.out.println("hi getMode");
	return true;


}
	public void cameraOn()

{

	System.out.println("hi cameraOn");


}

	public void cameraOff()
{

	System.out.println("hi cameraOff");


}

	public void moveTelescope()
{

	System.out.println("hi moveTelescope");


}
	public int getTelescopePosition(){

	System.out.println("hi getTelescopePosition");
	return 1;

}
	public int getCameraImage(){

	System.out.println("hi getCameraImage");

	return 1;
}

	public void setRGB(){

	System.out.println("hi setRGB");


}

	public void setPixelBias(){

	System.out.println("hi setPixelBias");


}

	public void setResetLevel(){

	System.out.println("hi setResetLevel");


}


public ComponentStates componentState() {
		return m_containerServices.getComponentStateManager().getCurrentState();
	}
	public String name() {
		return m_containerServices.getName();
	}

}
























