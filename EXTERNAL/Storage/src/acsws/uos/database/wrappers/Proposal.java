package acsws.uos.database.wrappers;

import java.util.ArrayList;
import java.util.List;

public class Proposal {

	private final acsws.TYPES.Proposal proposal;
	
	public Proposal() {
		this.proposal = new acsws.TYPES.Proposal();
	}
	
	public Proposal(acsws.TYPES.Proposal proposal) {
		this.proposal = proposal;
	}
	
	public void setPid(int pid) {
		proposal.pid = pid;
	}
	
	public int getPid() {
		return proposal.pid;
	}
	
	public int getStatus() {
		return proposal.status;
	}
	
	public void setStatus(int status) {
		proposal.status = status;
	}
	
	public List<Target> getTargets() {
		ArrayList<Target> retVal =  new ArrayList<>(proposal.targets.length);
		for (acsws.TYPES.Target t: proposal.targets) {
			Target tmp = new Target(t);
			tmp.setPid(proposal.pid);
			retVal.add(tmp);
		}
		return retVal;
	}
	
	public void setTargets(List<Target> targets) {
		ArrayList<acsws.TYPES.Target> retVal = new ArrayList<>(targets.size());
		for (Target t: targets) {
			retVal.add(t.getTarget());
		}
		proposal.targets = retVal.toArray(new acsws.TYPES.Target[0]);
	}
}
