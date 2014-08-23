package astri.sort.database;

import alma.acs.component.client.ComponentClientTestCase;

import astri.DATABASE_MODULE.DataBaseHelper;
import astri.DATABASE_MODULE.DataBaseOperations;
import astri.SORTErr.ProposalNotYetReadyEx;

public class DatabaseTest extends ComponentClientTestCase {

public DatabaseTest(String name) throws Exception {
		super(name);
		// TODO Auto-generated constructor stub
	}
	//	public DatabaseTest(String name) throws Exception {
//		super("");
//		// TODO Auto-generated constructor stub
//	}
	private astri.DATABASE_MODULE.DataBase comp;
	
	protected void setUp() throws Exception {
		super.setUp();
		this.comp = DataBaseHelper.narrow(this.getContainerServices().getComponent("DATABASE"));
	}

	public void testStoreProposal() throws Exception {
		astri.TYPES.Position pos = new astri.TYPES.Position(12.345, 54.321);
		astri.TYPES.Target target1 = new astri.TYPES.Target(2, pos, 100);
		astri.TYPES.Target target2 = new astri.TYPES.Target(4, pos, 300);
		astri.TYPES.Target[] targets = {target1, target2};
		int propId = comp.storeProposal(targets);
		assertTrue(propId>0);
	}
	
	public void testGetProposalStatus() throws Exception {
		int propStatus = comp.getProposalStatus(2);
		System.out.println("************* returned status = "+propStatus);
		assertTrue(propStatus==DataBaseOperations.STATUS_INITIAL_PROPOSAL);
	}
	
	public void testRemoveProposal() throws Exception {
		comp.removeProposal(99);
	}
	
	public void testGetProposalObservations() throws Exception {
		int[][] propObs;
		try {
			propObs = comp.getProposalObservations(5);
			assertTrue(propObs[0][0]==0);
		} catch (ProposalNotYetReadyEx e) {
			// TODO Auto-generated catch block
			assertTrue(true);
		}
		
	}
	
	public void testSetProposalStatus() throws Exception {
		comp.setProposalStatus(2, 1);
		comp.setProposalStatus(2, 2);
		try {
			comp.setProposalStatus(3, 2);
		} catch (Exception e) {
			// TODO Auto-generated catch
			return;
		}
		fail("Didn't throw an exception for invalid state change.");
	}
	
	public void testStoreImage() throws Exception {
		int [] img = {4, 5, 6, 7};
		comp.storeImage(99, 123, img);
		//assert(propStatus==0);
	}
	
	public void testGetProposals() throws Exception {
		astri.TYPES.Proposal[] proposals = comp.getProposals();
		int sizeProp = proposals.length;
		assertTrue(sizeProp>0);
	}
	
	protected void tearDown() throws Exception {
		this.getContainerServices().releaseComponent("DATABASE");
		super.tearDown();
	}

}
