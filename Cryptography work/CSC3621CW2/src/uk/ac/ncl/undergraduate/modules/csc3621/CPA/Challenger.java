package uk.ac.ncl.undergraduate.modules.csc3621.CPA;

import java.math.BigInteger;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;

/**
 * This class represents the challenger in the CPA experiment. You should not modify this class. 
 *
 * @author Changyu Dong
 */

public class Challenger {
	 /**
     * AES cipher to encrypt queries and the challenge.
     */
	private Cipher aes;
	 /**
     * AES key used for encryption.
     */
	private SecretKey  aesKey;
	 /**
     * The challenger can answer queries. This records how many queries are allowed. You are allowed at most 1000 queries during the experiment.
     */
	private int queryRemained =1000;
	 /**
     * Whether the challenger has generated the challenge. 
     */
	private boolean challenged;
	 /**
     * The counter used to generate IVs. It starts from 0 and increase by 1 after each encryption.
     */
	private BigInteger counter;
	 /**
     * The choice of which plaintexts submitted by the adversary is encrypted. either 0 or 1.
     */
	private int choice;
	/**
	 * The outcome of the experiment, i.e. whether the adversary wins or not in this run.
	 */
	private int outcome;
	/**
	 * Constructor. 
	 */
	public Challenger() {
		try {
			this.aes = Cipher.getInstance("AES/CBC/PKCS5Padding");
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try {
			KeyGenerator gen = KeyGenerator.getInstance("AES");
			gen.init(new SecureRandom());
			this.aesKey = gen.generateKey();
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		this.counter =BigInteger.ZERO;
		this.challenged=false;
		
		this.choice =-1;
		
		this.outcome=-1;
	}
	
	private String byteArrayToHexString(byte[] input) {
	    StringBuilder sb = new StringBuilder();
	    for (byte b : input) {
	        sb.append(String.format("%02X ", b));
	    }
	    return sb.toString();
	}
	
	
	/**
	 * Encrypt the input bytes, and increase the iv by 1 at the end.
	 * @param pBytes input bytes to be encrypted
	 * @return
	 */
	
	private String doEncryption(byte[] pBytes) {
		byte[] iv = new byte[16];
		
		byte[] counterNow = this.counter.toByteArray();
		
		System.arraycopy(counterNow, 0, iv, iv.length-counterNow.length, counterNow.length);
		
		IvParameterSpec ivspec = new IvParameterSpec(iv);
		
		try {
			this.aes.init(Cipher.ENCRYPT_MODE, this.aesKey, ivspec);
		} catch (InvalidKeyException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvalidAlgorithmParameterException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		byte[] cByte=null;
		try {
			cByte = this.aes.doFinal(pBytes);
		} catch (IllegalBlockSizeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		this.counter=this.counter.add(BigInteger.ONE);
		return this.byteArrayToHexString(cByte);
		
	}
	
	/**
	 * This allows the adversary to query with a chosen plaintext, and get the corresponding ciphertext. The adversary can query at most 1000 times.
	 * @param plaintext The plaintext to be encrypted. Must not be longer than 128-bit (e.g. 16 ASCII characters). It will be converted into
	 * a byte array before encryption (using the getBytes() method of String class).
	 * @return a pair of hexadecimal strings, the first encode the IV, the second the ciphertext.
	 * @throws IllegalArgumentException if the input plaintext is in a wrong format.
	 */
	
	public String[] query(String plaintext){
		if (this.queryRemained <=0) {
			System.out.println("You have already queried the oracle 1000 times. No more querying is allowed");
			return null;
		}
		
		byte [] pBytes = plaintext.getBytes();
		
		if (pBytes.length >16) {
			throw new IllegalArgumentException("The plaintext is too long."); 
		}
		
		String cText = this.doEncryption(pBytes);
		
		String IV = this.byteArrayToHexString(this.aes.getIV());
		
		this.queryRemained--;
		
		return new String[]{IV,cText};

	}
	
	/**
	 * This allows the adversary to submit two chosen plaintexts. The challenger will randomly encrypt one and return the ciphertext. 
	 * This function can only be called once in a experiment.
	 * @param m0 The first plaintext to be encrypted. Must not be longer than 128-bit (e.g. 16 ASCII characters). It will be converted into
	 * a byte array before encryption (using the getBytes() method of String class).
	 * @param m1 The second plaintext to be encrypted. Must not be longer than 128-bit (e.g. 16 ASCII characters). It will be converted into
	 * a byte array before encryption (using the getBytes() method of String class).
	 * @return a pair of hexadecimal strings, the first encode the IV, the second the ciphertext.
	 * @throws IllegalArgumentException if the input plaintext is in a wrong format.
	 */
	
	public String[] challenge(String m0, String m1){
		if(this.challenged==true) {
			System.out.println("You have already challenged the oracle");
			return null;
		}
		
		
		if(m0==null || m1==null|| m0.getBytes().length!=m1.getBytes().length) {
			throw new IllegalArgumentException("The plaintext is in wrong format");
		}
		
		SecureRandom rnd = new SecureRandom();
		
		this.choice=rnd.nextInt(2);
		
		String plaintext;
		
		if(choice == 0) {
			plaintext = m0;
		}else {
			plaintext=m1;
		}
		
		
		byte [] pBytes = plaintext.getBytes();
		
		if (pBytes == null ||pBytes.length >16) {
			throw new IllegalArgumentException("The plaintext is in wrong format"); 
		}
		
		String cText = this.doEncryption(pBytes);
		String IV = this.byteArrayToHexString(this.aes.getIV());
		this.challenged=true;
		
		
		
		return new String[]{IV,cText};

	}
	/**
	 * This allows the adversary to submit the guessed index of the plaintext that was chosen by the challenger in the challenge phase.
	 * The challenger then decide whether the adversary wins.
	 * @param index The adversary's guess
	 */
	public void guess(int index){
		if(index<0 ||index>1) {
			System.out.println("The index must be 0 or 1");
			return;
		}
		
		if(index== this.choice) {
			this.outcome=1;
		}else {
			this.outcome=0;
		}
		
	}
	
	/**
	 * 
	 * @return The outcome of the experiment. True if the adversary wins, false otherwise.
	 */
	public boolean result() {
		return this.outcome==1;
	}

}
