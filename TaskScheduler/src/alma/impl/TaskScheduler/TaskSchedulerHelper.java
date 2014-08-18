package alma.impl.TaskScheduler;
import java.util.logging.Logger;
import  alma.MARS.*;

import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;

public class TaskSchedulerHelper extends ComponentHelper {
	/**
	 * Passes a logger to the callback object.
	 * @param containerLogger
	 */
	public TaskSchedulerHelper(Logger containerLogger) {
		super(containerLogger);
	}

	/**
	 * Gets an instance of the implementation class of the TaskScheduler component.
	 * @return ComponentLifecycle
	 * @see alma.acs.container.ComponentHelper#_createComponentImpl()
	 */
	protected ComponentLifecycle _createComponentImpl() {
		return new TaskScheduler();
	}

	/**
	 * Gets an instance of the POATie class of the TaskScheduler component.
	 * @return Class
	 * @see alma.acs.container.ComponentHelper#_getPOATieClass()
	 */
	protected Class _getPOATieClass() {
		return TSBasePOATie.class;
	}

	/**
	 * Gets an instance of the operations of the TaskScheduler component.
	 * @return Class
	 * @see alma.acs.container.ComponentHelper#getOperationsInterface()
	 */
	protected Class _getOperationsInterface() {
		return TSBaseOperations.class;
	}

}
