package acsws.uos.database.wrappers;

import java.io.Serializable;

public class ImageTypeBlob implements Serializable {

/**
	 * 
	 */
	private static final long serialVersionUID = -1891148761495891967L;
	//	private ImageTypeHolder image;
	private byte[] image;
	private int tid;
	private int pid;
	
	public ImageTypeBlob() {
		
	}

	public byte[] getImage() {
		return image;
	}

	public void setImage(byte[] image) {
		this.image = image;
	}

	public int getTid() {
		return tid;
	}

	public void setTid(int tid) {
		this.tid = tid;
	}

	public int getPid() {
		return pid;
	}

	public void setPid(int pid) {
		this.pid = pid;
	}
}
