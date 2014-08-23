package sort.database;

import java.io.Serializable;
import java.nio.ByteBuffer;
import java.sql.Blob;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.sql.rowset.serial.SerialBlob;

import org.hibernate.HibernateException;
import org.hibernate.usertype.UserType;

public class IntegerArrayBlobType implements UserType {

	@Override
	public Object assemble(Serializable arg0, Object arg1)
			throws HibernateException {
		return arg0;
	}

	@Override
	public Object deepCopy(Object arg0) throws HibernateException {
		return arg0;
	}

	@Override
	public Serializable disassemble(Object arg0) throws HibernateException {
		return (Serializable) arg0;
	}

	@Override
	public boolean equals(Object arg0, Object arg1) throws HibernateException {
		if (arg0 == null || arg1 == null)
			return false;
		return arg0.equals(arg1);
	}

	@Override
	public int hashCode(Object arg0) throws HibernateException {
		if (arg0 == null)
			return 0;
		return arg0.hashCode();
	}

	@Override
	public boolean isMutable() {
		return false;
	}

	@Override
	public Object nullSafeGet(ResultSet rs, String[] names, Object owner)
			throws HibernateException, SQLException {
		if (rs.wasNull())
			return null;
		Blob b = rs.getBlob(names[0]);
		byte[] value = b.getBytes(1, (int) b.length());
		int[] retVal = ByteBuffer.allocate(value.length).put(value).asIntBuffer().array();
		return retVal;
	}

	@Override
	public void nullSafeSet(PreparedStatement st, Object value, int idx)
			throws HibernateException, SQLException {
		if (value == null || ((int[]) value).length == 0) {
			st.setNull(idx, sqlTypes()[0]);
			return;
		}
		int[] blob = (int[]) value;
		ByteBuffer bf = ByteBuffer.allocate(blob.length * 4);
		for (int v: blob) 
			bf.putInt(v);
		st.setBlob(idx, new SerialBlob(bf.array()));

	}

	@Override
	public Object replace(Object arg0, Object arg1, Object arg2)
			throws HibernateException {
		return arg0;
	}

	@SuppressWarnings("rawtypes")
	@Override
	public Class returnedClass() {
		return int[].class;
	}

	@Override
	public int[] sqlTypes() {
		return new int[] {java.sql.Types.BLOB};
	}

}
