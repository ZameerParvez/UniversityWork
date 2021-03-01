package uk.ac.ncl.undergraduate.modules.csc3621.CPA;

import java.math.BigInteger;
import java.util.Arrays;
import java.util.HashMap;

/**
 * The adversary in the CPA game
 * @author Changyu Dong
 * @author Zameer Parvez
 *
 */

public class Adversary{
	final static int MAX_BYTE_ARRAY_SIZE = 16;
	final static boolean ENABLE_DEBUG_PRINTS = true;
	int guess;

	public Adversary() {
		// TODO Auto-generated constructor stub
	}
	/**
     * Run the CPA experiment with the challenger cha. You can query the challenger with chosen plaintexts in the query phpase.
     * You can send two plaintext m0 and m1 to the challenger (once) in the challenge phase.
     * At the end of the experiment, guess which one of m0 and m1 was encrypted, and output your guess (through the variable bPrime, either 0 or 1).
     * Your guess will be submitted through cha.guess(bPrime) and the outcome will be recorded.
     *
     * <p>
     * TODO:
     * <ul>
     * <li> Find and implement a strategy to win the CPA game.
     * </ul>
     *
	 * @param cha the challenger that plays the game with you
	 */
	
	public void run(Challenger cha) {
		
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">
		
		

        //</editor-fold> // END OF YOUR COD

		// xoring the message with the IV and sending that to the oracle as the plaintext would make the ciphertext the product of only the message and the key
		// This is because plaintext = m xor IV
		// and the first step of the CBC is to calculate,
		// plaintext xor IV
		// = (m xor IV) xor IV
		// = m xor (IV xor IV), because xor is associative
		// = m
		
		
		// messages to use in querying
		String m1 = "hello";
		String m2 = "bye";

		// counter initialised to the same as the challenger
		BigInteger counter = BigInteger.ZERO;

		// This will store a map of messages to ciphertexts produced,
		// because the ciphertext is the only part of the queries output we need for the test
		HashMap<String, String> queryResults = new HashMap<>();
		
		// The plaintext sent to the challenger should be IV xor message
		String p1 = new String(xorMessageAndCounter(counter, m1));
		String[] q1 = cha.query(p1);
		queryResults.put(m1, q1[1]);
		counter = counter.add(BigInteger.ONE);

		String p2 = new String(xorMessageAndCounter(counter, m1));
		String[] q2 = cha.query(p2);
		queryResults.put(m1, q2[1]);
		counter = counter.add(BigInteger.ONE);

		// The cipherText produced by q1 and q2 should be the same,
		// as long as the IV that the challenger is using is the same value as the IV I am xoring the message with
		debugPrint("q1 IV+ciphertext: " + Arrays.toString(q1));
		debugPrint("q2 IV+ciphertext: " + Arrays.toString(q2));
		debugPrint(q1[1].equals(q2[1]), "Messages are the same", "Messages are Different");

		// For the challenge, the IV should be the same when preparing each message because only one will be encrypted
		// The plaintext sent here is different to the plaintexts sent before because the counter has changed, so IV xor m is different to before
		String plaintext1 = new String(xorMessageAndCounter(counter, m1));
		String plaintext2 = new String(xorMessageAndCounter(counter, m2));
		counter = counter.add(BigInteger.ONE);

		String[] result = cha.challenge(plaintext1, plaintext2);

		// The ciphertext produced by the queries should only be determined by the message, so we can check if the ciphertext for one of the messages has been seen before
		if (queryResults.containsKey(m1) && result[1].equals(queryResults.get(m1))) {
			this.guess = 0;
		} else {
			this.guess = 1;
		}

		debugPrint("Guess = " + this.guess + "\n=================================================");

		int bPrime = decide();
		
		cha.guess(bPrime);
		
	}

	// This method is used to xor the message and counter and assumes that counter and message are less than 16 bytes
	// This 16 byte condition is ok for this exercise, because the Challenger uses a 16 byte array
	private byte[] xorMessageAndCounter(BigInteger counter, String message) {
		// Get counter and string as byte array so they can be xored 
		byte[] messageAsByteArray = padMessageByteArrayPKCS5(message.getBytes());
		byte[] counterAsByteArray = padCounterByteArray(counter.toByteArray());

		debugPrint("IVBytes:\t\t" + byteArrayToHexString(counterAsByteArray));
		debugPrint("MessageBytes:\t" + byteArrayToHexString(messageAsByteArray));

		for (int i = 0; i < counterAsByteArray.length; i++) {
			messageAsByteArray[i] = (byte) (messageAsByteArray[i] ^ counterAsByteArray[i]);
		}

		debugPrint("XoredMessageWithIVBytes: " + byteArrayToHexString(messageAsByteArray));

		return messageAsByteArray;
	}
	
	private byte[] padCounterByteArray(byte[] counter) {
		byte[] paddedCounter = new byte[MAX_BYTE_ARRAY_SIZE];
		for (int i = 0; i < counter.length; i++) {
			paddedCounter[paddedCounter.length - i - 1] = counter[i];
		}

		return paddedCounter;
	}

	// Any form of padding would have worked here, because it only needs to fill up the 16 bytes to match the size of the IV
	// This might not be a correct implementation of PKCS5 but for this purpose it does not matter
	private byte[] padMessageByteArrayPKCS5(byte[] message) {
		int padValue = MAX_BYTE_ARRAY_SIZE - (message.length % MAX_BYTE_ARRAY_SIZE);

		byte[] paddedMessage = new byte[message.length + padValue];
		for (int i = message.length; i < paddedMessage.length; i++) {
			paddedMessage[i] = (byte) padValue;
		}

		return paddedMessage;
	}

	private static final String byteArrayToHexString(byte[] input) {
		StringBuilder sb = new StringBuilder();
		for (byte b : input) {
			sb.append(String.format("%02X ", b));
		}
		return sb.toString();
	}

	private static final void debugPrint(boolean condition, String trueText, String falseText) {
		if (!ENABLE_DEBUG_PRINTS) return;
		if (condition) {
			System.out.println(trueText);
		} else {
			System.out.println(falseText);
		}
	}

	private static final void debugPrint(boolean condition, String trueText) {
		debugPrint(condition, "", trueText);
	}

	private static final void debugPrint(String text) {
		debugPrint(true, text, "");
	}
	
	/**
	 * Return your guess of which plaintext was encrypted. You should not modify this method.
	 * @return the guess of index
	 */
	private int decide() {

		return this.guess;
	}

}
