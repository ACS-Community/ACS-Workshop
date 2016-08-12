package acsws.uos.database.wrappers;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.Serializable;
import java.sql.Blob;
import java.sql.SQLException;

import javax.sql.rowset.serial.SerialBlob;

public class ImageTypeBlob implements Serializable {

/**
	 * 
	 */
	private static final long serialVersionUID = -1891148761495891967L;
	//	private ImageTypeHolder image;
	private byte[] image;
	private Blob imageBlob;
	private int tid;
	private int pid;
	
	public ImageTypeBlob() {
		
	}

	public Blob getImage() {
		return imageBlob;
	}
	
	public byte[] getImageAsBytes() {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
		InputStream is = imageBlob.getBinaryStream();
		byte buf[] = new byte[4000];
		int dataSize = 0;
		try {
		      while((dataSize = is.read(buf)) != -1) {
		        baos.write(buf, 0, dataSize);
		      }    
		    } finally {
		      if(is != null) {
		        is.close();
		      }
		    }
		    return baos.toByteArray();
		} catch (Exception ex) {
			
		}
		return image;
	}

	public void setImage(byte[] image) {
		try {
			imageBlob = new SerialBlob(image);
			this.image = image;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void setImage(Blob image) {
		imageBlob = image;
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
