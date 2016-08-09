package acsws;

import java.util.logging.Logger;

import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;

import acsws.SCHEDULER_MODULE.SchedulerOperations;
import  acsws.SCHEDULER_MODULE.SchedulerPOATie;

/**
  * To create an entry for your component in the Configuration Database,
  * copy the line below into a new entry in the file $ACS_CDB/CDB/MACI/Components/Components.xml
  * and modify the instance name of the component and the container:
  * Name="SCHEDULER1" Code="acsws.SchedulerHelper" 
  * Type="IDL:acsws/SCHEDULER_MODULE/Scheduler:1.0" Container="frodoContainer"
  * Default="true" ImplLang="java"
  */
public class Scheduler1Helper extends ComponentHelper {
	public Scheduler1Helper(Logger containerLogger) {
		super(containerLogger);
	}
	
	protected ComponentLifecycle _createComponentImpl() {
		return new Scheduler1Impl();
	}
	
	protected Class _getPOATieClass() {
		return SchedulerPOATie.class;
	}
	
	protected Class _getOperationsInterface() {
		return SchedulerOperations.class;
	}
}
