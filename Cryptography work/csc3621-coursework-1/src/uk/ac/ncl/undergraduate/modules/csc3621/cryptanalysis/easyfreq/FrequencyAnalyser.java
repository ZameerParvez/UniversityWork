package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

/**
 * This class is to compute a frequency table of a texts.
 *
 * @author Changyu Dong
 * @author Roberto Metere
 * @author Zameer Parvez
 */
public class FrequencyAnalyser {

    /**
     * The text to analyse
     */
    private String text;

    /**
     * Get the text to analyse.
     *
     * @return the text to analyse.
     */
    public String getText() {
        return text;
    }

    /**
     * Set the text to analyse.
     *
     * @param text the text to analyse.
     */
    public void setText(String text) {
        this.text = text;
    }

    /**
     * This method returns a frequency table as a result of the analysis of the
     * text.
     *
     * TODO: complete the function that conduct a frequency analysis of the
     * internal buffer and produce a frequency table based on the analysis.
     * Please, write your code between the comments as appropriate.
     *
     * @return frequency table as a result of the analysis of the text
     */
    public FrequencyTable analyse() {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">
        String text = getText().toUpperCase();
        
        double[] charCounts = new double[26];
        int totalNumOfChars = 0;

        for (char c : text.toCharArray()) {
            int charAsInt = c;
            
            if (charAsInt >= Util.OFFSET && charAsInt < Util.OFFSET + 26) {
                int index = charAsInt - Util.OFFSET;
                charCounts[index] += 1;
                totalNumOfChars++;
            }

        }        

        FrequencyTable freqTable = new FrequencyTable();
        for (int i = 0; i < charCounts.length; i++) {
            char character = (char) (i + Util.OFFSET);
            double freq = charCounts[i] / totalNumOfChars;
            freqTable.setFrequency(character , freq);
        }

        return freqTable;

        //</editor-fold> // END OF YOUR CODE
    }

}
