/**
 * 
 */
package alma.impl.TaskScheduler.utils;

import alma.MARS.pos_struct;
import alma.MARS.action_type;

/**
 * Definition of a RAB
 * @author dclarke
 *
 */
public class RAB {
	
	
	public enum RABMode { MANUAL, AUTOMATIC; }
	
	private action_type type;
	private alma.MARS.pos_struct pos;
	private RABMode mode;
	private int sensorId;
	
	public static RAB newAutomaticRAB(action_type type, pos_struct pos, int sensorId) {
		return new RAB(RABMode.AUTOMATIC, type, pos, sensorId);
	}
	public static RAB newManualRAB(action_type type, pos_struct pos) {
		return new RAB(RABMode.MANUAL, type, pos, -1);
	}
	private RAB(RABMode mode, action_type type, pos_struct pos, int sensorId) {
		super();
		this.mode = mode;
		this.type = type;
		this.pos = pos;
		this.sensorId = sensorId;
	}
	
	public alma.MARS.pos_struct getPos() {
		return pos;
	}

	public action_type getType() {
		return type;
	}

	public int getSensorId() {
		return sensorId;
	}

	public RABMode getMode() {
		return mode;
	}

	public boolean isAutomatic() {
		return mode == RABMode.AUTOMATIC;
	}
	
	

}
