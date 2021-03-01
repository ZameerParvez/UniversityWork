package uk.ac.ncl.undergraduate.modules.csc3621.GM;

import java.math.BigInteger;

/**
 * This class represents a private key in the GM scheme. It merely store the key.
 * The key should be generated externally.
 *
 * @author Changyu Dong
 */

public class PrivateKey {
	
	/**
	 * Integer p, a big prime number
	 */
	private BigInteger p;
	/**
	 * Integer q, another big prime number
	 */
	private BigInteger q;

	/**
	 * Create a private key given two prime big integers
	 * @param p A prime big integer 
	 * @param q A prime big integer 
	 */
	
	public PrivateKey(BigInteger p, BigInteger q) {
		this.p=p;
		this.q=q;

	}
	

	
	public void setKey(BigInteger p, BigInteger q) {
		this.p=p;
		this.q=q;

	}
	

	
	public BigInteger getp() {
		return this.p;
	}


	public BigInteger getq() {
		return this.q;
	}

}
