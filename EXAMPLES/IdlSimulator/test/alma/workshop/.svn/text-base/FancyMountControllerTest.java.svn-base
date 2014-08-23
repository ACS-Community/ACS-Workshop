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

import alma.ACSSim.Simulator;
import alma.ACSSim.SimulatorHelper;
import alma.acs.component.client.ComponentClientTestCase;

public class FancyMountControllerTest extends ComponentClientTestCase {

	FancyMountController controller;
	Simulator simulator;
	
	public FancyMountControllerTest(String name) throws Exception {
		super(name);
	}

	@Override
	protected void setUp() throws Exception {
		super.setUp();
		controller = FancyMountControllerHelper.narrow(getContainerServices().getComponent("DancingMountController"));
		simulator = SimulatorHelper.narrow(getContainerServices().getComponent("SIMULATOR"));
		simulator.removeAllMethods();
	}

	@Override
	protected void tearDown() throws Exception {
		getContainerServices().releaseComponent("DancingMountController");
		getContainerServices().releaseComponent("SIMULATOR");
		super.tearDown();
	}

	public void testAntennaDance() throws Exception {
		controller.makeAntennasDance();
	}
	
	public void testSetTrajectoryError() throws Exception {
		String code = "LOGGER.logInfo('setTrajectory called from SimServer')\n";
		code += "from ControlExceptionsImpl import IllegalParameterErrorExImpl\n";
		code += "raise IllegalParameterErrorExImpl()";
	    simulator.setMethod("Mount", "setTrajectory", code, 0.0);

		try {
			controller.makeAntennasDance();		
		} catch (DanceErrorEx ex) {
			// good, the expected exception was thrown
			return;
		}
		
		// bad, it shouldn't have reached this point
		fail("Expected DanceErrorEx exception was not received");
	}
	
	public void testSetTrajectory() throws Exception {
		String code = "LOGGER.logInfo('setTrajectory called from SimServer')\n";
		code += "container = parameters[len(parameters)-1]\n";
		code += "traj = parameters[0]\n";
		code += "setGlobalData('Mount:traj_length', len(traj))\n";
		code += "return";
	    simulator.setMethod("Mount", "setTrajectory", code, 0.0);
		
		controller.makeAntennasDance();		
		
		String trajLength = simulator.getGlobalData("Mount:traj_length");
		assertEquals("10", trajLength);
	}
}
