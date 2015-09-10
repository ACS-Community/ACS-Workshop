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
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USAo
 */
package teamt2;
import java.util.logging.Logger;

import org.omg.CORBA.DoubleHolder;
import org.omg.CORBA.IntHolder;

import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import acsws.TELESCOPE_MODULE.TelescopeOperations;

import acsws.INSTRUMENT_MODULE.Instrument;
import acsws.INSTRUMENT_MODULE.InstrumentHelper;
import acsws.INSTRUMENT_MODULE.InstrumentOperations;

import java.lang.*;
import java.util.Random;

/** 
 * A very simple component that does not make use of 
 * {@link alma.acs.component.ComponentImplBase}.
 * 
 * Javadoc comments have been removed to keep the
 * listing for the tutorial shorter.
 * 
 * @author hsommer
 */
public class Telescope2Impl implements ComponentLifecycle, TelescopeOperations
{
	private ContainerServices m_containerServices;
	private Logger m_logger;
	private Instrument m_comp;

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
        	System.out.println("Telescope2 component managed to abort... you should know this even if the logger did not flush correctly!");
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
	// Implementation of Telescope2
	/////////////////////////////////////////////////////////////
	
	public int observe() {
		int x,y,image=0;

		m_logger.info("observe()-Telescope2");
		System.out.println("observe()-Telescope2");	
					
		/*for(x=0;x<500;x++){
			for (y=0;y<500;y++){
			}
		}*/
				
		try
		{
			//Thread.sleep(2000);		
			org.omg.CORBA.Object compObj = m_containerServices.getComponent("INSTRUMENT");  // crea objeto CORBA
			m_comp = InstrumentHelper.narrow(compObj); // retorna objeto y lo guarda en m_comp
			image = m_comp.takeImage();
			System.out.println("Imagen");
			m_logger.info("Imagen");
				
		}
		catch (Exception ex)
		{
			//throw new InterruptedException();
			m_logger.info("Instrumento no disponible");
		}		

		return image;
	}
	
	public void moveTo() {
		m_logger.info("moveTo()-Telescope2");
		System.out.println("moveTo()-Telescope2");

		for(int i=0;i<9;i++){
			System.out.print("\007");
			System.out.flush();
			System.out.print("BEEP ");			
			try{
				Thread.sleep(300);
			}
			catch(Exception ex) {
				m_logger.info("Instrumento no disponible");
				//Thread.currentThread().interrup();
			}
		}
	}

	public int getCurrentPosition(){
		int rand=0;
		m_logger.info("getCurrentPosition()-Telescope2");
		System.out.println("getCurrentPosition()-Telescope2");
		Random rnd = new Random();
		rnd.setSeed(18);
		System.out.println(rnd);	
		return 0;
	}
}
