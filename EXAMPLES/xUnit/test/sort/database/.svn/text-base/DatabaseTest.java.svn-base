package sort.database;

import sort.DATABASE_MODULE.DataBase;
import sort.DATABASE_MODULE.DataBaseHelper;
import sort.SORTErr.InvalidProposalStatusTransitionEx;
import sort.TYPES.Position;
import sort.TYPES.Proposal;
import sort.TYPES.Target;
import alma.acs.component.client.ComponentClientTestCase;

public class DatabaseTest extends ComponentClientTestCase {

	private DataBase comp;
	
	public DatabaseTest(String name) throws Exception {
		super(name);
	}

	@Override
	protected void setUp() throws Exception {
		super.setUp();
		comp = DataBaseHelper.narrow(getContainerServices().getComponent("DATABASE"));
		comp.clean();
	}

	@Override
	protected void tearDown() throws Exception {
		comp.clean();
		getContainerServices().releaseComponent("DATABASE");
		comp = null;
		super.tearDown();
	}
	
	public void testStoreProposal() throws Exception {
		Target[] targets = new Target[1];
		Target t = new Target();
		Position p = new Position();
		p.az = 0.0;
		p.el = 90.0;
		t.coordinates = p;
		t.tid = 0;
		t.expTime = 1;
		targets[0] = t;
		
		int pid = comp.storeProposal(targets);
		
		Proposal[] proposals = comp.getProposals();
		assertEquals(1, proposals.length);
		assertEquals(DataBase.STATUS_INITIAL_PROPOSAL, comp.getProposalStatus(pid));
		assertEquals(1, proposals[0].targets.length);
	}
	
	public void testException() {
		Target[] targets = new Target[1];
		Target t = new Target();
		Position p = new Position();
		p.az = 0.0;
		p.el = 90.0;
		t.coordinates = p;
		t.tid = 0;
		t.expTime = 1;
		targets[0] = t;
		
		int pid = comp.storeProposal(targets);
		try {
			comp.setProposalStatus(pid, 2);
		} catch (InvalidProposalStatusTransitionEx e) {
			this.m_logger.info("Exception Expected!! :D");
		}
		comp.clean();
	}
	
}
