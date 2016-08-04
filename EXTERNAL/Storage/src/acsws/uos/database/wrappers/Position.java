package acsws.uos.database.wrappers;

public class Position {
	
	private final acsws.TYPES.Position pos;
	
	public Position() {
		this.pos = new acsws.TYPES.Position();
	}
	
	public Position(acsws.TYPES.Position pos) {
		this.pos = pos;
	}
	
	public void setAz(double az) {
		pos.az = az;
	}
	
	public double getAz() {
		return pos.az;
	}
	
	public void setEl(double el) {
		pos.el = el;
	}
	
	public double getEl() {
		return pos.el;
	}
	
}
