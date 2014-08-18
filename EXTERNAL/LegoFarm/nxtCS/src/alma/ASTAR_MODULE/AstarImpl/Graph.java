package alma.ASTAR_MODULE.AstarImpl;
import java.io.*;


public class Graph {
	static int SIZE = 6;
	static int START = 2;
	static int FINISH = 3;
	static int ROAD = 8;

	static int NORTH = 1;
	static int EAST = 2;
	static int SOUTH = 3;
	static int WEST = 4;

	static short FORWARD = 1;
	static short TURNRIGHT = 2;
	static short TURNLEFT = 3;
	static short SPIN = 4;


	short graph[][] = new short[SIZE][SIZE];
	int start[] = new int[2];
	int finish[] = new int[2];


	public Graph(short graph[][])throws IOException {
		this.graph=graph;	
	}



	public int[] searchStart(){
		for(int i=0; i<SIZE; i++){
			for (int j=0; j<SIZE; j++){
				if (graph[i][j]==START){
					this.start[0]=i;
					this.start[1]=j;
				}
			}
		}
		return this.start;
	}

	public int[] searchFinish(){
		for(int i=0; i<SIZE; i++){
			for (int j=0; j<SIZE; j++){
				if (graph[i][j]==FINISH){
					this.finish[0]=i;
					this.finish[1]=j;
				}
			}
		}
		return this.finish;
	}

	public Boolean isTransitable(int coord[]){
		Boolean state = true;

		if(graph[coord[0]][coord[1]] == 0 ) state = false;

		return state;
	}



	public short[] getSolution(List closeList){

		int actualOrientation = NORTH;
		short action = 0;
		List orderedList = new List();
		short solution[] = new short[36];
		int coord[] = new int[2];
		int s=0;

		Node current = (Node)closeList.get(closeList.searchNode(this.finish));



		do{
			orderedList.add(current);
			coord=current.getfather();
			if (coord[0]==start[0] && coord[1]==start[1]) {
				int i = closeList.searchNode(coord);
				current=(Node)closeList.get(i);
				orderedList.add(current);
				break; 
			}
			int i = closeList.searchNode(coord);
			current=(Node)closeList.get(i);
		}while(true);


		//		System.out.println("ordered " );
		//		for(int g = orderedList.size()-1; g>=0; g--){
		//			Node nodo;
		//			nodo=(Node)orderedList.get(g);
		//			System.out.println("ordered "+nodo.coord[0]+nodo.coord[1] );
		//		}
		Node temp = (Node)orderedList.remove(orderedList.size()-1);
		int j = orderedList.size();
		for (int i=j-1; i>=0; i--){
			Node temp2 = (Node) orderedList.get(i);
			if (temp2.coord[1] > temp.coord[1]){ //to east
				if (actualOrientation == NORTH){
					solution[s]=TURNRIGHT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation == EAST){
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation == SOUTH){
					solution[s]=TURNLEFT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				actualOrientation = EAST;
			}
			if (temp2.coord[1] < temp.coord[1]){ //to west
				if (actualOrientation == SOUTH){
					solution[s]=TURNRIGHT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation ==WEST){
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation == NORTH){
					solution[s]=TURNLEFT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				actualOrientation = WEST;
			}
			if (temp2.coord[0] > temp.coord[0]){ //to south
				if (actualOrientation == SOUTH){
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation ==EAST){
					solution[s]=TURNRIGHT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation == WEST){
					solution[s]=TURNLEFT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation == NORTH){
					solution[s]=SPIN;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				actualOrientation = SOUTH;
			}
			if (temp2.coord[0] < temp.coord[0]){ //to north
				if (actualOrientation == NORTH){
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation ==EAST){
					solution[s]=TURNLEFT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				if (actualOrientation == WEST){
					solution[s]=TURNRIGHT;
					s++;
					solution[s]=FORWARD;
					s++;
				}
				actualOrientation = NORTH;
			}
			temp=temp2;
		}
		if (actualOrientation == SOUTH)
			solution[s]=SPIN;
		if (actualOrientation == EAST)
			solution[s]=TURNLEFT;
		if (actualOrientation == WEST)
			solution[s]=TURNRIGHT;

		return solution;
	}
}
