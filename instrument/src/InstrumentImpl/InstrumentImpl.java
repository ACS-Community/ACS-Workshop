/*
 * ALMA - Atacama Large Millimiter Array (c) European Southern Observatory,
 * 2002 Copyright by ESO (in the framework of the ALMA collaboration), All
 * rights reserved
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
package InstrumentImpl;
import java.util.logging.Logger;
import java.lang.Thread;

import org.omg.CORBA.DoubleHolder;
import org.omg.CORBA.IntHolder;

import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import acsws.INSTRUMENT_MODULE.InstrumentOperations;


/**
 * A very simple component that does not make use of 
 * {@link alma.acs.component.ComponentImplBase}.
 * 
 * Javadoc comments have been removed to keep the
 * listing for the tutorial shorter.
 * 
 * @author hsommer
 */
public class InstrumentImpl implements ComponentLifecycle, InstrumentOperations
{
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
	// Implementation of HelloDemoOperations
	/////////////////////////////////////////////////////////////

	boolean statecam=false;	

	public void cameraOn(){
try{
	m_logger.info("camaraOn() fue llamada");
	if(statecam==false){
		m_logger.info("Encendiendo camara");		
		Thread.sleep(1000);
		m_logger.info("Camara encendida");
		statecam=true;
	}else{
		m_logger.info("La camara ya esta encendida");
	}
	}catch (Exception e1){}

	}
	public void cameraOff(){


		try{
			if(statecam==true){
				m_logger.info("camaraOff() fue llamada");
				m_logger.info("Apagando camara");
				Thread.sleep(1000);
				m_logger.info("Camara apagada");
				statecam=false;
	
		
			}else{
				m_logger.info("La camara esta apagada");
			}	
		}catch (Exception e1){}
	}


	public int takeImage(){
	try{

		Thread.sleep(1000);
		m_logger.info("takeImage() fue llamada");

		if(statecam==true){
		Thread.sleep(2000);
		m_logger.info("Tomando Imagen");
		Thread.sleep(2000);
		m_logger.info("Flashh!!");
		m_logger.info("Felicitaciones Muy fotogenico :)");
		return  1;

		}
else{
		m_logger.info("la camara esta apagada");
		return  -1;
		}
	}catch (Exception e){return -1;}
	}
	public void setRGB(){
	m_logger.info("setRGB() fue llamada");
	}
	public void setPixelBias(){
	m_logger.info("setPixelBias() fue llamada");
	}
	public void setResetLevel(){
	m_logger.info("setResetLevel() fue llamada");
	}


	
}
