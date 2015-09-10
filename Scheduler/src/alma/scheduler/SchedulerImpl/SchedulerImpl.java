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
package alma.scheduler.SchedulerImpl;
import java.util.logging.Logger;
import java.util.logging.Level;
import java.lang.Thread;
import java.lang.InterruptedException;

import org.omg.CORBA.DoubleHolder;
import org.omg.CORBA.IntHolder;

import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import alma.acs.exceptions.AcsJException;
import acsws.SCHEDULER_MODULE.SchedulerOperations;

// Component modules
import acsws.DATABASE_MODULE.*;
import acsws.TELESCOPE_MODULE.*;
import acsws.INSTRUMENT_MODULE.*;

/**
 * The Scheduler component implementation
 *
 * @author teams
 */
public class SchedulerImpl implements ComponentLifecycle, SchedulerOperations
{
	private ContainerServices m_containerServices;
	private Logger m_logger;

	// System Components
	private DataBase database;
	private Telescope telescope;
	private Instrument instrument;

	private int propCount = -1;
	private boolean running = false;
	private Thread mainLoop;

	/////////////////////////////////////////////////////////////
	// Implementation of ComponentLifecycle
	/////////////////////////////////////////////////////////////

	/**
	 * Initializes the components, thread loop and logger.
	 * @param ContainerServices containerServices
	 */
	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.finer("initialize() called...");

		// Instantiate Scheduler loop
		m_logger.fine("Instantiating loop...");
		initLoop();

		// Connect to remote components
		try {
			m_logger.fine("Retrieving component references...");
			database = DataBaseHelper.narrow(m_containerServices.getComponent("DATABASE"));
			telescope = TelescopeHelper.narrow(m_containerServices.getComponent("TELESCOPE"));
			instrument = InstrumentHelper.narrow(m_containerServices.getComponent("INSTRUMENT"));
		} catch(AcsJException e) {
			m_logger.log(Level.SEVERE, "Could not connect to a component. ", e);
		}
	}

	public void execute() {
		m_logger.finer("execute() called...");
	}

	public void cleanUp() {
		m_logger.fine("cleanUp() called...");
		//m_logger.fine("cleanUp(): Releasing components...");
		//m_containerServices.releaseComponent("DATABASE");
		//m_containerServices.releaseComponent("TELESCOPE");
		//m_containerServices.releaseComponent("INSTRUMENT");
	}

	public void aboutToAbort() {
		cleanUp();
		m_logger.fine("managed to abort...");
        System.out.println("SchedulerComponent component managed to abort... you should know this even if the logger did not flush correctly!");
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
	// Implementation of SchedulerComponentOperations
	/////////////////////////////////////////////////////////////

	/**
	 * Starts the scheduler task that loops over targets
	 */
	public void start() {
		m_logger.finer("start called...");
		running = true;

		m_logger.info("Start scheduling operations...");
		mainLoop.start();
	}

	/**
	 * Sets the stop flag for the scheduler task thread
	 */
	public void stop() {
		m_logger.finer("stop called...");
		running = false;
	}

	/**
	 * Gets the proposal under execution.
	 * @return int Returns -1 if no proposal is under execution,
	 * or the incremental number for the proposal.
	 */
	public int proposalUnderExecution() {
		m_logger.finer("proposalUnderExecution() called...");
		return propCount;
	}

	/**
	 * Instantiates the main loop (logic) of the Scheduler
	 */
	private void initLoop() {
		m_logger.finer("-initLoop() called...");
		mainLoop = new Thread() {
			public void run() {
				m_logger.finer("Entering main loop (while)...");
				while(isRunning()) {
					// Loop over proposals
					int totalProps = database.getProposals();

					if(totalProps > 0) {
						m_logger.fine("Looping over " + totalProps + " proposals...");
					} else {
						try {
							m_logger.info("No pending proposals! Checking again in 5s...");
							getLoop().sleep(5000);
						} catch(InterruptedException e) { }
					}

					for(propCount = 1; propCount <= totalProps; propCount++) {
						m_logger.info("Executing proposal #" + propCount);
						database.setProposalStatus(); // (running)
						instrument.cameraOn();

						int obs = telescope.observe(); // loop over targets
						database.storeImage(); // obs ??? ???

						try {
							m_logger.info("Waiting for observation 1s...");
							getLoop().sleep(1000);
						} catch(InterruptedException e) { }

						instrument.cameraOff();
						database.setProposalStatus(); // (ready)
						m_logger.info("Proposal #" + propCount + " ready!");
					}

				}

				m_logger.info("Scheduler task stopped");
			}
		};
	}

	private final boolean isRunning() {
		return running;
	}

	private final Thread getLoop() {
		return mainLoop;
	}
}
