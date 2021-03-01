package uk.ac.ncl.undergraduate.modules.csc3621.GM;

import java.math.BigInteger;
/**
 * This class represents a public key in the GM scheme. It merely store the key.
 * The key should be generated externally.
 *
 * @author Changyu Dong
 */
public class PublicKey {
	
	/**
	 * Integer N = p*q where p and q are two big prime numbers
	 */
	private BigInteger N;
	
	/**
	 * Integer x satisfies the conditions x ^{(p-1)/2} =p-1 mod p and x ^{(q-1)/2} =q-1 mod q
	 */
	private BigInteger x;
	
	public PublicKey(BigInteger N,BigInteger x) {
		this.N = N;
		this.x = x;
	}

	public void setKey(BigInteger N,BigInteger x) {
		this.N = N;
		this.x = x;
	}
	
	public BigInteger getN() {
		return this.N;
	}
	
	
	public BigInteger getx() {
		return this.x;
	}
	
}
