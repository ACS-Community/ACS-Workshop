package alma.DATABASE_MODULE;

import alma.DATABASE_MODULE.DataBaseImpl.DataBaseImpl;
import alma.SORTErr.ImageAlreadyStoredEx;
import alma.SORTErr.InvalidProposalStatusTransitionEx;
import alma.SORTErr.ProposalNotYetReadyEx;
import alma.SORTErr.wrappers.AcsJImageAlreadyStoredEx;
import alma.SORTErr.wrappers.AcsJInvalidProposalStatusTransitionEx;
import alma.SORTErr.wrappers.AcsJProposalNotYetReadyEx;
import alma.TYPES.Position;
import alma.TYPES.Proposal;
import alma.TYPES.Target;
import alma.acs.component.client.ComponentClientTestCase;

public class DataBaseTest extends ComponentClientTestCase {
	private DataBase comp;

	public void testSetProposalStatus(int pid, int status) {
		try {
			comp.setProposalStatus(100, 200);
			
		} catch (InvalidProposalStatusTransitionEx e) {
			e.printStackTrace();
			fail(e.toString());
		}
		
	}

	public void  testGetProposals() {
		Proposal[] props = comp.getProposals();
		assertNotNull(props);
		//fail("always failed to check test code");
	}

	public void  testAbnormalCases() throws Exception {
		comp.clean();
		assertEquals(DataBase.STATUS_NO_SUCH_PROPOSAL, comp.getProposalStatus(1));
		comp.removeProposal(2);
		
		Proposal[] props = comp.getProposals();
		assertEquals(0, props.length);

		{
			final int tid = 100;
			Target target = new Target();
			target.coordinates = new Position();
			target.tid = tid;
			Target[] targets = new Target[] { target};
			int pid = comp.storeProposal(targets);
			try {
				comp.storeImage(pid, tid, new int[3]);
			} catch (ImageAlreadyStoredEx e1) {
				throw AcsJImageAlreadyStoredEx.fromImageAlreadyStoredEx(e1);
			}
			try {
				comp.storeImage(pid, tid, new int[3]);
				fail("must fail");
			} catch (ImageAlreadyStoredEx e) {
			}
		}
	}

	public void testSenario() throws Exception {
		comp.clean();

		final int tid = 100;
		Target target = new Target();
		target.coordinates = new Position();
		target.tid = tid;
		Target[] targets = new Target[] { target};
		int pid = comp.storeProposal(targets);
		try {
			comp.setProposalStatus(pid, 0);
		} catch (InvalidProposalStatusTransitionEx e) {
			throw AcsJInvalidProposalStatusTransitionEx.fromInvalidProposalStatusTransitionEx(e);
		}

		int status = comp.getProposalStatus(pid);
		assertEquals(0, status);

		Proposal[] props = comp.getProposals();
		assertEquals(1, props.length);
		assertEquals(0, props[0].status);
		assertEquals(pid, props[0].pid);

		try {
			comp.storeImage(pid, tid, new int[] {1, 2, 3, 4, 5, 6});
		} catch (ImageAlreadyStoredEx e) {
			throw AcsJImageAlreadyStoredEx.fromImageAlreadyStoredEx(e);
		}

		int[][] imageList = null;
		try {
			imageList = comp.getProposalObservations(pid);
		} catch (ProposalNotYetReadyEx e) {
			throw AcsJProposalNotYetReadyEx.fromProposalNotYetReadyEx(e);
		}
		assertEquals(1, imageList.length);
		assertEquals(6, imageList[0].length);
		
		comp.removeProposal(pid);
		props = comp.getProposals();
		assertEquals(0, props.length);
		
	}

	@Override
	protected void setUp() throws Exception {
		super.setUp();
		System.out.println("setUp");
		this.comp = DataBaseHelper.narrow(this.getContainerServices().getComponent(DataBaseImpl.DATABASE_INSTANCE_NAME));
	}

	@Override
	protected void tearDown() throws Exception {
		 
	
		System.out.println("tearDown");
		
		comp = null;
		this.getContainerServices().releaseComponent(DataBaseImpl.DATABASE_INSTANCE_NAME);
		super.tearDown();
	}

	public DataBaseTest(String name) throws Exception {
		super(name);
	}

}
