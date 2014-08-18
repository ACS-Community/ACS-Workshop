package alma.scheduler;


import alma.SCHEDULER_MODULE.Scheduler;
import alma.SCHEDULER_MODULE.SchedulerHelper;
import alma.SORTErr.SchedulerAlreadyRunningEx;
import alma.acs.component.client.ComponentClientTestCase;

public class SchedulerIdlTest extends ComponentClientTestCase {

	private Scheduler scheduler;
	
	public SchedulerIdlTest() throws Exception {
		super("SchedulerIdlTest");
	}


	public void setUp() throws Exception {
		super.setUp();
		org.omg.CORBA.Object schedulerRawObj = getContainerServices().getComponent("SCHEDULER1");
		scheduler = SchedulerHelper.narrow(schedulerRawObj);
		assertNotNull(scheduler);
	}

	
	public void tearDown() throws Exception {
		getContainerServices().releaseComponent("SCHEDULER1");
		super.tearDown();
	}

	public void testStart() throws Exception {
		// start does not return any value. Here we can only test that no exception flies.
		scheduler.start();
	}

	public void testStartTwice() throws SchedulerAlreadyRunningEx {
		scheduler.start();
		try {
			scheduler.start();
			fail("Should fail to start the scheduler twice.");
		} catch (SchedulerAlreadyRunningEx e) {
			// fine
		}
	}
	
}
