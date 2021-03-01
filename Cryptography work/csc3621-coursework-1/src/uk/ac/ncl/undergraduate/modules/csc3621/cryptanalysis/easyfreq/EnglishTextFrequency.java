package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Paths;

/**
 * This class is a singleton to get a frequency table of an English plaintext
 *
 * @author Zameer Parvez
 */
public class EnglishTextFrequency {
    private static EnglishTextFrequency englishTextFrequency;
    private static boolean isInstantiated = false;
    private final FrequencyTable englishFrequencyTable;

    private EnglishTextFrequency() throws IOException, URISyntaxException {
        String mainPath = Paths.get(FrequencyCryptanalysis.class.getResource("/").toURI()).toString();;
        String englishText = Util.readFileToBuffer(mainPath + "/res/pg1661.txt");

        // Create a frequency table for english text
        FrequencyAnalyser analyser = new FrequencyAnalyser();
        analyser.setText(englishText);
        this.englishFrequencyTable = analyser.analyse();
        this.isInstantiated = true;
    }

    public static FrequencyTable getEngishFrequencyTable() {
        if (!EnglishTextFrequency.isInstantiated) {
            try {
                englishTextFrequency = new EnglishTextFrequency();
            } catch (IOException e) {
                e.printStackTrace();
            } catch (URISyntaxException e) {
                e.printStackTrace();
            }
        }

        return EnglishTextFrequency.englishTextFrequency.englishFrequencyTable;
    }
}
