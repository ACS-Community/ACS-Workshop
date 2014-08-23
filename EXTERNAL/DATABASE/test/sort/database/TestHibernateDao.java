package sort.database;

import java.util.HashSet;

import org.hibernate.Hibernate;

import sort.database.pojos.Image;
import sort.database.pojos.Position;
import sort.database.pojos.Proposal;
import sort.database.pojos.Target;
import junit.framework.TestCase;

public class TestHibernateDao extends TestCase {
	
	private HibernateDaoImpl dao = null;
	
	public void testOrmMapping() {
		dao = new HibernateDaoImpl();
		dao.setConfiguration(HibernateDaoImpl.getDefaultConfiguration());
		
		for (int i = 0; i < 100 ; i++) {
			Proposal pp = new Proposal();
			pp.setStatus(0);
			pp.setTargets(new HashSet<Target>());
			for (int j = 0; j < 100; j++) {
				Target t = new Target();
				t.setTid(j);
				t.setExpTime((int)(Math.random() * 100));
				Position p = new Position();
				p.setAz(Math.random() * 360.0D - 180.0D);
				p.setEl(Math.random() * 180.0D - 90.0D);
				t.setCoordinates(p);
				pp.getTargets().add(t);
				Image ip = new Image();
				ip.setPid(i);
				ip.setTid(j);
				byte[] image = new byte[1000];
				for (int k = 0; k < 1000; k++) {
					image[k] = (byte) (Math.random() * 255);
					ip.setImage(image);
				}
				dao.saveImage(ip);
			}
			dao.saveProposal(pp);
		}
		dao.getSession().close();
		
		System.out.println(dao.getMaxPid());
		Proposal p = dao.getProposal(dao.getMaxPid());
		assertEquals(100, p.getTargets().size());
		
//		dao.clearDatabase();
//		dao.getSession().close();
		
//		assertEquals(null, dao.getMaxPid());
	}
}
