package acsws.uos.database.wrappers;

import java.io.Serializable;

public class Target implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = -5776321599710858187L;

	private final acsws.TYPES.Target target;
	
	private int pid;
	
	public Target() {	
		this.target = new acsws.TYPES.Target();
	}
	
	public Target(acsws.TYPES.Target target) {
		this.target = target;
	}
	
	public void setTid(int tid) {
		target.tid = tid;
	}
	
	public int getTid() {
		return target.tid;
	}
	
	public void setCoordinates(Position pos) {
		target.coordinates = new acsws.TYPES.Position(pos.getAz(), pos.getEl());
	}
	
	public Position getCoordinates() {
		return new Position(target.coordinates);
	}
	
	public int getExpTime() {
		return target.expTime;
	}
	
	public void setExpTime(int expTime) {
		target.expTime = expTime;
	}
	
	public acsws.TYPES.Target getTarget() {
		return target;
	}

	public int getPid() {
		return pid;
	}

	public void setPid(int pid) {
		this.pid = pid;
	}
	
	
}
