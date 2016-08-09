package acsws.Database2;

import java.util.logging.Logger;

import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;
import acsws.DATABASE_MODULE.DataBaseOperations;
import acsws.DATABASE_MODULE.DataBasePOATie;
import acsws.Database2.DataBaseImpl;

public class DatabaseComponentHelper extends ComponentHelper{

	public DatabaseComponentHelper(Logger containerLogger)
	   {
	     super(containerLogger); 
	   }
	protected ComponentLifecycle _createComponentImpl()
	   {
	     return new DataBaseImpl();
	   }
	protected Class _getPOATieClass()
	   {
	     return DataBasePOATie.class;
	   }
	protected Class _getOperationsInterface()
	   {
	     return DataBaseOperations.class;
	   }
}
