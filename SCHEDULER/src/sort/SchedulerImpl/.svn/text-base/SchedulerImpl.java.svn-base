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
package sort.SchedulerImpl;
import java.util.logging.Logger;

import sort.DATABASE_MODULE.DataBase;
import sort.DATABASE_MODULE.DataBaseHelper;
import sort.INSTRUMENT_MODULE.Instrument;
import sort.INSTRUMENT_MODULE.InstrumentHelper;
import sort.SCHEDULER_MODULE.SchedulerOperations;
import sort.SORTErr.NoProposalExecutingEx;
import sort.SORTErr.SchedulerAlreadyRunningEx;
import sort.SORTErr.SchedulerAlreadyStoppedEx;
import sort.SORTErr.wrappers.AcsJSchedulerAlreadyRunningEx;
import sort.SORTErr.wrappers.AcsJSchedulerAlreadyStoppedEx;
import sort.TELESCOPE_MODULE.Telescope;
import sort.TELESCOPE_MODULE.TelescopeHelper;
import sort.TYPES.Proposal;

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
public class SchedulerImpl implements ComponentLifecycle, SchedulerOperations
{
	private ContainerServices m_containerServices;
	private DataBase m_db; 
	private Instrument m_inst;
	private Telescope m_tele;
	private RunProposals m_thread;
	private Logger m_logger;
	private int currPid;

	/////////////////////////////////////////////////////////////
	// Implementation of ComponentLifecycle
	/////////////////////////////////////////////////////////////
	
	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.info("initialize() called...");
		currPid = -1;
		try {
			m_db = DataBaseHelper.narrow(m_containerServices.getComponent("DATABASE"));
			m_inst = InstrumentHelper.narrow(m_containerServices.getComponent("INSTRUMENT"));
			m_tele = TelescopeHelper.narrow(m_containerServices.getComponent("TELESCOPE"));
		} catch (AcsJContainerServicesEx e) {
			e.printStackTrace();
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
	public void start() throws SchedulerAlreadyRunningEx {
		if (m_db == null || m_tele == null || m_inst == null)
			throw new RuntimeException("Initalization failure");
		if (m_thread != null)
			throw new AcsJSchedulerAlreadyRunningEx().toSchedulerAlreadyRunningEx();
		// call database and get list of proposals
		Proposal[] propList = m_db.getProposals();
		if (propList.length != 0)
		{
			m_thread = new RunProposals(m_logger, propList, m_db, m_tele, m_inst);
			m_thread.start();
		}
	}

	@Override
	public void stop() throws SchedulerAlreadyStoppedEx {
		if (m_thread == null)
			throw new AcsJSchedulerAlreadyStoppedEx().toSchedulerAlreadyStoppedEx();
		// Tell thread to stop after current proposal
		m_thread.FinishObserving();
		
		// wait for current observation to complete
		try {
			m_thread.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		m_thread = null;
		
	}

	@Override
	public int proposalUnderExecution() throws NoProposalExecutingEx {
		return m_thread.GetProposalPidUnderExecution();
	}
}
