package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Paths;
import java.util.*;

/**
 * This class is for frequency cryptanalysis of ciphertext when the key is an
 * integer.
 *
 * @author Changyu Dong
 * @author Roberto Metere
 * @author Zameer Parvez
 */
public class FrequencyCryptanalysis {

    /**
     * The ciphertext (encryption of the plaintext).
     */
    private String ciphertext;

    /**
     * The plaintext (readable content).
     */
    private String plaintext;

    /**
     * The key such that the encryption of the plaintext with such key gives the
     * ciphertext.
     */
    private int key;

    /**
     * Frequency table (of the ciphertext).
     */
    private FrequencyTable table;


    /**
     * Set the ciphertext to analyse.
     *
     * @param text the text to set as ciphertext
     */
    public void setCiphertext(String text) {
        this.ciphertext = text;
    }

    /**
     * Create an new class to cryptanalyze texts.
     */
    public FrequencyCryptanalysis() {
    }

    public double calculateChiSquared(FrequencyTable englishTextFrequencies, FrequencyTable ciphertextfrequencies) {
        int totalCount = ciphertext.length();
        double sum = 0;
        for (int i = 0; i < 26; i++) {
            char letter = (char) (i + Util.OFFSET);
            int observedCount = (int) Math.floor(totalCount * ciphertextfrequencies.getFrequency(letter));
            double expectedCount = totalCount * englishTextFrequencies.getFrequency(letter);
            sum += Math.pow(observedCount - expectedCount, 2) / expectedCount;
        }

        return sum;
    }

    // Positive offset encrypts, negative offset decrypts
    public String shiftText(String inText, int offset) {
        StringBuilder s = new StringBuilder();
        String text = inText.toUpperCase();

        for (int i = 0; i < text.length(); i++) {
            char c = text.charAt(i);

            if (c >= Util.OFFSET && c < Util.OFFSET + 26) {
                // This normalisedChar is the int value that the char should be once the key is used to decrypt the message
                // (c - Util.OFFSET), gives the value of the char, 0 to 26
                // ((c - Util.OFFSET) - key) % 26), applies the key shift, the resulting value is from -25 to 25
                // ((((c - Util.OFFSET) - key) % 26) + 26) % 26, The surrounding "+ 26) % 26" makes it so that the resulting value is from 0 to 25
                int normalisedChar = ((((c - Util.OFFSET) + offset) % 26) + 26) % 26;
                c = (char) (normalisedChar + Util.OFFSET);
            }

            s.append(c);
        }

        return s.toString();
    }

    // This method gets the key using chi squared to check which of the 26 keys applied to the ciphertext, produces
    // a distribution that is most similar to the distribution of characters in an english plaintext
    public int getKeyChiSquared() {
        // Get a frequency table for english plaintext
        FrequencyTable englishTextFrequencies = EnglishTextFrequency.getEngishFrequencyTable();
//        System.out.println("------------------\nEnglish Text Frequency Analysis\n------------------\n" + englishTextFrequencies);

        FrequencyAnalyser analyser = new FrequencyAnalyser();
        List<Double> chiValues = new ArrayList<>();
        int candidateKey = 0;
        double candidateKeysChi = Double.MAX_VALUE;

        for (int i = 0; i < 26; i++) {
            String shiftedCiphertext = shiftText(ciphertext, i);
            analyser.setText(shiftedCiphertext);
            FrequencyTable f = analyser.analyse();
            double chiValue = calculateChiSquared(englishTextFrequencies, f);
            chiValues.add(chiValue);
            if (chiValue < candidateKeysChi) {
                candidateKeysChi = chiValue;
                candidateKey = i;
            }
        }

        return 26 - candidateKey;
    }

    // This method uses only the most frequent character in the ciphertext to obtain the key, it ignores the rest of the character distribution
    public int getKeyNaiveMethod() {
        // Create a frequency table for the ciphertext
        FrequencyAnalyser analyser = new FrequencyAnalyser();
        analyser.setText(ciphertext);
        table = analyser.analyse();
//    	System.out.println("------------------\nCiphertext Frequency Analysis\n------------------\n" + table);

        // It is most likley that the most frequent character in the cipher text should map to the most frequent character in the plaintext.
        // Therefore the key is likley the difference between the most frequent characters in the plaintext and ciphertext
        int charWithLargestFrequency = 0;
        double largestFrequency = 0;
        for (int i = 0; i < 26; i++) {
            char letter = (char) (i + Util.OFFSET);
            double freq = table.getFrequency(letter);

            if (freq > largestFrequency) {
                largestFrequency = freq;
                charWithLargestFrequency = letter;
            }
        }

        // E should be the most frequent for a plain english text, so sorting a frequency table for it should not be neccassary
        return charWithLargestFrequency - 'E';
    }

    /**
     * This method conducts cryptanalysis of the frequency of letters in the
     * ciphertext to retrieve the encryption key.
     *
     * <p>
     * TODO:
     * <ul>
     * <li>Conduct a frequency analysis of the internal buffer.
     * <li>Find the key. You should try your best to find the key based on your
     * analysis.
     * <li>Store the key in the class variable <code>this.key</code>.
     * </ul>
     *
     *
     * @return the key as the result of the cryptanalysis
     */
    public int cryptanalysis() {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">

        // Calculate a key whith either a naive method or using chisquared.
        int keyWithChiSquared = getKeyChiSquared();
//        int keyWithNaiveMethod = getKeyNaiveMethod();

//        System.out.printf("===key===\nchi-squared: %d\tnaive method: %d\n=======", keyWithChiSquared, keyWithNaiveMethod);

        // Chi squared method is used because for a large ciphertext, it finds the correct key more often than the naive method
        this.key = keyWithChiSquared;
    	//</editor-fold> // END OF YOUR CODE
        

        return this.key;
    }
    

    /**
     * This method reconstructs the plaintext from the ciphertext with the key.
     *
     * <p>
     * TODO:
     * <ul>
     * <li>After finding the key, use the key to decrypt the ciphertext
     * <li>Store the plaintext in the class variable
     * <code>this.plaintext</code>.
     * </ul>
     */
    public void decrypt() {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">
        this.plaintext = shiftText(ciphertext, -1 * key);
        //</editor-fold> // END OF YOUR CODE
    }

    /**
     * Show the results of the complete analysis.
     */
    public void showResult() {
        System.out.println("The key is " + this.key);
        this.decrypt();
        System.out.println("The plaintext is:");
        System.out.println(this.plaintext);
    }

    /**
     * @param args the command line arguments
     * @throws java.io.IOException errors reading from files
     * @throws java.net.URISyntaxException Errors in retrieving resources
     */
    public static void main(String[] args) throws IOException, URISyntaxException {
        String mainPath, plaintextFilePath, ciphertextFilePath, plaintext, ciphertext;
        FrequencyAnalyser frequencyAnalyser;
        FrequencyTable frequencyTable;
        FrequencyCryptanalysis cryptanalysis;
        File solutionDirectory;
        String solutionFrequencyFilePath, solutionKeyFilePath, solutionPlaintextFilePath;


        cryptanalysis = new FrequencyCryptanalysis();


        // Get resources
        mainPath = Paths.get(FrequencyCryptanalysis.class.getResource("/").toURI()).toString();
        plaintextFilePath = mainPath + "/res/pg1661.txt";
        ciphertextFilePath = mainPath + "/res/Exercise1Ciphertext.txt";
        solutionDirectory = new File(mainPath + "/solution1");
        solutionFrequencyFilePath = solutionDirectory + "/frequency.txt";
        solutionKeyFilePath = solutionDirectory + "/key.txt";
        solutionPlaintextFilePath = solutionDirectory + "/plaintext.txt";

        // Analyse the readable text
        plaintext = Util.readFileToBuffer(plaintextFilePath);
        frequencyAnalyser = new FrequencyAnalyser();
        frequencyAnalyser.setText(plaintext);
        frequencyTable = frequencyAnalyser.analyse();
        frequencyTable.print();

        // Crack the ciphertext
        ciphertext = Util.readFileToBuffer(ciphertextFilePath);
        cryptanalysis.setCiphertext(ciphertext);
        cryptanalysis.cryptanalysis();
        cryptanalysis.showResult();

        // Write solution in res path
        if (!solutionDirectory.exists()) {
            solutionDirectory.mkdir();
        }
        Util.printBufferToFile(frequencyTable.toString(), solutionFrequencyFilePath);
        Util.printBufferToFile(Integer.toString(cryptanalysis.key), solutionKeyFilePath);
        Util.printBufferToFile(cryptanalysis.plaintext, solutionPlaintextFilePath);
    }
}
