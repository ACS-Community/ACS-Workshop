package alma.workshop.console2Impl;
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

import java.util.logging.Logger;

import acsws.CONSOLE_MODULE.ConsoleOperations;
import acsws.TELESCOPE_MODULE.Telescope;
import acsws.INSTRUMENT_MODULE.Instrument;
import acsws.SCHEDULER_MODULE.Scheduler;
import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;

/**
 * A very simple component that does not make use of 
 * {@link alma.acs.component.ComponentImplBase}.
 * 
 * Javadoc comments have been removed to keep the
 * listing for the tutorial shorter.
 * 
 * @author hsommer
 */
public class Console2Impl implements ComponentLifecycle, ConsoleOperations
{	
	private ContainerServices m_containerServices;
	private Logger m_logger;
	
	// This variable stores the status of the scheduler
	private boolean schedulerStarted;

	/////////////////////////////////////////////////////////////
	// Implementation of ComponentLifecycle
	/////////////////////////////////////////////////////////////
	
	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.info("initialize() called...");
		schedulerStarted = false;
	}
    
	public void execute() {
		m_logger.info("Executing Console 2...");
	}
    
	public void cleanUp() {
		m_logger.info("Cleaning Up Console 2.");
	}
    
	public void aboutToAbort() {
		cleanUp();
		m_logger.info("managed to abort...");
        System.out.println("Console 2 component managed to abort... you should know this even if the logger did not flush correctly!");
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

	@Override
	public void setMode() {
		
		try{org.omg.CORBA.Object compObj = m_containerServices.getComponent("SCHEDULER");
		
		Scheduler SchedulerObj = acsws.SCHEDULER_MODULE.SchedulerHelper.narrow(compObj);
		
		if(!schedulerStarted) {
			SchedulerObj.start();
			schedulerStarted = true;
			m_logger.info("Se ha iniciado el Scheduler (started)");
		}
		else {
			SchedulerObj.stop();
			schedulerStarted = false;
			m_logger.info("Se ha detenido el Scheduler (stopped)");
		}
		
	} catch (AcsJContainerServicesEx e) {	
		m_logger.info("Error al interactuar con el Scheduler");	
	}		
		
		
		
	}

	@Override
	// Return 
	public boolean getMode() {
		
		return schedulerStarted;
	}

	@Override
	public void cameraOn() {
		// TODO Auto-generated method stub
		// Get the Corba object
					try{org.omg.CORBA.Object compObj = m_containerServices.getComponent("INSTRUMENT");
					
					
					Instrument InstrumentObj = acsws.INSTRUMENT_MODULE.InstrumentHelper.narrow(compObj);
					
					// Execute the method move from the telescope
					InstrumentObj.cameraOn();
					
					m_logger.info("La camara se ha encendido");
					
				} catch (AcsJContainerServicesEx e) {
					// TODO Auto-generated catch block
					
					m_logger.info("Error al intentar encender la camara");	
				}
	}

	@Override
	public void cameraOff() {
		// TODO Auto-generated method stub
		try{org.omg.CORBA.Object compObj = m_containerServices.getComponent("INSTRUMENT");
		
		
		Instrument InstrumentObj = acsws.INSTRUMENT_MODULE.InstrumentHelper.narrow(compObj);
		
		// Execute the method move from the telescope
		InstrumentObj.cameraOff();
		
		m_logger.info("La camara se ha apagado");
		
	} catch (AcsJContainerServicesEx e) {
		// TODO Auto-generated catch block
		
		m_logger.info("Error al intentar encender la camara");	
	}		
	}

	@Override
	public void moveTelescope() {
		// TODO Auto-generated method stub
		
		try {
			// Get the Corba object
			org.omg.CORBA.Object compObj = m_containerServices.getComponent("TELESCOPE");
			
			
			Telescope telescopeObj = acsws.TELESCOPE_MODULE.TelescopeHelper.narrow(compObj);
			
			// Read initial position of the telescope
			int initialPos = telescopeObj.getCurrentPosition();
			m_logger.info("El telescopio se encuientra en : "+ initialPos);
			
			// Execute the method move from the telescope
			telescopeObj.moveTo();
			m_logger.info("El telescopio se movio con exito");
			
			// Read initial position of the telescope
			int finalPos = telescopeObj.getCurrentPosition();
			m_logger.info("El telescopio se encuentra en : "+ finalPos);			
			
			
		} catch (AcsJContainerServicesEx e) {
			// TODO Auto-generated catch block
			
			m_logger.info("Error al intentar mover el telescopio");
		}
		
		
		
		
	}

	@Override
	public int getTelescopePosition() {
		// TODO Auto-generated method stub
		return 999;
	}

	@Override
	public int getCameraImage() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void setRGB() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setPixelBias() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setResetLevel() {
		// TODO Auto-generated method stub
		
	}

	
}
