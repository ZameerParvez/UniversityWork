package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

import java.util.Arrays;

/**
 * An instance of this class is a mapping of 26 indexes (corresponding to
 * English alphabet) to <strong>relative</strong> frequencies. Relative
 * frequencies are real numbers in the interval [0, 1].
 *
 * @author Changyu Dong
 * @author Roberto Metere
 * @author Zameer Parvez
 */
public class FrequencyTable {

    /**
     * We want to distinguish an incomplete entry in the table from a frequency
     * that genuinely is 0.
     */
    public static final double ENTRY_INCOMPLETE = -1.0;

    /**
     * Frequency table. We assume the convention that A, B, ..., Z correspond to
     * 0, 1, ..., 25 respectively.
     *
     * In practice: table[0] stores the frequency of character 'A', table[1]
     * stores the frequency of character 'B', ... table[25] stores the frequency
     * of character 'Z'
     */
    private final double[] table;


    /**
     * The constructor initialises the table to all incomplete values.
     */
    public FrequencyTable() {
        this.table = new double[26];
        clear();
    }

    /**
     * This method re-initialises the table to all incomplete values.
     */
    public final void clear() {
        Arrays.fill(this.table, ENTRY_INCOMPLETE);
    }

    /**
     * This method checks whether a frequency is incomplete.
     *
     * @param value the frequency value to check
     * @return <code>true</code> only if the value is not incomplete.
     */
    private static boolean isFrequencyIncomplete(double value) {
        return value < 0.0d;
    }

    /**
     * This method checks whether a frequency is valid (0 <= freq <= 1).
     *
     * @param value the frequency value to check
     * @return <code>true</code> only if the value is a relative frequency.
     */
    private static boolean isValidRelativeFrequency(double value) {
        return value >= 0.0d && value <= 1.0d;
    }

    /**
     * Print out the current frequency table. Print NA for any frequency entry
     * that has not been set.
     */
    public void print() {
        System.out.print(this);
    }

    /**
     * Get a copy of the internal frequency table.
     *
     * @return the internal frequency table.
     */
    public double[] getTable() {
        return this.table.clone();
    }



    /**
     * Read the frequency given the character from the table. If the frequency
     * has not be entered (yet), the result will be ENTRY_INCOMPLETE.
     *
     * @param letter the letter to read the frequency of
     * @return the relative frequency of the letter
     */
    public double getFrequency(char letter) {
        int index = Util.charToIndex(letter);
        return this.table[index];
    }

    /**
     * Set the frequency of the given character. The frequency must neither be
     * negative nor greater than 1.
     *
     * @param letter the letter to write the frequency of
     * @param freq the frequency to set
     */
    public void setFrequency(char letter, double freq) {
        int index = Util.charToIndex(letter);

        if (isFrequencyIncomplete(freq)) {
            throw new IllegalArgumentException("Frequency must not be negative");
        } else if (!isValidRelativeFrequency(freq)) {
            throw new IllegalArgumentException("Frequency must not be greater than 1");
        }

        this.table[index] = freq;

    }

    /**
     * This method checks whether the frequency table has been completed.
     *
     * @return <code>true</code> if all entries have been filled.
     */
    public boolean isComplete() {
        for (int i = 0; i < 26; i++) {
            if (isFrequencyIncomplete(this.table[i])) {
                return false;
            }
        }

        return true;
    }

    /**
     * Method to tell whether two double values are (almost) equal.
     *
     * @param value1 first value
     * @param value2 second value
     * @return <code>true</code> if the two values are very close each other.
     */
    private boolean doubleEquals(double value1, double value2) {
        return doubleEquals(value1, value2, 10E-6);
    }

    /**
     * Method to tell whether two double values are close within an interval.
     *
     * @param value1 first value
     * @param value2 second value
     * @param uncertainty half the interval
     * @return <code>true</code> if the difference of the two values is less
     * than uncertainty.
     */
    private boolean doubleEquals(double value1, double value2, double uncertainty) {
        if (uncertainty < 0.0d) {
            uncertainty = -uncertainty;
        }
        return (value1 >= value2 - uncertainty) && (value1 <= value2 + uncertainty);
    }

    /**
     * This method checks whether the frequency table shapes a distribution
     * probability. (This is very likely to be the case).
     *
     * @return <code>true</code> if all entries have been filled.
     */
    public boolean isConsistent() {
        double total = 0.0d;

        for (int i = 0; i < 26; i++) {
            total += this.table[i];
        }

        return isComplete() && doubleEquals(total, 1.0d);
    }

    @Override
    public String toString() {
        String res = "";
        
        for (int i = 0; i < 26; i++) {
            char letter = (char) (Util.OFFSET + i);
            res += "(" + letter + ", " + ((table[i] < 0.0d) ? "NA" : table[i]) + ")\n";
        }
        
        return res;
    }

}
