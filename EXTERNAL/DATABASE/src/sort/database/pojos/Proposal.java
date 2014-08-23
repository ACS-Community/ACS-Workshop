package sort.database.pojos;

import java.util.Set;

public class Proposal {

	private Integer pid;
	private Set<Target> targets;
	private Integer status;
	
	public Integer getPid() {
		return pid;
	}

	public void setPid(Integer pid) {
		this.pid = pid;
	}

	public Set<Target> getTargets() {
		return targets;
	}
	
	public void setTargets(Set<Target> targets) {
		this.targets = targets;
	}
	
	public Integer getStatus() {
		return status;
	}
	
	public void setStatus(Integer status) {
		this.status = status;
	}

}
