package sort.database;

import java.util.List;

import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.Configuration;

import sort.database.pojos.Image;
import sort.database.pojos.Proposal;

public class HibernateDaoImpl {

	private Configuration currConfig = null;
	private SessionFactory sFactory = null;
	
	public static Configuration getDefaultConfiguration() {
		Configuration config = new Configuration();
		config.setProperty("hibernate.connection.url", "jdbc:hsqldb:file:hsqldb/sort;shutdown=true");
		config.setProperty("hibernate.connection.username", "sa");
		config.setProperty("hibernate.connection.password", "");
		config.setProperty("hibernate.connection.driver_class", "org.hsqldb.jdbcDriver");
		config.setProperty("hibernate.cache.provider_class", "org.hibernate.cache.NoCacheProvider");
		config.setProperty("hibernate.dialect", "org.hibernate.dialect.HSQLDialect");
		config.setProperty("hibernate.hbm2ddl.auto", "update");
		config.setProperty("hibernate.show_sql", "true");
		config.setProperty("hibernate.current_session_context_class", "thread");
		config.setProperty("hibernate.connection.pool_size", "3");
		//c3p0 stuff
		config.setProperty("c3p0.min_size", "3");
		config.setProperty("c3p0.max_size", "10");
		config.setProperty("c3p0.timeout", "10");
		//mapping
		config.addResource("sort/database/database.hbm.xml");
		return config;
	}
	
	public void setConfiguration(Configuration config) {
		currConfig = config;
	}
	
	public Session getSession() throws HibernateException {
		Session session = null;
		if (sFactory == null)
			sFactory = currConfig.buildSessionFactory();
		try {
			session = sFactory.getCurrentSession();
		} catch (HibernateException ex) {
			session = sFactory.openSession();
		}
		return session;
	}
	
	public void saveProposal(Proposal prop) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		try {
			session.save(prop);
			tx.commit();
		} catch (Exception ex) {
			tx.rollback();
		}
	}
	
	public Proposal getProposal(int pid) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		Query q = session.createQuery("select p from Proposal as p where p.pid = " + pid);
		Proposal retVal = (Proposal) q.uniqueResult();
		tx.commit();
		return retVal;
	}
	
	public Integer getMaxPid() {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		Query q = session.createQuery("select max (p.pid) from Proposal as p");
		Integer retVal = (Integer) q.uniqueResult();
		tx.commit();
		return retVal;
	}
	
	public Integer getProposalStatus(int pid) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		Query q = session.createQuery("select p.status from Proposal as p where p.pid = " + pid);
		Integer retVal = (Integer) q.uniqueResult();
		tx.commit();
		return retVal;
	}
	
	public void deleteProposal(int pid) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		Query q = session.createQuery("delete Proposal p where p.pid = " + pid);
		q.executeUpdate();
		tx.commit();
	}
	
	@SuppressWarnings("unchecked")
	public List<Proposal> getNonObservedProposals() {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		Query q = session.createQuery("select p from Proposal as p where p.status = 0");
		List<Proposal> retVal = q.list();
		tx.commit();
		return retVal;
	}
	
	public void updateProposal(Proposal p) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		session.update(p);
		tx.commit();
	}
	
	public void saveImage(Image image) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		session.save(image);
		tx.commit();
	}
	
	public boolean imageExists(int pid, int tid) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		Query q = session.createQuery("select i from Image as i where i.tid = " + tid + " and i.pid = " + pid);
		@SuppressWarnings("unchecked")
		List<Image> result =  q.list();
		tx.commit();
		if (result.size() == 1)
			return true;
		else
			return false;
	}
	
	public void clearDatabase() {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		session.createQuery("delete Target").executeUpdate();
		session.createQuery("delete Proposal").executeUpdate();
		session.createQuery("delete Image").executeUpdate();
		tx.commit();
	}
	
	@SuppressWarnings("unchecked")
	public List<Image> getImagesForProposal(int pid) {
		Session session = getSession();
		Transaction tx = session.beginTransaction();
		Query q = session.createQuery("select i from Image as i where i.pid = " + pid);
		List<Image> result =  q.list();
		tx.commit();
		return result;
	}
}
