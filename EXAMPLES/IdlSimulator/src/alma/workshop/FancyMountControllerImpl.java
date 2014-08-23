/* @(#) $Id: MonitoringTester.midl,v 1.1 2007/06/06 23:04:30 rhiriart Exp $
 *
 * Copyright (C) 2001
 * Associated Universities, Inc. Washington DC, USA.
 *
 * Produced for the ALMA project
 *
 * This library is free software; you can redistribute it and/or modify it it 
 * under the terms of the GNU Library General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License 
 * along with this library; if not, write to the Free Software Foundation, 
 * Inc., 675 Massachusetts Ave, Cambridge, MA, 02139, USA.
 *
 * Correspondence concerning ALMA should be addressed as follows:
 * Internet email: alma-sw-admin@nrao.edu
 */

package alma.workshop;

import java.util.logging.Logger;

import alma.ACS.ComponentStates;
import alma.Control.Mount;
import alma.Control.MountHelper;
import alma.Control.Trajectory;
import alma.Control.Common.Util;
import alma.ControlExceptions.CAMBErrorEx;
import alma.ControlExceptions.INACTErrorEx;
import alma.ControlExceptions.IllegalParameterErrorEx;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;

public class FancyMountControllerImpl implements
		FancyMountControllerOperations, ComponentLifecycle {
	
    private ContainerServices container;
    private Logger logger;

	@Override
	public String name() {
		return container.getName();
	}

	@Override
	public ComponentStates componentState() {
		return container.getComponentStateManager().getCurrentState();
	}

	@Override
	public void initialize(ContainerServices containerServices)
			throws ComponentLifecycleException {
		container = containerServices;
		logger = container.getLogger();
	}

	@Override
	public void execute() throws ComponentLifecycleException {
		logger.fine("executing...");
	}

	@Override
	public void cleanUp() throws AcsJComponentCleanUpEx {
		logger.fine("cleaning up...");
	}

	@Override
	public void aboutToAbort() {
		logger.fine("about to abort...");
	}

	@Override
	public void makeAntennasDance() throws DanceErrorEx {
		logger.info("dancing...");
		Mount mount = null;
		try {
			mount = MountHelper.narrow(container.getComponent("Mount"));
		} catch (AcsJContainerServicesEx e) {
			e.printStackTrace();
			DanceErrorEx ex = new DanceErrorEx();
			ex.message = "Error accessing Mount component";
			throw ex;
		}
		
		try {
			//
			// The Mount needs to be in remote mode.
			// (Put IDL Simulator behaviour in the CDB.)
			//
			if (mount.inLocalMode()) {
				DanceErrorEx ex = new DanceErrorEx();
				ex.message = "Mount is in local mode";
				throw ex;
			}
			
			//
			// Set tolerance and check that the Mount will actually use this
			// tolerance.
			// (Use set/getGlobalData() from the CDB.)
			//
			mount.setTolerance(0.3);
			double tolerance = mount.getTolerance();
			logger.info("tolerance is " + tolerance);
			if ( 0.299 > tolerance || tolerance > 3.09 ) {
				DanceErrorEx ex = new DanceErrorEx();
				ex.message = "Tolerance is not 0.3 arcseconds as expected";
				throw ex;				
			}
			
			//
			// Make antenna antenna execute a silly "dance"
			// (Change the behaviour of setTrajectory from the test cases.)
			//
			double azStart = 0.2; // radians
			double elStart = 0.1; // radians
			long now = Util.arrayTimeToACSTime(Util.getArrayTime().get());
			long lead = (long) 10e7; // 10 seconds
			long startTime = now + lead;
			int steps = 10;
			double[] az = new double[steps];
			double[] el = new double[steps];
			long[] timestamps = new long[steps];
			double baseFreq = 0.1 / 1E7; // 0.1 in units of 1 / ACS::Time
			double azAmp = 0.1; // Azimuth amplitude in radians
			double elAmp = 0.2; // Elevation amplitude in radians
			long deltaT = (long) ( 1.0 * 1e7 ); // 1 second
			for (int i=0; i < steps; i++) {
				timestamps[i] = startTime + i * deltaT;
				az[i] = azStart + azAmp * Math.sin(2 * Math.PI * baseFreq * timestamps[i]);
				el[i] = elStart + elAmp * Math.sin(2 * Math.PI * baseFreq * timestamps[i]);
			}
			double[] azv = new double[steps];
			double[] elv = new double[steps];
			for (int i=1; i < steps; i++) {
				azv[i] = 1e7 * ( az[i] - az[i-1] ) / deltaT;
				elv[i] = 1e7 * ( el[i] - el[i-1] ) / deltaT;
			}
			
			Trajectory[] traj = new Trajectory[steps];
			for (int i = 0; i < steps; i++) {
				traj[i] = new Trajectory();
				traj[i].az = az[i];
				traj[i].el = el[i];
				traj[i].azRate = azv[i];
				traj[i].elRate = elv[i];
				traj[i].time = timestamps[i];
			}
			mount.setTrajectory(traj);
			
		} catch (INACTErrorEx e) {
			DanceErrorEx ex = new DanceErrorEx();
			ex.message = "Mount is inaccessible";
			throw ex;
		} catch (CAMBErrorEx e) {
			DanceErrorEx ex = new DanceErrorEx();
			ex.message = "Error communicating with Mount";
			throw ex;
		} catch (IllegalParameterErrorEx e) {
			DanceErrorEx ex = new DanceErrorEx();
			ex.message = "Illegal parameter error";
			throw ex;
		} finally {
			container.releaseComponent("Mount");			
		}
	}

}
