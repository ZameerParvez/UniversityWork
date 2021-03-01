package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Paths;

/**
 * This class is capable of encrypt and decrypt according to the Vigen&egrave;re
 * cipher.
 *
 * @author Changyu Dong
 * @author Roberto Metere
 * @author Zameer Parvez
 */
public class VigenereCipher {

    /**
     * Encryption function of the Vigen&egrave;re cipher.
     *
     * <p>
     * TODO: Complete the Vigen&egrave;re encryption function.
     *
     * @param plaintext the plaintext to encrypt
     * @param key the encryption key
     * @return the ciphertext according with the Vigen&egrave;re cipher.
     */
    public static String encrypt(String plaintext, String key) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">
        plaintext = plaintext.toUpperCase();
        key = key.toUpperCase();
        int keyLength = key.length();
        StringBuilder ciphertext = new StringBuilder();

        int i = 0;
        int j = 0;
        while (i < plaintext.length()) {
            char c = plaintext.charAt(i);
            if (c >= Util.OFFSET && c < Util.OFFSET + 26) {
                char keyChar = key.charAt(j % keyLength);
                c = (char) (((c + keyChar) % 26) + Util.OFFSET);
                j++;
            }

            ciphertext.append(c);
            i++;
        }

    	return ciphertext.toString();
        //</editor-fold> // END OF YOUR CODE
    }

    /**
     * Decryption function of the Vigen&egrave;re cipher.
     *
     * <p>
     * TODO: Complete the Vigen&egrave;re decryption function.
     *
     * @param ciphertext the encrypted text
     * @param key the encryption key
     * @return the plaintext according with the Vigen&egrave;re cipher.
     */
    public static String decrypt(String ciphertext, String key) {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">
        ciphertext = ciphertext.toUpperCase();
        key = key.toUpperCase();
        int keyLength = key.length();
        StringBuilder plaintext = new StringBuilder();

        int i = 0;
        int j = 0;
        while (i < ciphertext.length()) {
            char c = ciphertext.charAt(i);
            if (c >= Util.OFFSET && c < Util.OFFSET + 26) {
                char keyChar = key.charAt(j % keyLength);
                // This requires the extra "+ 26) % 26)" because the inner value can sometimes be 0, which is not the case when encrypting
                c = (char) (((((c - keyChar) % 26) + 26) % 26) + Util.OFFSET);
                j++;
            }

            plaintext.append(c);
            i++;
        }

        return plaintext.toString();
        //</editor-fold> // END OF YOUR CODE
    }

    public static void test_VigenereCipher() throws URISyntaxException, IOException {
        final FrequencyAnalyser analyser = new FrequencyAnalyser();
        final String mainPath = Paths.get(FrequencyCryptanalysis.class.getResource("/").toURI()).toString();
        final String plaintext = Util.readFileToBuffer(mainPath + "/res/pg1661.txt");
        String[] plaintexts = {plaintext, "system security and control", "newcastle university"};
        String[] keys = {"ncl", "security", "ncl"};

        for (int i = 0; i < plaintexts.length; i++) {
            String ciphertext = VigenereCipher.encrypt(plaintexts[i], keys[i]);
            String decryptesPlaintext = VigenereCipher.decrypt(ciphertext, keys[i]);
            analyser.setText(plaintext);
            FrequencyTable plainTextFrequencies = analyser.analyse();
            analyser.setText(ciphertext);
            FrequencyTable cipherTextFrequencies = analyser.analyse();

            System.out.println("-----------------------------------------");
            System.out.printf("Plaintext:\n\t%s\nKey:\n\t%s\nCiphertext:\n\t%s\nDecrypted Plaintext:\n\t%s\nPlaintext Frequency Analysis:\n%s\nCiphertext Frequency Analysis:\n%s\n", plaintexts[i], keys[i], ciphertext, decryptesPlaintext, plainTextFrequencies, cipherTextFrequencies);
            System.out.println("-----------------------------------------");
        }
    }
}
