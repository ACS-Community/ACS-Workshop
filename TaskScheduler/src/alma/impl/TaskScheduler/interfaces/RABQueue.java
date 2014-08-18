/**
 * 
 */
package alma.impl.TaskScheduler.interfaces;

import alma.impl.TaskScheduler.utils.RAB;

/**
 * The operations required from a queue of RABs. Implementations should
 * handle synchronization issues as necessary.
 * 
 * @author dclarke
 * $Id: RABQueue.java,v 1.3 2009/11/12 12:46:35 almamgr Exp $
 */
public interface RABQueue {

	public void addManual(RAB rab);
	public void addAutomatic(RAB rab);
	public RAB  getNext();
	public boolean hasElements();
	
}
