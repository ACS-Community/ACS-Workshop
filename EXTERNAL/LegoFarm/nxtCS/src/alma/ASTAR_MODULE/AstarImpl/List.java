package alma.ASTAR_MODULE.AstarImpl;
import java.util.*;

public class List extends ArrayList<Node>{

	private static final long serialVersionUID = 1L;

	public int searchNode(int coord[]){
		int r = -1;
		for(int i=0;i< this.size(); i++){
			Node current = (Node)this.get(i);
			if(coord[0]==current.coord[0] && coord[1]==current.coord[1]){
				r=i;
				break;
			}
		}
		return r;
	}

	public int searchMinF(List openList){
		int index = 0;
		Node temp = (Node) openList.get(0);
		int f = temp.getF();
		for (int i=1; i<openList.size();i++){
			temp = (Node)openList.get(i);
			if(temp.getF()<f){ 
				index = i;
			}
		}
		return index;
	}
}
