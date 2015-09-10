/*
 *    ALMA - Atacama Large Millimiter Array
 *    (c) European Southern Observatory, 2002
 *    Copyright by ESO (in the framework of the ALMA collaboration),
 *    All rights reserved
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *    MA 02111-1307  USA
 */

package acsws.CONSOLE_MODULE.ConsoleImpl;

import java.util.logging.Logger;

import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;
import acsws.CONSOLE_MODULE.ConsoleOperations;
import acsws.CONSOLE_MODULE.ConsolePOATie;
import acsws.CONSOLE_MODULE.*;

/**
 * Component helper class. 
 * Generated for convenience, but can be modified by the component developer. 
 * Must therefore be treated like any other Java class (CVS, ...). 
 * <p>
 * To create an entry for your component in the Configuration Database, 
 * copy the line below into a new entry in the file $ACS_CDB/MACI/Components/Components.xml 
 * and modify the instance name of the component and the container: 
 * <p>
 * Name="Console11" Code="alma.demo.Console1Impl.Console1Helper" Type="IDL:alma/demo/Console1:1.0" Container="frodoContainer"
 * <p>
 * @author alma-component-helper-generator-tool
 */
public class Console1Helper extends ComponentHelper
{
	/**
	 * Constructor
	 * @param containerLogger logger used only by the parent class.
	 */
	public Console1Helper(Logger containerLogger)
	{
		super(containerLogger);
	}

	/**
	* @see alma.acs.container.ComponentHelper#_createComponentImpl()
	*/
	protected ComponentLifecycle _createComponentImpl()
	{
		return new demoConsole1();
	}

	/**
	* @see alma.acs.container.ComponentHelper#_getPOATieClass()
	*/
	protected Class _getPOATieClass()
	{
		return ConsolePOATie.class;
	}

	/**
	* @see alma.acs.container.ComponentHelper#getOperationsInterface()
	*/
	protected Class _getOperationsInterface()
	{
		return ConsoleOperations.class;
	}

}
