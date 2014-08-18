package alma.impl.TRD;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.logging.Logger;

import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.acs.logging.AcsLogLevel;
import alma.ACS.ComponentStates;
import alma.MARS.TRDBaseOperations;

import alma.MARS.command_struct;
import alma.MARS.report_struct;
import alma.MARS.robot_type;
import alma.MARS.action_type;
import alma.MARS.command_type;

import java.sql.*;

public class TRD implements ComponentLifecycle, TRDBaseOperations {
	private ContainerServices m_containerServices;
	private Logger m_logger;
	private String DB_location;

	public TRD() {
		DB_location = System.getenv("INTROOT") +"/TRD.db";
	}

	@Override
	public void aboutToAbort() {

		m_logger.log(AcsLogLevel.TRACE, "aboutToAbort");
	}

	@Override
	public void cleanUp() {

		m_logger.log(AcsLogLevel.TRACE, "cleanUp");
	}

	@Override
	public void execute() throws ComponentLifecycleException {

		m_logger.log(AcsLogLevel.TRACE, "execute");
	}

	@Override
	public void initialize(ContainerServices containerServices)
	throws ComponentLifecycleException {

		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.log(AcsLogLevel.TRACE, "Initialize");

		//Initialize  SQLite
		try {
			Class.forName("org.sqlite.JDBC");
		} catch (ClassNotFoundException e) {
			m_logger.log(AcsLogLevel.SEVERE, "Failed to initialize SQLite");
			throw new ComponentLifecycleException("Failed to initialize SQLite",e);
		}


		try {
			Connection conn = getTRDConnection();
			Statement stat = conn.createStatement();
			//Checking Report Table
			stat.executeUpdate("CREATE TABLE IF NOT EXISTS reportTable (id INTEGER PRIMARY KEY AUTOINCREMENT, isSuccessful BOOLEAN, rabType TEXT, errorMsg TEXT);");
			conn.close();

			//Checking Command Table
			if(checkCommandDBClean()){
				resetCommandDB();
			}			
		} catch (SQLException e) {
			m_logger.log(AcsLogLevel.SEVERE, "There where problems initializing the Database");
			throw new ComponentLifecycleException("There where problems initializing the Database", e);
		}
	}

	private Connection getTRDConnection() throws SQLException {		
		try {
			return DriverManager.getConnection("jdbc:sqlite:" + DB_location);
		} catch (SQLException e) {
			m_logger.log(AcsLogLevel.ERROR, "Error while Connecting to the DataBase file");
			throw e;
		}
	}

	private boolean checkCommandDBClean() throws SQLException {
		m_logger.log(AcsLogLevel.TRACE, "Checking if Tables are already Populated");
		boolean out = false;

		try {
			Connection conn = getTRDConnection();
			Statement stat = conn.createStatement();

			ResultSet rs = stat.executeQuery("SELECT count(*) FROM sqlite_master WHERE name='commandStructTable';");

			rs.next();
			if(rs.getInt(1) == 0){
				out = true;
			}

			conn.close();
			return out;

		} catch (SQLException e) {
			m_logger.log(AcsLogLevel.ERROR, "Error while checking commandStructTable");
			throw e;
		}
	}

	private void resetCommandDB() throws SQLException {

		m_logger.log(AcsLogLevel.TRACE, "Populating initial Tables");

		Map<Integer,int[][]> RAB = new HashMap<Integer, int[][]>();

		RAB.put(
				action_type._fire,
				new int[][] {
						{0,robot_type._welding,command_type._firefight},
						{1,robot_type._debris,command_type._primary}
				});

		RAB.put(
				action_type._piping,
				new int[][] {
						{0,robot_type._transporter,command_type._primary},
						{1,robot_type._assembly,command_type._primary},
						{2,robot_type._debris,command_type._primary}
				});

		RAB.put(
				action_type._vacuum,
				new int[][] {
						{0,robot_type._transporter,command_type._primary},
						{1,robot_type._welding,command_type._primary},
						{2,robot_type._debris,command_type._primary}
				});

		try {
			Connection conn = getTRDConnection();
			Statement stat = conn.createStatement();
			stat.executeUpdate("DROP TABLE IF EXISTS commandStructTable");
			stat.executeUpdate("CREATE TABLE IF NOT EXISTS commandStructTable (actionTypeID INTEGER, sequenceOrder INTEGER, robotID INTEGER, actionID INTEGER);");

			PreparedStatement prep = conn.prepareStatement("INSERT INTO commandStructTable (actionTypeID, sequenceOrder, robotID, actionID) VALUES (?, ?, ?, ?);");

			Iterator< Map.Entry<Integer,int[][]> > it = RAB.entrySet().iterator();

			while(it.hasNext())
			{
				Map.Entry<Integer,int[][]> m = it.next();
				int key = m.getKey();
				int[][] value = m.getValue();

				prep.setInt(1, key);
				for(int i=0; i < value.length ; i++ )
				{
					prep.setInt(2, value[i][0]);
					prep.setInt(3, value[i][1]);
					prep.setInt(4, value[i][2]);
					prep.addBatch();						
				}
			}

			prep.executeBatch();

			conn.close();

		} catch (SQLException e) {
			m_logger.log(AcsLogLevel.ERROR, "Error while creating commandStructTable");
			throw e;
		}
	}

	@Override
	public int[] getReportsList(){
		m_logger.log(AcsLogLevel.TRACE, "Getting Report List");


		int[] list = null;


		try {
			Connection conn = getTRDConnection();

			Statement stat;
			stat = conn.createStatement();

			ResultSet rs = stat.executeQuery("SELECT count(*) FROM reportTable;");
			rs.next();
			int size = rs.getInt(1);
			list = new int[size];

			if(size == 0){
				m_logger.log(AcsLogLevel.INFO, "There are no Reports in the database");
			}

			rs = stat.executeQuery("SELECT id FROM reportTable;");

			for(int i=0; i<size; i++)
			{
				rs.next();
				list[i] = rs.getInt("id");
			}

			conn.close();
		} catch (SQLException e) {
			m_logger.log(AcsLogLevel.ERROR, "There was an SQLite problem :" + e.getMessage());
			e.printStackTrace();
		}

		return list;

	}

	@Override
	public int createReport(report_struct arg0) {
		int ret_id = -1;

		m_logger.log(AcsLogLevel.TRACE, "Saving Report into the DataBase");

		try {
			Connection conn = getTRDConnection();

			PreparedStatement prep = conn.prepareStatement("INSERT INTO reportTable (isSuccessful, rabType, errorMsg) VALUES (?, ?, ?);");

			prep.setBoolean(1, arg0.isSuccessful);
			prep.setString(2, arg0.rabType);
			prep.setString(3, arg0.errorMsg);
			prep.addBatch();

			prep.executeBatch();

			conn.close();
			
			conn = getTRDConnection();
			
			Statement stat = conn.createStatement();
			ResultSet rs = stat.executeQuery("SELECT id FROM reportTable ORDER BY id DESC LIMIT 1;");

			while (rs.next()) {
				ret_id = rs.getInt("id");
			}

			conn.close();

		} catch (SQLException e) {
			m_logger.log(AcsLogLevel.ERROR, "There was an SQLite problem :" + e.getMessage());
			e.printStackTrace();
		}
		return ret_id;
	}

	@Override
	public command_struct[] getRAB(int arg0) {
		m_logger.log(AcsLogLevel.TRACE, "Getting RAB");

		command_struct[] cm = null;

		try {
			Connection conn = getTRDConnection();

			Statement stat;
			stat = conn.createStatement();
			ResultSet rs = stat.executeQuery("SELECT count(*) FROM commandStructTable WHERE actionTypeID = "+ arg0 + ";");

			rs.next();
			cm = new command_struct[rs.getInt(1)];
			if(cm.length == 0)
			{
				conn.close();
				throw new Exception("That command is not defined in the DB");
			}

			rs = stat.executeQuery("SELECT * FROM commandStructTable WHERE actionTypeID = "+ arg0 + " ORDER BY sequenceOrder ;");
			int i = 0;
			while (rs.next()) {
				cm[i] = new command_struct();
				cm[i].robotId = rs.getInt("robotID");
				cm[i].actionId = rs.getInt("actionID");
				i++;
			}

			conn.close();
		} 
		catch (SQLException e) {
			m_logger.log(AcsLogLevel.ERROR, "There was an SQLite problem :" + e.getMessage());
			e.printStackTrace();
		} 
		catch (Exception e) {
			m_logger.log(AcsLogLevel.ERROR, e.getMessage() );
			e.printStackTrace();
		}
		return cm;
	}

	@Override
	public report_struct getReport(int arg0) {
		m_logger.log(AcsLogLevel.TRACE, "Getting Report");

		report_struct repstr = null;

		try {

			Connection conn = getTRDConnection();

			Statement stat = conn.createStatement();
			ResultSet rs = stat.executeQuery("SELECT * FROM reportTable WHERE id = "
					+ Integer.toString(arg0) + " ;");

			repstr = new report_struct();

			if(rs.next()){
				repstr.id = rs.getInt("id");
				repstr.errorMsg = rs.getString("errorMsg");
				repstr.rabType = rs.getString("rabType");
				repstr.isSuccessful = rs.getBoolean("isSuccessful");
			}
			else
			{
				repstr.id = -1;
				repstr.errorMsg = "There is no report with that ID";
				repstr.rabType = "";
				repstr.isSuccessful = false;
				conn.close();
				throw new Exception("There is no report with that ID");
			}
			conn.close();
		}
		catch (SQLException e) {
			m_logger.log(AcsLogLevel.ERROR, "There was an SQLite problem :" + e.getMessage());
			e.printStackTrace();
		}
		catch (Exception e)
		{
			m_logger.log(AcsLogLevel.ERROR, e.getMessage());
			e.printStackTrace();
		}
		return repstr;
	}

	@Override
	public ComponentStates componentState() {
		m_logger.log(AcsLogLevel.TRACE, "Getting Component State");
		
		return m_containerServices.getComponentStateManager().getCurrentState();
	}

	@Override
	public String name() {
		m_logger.log(AcsLogLevel.TRACE, "Getting Name");
		
		return "TRD";
	}


}
