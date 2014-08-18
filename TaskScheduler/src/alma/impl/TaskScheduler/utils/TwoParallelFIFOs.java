/**
 * 
 */
package alma.impl.TaskScheduler.utils;

import java.util.ArrayList;

import alma.impl.TaskScheduler.interfaces.RABQueue;

/**
 * @author dclarke
 *
 */
public class TwoParallelFIFOs implements RABQueue {

	private ArrayList<RAB> automaticFIFO;
	private ArrayList<RAB> manualFIFO;
	
	public TwoParallelFIFOs() {
		super();
		automaticFIFO = new ArrayList<RAB>();
		manualFIFO = new ArrayList<RAB>();
	}

	/* (non-Javadoc)
	 * @see alma.impl.TaskScheduler.interfaces.RABQueue#addAutomatic(alma.impl.TaskScheduler.interfaces.RAB)
	 */
	public synchronized void addAutomatic(RAB rab) {
		automaticFIFO.add(rab);
	}

	/* (non-Javadoc)
	 * @see alma.impl.TaskScheduler.interfaces.RABQueue#addManual(alma.impl.TaskScheduler.interfaces.RAB)
	 */
	public synchronized void addManual(RAB rab) {
		manualFIFO.add(rab);
	}

	/* (non-Javadoc)
	 * @see alma.impl.TaskScheduler.interfaces.RABQueue#getNext()
	 */
	public synchronized RAB getNext() {
		RAB result = null;
		
		try {
			result = automaticFIFO.remove(0);
		} catch (IndexOutOfBoundsException e) {
			try {
				result = manualFIFO.remove(0);
			} catch (IndexOutOfBoundsException f) {
				result = null;
			}
		}
		return result;
	}

	/* (non-Javadoc)
	 * @see alma.impl.TaskScheduler.interfaces.RABQueue#hasElements()
	 */
	public synchronized boolean hasElements() {
		return !automaticFIFO.isEmpty() || !manualFIFO.isEmpty();
	}

}
