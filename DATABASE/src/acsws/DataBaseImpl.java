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
package acsws;
import java.util.logging.Logger;

import org.omg.CORBA.DoubleHolder;
import org.omg.CORBA.IntHolder;

import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;

import acsws.DATABASE_MODULE.*;
/**
 * A very simple component that does not make use of 
 * {@link alma.acs.component.ComponentImplBase}.
 * 
 * Javadoc comments have been removed to keep the
 * listing for the tutorial shorter.
 * 
 * @author hsommer
 */
public class DataBaseImpl implements ComponentLifecycle, DataBaseOperations
{
	private ContainerServices m_containerServices;
	private Logger m_logger;

	/////////////////////////////////////////////////////////////
	// Implementation of ComponentLifecycle
	/////////////////////////////////////////////////////////////
	
	public void initialize(ContainerServices containerServices) {
	 try{			
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		
		m_logger.info("initialize() called...");
		}
	
	 catch (Exception e){
		m_logger.info("No funciono la comunikeichon, #include improperios.h");
		}	
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
        System.out.println("DataBase component managed to abort... you should know this even if the logger did not flush correctly!");
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
	// Implementation of DataBaseOperations
	/////////////////////////////////////////////////////////////
	
	public int storeProposal()
	{
		m_logger.info("storeProposal() called... ");
		return 1;
	}

	public int getProposalStatus()
	{
		m_logger.info("getProposalStatus() called... ");
	
		return 1;	
	}

	public void removeProposal()

	{
		m_logger.info("removeProposal() called... ");
	}

	public int getProposalObservations()
	{
		m_logger.info("getProposalObservations() called... ");

		return 1;

	}

	public int getProposals()
	{
		m_logger.info("getProposals() called... ");
		return 1;
	}

	public void setProposalStatus()
	{
		m_logger.info("setProposalStatus() called...");

	}

	public void storeImage()
	{
		m_logger.info("storeImage() called...");
	}

	public void clean()
	{
		m_logger.info("clean() called...");
		
	}
}
