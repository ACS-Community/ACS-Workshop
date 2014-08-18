package alma.impl.TRD;

import alma.ACS.ACSComponentOperations;
import alma.MARS.TRDBaseOperations;
import alma.MARS.TRDBasePOATie;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ComponentHelper;
import alma.maciErrType.wrappers.AcsJComponentCreationEx;

import java.util.logging.Logger;

import org.omg.PortableServer.Servant;


/**
 * 
 */

/**
 * @author almadev
 *
 */
public class TRDHelper extends ComponentHelper {
	
	public TRDHelper (Logger containerLogger){
		super (containerLogger);
	}

	@Override
	protected ComponentLifecycle _createComponentImpl()
			throws AcsJComponentCreationEx {
		
		return new TRD();
	}

	@Override
	protected Class<? extends ACSComponentOperations> _getOperationsInterface() {
		// TODO Auto-generated method stub
		return TRDBaseOperations.class;
	}

	@Override
	protected Class<? extends Servant> _getPOATieClass() {
		// TODO Auto-generated method stub
		return TRDBasePOATie.class;
	}

}
