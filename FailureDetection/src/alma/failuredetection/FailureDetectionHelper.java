package alma.failuredetection;
import java.util.logging.Logger;
import alma.MARS.FDBasePOATie;
import alma.MARS.FDBaseOperations;	

import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;

public class FailureDetectionHelper extends ComponentHelper {
	/**
	 * Passes a logger to the callback object.
	 * @param containerLogger
	 */
	public FailureDetectionHelper(Logger containerLogger) {
		super(containerLogger);
	}

	/**
	 * Gets an instance of the implementation class of the FailureDetection component.
	 * @return ComponentLifecycle
	 * @see alma.acs.container.ComponentHelper#_createComponentImpl()
	 */
	protected ComponentLifecycle _createComponentImpl() {
		return new FailureDetection();
	}

	/**
	 * Gets an instance of the POATie class of the FailureDetection component.
	 * @return Class
	 * @see alma.acs.container.ComponentHelper#_getPOATieClass()
	 */
	protected Class _getPOATieClass() {
		return FDBasePOATie.class;
	}

	/**
	 * Gets an instance of the operations of the LampAccess component.
	 * @return Class
	 * @see alma.acs.container.ComponentHelper#getOperationsInterface()
	 */
	protected Class _getOperationsInterface() {
		return FDBaseOperations.class;
	}
	

}