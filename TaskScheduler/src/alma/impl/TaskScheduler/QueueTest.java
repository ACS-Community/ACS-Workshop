package alma.impl.TaskScheduler;
import alma.MARS.action_type;
import alma.acs.container.ContainerServices;
import alma.impl.TaskScheduler.interfaces.RABExecutor;
import alma.impl.TaskScheduler.interfaces.RABQueue;
import alma.impl.TaskScheduler.utils.RAB;
import alma.impl.TaskScheduler.utils.TwoParallelFIFOs;

public class QueueTest implements RABExecutor {
	
	private RABQueue rabQueue;
	private RABQueueConsumer consumer;
	private Thread           consumerThread;

	// Implementation of  ComponentLifecycle
        public QueueTest() {
        	rabQueue = new TwoParallelFIFOs();
        	consumer = new RABQueueConsumer(this, rabQueue);
        }

	public void initialize(ContainerServices containerServices) {
		consumerThread = new Thread(consumer);
		consumerThread.start();
	}

	public void execute() {
	}

	public void cleanUp() {
		consumer.stop();
		consumerThread.interrupt();
	}

	public void aboutToAbort() {
		cleanUp();
	}

	// Definition of member TaskSchedulerOperations
	public void runManualRAB(int type, alma.MARS.pos_struct pos) {
		final RAB rab = RAB.newManualRAB(action_type.from_int(type), pos);

		System.out.format(
				"%s.runManualRAB((%d, %d) of type %s)%n",
				Thread.currentThread().getName(),
				rab.getPos().x,
				rab.getPos().y,
				rab.getType());
		
		rabQueue.addManual(rab);
		consumerThread.interrupt();
	}
	
	public void runAutoRAB(int type, alma.MARS.pos_struct pos, int sensorId) {
		final RAB rab = RAB.newAutomaticRAB(action_type.from_int(type), pos, sensorId);

		System.out.format(
				"%s.runAutoRAB((%d, %d) of type %s)%n",
				Thread.currentThread().getName(),
				rab.getPos().x,
				rab.getPos().y,
				rab.getType());

		rabQueue.addAutomatic(rab);
//		consumerThread.interrupt();
	}
	
	@Override
	public void processOne(RAB rab) {
		System.out.format(
				"\t\t%s.processOne((%d, %d) of type %s)%n",
				Thread.currentThread().getName(),
				rab.getPos().x,
				rab.getPos().y,
				rab.getType());
	}


	public static void main(String argv[]) {
		final QueueTest qt = new QueueTest();
		boolean auto = false;
		int     type = 0;
		
		qt.initialize(null);
		
		for (int i = 0; i < 10; i++) {
			for (int x = 1; x < 4; x++) {
				for (int y = 1; y < 4; y ++) {
					final alma.MARS.pos_struct pos = new alma.MARS.pos_struct(x, y);
					if (auto) {
						qt.runAutoRAB(type, pos, i&x&y);
					}
					qt.runManualRAB(type, pos);
					auto = ! auto;
					type ++;
					if (type > 2) {
						type = 0;
					}
					try {
						Thread.sleep((x+y)*60);
					} catch (InterruptedException e) {
					}
				}
			}
		}
		
		qt.cleanUp();
	}
}
