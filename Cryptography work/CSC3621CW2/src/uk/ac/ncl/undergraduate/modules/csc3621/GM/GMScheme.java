package uk.ac.ncl.undergraduate.modules.csc3621.GM;

import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.Map;
import java.util.HashMap;

/**
 * This class is for algorithms in the Goldwasser-Micali Encryption scheme.
 *
 * @author Changyu Dong
 * @author Zameer Parvez
 */

public class GMScheme {

	static SecureRandom rnd = new SecureRandom();

	/**
	 * The key generation algorithm.
	 * 
	 * @param n determines the bit length of prime numbers p and q, i.e |p| = |q| =
	 *          n.
	 * @return a valid key pair in GM scheme.
	 */
	public static KeyPair Gen(int n) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">
		
		// probablePrime finds a prime number which is n bits long, using a random source rnd
		BigInteger p = BigInteger.probablePrime(n, rnd);
		BigInteger q = BigInteger.probablePrime(n, rnd);

		BigInteger N = p.multiply(q);
		BigInteger x = BigInteger.ZERO;
		
		// This loop is required because x is not guaranteed to satisfy the 2 halftests, if it is randomly sampled from Zn*
		boolean isValidX = false;
		while(!isValidX) {
			x = randomZnStar(N);
			isValidX = HalfTest(x, p) && HalfTest(x, q);
		}

		PublicKey pk = new PublicKey(N, x);
		PrivateKey sk = new PrivateKey(p, q);
		KeyPair keyPair= new KeyPair(pk, sk);

		return keyPair;
		
		//</editor-fold> // END OF YOUR CODE
	}

	// This helper method performs the check x^((y - 1) / 2) === y - 1 mod y,
	// This is used in Gen and Dec
	private static boolean HalfTest(BigInteger x, BigInteger y) {
		// x^((y - 1) / 2) === y - 1 mod y
		BigInteger lhs = x.modPow(y.subtract(BigInteger.ONE).divide(BigInteger.TWO), y);
		BigInteger rhs = y.subtract(BigInteger.ONE).mod(y);

		if (lhs.compareTo(rhs) == 0) return true;

		return false;
	}
	
	/**
	 * Sample a number uniformly at random from the group Z_N^*
	 * 
	 * @param N the group modulus
	 * @return a random number from the group Z_N^*
	 */

	// This method does not correctly choose from Zn*
	public static BigInteger randomZnStar(BigInteger N) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">


//		// This commented out method may not actually be choosing uniformly randomly because of the reduced precision of doubles
//		// for example if N was large and the smallest doubles can differ by is 0.01, then samples will only be from 0%,1%-99% of N
//		// Get random number from 0 to 1 and multiply by N
//		BigDecimal random = new BigDecimal(rnd.nextDouble());
//		// If we need a random number to be between 0 and N (inclusively) then we can set num = N + 1
//		BigDecimal num = new BigDecimal(N);
//		BigInteger sample = num.multiply(random).toBigInteger();

		// This method ensures randomness across all of the bits in a BigInteger, but it can also create integers bigger than N
		int bitLengthOfN = N.bitLength();
		BigInteger candidateInteger = BigInteger.ZERO;

		// This loop ensures that the number generated is in Zn*, by looping if it is not smaller than N
		boolean sampleIsValid = false;
		while (!sampleIsValid) {
			candidateInteger = new BigInteger(bitLengthOfN, rnd);

			sampleIsValid = candidateInteger.compareTo(N) < 0;
		}

		return candidateInteger;

        //</editor-fold> // END OF YOUR CODE
	}


	/**
	 * The encryption algorithm
	 * 
	 * @param pk the public key
	 * @param m  the plaintext to be encrypted, must be either 1 or 0
	 * @return the ciphertext of m
	 */

	public static BigInteger Enc(PublicKey pk, BigInteger m) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">

		BigInteger x = pk.getx();
		BigInteger N = pk.getN();
		BigInteger r = randomZnStar(N);

		// Encrypts by computing c = x^m * r^2 mod N
		BigInteger c = x.modPow(m, N).multiply(r.modPow(BigInteger.TWO, N)).mod(N);

		return c;

        //</editor-fold> // END OF YOUR CODE
	}

	/**
	 * The decryption algorithm
	 * 
	 * @param sk the private key
	 * @param c  the ciphertext to be decrypted
	 * @return the plaintext decrypted from c
	 */
	public static BigInteger Dec(PrivateKey sk, BigInteger c) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">

		BigInteger p = sk.getp();
		BigInteger q = sk.getq();

		// Decrypts by testing if c^((p - 1) / 2) === p - 1 mod p && c^((q - 1) / 2) === q - 1 mod q
		boolean check = HalfTest(c, p) && HalfTest(c, q);
		int result = check ? 1 : 0;

		return BigInteger.valueOf(result);

        //</editor-fold> // END OF YOUR CODE
	}

	/**
	 * The homomorphic addition algorithm
	 * 
	 * @param pk the public key
	 * @param c1 the first ciphertext 
	 * @param c2 the second ciphertext
	 * @return the ciphertext contains the XOR result
	 */
	public static BigInteger XOR(PublicKey pk, BigInteger c1, BigInteger c2) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">


		// Getting c1 and c2 in the form c1 mod N and c2 mod N, could improve performance if c > N
		// However this will not be true because c is an element of Zn*, therefore it is not necessary
//		BigInteger reducedC1 = c1.mod(pk.getN());
//		BigInteger reducedC2 = c2.mod(pk.getN());

		// This mod method uses an efficient algorithm to get (c1 * c2) mod N
		BigInteger c3 = c1.multiply(c2).mod(pk.getN());

		return c3;

		//</editor-fold> // END OF YOUR CODE
	}

	/**
	 * The homomorphic negation algorithm
	 * 
	 * @param pk the public key
	 * @param c the ciphertext to be negated
	 * @return the ciphertext contains the negation result
	 */

	public static BigInteger Negate(PublicKey pk, BigInteger c) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">

		// To negate this computes c3 = Enc(pk, 1) * c mod n
		BigInteger N = pk.getN();
		BigInteger c2 = Enc(pk, BigInteger.valueOf(1));
		BigInteger c3 = c2.multiply(c).mod(N);

		return c3;

        //</editor-fold> // END OF YOUR CODE
	}

	public static void main(String[] args) {
//		BigInteger n = BigInteger.valueOf(10);
//
//		for (int i = 0; i < 100; i++) {
//			System.out.println(i + ":\t" + randomZnStar(n));
//		}
//
//		BigInteger val = BigInteger.valueOf(5);
//		BigInteger a = BigInteger.valueOf(23);
//		BigInteger b = BigInteger.valueOf(13);
//		BigInteger xored = XOR(new PublicKey(val, val), a, b);
//
//		System.out.println(a + " * " + b + " (mod " + val + ") = " + xored);
//
//		int n1 = 15;
//		KeyPair kp = Gen(n1);
//		PublicKey pk = kp.getPublicKey();
//		PrivateKey sk = kp.getPrivateKey();
//
//		BigInteger m = BigInteger.ONE;
//		BigInteger c = Enc(pk, m);
//		BigInteger plaintext = Dec(sk, c);
//
//		System.out.println("CipherText: " + c + " PlainText: " + plaintext);

		// These tests take about 3 minute to run
		int numberOfTries = 1000;
		int bitLength = 512;
		testCorrectness(numberOfTries,bitLength);
		testHomomorphicXOR(numberOfTries,bitLength);
		testHomomorphicNegation(numberOfTries,bitLength);
		testRandomness(100000, BigInteger.valueOf(100));
	}

	// The methods below are used to test that Correctness, HomomorphicXor and Homormophic Negation hold for this encryption scheme

	private static BigInteger getRandomMessage() {
		return rnd.nextBoolean() ? BigInteger.ZERO : BigInteger.ONE;
	}

	// This should check if the distribution of choosing something from Zn* are about equal
	// Currently the frequencies of each value being sampled is just printed out, so this is not an automated test
	public static void testRandomness(int numberOfTries, BigInteger modulus) {
		Map<BigInteger, Integer> sampleFrequencies = new HashMap<>();

		for(int i = 0; i < numberOfTries; i++) {
			BigInteger sample = randomZnStar(modulus);

			if (sampleFrequencies.containsKey(sample)) {
				sampleFrequencies.put(sample, sampleFrequencies.get(sample) + 1);
			} else {
				sampleFrequencies.put(sample, 1);
			}
		}

		System.out.println(sampleFrequencies.toString());
	}

	public static void testCorrectness(int numberOfTries, int bitLength) {
		boolean isCorrect = true;
		int numCompleted = 0;

		while(numCompleted < numberOfTries && isCorrect) {
			isCorrect = CorrectnessTest(bitLength, getRandomMessage());
			numCompleted++;
		}

		System.out.println("Correctness: " + isCorrect);
	}

	public static void testHomomorphicXOR(int numberOfTries, int bitLength) {
		boolean isCorrect = true;
		int numCompleted = 0;

		while(numCompleted < numberOfTries && isCorrect) {
			isCorrect = HomomorphicXORTest(bitLength, getRandomMessage(), getRandomMessage());
			numCompleted++;
		}

		System.out.println("HomomorphicXOR: " + isCorrect);
	}

	public static void testHomomorphicNegation(int numberOfTries, int bitLength) {
		boolean isCorrect = true;
		int numCompleted = 0;

		while(numCompleted < numberOfTries && isCorrect) {
			isCorrect = HomomorphicNegationTest(bitLength, getRandomMessage());
			numCompleted++;
		}

		System.out.println("HomomorphicNegation: " + isCorrect);
	}

	// n is the security parameter for the bitlength
	public static boolean CorrectnessTest(int n, BigInteger m) {
		KeyPair kP = Gen(n);
		PublicKey pk = kP.getPublicKey();
		PrivateKey sk = kP.getPrivateKey();

		BigInteger c = Enc(pk, m);

		BigInteger lhs = Dec(sk, c);
		BigInteger rhs = m;

		if (lhs.compareTo(rhs) == 0) return true;

		return false;
	}

	public static boolean HomomorphicXORTest(int n, BigInteger m1, BigInteger m2) {
		KeyPair kP = Gen(n);
		PublicKey pk = kP.getPublicKey();
		PrivateKey sk = kP.getPrivateKey();
		BigInteger N = pk.getN();

		BigInteger c1 = Enc(pk, m1);
		BigInteger c2 = Enc(pk, m2);

		BigInteger lhs = Dec(sk, XOR(pk, c1, c2));
		BigInteger rhs = m1.xor(m2).mod(N);

		if (lhs.compareTo(rhs) == 0) return true;

		return false;
	}

	public static boolean HomomorphicNegationTest(int n, BigInteger m) {
		KeyPair kP = Gen(n);
		PublicKey pk = kP.getPublicKey();
		PrivateKey sk = kP.getPrivateKey();

		BigInteger c = Enc(pk, m);

		BigInteger lhs = Dec(sk, Negate(pk, c));
		BigInteger rhs = (m.compareTo(BigInteger.ONE) == 0) ? BigInteger.ZERO : BigInteger.ONE;

		if (lhs.compareTo(rhs) == 0) return true;

		return false;
	}

}
