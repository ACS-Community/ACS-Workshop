package alma.impl.TaskScheduler;
import java.util.SortedMap;

import java.util.TreeMap;
import java.util.logging.Logger;

import alma.ACS.ComponentStates;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.MARS.FDBase;
import alma.MARS.FDBaseHelper;
import alma.MARS.RCBase;
import alma.MARS.RCBaseHelper;
import alma.MARS.TRDBase;
import alma.MARS.TRDBaseHelper;
import alma.MARS.TSBaseOperations;
import alma.MARS.action_type;
import alma.MARS.command_type;
import alma.MARS.command_struct;
import alma.MARS.pos_struct;
import alma.MARS.report_struct;
import alma.MARS.robot_status_struct;
import alma.MARS.robot_type;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.impl.TaskScheduler.interfaces.RABExecutor;
import alma.impl.TaskScheduler.interfaces.RABQueue;
import alma.impl.TaskScheduler.utils.RAB;
import alma.impl.TaskScheduler.utils.TwoParallelFIFOs;
import alma.TaskSchedulerExceptions.RobotNotFoundEx;
import alma.TaskSchedulerExceptions.wrappers.AcsJRobotNotFoundEx;

public class TaskScheduler
		implements
			ComponentLifecycle,
			TSBaseOperations,
			RABExecutor {
	
	final static int GRIDSIZE = 5;

	private ContainerServices m_containerServices;
	private Logger m_logger;
	
	private RABQueue rabQueue;
	private RABQueueConsumer consumer;
	private Thread           consumerThread;
	private SortedMap<Long, RCBase> robots;
	private TRDBase taskAndReportDB;
	private FDBase failureDetector;

	// Implementation of  ComponentLifecycle
        public TaskScheduler() {
        	robots   = new TreeMap<Long, RCBase>();
        	rabQueue = new TwoParallelFIFOs();
        	consumer = new RABQueueConsumer(this, rabQueue);
        }

	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.info("initialize() called...");
		consumerThread = new Thread(consumer);
		consumerThread.start();
	}

	public void execute() throws ComponentLifecycleException {
		m_logger.info("execute() called...");

		// Getting refereces of robots components
		try {
			String componentList[]=m_containerServices.findComponents(null,"IDL:alma/MARS/RCBase:1.0");
			if (componentList.length == 0)
				throw new ComponentLifecycleException(
				"There is no robots components found in system");
			for (int i=0; i< componentList.length ; i++)
			{
				RCBase robot=RCBaseHelper.narrow(m_containerServices.getComponent(componentList[i]));
				robots.put((long)robot.robotType(),robot);
			}
		}
		catch (IndexOutOfBoundsException e) {
			throw new ComponentLifecycleException(String.format(
					"No Tracking DB components found in system: %s", e.getMessage()), e);
		}
		catch (AcsJContainerServicesEx e) {
			// Here we have to raise a beautiful exception
			throw new ComponentLifecycleException(String.format(
					"Cannot find Tracking DB component: %s", e.getMessage()), e);
		}


		// Getting reference of Task and Report Database 
		try {
			String trdComponent[]=m_containerServices.findComponents(null,"IDL:alma/MARS/TRDBase:1.0");
			taskAndReportDB=TRDBaseHelper.narrow(m_containerServices.getComponent(trdComponent[0]));
		}
		catch (IndexOutOfBoundsException e) {
			throw new ComponentLifecycleException(String.format(
					"No Tracking DB components found in system: %s", e.getMessage()), e);
		}
		catch (AcsJContainerServicesEx e) {
			// Here we have to raise a beautiful exception
			throw new ComponentLifecycleException(String.format(
					"Cannot find Tracking DB component: %s", e.getMessage()), e);
		}
		// Getting reference of failure detection component	
		try {
			String fdComponent[]=m_containerServices.findComponents(null,"IDL:alma/MARS/FDBase:1.0");
			failureDetector=FDBaseHelper.narrow(m_containerServices.getComponent(fdComponent[0]));
		}
		catch (IndexOutOfBoundsException e) {
			throw new ComponentLifecycleException(String.format(
					"No failure detection components found in system: %s", e.getMessage()), e);
		}
		catch (AcsJContainerServicesEx e) {
			// Here we have to raise a beautiful exception
			throw new ComponentLifecycleException(String.format(
					"Cannot find failure detection component: %s", e.getMessage()), e);
		}
	}

	public void cleanUp() {
		consumer.stop();
		consumerThread.interrupt();
		m_logger.info("cleanUp() called...");
	}

	public void aboutToAbort() {
		cleanUp();
		m_logger.info("aboutToAbort() called...");
	}

	// Implementation of ACSComponent

	public ComponentStates componentState() {
		return m_containerServices.getComponentStateManager().getCurrentState();
	}

	public String name() {
		return m_containerServices.getName();
	}

	// Definition of member TaskScheduler
	public void runManualRAB(int type, alma.MARS.pos_struct pos) {
		final RAB rab = RAB.newManualRAB(action_type.from_int(type), pos);
		
		m_logger.info(String.format(
				"incoming manual RAB of type %s at (%d, %d)",
				rab.getType(),
				rab.getPos().x,
				rab.getPos().y));
		
		rabQueue.addManual(rab);
		consumerThread.interrupt();
	}
	
	public void runAutoRAB(int type, alma.MARS.pos_struct pos, int sensorId) {
		final RAB rab = RAB.newAutomaticRAB(action_type.from_int(type), pos, sensorId);
		
		m_logger.info(String.format(
				"incoming automatic RAB of type %s at (%d, %d)",
				rab.getType(),
				rab.getPos().x,
				rab.getPos().y));
		
		rabQueue.addAutomatic(rab);
		consumerThread.interrupt();
	}
	
	public int[] getRobotsList() {
		final int[] result = new int[robots.size()];
		int i = 0;
		for (long robotId : robots.keySet() ) {
			result[i++] = (int)robotId;
		}
		return result;
	}
	
	public robot_status_struct getRobotStatus(int id)
		throws RobotNotFoundEx {
		robot_status_struct result = null;
		if (robots.containsKey(id)) {
			final RCBase robot = robots.get(id);
			result = robot.status();
		}
		
		if (result == null) {
			final String message = String.format("cannot find status for robot %d", id);
			m_logger.warning(message);
			AcsJRobotNotFoundEx ex = new AcsJRobotNotFoundEx();
			ex.setRobotID(id);
			throw ex.toRobotNotFoundEx();
		} else {
			m_logger.info(String.format(
					"status for robot %d is%n\tid: %d%n\ttype: %d%n\tpos: (%d, %d)%n\tisOperational: %s%n\tisParked: %s",
					id,
					result.id,
					result.type,
					result.position.x, result.position.y,
					result.isOperational,
					result.isParked));
		}
		return result;
	}

	
	// Implementation of RABExecutor
	
	@Override
	public void processOne(RAB rab) {

		m_logger.info(String.format(
				"processing %s RAB of type %s at (%d, %d)",
				rab.getMode(),
				rab.getType(),
				rab.getPos().x,
				rab.getPos().y));

		// Get the sequence of commands from the task DB
		final command_struct[] commands = taskAndReportDB.getRAB(rab.getType().value());

		m_logger.info(String.format(
				"got %d commands",
				commands.length));

		// Create the report structure for later writing to the task DB
		final report_struct report = new report_struct(true, -1, "",
				String.format("%s %s", rab.getMode(), rab.getType()));

		// Execute each command in sequence. Most of them specify a
		// particular robot, but for a fire-fighting action we use the
		// closest robot.
		for (final command_struct command : commands) {
			final int commandId = command.actionId;
			long robotId;

			if (commandId == command_type.firefight.value()) {
				// Fire! Fire! Don't Panic! Don't Panic! Find the robot
				// which is closest to the fire and use that.
				m_logger.info("fire action - finding nearest robot");
				try {
					robotId = nearestRobotTo(rab.getPos());
				} catch (RobotNotFoundEx e) {
					if (report.isSuccessful) {
						// First error
						report.isSuccessful = false;
						report.errorMsg = e.getMessage();
					} else {
						// subsequent error
						report.errorMsg = String.format("%s%n%s",
								report.errorMsg,
								e.getMessage());
					}
					m_logger.warning(String.format(
							"Error during action %d (%s): %s",
							commandId,
							command_type.from_int(commandId),
							e.getMessage()));
					m_logger.severe("Cannot find a fire fighting robot - we're all going to DIE!!!!");
					m_logger.severe("Running, screaming, to the escape pods...");
					continue;
				}
			} else {
				// Normal action, use the specified robot
				robotId = command.robotId;
			}

			{
				String commandType;
				String robotType;

				try {
					commandType = command_type.from_int(commandId).toString();
				} catch (Exception e) {
					commandType = "** unknown type **";
				}
				try {
					robotType = robot_type.from_int((int)robotId).toString();
				} catch (Exception e) {
					robotType = "** unknown type **";
				}
				m_logger.info(String.format(
						"action %d (%s) for robot %d (%s)",
						commandId,
						commandType,
						robotId,
						robotType));
			}

			RCBase robot = null;
			try {
				if (robots.containsKey(robotId)) {
					robot = robots.get(robotId);
				} else {
					AcsJRobotNotFoundEx ex = new AcsJRobotNotFoundEx();
					ex.setRobotID((int)robotId);
					throw ex.toRobotNotFoundEx();
				}

				m_logger.info(String.format(
						"Moving robot %d to (%d, %d)",
						robotId,
						rab.getPos().x,
						rab.getPos().y));
				robot.move(rab.getPos());

				m_logger.info(String.format(
						"Executing command %d with robot %d",
						commandId,
						robotId));
				robot.executeCommand(commandId);
			} catch (Exception e) {
				if (report.isSuccessful) {
					// First error
					report.isSuccessful = false;
					report.errorMsg = e.getMessage();
				} else {
					// subsequent error
					report.errorMsg = String.format("%s%n%s",
							report.errorMsg,
							e.getMessage());
				}
				m_logger.warning(String.format(
						"Error during action %d (%s) for robot %d (%s): %s",
						commandId,
						command_type.from_int(commandId),
						robotId,
						robot_type.from_int((int)robotId),
						e.getMessage()));
			} finally {
				if (robot != null) {
					m_logger.info(String.format(
							"Parking robot %d",
							robotId));
					robot.park();
				}
			}
		}

		// Write the status to the DB
		final int reportId = taskAndReportDB.createReport(report);

		// Reset the alarm sensor if this is an automatic command
		if (rab.isAutomatic()) {
			failureDetector.resetSensor(rab.getSensorId());
		}


	}

	/**
	 * Find the robot nearest to the given position.
	 * 
	 * @param firePos - the position we want a robot to get to.
	 * @return long - the id of the robot we found. -1 if no robot is found
	 */
	private long nearestRobotTo(pos_struct firePos) throws RobotNotFoundEx {
		double minDist = 0.0;
		long result  = -1;
		boolean first = true;
		boolean found = false;
		
		for (long robotId : robots.keySet()) {
			final pos_struct robotPos = robots.get(robotId).position();
			final double dist = (1.0*firePos.x - 1.0*robotPos.x)*(1.0*firePos.x - 1.0*robotPos.x) +
			                    (1.0*firePos.y - 1.0*robotPos.y)*(1.0*firePos.y - 1.0*robotPos.y);
			
			m_logger.finest(String.format(
					"robot %d @ (%d, %d) distance^2 to (%d, %d) is %f",
					robotId,
					robotPos.x, robotPos.y,
					firePos.x, firePos.y,
					dist));
			// no need to take sqrt as if a*a < b*b then |a| < |b|
			if (first || (dist < minDist)) {
				minDist = dist;
				result = robotId;
				first = false;
				found = true;
			}
		}
		if (!found) {
			m_logger.warning(String.format(
					"Cannot find a robot close to (%d, %d)",
					firePos.x,
					firePos.y));
			AcsJRobotNotFoundEx ex = new AcsJRobotNotFoundEx();
			ex.setRobotID(666);
			throw ex.toRobotNotFoundEx();
		}
		return result;
	}


}
