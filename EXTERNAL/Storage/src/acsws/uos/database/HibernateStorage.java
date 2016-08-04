package acsws.uos.database;

import java.util.ArrayList;
import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.Configuration;
import org.hibernate.classic.Session;
import org.hibernate.criterion.Projections;
import org.hibernate.criterion.Restrictions;

import acsws.STORAGE_MODULE.StorageOperations;
import acsws.TYPES.Proposal;
import acsws.uos.database.wrappers.ImageTypeBlob;
import acsws.uos.database.wrappers.Target;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;

public class HibernateStorage implements ComponentLifecycle, StorageOperations {

	private ContainerServices cs;
	
	private final Configuration cfg;
	private SessionFactory factory;
	
	public HibernateStorage () {
		cfg = new Configuration();
		cfg.addResource("acsws/uos/database/datamodel.hbm.xml");
		cfg.setProperty("hibernate.dialect", "org.hibernate.dialect.HSQLDialect");
	    cfg.setProperty("hibernate.connection.driver_class", "org.hsqldb.jdbcDriver");
	    cfg.setProperty("hibernate.connection.url", "jdbc:hsqldb:hsql://localhost/db");
	    cfg.setProperty("hibernate.connection.username", "sa");
	    cfg.setProperty("hibernate.connection.password", "");
	    cfg.setProperty("hibernate.connection.pool_size", "2");
	    cfg.setProperty("hibernate.hbm2ddl.auto", "update");
	    cfg.setProperty("hibernate.current_session_context_class", "thread");
	    cfg.setProperty("hibernate.show_sql", "true");
	    factory = cfg.buildSessionFactory();
	}
	
	@Override
	public String name() {
		return cs.getName();
	}

	@Override
	public ComponentStates componentState() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int getNextValidId() {
		Session s = factory.getCurrentSession();
		Integer retVal = null;
		Transaction tx = s.beginTransaction();
		Criteria crit = s.createCriteria(acsws.uos.database.wrappers.Proposal.class);
		crit.setProjection(Projections.max("pid"));
		try {
			retVal = (Integer) crit.uniqueResult();
		} finally {
			tx.rollback();
		}
		if (retVal == null)
			return 0;
		retVal+=1;
		return retVal;
	}

	@Override
	public void clearAllData() {
		Session s = factory.getCurrentSession();
		Transaction tx = s.beginTransaction();
		try {
			s.createSQLQuery("TRUNCATE TABLE IMAGE").executeUpdate();
			s.createSQLQuery("TRUNCATE TABLE TARGET").executeUpdate();
			s.createSQLQuery("TRUNCATE TABLE PROPOSAL").executeUpdate();
			tx.commit();
		} catch (Exception ex) {
			tx.rollback();
			throw ex;
		}
		
	}

	@Override
	public void initialize(ContainerServices containerServices) throws ComponentLifecycleException {
		cs = containerServices;
	}

	@Override
	public void execute() throws ComponentLifecycleException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void cleanUp() throws AcsJComponentCleanUpEx {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void aboutToAbort() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void storeObservation(Proposal prop, byte[][] images) {
		Session s = factory.getCurrentSession();
		acsws.uos.database.wrappers.Proposal tmp = new acsws.uos.database.wrappers.Proposal(prop);
		if (images.length != tmp.getTargets().size())
			cs.getLogger().warning("The number of images is different from the number of targets. Really?");
		Transaction tx = s.beginTransaction();
		try {
			s.save(tmp);
			int i = 0;
			List<Target> targets = tmp.getTargets();
			for(byte[] img: images) {
				Target t = targets.get(i++);
				s.save(t);
				ImageTypeBlob blob = new ImageTypeBlob();
				blob.setPid(t.getPid());
				blob.setTid(t.getTid());
				blob.setImage(img);
				s.save(blob);
			}
			tx.commit();
		} catch (Exception ex) {
			tx.rollback();
			throw ex;
		}
	}

	@Override
	public byte[][] getObservation(int pid) {
		Session s = factory.getCurrentSession();
		Transaction tx = s.beginTransaction();
		Criteria crit = s.createCriteria(ImageTypeBlob.class);
		crit.add(Restrictions.eq("pid", pid));
		List<ImageTypeBlob> result = null;
		try {
			result = crit.list();
		} finally {
			tx.rollback();
		}
		if (result == null)
			return new byte[0][0];
		ArrayList<byte[]> retVal = new ArrayList<>(result.size());
		for (ImageTypeBlob blob: result) {
			retVal.add(blob.getImage());
		}
		return retVal.toArray(new byte[0][0]);
	}

	
	
}
