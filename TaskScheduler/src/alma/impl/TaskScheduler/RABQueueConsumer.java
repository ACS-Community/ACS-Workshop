/**
 * 
 */
package alma.impl.TaskScheduler;

import alma.impl.TaskScheduler.utils.RAB;
import alma.impl.TaskScheduler.interfaces.RABExecutor;
import alma.impl.TaskScheduler.interfaces.RABQueue;

/**
 * @author dclarke
 *
 */
public class RABQueueConsumer implements Runnable {

	final private static int PollIntervalMS = 1000;
	
	private RABQueue queue = null;
	private boolean stopped;
	private RABExecutor executor;
	
	public RABQueueConsumer(RABExecutor executor, RABQueue queue) {
		this.queue = queue;
		this.executor = executor;
	}

	public void stop() {
		stopped = true;
	}
	
    public void run() {
    	stopped = false;
        while(!stopped) {
            try {
                Thread.sleep(PollIntervalMS);
            } catch(InterruptedException e) {
            }
            if(!stopped){
            	processAllInQueue();
            }
        }
    }

	private void processAllInQueue() {
		while (queue.hasElements()) {
			final RAB rab = queue.getNext();
			try {
				executor.processOne(rab);
			} finally {
				// do nothing, but carry on regardless
			}
		}
	}

}
