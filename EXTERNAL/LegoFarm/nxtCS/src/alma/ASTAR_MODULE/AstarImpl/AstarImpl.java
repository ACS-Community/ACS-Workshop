package alma.ASTAR_MODULE.AstarImpl;

import java.util.logging.Logger;
import alma.ACS.*;
import alma.TYPES.*;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import java.util.*;
import alma.ASTAR_MODULE.AstarOperations;
import java.io.*;

public class AstarImpl implements AstarOperations, ComponentLifecycle
{
	private ContainerServices m_containerServices;
	private Logger m_logger;

	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.info("initialize() called");
	}

	public void execute() {
		m_logger.info("execute() called");
	}
	public void cleanUp() {
		m_logger.info("cleanUp() called");
	}
	public void aboutToAbort() {
		cleanUp();
		m_logger.info("managed to abort...");
	}

	/////////////////////////////////////////////////////////////
	// Implementation of ACSComponent
	/////////////////////////////////////////////////////////////

	public ComponentStates componentState() {
		return m_containerServices.getComponentStateManager().getCurrentState();
	}
	public String name() {
		return m_containerServices.getName();
	}

	/////////////////////////////////////////////////////////////
	// Implementation of AstarOperations
	/////////////////////////////////////////////////////////////

	public alma.TYPES.Solution solveAlgorithm(alma.TYPES.Grid plan) {
		Solution sol = new Solution();

		List openList = new List();
		List closeList = new List();
		int found = -1;

		try {
			Graph graph = new Graph(plan.p);
			int finish[]= new int[2];
			int start[]= new int[2];
			short solution[] = new short[36];
			finish=graph.searchFinish();
			start=graph.searchStart();
			Node current = new Node(start);
			current.setH(finish);
			current.setGStart();
			current.setF();
			current.setfather(start);
			openList.add(current);
			do{
				for(int i=0;i<4;i++){
					int neighborCoord[]= current.neighborCoord(i);
					if((neighborCoord[0]!=-1)&&(closeList.searchNode(current.neighborCoord(i))== -1) && graph.isTransitable(current.neighborCoord(i))){	
						if(openList.searchNode(current.neighborCoord(i)) == -1){
							Node neighbor = new Node(neighborCoord);
							neighbor.setfather(current.getCoord());
							neighbor.setG(current.getG() +1);
							neighbor.setH(finish);
							neighbor.setF();
							openList.add(neighbor);						
						}else{
							int index = openList.searchNode(current.neighborCoord(i));						
							Node neighbor = (Node)openList.get(index);
							if(current.getG() +1 < neighbor.getG()){						
								neighbor.setG(current.getG() +1);
								neighbor.setF();
								neighbor.setfather(current.getCoord());
								openList.remove(index);
								openList.add(index, neighbor);							
							}
						}	
					}
				}
				closeList.add(current);
				openList.remove(openList.searchNode(current.getCoord()));
				if(!openList.isEmpty())
					current= (Node)openList.get(openList.searchMinF(openList));

				if (current.coord[0]==finish[0] && current.coord[1]==finish[1]){
					found=1;
					closeList.add(current);
				}

			}while(!openList.isEmpty() && found!=1);	
			solution = graph.getSolution(closeList);
			sol.s = solution;
		} catch(IOException ex){
		}
		return sol;
	}
}
