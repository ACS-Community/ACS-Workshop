package astri.sort.database;


import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.logging.Level;

import alma.acs.logging.AcsLogger;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.component.ComponentLifecycleException;
import alma.acs.container.ContainerServices;
import alma.maciErrType.wrappers.AcsJComponentCleanUpEx;
import astri.DATABASE_MODULE.DataBaseOperations;
import astri.SORTErr.ImageAlreadyStoredEx;
import astri.SORTErr.InvalidProposalStatusTransitionEx;
import astri.SORTErr.ProposalNotYetReadyEx;
import astri.SORTErr.wrappers.AcsJInvalidProposalStatusTransitionEx;
import astri.SORTErr.wrappers.AcsJProposalNotYetReadyEx;
import astri.TYPES.Position;
import astri.TYPES.Proposal;
import astri.TYPES.Target;

public class DataBaseImpl implements ComponentLifecycle, DataBaseOperations {
	
	private ContainerServices m_containerServices;
	private AcsLogger m_logger;
	private String db_dirname = System.getenv("HOME")
			+"/prop-db/";
	private String propfile = "proptab.db";
	private String targfile = "targtab.db";
	private String imgdir = "imgdir";
	
	@Override
	public String name() {
		// TODO Auto-generated method stub
		return m_containerServices.getName();
	}

	@Override
	public ComponentStates componentState() {
		// TODO Auto-generated method stub
		return m_containerServices.getComponentStateManager().getCurrentState();
	}

	@Override
	public int storeProposal(Target[] targets) {
		// TODO Auto-generated method stub
		//read  proposal file and count rows
		//assign pid = nrows+1
		//assign status = 0
		//open target file
		//append target assigning "" to imgname
		
		ArrayList<String> proposal = readFromFile(db_dirname + propfile);
		
		int pid, maxpid = 0;
		
		for (Iterator<String> iterator = proposal.iterator(); iterator.hasNext();) {
			String line = (String) iterator.next();
			pid = Integer.parseInt(line.substring(0, line.indexOf(",")));
			if (pid > maxpid)
				maxpid = pid;
		}
		
		pid = maxpid + 1;
		
		writeToFile(db_dirname + propfile, Integer.toString(pid) + ",0");
		
		String row, pos;
		for (int i=0; i<targets.length; i++) {
			pos = Double.toString(targets[i].coordinates.az) + "," + Double.toString(targets[i].coordinates.el);
			row = Integer.toString(targets[i].tid) + "," + pos + "," + Integer.toString(targets[i].expTime) + ",NOIMAGE";
			writeToFile(db_dirname + targfile, Integer.toString(pid) + "," + row);
		}
		return pid;
	}

	@Override
	public int getProposalStatus(int pid) {
		// TODO Auto-generated method stub
		// open proposal file
		// find the requested pid and read status = second string
		// return status
		ArrayList<String> proposal = readFromFile(db_dirname + propfile);
		int status = DataBaseOperations.STATUS_NO_SUCH_PROPOSAL;
		for (Iterator<String> iterator = proposal.iterator(); iterator.hasNext();) {
			String line = (String) iterator.next();
			int p = Integer.parseInt(line.substring(0, line.indexOf(",")));
			if (p == pid) {
				status = Integer.parseInt(line.substring(line.indexOf(",")+1));
				//System.out.println("************ status in if stmt = " + status);
			}
		}
		//System.out.println("************ status before return = " + status);
		return status;
	}

	@Override
	public void removeProposal(int pid) {
		// TODO Auto-generated method stub
		// open proposal file
		// find the requested pid and erase the corresponding line
		//open target file
		// find the requested pid and erase the corresponding lines
	}

	@Override
	public Proposal[] getProposals() {
		// TODO Auto-generated method stub
		ArrayList<Proposal> proposals = new ArrayList<Proposal>();
		ArrayList<String> props = readFromFile(db_dirname + propfile);

		for (Iterator<String> iterator = props.iterator(); iterator.hasNext();) {
			String line = (String) iterator.next();

			String status = line.substring(line.indexOf(",")+1);

			if (status.equals("0")) {
				int pid = Integer.parseInt(line.substring(0, line.indexOf(",")));
				Proposal prop = constructProposal(pid);
				proposals.add(prop);
			}
		}
		int dim = proposals.size();
		m_logger.info("Number of proposals to return: "+dim);
		Proposal[] propArray = proposals.toArray(new Proposal[dim]);
		m_logger.info("Size of propArray to return: "+propArray.length);
		return propArray;

		// open proposal file
		// for each pid search targets in the target file and create a structure of proposals with status = 0 
	}
		
		
	private Proposal constructProposal(int pid) {
		// TODO Auto-generated method stub
		ArrayList<String> targs = readFromFile(db_dirname + targfile);
		Target targets[] = new Target[10];
		int count = 0;
		for (Iterator<String> iterator = targs.iterator(); iterator.hasNext();) {
			String line = (String) iterator.next();
			int pp = Integer.parseInt(line.substring(0, line.indexOf(",")));
			if (pp == pid) {
				String[] tokens = line.split(",");
				String ppid = tokens[0];
				String tid = tokens[1];
				String az = tokens[2];
				String el = tokens[3];
				String expTime = tokens[4];
				m_logger.log(Level.FINE,"pid,tid,az,el,expTime: "+pid+" "+tid
						+" "+az+" "+el+" "+expTime);
				int ttid = Integer.parseInt(tid);
				double aaz = Double.parseDouble(az);
				double eel = Double.parseDouble(el);
				int exp = Integer.parseInt(expTime);
				targets[count] = new Target(ttid, new Position(aaz,eel), exp);
				count++;
			}
		}
		m_logger.info("Number of targets: "+count);
		Target[] tgts = new Target[count];
		for (int i = 0; i < tgts.length; i++) {
			tgts[i] = targets[i];
		}
		
		Proposal prop = new Proposal(pid, tgts, 0);
		return prop;
	}

	@Override
	public void clean() {
		// TODO Auto-generated method stub
		// open file proposal and empty it
	}

	@Override
	public int[][] getProposalObservations(int pid)
			throws ProposalNotYetReadyEx {
		int get = getProposalStatus(pid);
		if (get != 2) {
			AcsJProposalNotYetReadyEx ex = new AcsJProposalNotYetReadyEx();
			throw ex.toProposalNotYetReadyEx();
		}
		int DIM1 = 4;
		int DIM2 = 200;
		int [][] i = new int[4][200];
		for (int j = 0; j < DIM1; j++) {
			for (int j2 = 0; j2 < DIM2; j2++) {
				i[j][j2] = j*j2;
			}
			
		}
		// TODO Auto-generated method stub
		//open proposal file
		//controls that status of pid is 2
		//if so, open target file and for each tid of the input pid retrieve the image name
		//open the file corresponding to the image name
		//load image data in array
		//return array
		return i;
	}

	@Override
	public void setProposalStatus(int pid, int status)
			throws InvalidProposalStatusTransitionEx {
		// TODO Auto-generated method stub
		String tempfilename = db_dirname + "tempproposals.dmp";
		
		ArrayList<String> proposal = readFromFile(db_dirname + propfile);
		File fileTemp = new File(tempfilename);
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(fileTemp, true));
			for (Iterator<String> iterator = proposal.iterator(); iterator.hasNext();) {
				String line = (String) iterator.next();
				int p = Integer.parseInt(line.substring(0, line.indexOf(",")));
				if (p == pid) {
					int sts = Integer.parseInt(line.substring(line.indexOf(",")+1));
					if((sts == 0 && status == 1) || (sts == 1 && status == 2))
						line = pid+","+status;
					else {
						bw.close();
						if (!fileTemp.delete())
							m_logger.log(Level.WARNING,"Can't delete temporary file.");
						AcsJInvalidProposalStatusTransitionEx ex = new AcsJInvalidProposalStatusTransitionEx();
					throw ex.toInvalidProposalStatusTransitionEx();					
					}
					//System.out.println("************ status in if stmt = " + status);
				}
				bw.write(line);
				bw.newLine();
			}
			bw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			m_logger.severe("Problem writing to temporary file.");
			e.printStackTrace();
		}
		fileTemp.renameTo(new File(db_dirname + propfile));
		
		//System.out.println("************ status before return = " + status);

		
		//open proposal file
		//retrieve status of pid
		//verify that old pid -> new pid is 0-> or 1-> 2 otherwise raise Ex
		//set new status and update file
		//close proposal file
	}

	@Override
	public void storeImage(int pid, int tid, int[] image)
			throws ImageAlreadyStoredEx {
		// TODO Auto-generated method stub
		// open target file
		// search for pid/tid combination
		// verify that image name is "" otherwise raise Ex
		// create image_file 
		// write image into image_file
		// update image_file entry in the target file
		// close target file
	}

	@Override
	public void initialize(ContainerServices containerServices)
			throws ComponentLifecycleException {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		
		// TODO Auto-generated method stub
		//Verify if directory exists
		//verify if files exist - if not creates them
		if (!(new File(db_dirname)).exists()) {
			boolean success = new File(db_dirname).mkdir();
			  if (success) {
				  m_logger.log(Level.INFO, "Directory: " 
			   + db_dirname + " created.");
			  }
			  else {
				  m_logger.log(Level.SEVERE, "Directory: " 
						   + db_dirname + "not created, error.");
				  ComponentLifecycleException ex = new ComponentLifecycleException();
				  throw ex;
			  }
		}
		else
			m_logger.log(Level.INFO, "Directory: " 
					   + db_dirname + " exists, skipping.");
		
		if (!(new File(db_dirname + propfile)).exists()) {
			File pf = new File(db_dirname +  propfile);
			try {
				pf.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				ComponentLifecycleException ex = new ComponentLifecycleException(e);
				  throw ex;
			}

		}
		
		if (!(new File(db_dirname + targfile)).exists()) {
			File tf = new File(db_dirname + targfile);
			try {
				tf.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				ComponentLifecycleException ex = new ComponentLifecycleException(e);
				  throw ex;
			}
		}
		
		if (!(new File(db_dirname + imgdir)).exists()) {
			boolean success = new File(db_dirname + imgdir).mkdir();
			  if (success) {
				  m_logger.log(Level.INFO, "Directory: " 
			   + db_dirname + imgdir + " created.");
			  }
			  else {
				  m_logger.log(Level.SEVERE, "Directory: " 
						   + db_dirname + imgdir + "not created, error.");
				  try {
					cleanUp();
				} catch (AcsJComponentCleanUpEx e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			  }
		}
		else
			m_logger.log(Level.INFO, "Directory: " 
					   + db_dirname + imgdir + " exists, skipping.");
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
	
	private void writeToFile(String fname, String text) {
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter(new File(
                                fname), true));
            bw.write(text);
            bw.newLine();
            bw.close();
        } catch (Exception e) {
        }
	}

	private ArrayList<String> readFromFile(String fname) {
		ArrayList<String> props = new ArrayList<String>();
		
		try {
			// Open the file that is the first 
			// command line parameter
			FileInputStream fstream = new FileInputStream(fname);
			// Get the object of DataInputStream
			DataInputStream in = new DataInputStream(fstream);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			String strLine;
			
			int i = 0;
			//Read File Line By Line
			while ((strLine = br.readLine()) != null) {
			    // Print the content on the console
			    props.add(strLine);
			    i++;
			}
			//Close the input stream
			in.close();
		}catch (Exception e){//Catch exception if any
			m_logger.log(Level.SEVERE, "Cannot read " + fname + ".");
			return null;
		}
		
		return props;
	}

}
