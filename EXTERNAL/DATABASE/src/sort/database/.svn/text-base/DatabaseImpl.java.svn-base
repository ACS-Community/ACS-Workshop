package sort.database;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.logging.Logger;

import sort.DATABASE_MODULE.DataBaseOperations;
import sort.SORTErr.ImageAlreadyStoredEx;
import sort.SORTErr.InvalidProposalStatusTransitionEx;
import sort.SORTErr.ProposalNotYetReadyEx;
import sort.SORTErr.wrappers.AcsJImageAlreadyStoredEx;
import sort.SORTErr.wrappers.AcsJInvalidProposalStatusTransitionEx;
import sort.SORTErr.wrappers.AcsJProposalNotYetReadyEx;
import sort.TYPES.Proposal;
import sort.TYPES.Target;
import sort.TYPES.Position;
import sort.database.pojos.Image;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;

public class DatabaseImpl implements DataBaseOperations, ComponentLifecycle {

	private ContainerServices services;
	private Logger logger;
	private HibernateDaoImpl dao;
	
	// ComponentLifecyle interface implementations
	@Override
	public String name() {
		return services.getName();
	}

	@Override
	public void initialize(ContainerServices containerServices)
			throws ComponentLifecycleException {
		this.services = containerServices;
		logger = containerServices.getLogger();
		dao = new HibernateDaoImpl();
		dao.setConfiguration(HibernateDaoImpl.getDefaultConfiguration());
		dao.getSession();
	}

	@Override
	public void execute() throws ComponentLifecycleException {

	}

	@Override
	public void cleanUp() throws AcsJComponentCleanUpEx {

	}

	@Override
	public void aboutToAbort() {

	}

	@Override
	public ComponentStates componentState() {
		return services.getComponentStateManager().getCurrentState();
	}
	
	// DataBaseOperations interface implementations
	@Override
	public int storeProposal(Target[] targets) {
		HashSet<sort.database.pojos.Target> targetsP =  new HashSet<sort.database.pojos.Target>();
		for (Target t: targets) {
			sort.database.pojos.Target tp = new sort.database.pojos.Target();
			tp.setExpTime(t.expTime);
			tp.setTid(t.tid);
			sort.database.pojos.Position pp = new sort.database.pojos.Position();
			pp.setAz(t.coordinates.az);
			pp.setEl(t.coordinates.el);
			tp.setCoordinates(pp);
			targetsP.add(tp);
		}
		sort.database.pojos.Proposal propp = new sort.database.pojos.Proposal();
		propp.setTargets(targetsP);
		propp.setStatus(0);
		synchronized (dao) {
			dao.saveProposal(propp);
			logger.info("Saved proposal successfully returned pid: " + dao.getMaxPid());
			return dao.getMaxPid();
		}
	}
	
	@Override
	public int getProposalStatus(int pid) {
		return dao.getProposalStatus(pid);
	}

	@Override
	public void removeProposal(int pid) {
		dao.deleteProposal(pid);
	}

	@Override
	public int[][] getProposalObservations(int pid)
			throws ProposalNotYetReadyEx {
		try {
			if (dao.getProposalStatus(pid) != 2)
				throw new AcsJProposalNotYetReadyEx().toProposalNotYetReadyEx();
		} catch (Exception ex) {
			throw new AcsJProposalNotYetReadyEx().toProposalNotYetReadyEx();
		}
		List<Image> images = dao.getImagesForProposal(pid);
		int[][] retVal = new int[images.size()][0];
		int i = 0;
		for (Image ip: images) {
			if (ip.getImage() == null) {
				retVal[i++] =  new int[0];
				continue;
			}
			ByteBuffer bf = ByteBuffer.allocate(ip.getImage().length).put(ip.getImage());
			retVal[i++] = bf.asIntBuffer().array();
		}
		return retVal;
	}

	@Override
	public Proposal[] getProposals() {
		ArrayList<Proposal> proposals = new ArrayList<Proposal>();
		for (sort.database.pojos.Proposal pp: dao.getNonObservedProposals()) {
			Proposal p = new Proposal();
			p.pid = pp.getPid();
			p.status = pp.getStatus();
			ArrayList<Target> targets = new ArrayList<Target>();
			for (sort.database.pojos.Target tp: pp.getTargets()) {
				Target t = new Target();
				t.tid = tp.getTid();
				t.expTime = tp.getTid();
				t.coordinates = new Position(tp.getCoordinates().getAz(), tp.getCoordinates().getEl());
				targets.add(t);
			}
			p.targets = new Target[targets.size()];
			targets.toArray(p.targets);
			proposals.add(p);
		}
		Proposal[] retVal = new Proposal[proposals.size()];
		proposals.toArray(retVal);
		return retVal;
	}

	@Override
	public void setProposalStatus(int pid, int status)
			throws InvalidProposalStatusTransitionEx {
		int currStatus = dao.getProposalStatus(pid);
		if (status > 2 || status < 0)
			throw new AcsJInvalidProposalStatusTransitionEx().toInvalidProposalStatusTransitionEx();
		switch (currStatus) {
		case 0:
			if (status != 1 )
				throw new AcsJInvalidProposalStatusTransitionEx().toInvalidProposalStatusTransitionEx();
			break;
		case 1:
			if (status != 2 )
				throw new AcsJInvalidProposalStatusTransitionEx().toInvalidProposalStatusTransitionEx();
			break;
		case 2:
			throw new AcsJInvalidProposalStatusTransitionEx().toInvalidProposalStatusTransitionEx();
		default:
			throw new AcsJInvalidProposalStatusTransitionEx().toInvalidProposalStatusTransitionEx();
		}
		sort.database.pojos.Proposal p = dao.getProposal(pid);
		p.setStatus(status);
		dao.updateProposal(p);
	}

	@Override
	public void storeImage(int pid, int tid, int[] image)
			throws ImageAlreadyStoredEx {
		if (dao.imageExists(pid, tid))
			throw new AcsJImageAlreadyStoredEx().toImageAlreadyStoredEx();
		Image ip = new Image();
		ip.setPid(pid);
		ip.setTid(tid);
		ByteBuffer bf = ByteBuffer.allocate(image.length * 4);
		for (int v: image) 
			bf.putInt(v);
		logger.info("Saving image. Size: " + bf.array().length);
		ip.setImage(bf.array());
		dao.saveImage(ip);
	}
	
	@Override
	public void clean() {
		logger.info("Removing all the data from the DB...");
		dao.clearDatabase();
		logger.info("Cleanning complete...");
	}
	
}
