package acsws.SCHEDULER_MODULE.SchedulerImpl;

import java.util.logging.Logger;

import org.omg.PortableServer.Servant;

import acsws.SCHEDULER_MODULE.SchedulerOperations;
import acsws.SCHEDULER_MODULE.SchedulerPOATie;
import alma.ACS.ACSComponentOperations;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;

/**
 * Component helper class. 
 * Generated for convenience, but can be modified by the component developer. 
 * Must therefore be treated like any other Java class (CVS, ...). 
 * <p>
 * To create an entry for your component in the Configuration Database, 
 * copy the line below into a new entry in the file $ACS_CDB/MACI/Components/Components.xml 
 * and modify the instance name of the component and the container: 
 * @author alma-component-helper-generator-tool
 */
public class SchedulerHelper extends ComponentHelper
{
        /**
         * Constructor
         * @param containerLogger logger used only by the parent class.
         */
        public SchedulerHelper(Logger containerLogger)
        {
                super(containerLogger);
        }

        /**
        * @see alma.acs.container.ComponentHelper#_createComponentImpl()
        */
        protected ComponentLifecycle _createComponentImpl()
        {
                return new SchedulerImpl();
        }

        /**
        * @see alma.acs.container.ComponentHelper#_getPOATieClass()
        */
        protected Class<? extends Servant> _getPOATieClass()
        {
                return SchedulerPOATie.class;
        }

        /**
        * @see alma.acs.container.ComponentHelper#getOperationsInterface()
        */
        protected Class<? extends ACSComponentOperations> _getOperationsInterface()
        {
                return SchedulerOperations.class;
        }

}
