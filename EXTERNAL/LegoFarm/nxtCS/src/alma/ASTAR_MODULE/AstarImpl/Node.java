package alma.ASTAR_MODULE.AstarImpl;
import java.lang.Math;

public class Node {
	int coord[] = new int [2];
	int father[] = new int [2];
	int g,  h, f;

	public Node(int coord[]){
		this.coord = coord;
	}

	public int getG(){
		return this.g;
	}

	public int getH(){
		return this.h;
	}

	public int getF(){
		return this.f;
	}


	public int[] getCoord(){
		return coord;
	}

	public void setH(int finish[]){
		this.h = Math.abs(coord[0]- finish[0])+Math.abs(coord[1]-finish[1]);
	}

	public void setGStart(){
		this.g = 0;
	}

	public void setCoord(int coord[]){
		this.coord[0]=coord[0];
		this.coord[1]=coord[1];
	}

	public void setG(int g){
		this.g = g;
	}

	public void setF(){
		this.f = this.g + this.h;
	}

	public void setfather(int coord[]){
		this.father[0]=coord[0];
		this.father[1]=coord[1];
	}

	public int[] getfather(){
		return this.father;
	}
	public int[] neighborCoord(int i){
		int sonCoord[] = new int[2];
		sonCoord[0]=-1;
		switch(i){
			case 0:
				{
					if(this.coord[1]!=0){
						sonCoord[0]=this.coord[0];
						sonCoord[1]=this.coord[1]-1;
					}
					break;
				}
			case 1:
				{
					if(this.coord[1]!=5){
						sonCoord[0]=this.coord[0];
						sonCoord[1]=this.coord[1]+1;
					}
					break;
				}
			case 2:
				{
					if(this.coord[0]!=0){
						sonCoord[0]=this.coord[0]-1;
						sonCoord[1]=this.coord[1];
					}
					break;
				}
			case 3:
				{
					if(this.coord[0]!=5){
						sonCoord[0]=this.coord[0]+1;
						sonCoord[1]=this.coord[1];
					}
					break;
				}
		}
		return sonCoord;
	}
}
