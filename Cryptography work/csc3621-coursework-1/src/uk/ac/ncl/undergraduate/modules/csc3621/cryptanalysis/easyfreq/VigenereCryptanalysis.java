package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Paths;
import java.util.*;

import static java.util.stream.Collectors.toMap;

/**
 * This class is for frequency cryptanalysis of ciphertext.
 *
 * @author Changyu Dong
 * @author Roberto Metere
 * @author Zameer Parvez
 */
public class VigenereCryptanalysis {

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
    private final StringBuffer key = new StringBuffer();


    /**
     * Create an new class to cryptanalyze texts.
     */
    public VigenereCryptanalysis() {
    }


    /**
     * Set the ciphertext to analyse.
     *
     * @param text the text to set as
     */
    public void setCiphertext(String text) {
        this.ciphertext = text;
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
     * @return the key as result of the cryptanalysis
     */
    public String cryptanalysis() {
        // Please, do not remove the editor-fold comments.
        //<editor-fold defaultstate="collapsed" desc="Write your code here below!">

        // we know that a key of length n was used to encrypt the plaintext

        String ciphertext = this.ciphertext.toUpperCase().replaceAll("[^a-zA-Z]", "");
        final double INDEX_COINCIDENCE_LOWERBOUND = 0.06;
        final double INDEX_COINCIDENCE_UPPERBOUND = 0.08;
        final int KEY_SIZE_LOWERBOUND = 1;
        final int KEY_SIZE_UPPERBOUND = 100;

        List<Pair<List<String>, Double>>  splitCiphertextMeanICPair = new ArrayList<>();

        for (int i = KEY_SIZE_LOWERBOUND; i < KEY_SIZE_UPPERBOUND; i++) {
            List<String> ciphertextColumns = splitNColumns(ciphertext, i);
            double meanIC = getMeanIC(ciphertextColumns);
            if (meanIC > INDEX_COINCIDENCE_LOWERBOUND && meanIC < INDEX_COINCIDENCE_UPPERBOUND) {
                splitCiphertextMeanICPair.add(new Pair<>(ciphertextColumns, meanIC));
            }
        }

        List<Pair<String, Double>> candidateKeysMeanICPair = new ArrayList<>();

        for (int i = 0; i < splitCiphertextMeanICPair.size(); i++) {
            Pair<List<String>, Double> temp = splitCiphertextMeanICPair.get(i);
            List<String> ciphertextColumns = splitCiphertextMeanICPair.get(i).getKey();

            StringBuilder candidateKey = new StringBuilder();

            for (int j = 0; j < ciphertextColumns.size(); j++) {
                FrequencyCryptanalysis f = new FrequencyCryptanalysis();
                f.setCiphertext(ciphertextColumns.get(j));
                candidateKey.append((char) (((f.cryptanalysis() + 26) % 26) + Util.OFFSET));
            }

            candidateKeysMeanICPair.add(new Pair<>(candidateKey.toString(), splitCiphertextMeanICPair.get(i).getValue()));
            System.out.println("CandidateKeySize=" + temp.getKey().size() + " MeanIC=" + temp.getValue() + " CandidateKey=" + candidateKey);
        }

        this.key.delete(0, this.key.length());

        if (candidateKeysMeanICPair.size() != 0) {
            // This gets the first candidate key which has a meanIC associated with it of above 0.06 (INDEX_COINCIDENCE_UPPERBOUND)
            // This key is not always correct, and can have some errors in it, which come from the ceaser ciphers
            // cryptanalysis method
            Pair<String, Double> mostLikleyKey = candidateKeysMeanICPair.get(0);
            this.key.append(mostLikleyKey.getKey());
        }

        //</editor-fold> // END OF YOUR CODE
        
        return this.key.toString();
    }

    // Splits ciphertext into n columns, where letter i goes into column i % n, for n /= 0
    public static List<String> splitNColumns(String s, int n) {
        if (n == 0) return new ArrayList<String>();
        String ciphertext = s.toUpperCase().replaceAll("[^a-zA-Z]", "");
        List<StringBuilder> tempTable = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            tempTable.add(new StringBuilder());
        }

        for (int i = 0; i < ciphertext.length(); i++) {
            tempTable.get(i % n).append(ciphertext.charAt(i));
        }

        List<String> table = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            table.add(tempTable.get(i).toString());
        }

        return table;
    }

    // Calculates the mean Index Coincidence of an array of strings
    public static double getMeanIC(List<String> textColumns) {
        double meanIC = 0;

        for (int i = 0; i < textColumns.size(); i++) {
            meanIC += getIC(textColumns.get(i)) / textColumns.size();
        }

        return meanIC;
    }

    // Calculated the Index Coincidence of a string S
    public static double getIC(String s) {
    	int[] counts = new int[26];
    	int totalCount = 0;
    	
    	for (int i = 0; i < s.length(); i++) {
    		char c = s.charAt(i);
    		
    		if (c >= Util.OFFSET && c < Util.OFFSET + 26) {
    			counts[c - Util.OFFSET]++;
    			totalCount++;
    		}
    	}
    	
    	int sum = 0;
    	for (int i = 0; i < 26; i++) {
    		sum += counts[i]*(counts[i]-1);
    	}
    	
    	double indexOfCoincidence = ((double) sum) / (totalCount * (totalCount - 1));
    	
    	return indexOfCoincidence;
    }
    
    // Default behaviour gives non overlapping ngrams
    public static Map<String, List<Integer>> findNGrams(String s, int n) {
        return findNGrams(s, n, false);
    }

    // Currently unused method
    // Finds ngrams of a given length in a string, and returns the indicies of where they start
    // This only gives the correct indicies if your working with a string with only alphabetic characters
    public static Map<String, List<Integer>> findNGrams(String s, int n, boolean isOverlappingNGrams) {
        final int increment = isOverlappingNGrams ? n : 1;
        String ciphertext = s.toUpperCase().replaceAll("[^a-zA-Z]", "");
        HashMap<String, List<Integer>> nGramsAndMatchIndicies = new HashMap<>();

        for (int i = 0; i < ciphertext.length() - n; i+=increment) {
            String candidateNgram = s.substring(i, i + n);
            List<Integer> indicies = nGramsAndMatchIndicies.containsKey(candidateNgram) ? nGramsAndMatchIndicies.get(candidateNgram) : new ArrayList<>();
            indicies.add(i);
            nGramsAndMatchIndicies.put(candidateNgram, indicies);
        }

        Map<String, List<Integer>> filtered = nGramsAndMatchIndicies.entrySet()
                .stream()
                .filter(entry -> entry.getValue().size() > 1)
                .collect(toMap(Map.Entry::getKey, Map.Entry::getValue));


        return filtered;
    }

    /**
     * This method reconstructs the plaintext from the ciphertext with the key.
     */
    public void decrypt() {
        this.plaintext = VigenereCipher.decrypt(this.ciphertext, this.key.toString());
    }
    
    
    /**
     * Show the results of the complete analysis.
     */
    public void showResult() {
        System.out.println("The key is " + this.key.toString());
        this.decrypt();
        System.out.println("The plaintext is:");
        System.out.println(this.plaintext);
    }
    
    // In Eclipse Version: 2019-06 (4.12.0), the plaintext read from the file /res/pg1661.txt does not print to the console successfully, although it can still be copied from their
    /**
     * @param args the command line arguments
     * @throws java.io.IOException errors reading from files
     * @throws java.net.URISyntaxException Errors in retrieving resources
     */
    public static void main(String[] args) throws IOException, URISyntaxException {
        String mainPath, ciphertextFilePath, ciphertext;
        VigenereCryptanalysis cryptanalysis;
        File solutionDirectory;
        String solutionKeyFilePath, solutionPlaintextFilePath;

        cryptanalysis = new VigenereCryptanalysis();
        // These method calls can be uncommented to run the tests of the vigenere cipher and the vigenere cipher cryptanalysis tool
//        VigenereCipher.test_VigenereCipher();
        test_Cryptanalysis();

        // Get resources
        mainPath = Paths.get(FrequencyCryptanalysis.class.getResource("/").toURI()).toString();
        ciphertextFilePath = mainPath + "/res/Exercise2Ciphertext.txt";
        solutionDirectory = new File(mainPath + "/solution2");
        solutionKeyFilePath = solutionDirectory + "/key.txt";
        solutionPlaintextFilePath = solutionDirectory + "/plaintext.txt";

        // Do the job
        ciphertext = Util.readFileToBuffer(ciphertextFilePath);
        cryptanalysis.setCiphertext(ciphertext);
        cryptanalysis.cryptanalysis();
        cryptanalysis.showResult();


        // Write solution in res path
        if (!solutionDirectory.exists()) {
            solutionDirectory.mkdir();
        }
        Util.printBufferToFile(cryptanalysis.key.toString(), solutionKeyFilePath);
        Util.printBufferToFile(cryptanalysis.plaintext, solutionPlaintextFilePath);
    }

    public static void test_Cryptanalysis() {
        VigenereCryptanalysis cryptanalysis = new VigenereCryptanalysis();
        String[] plaintexts = {
                "NEITHERMUSTWEFORGETTHATTHEREHAVEINCLUDEDANIDEALHISTORYOFATHENSASWELLASAPOLITICALANDPHYSICALPHILOSOPHYTHEFRAGMENTOFTHECRITIASHASGIVENBIRTHTOAWORLDFAMOUSFICTIONSECONDONLYINIMPORTANCETOTHETALEOFTROYANDTHELEGENDOFARTHURANDISSAIDASAFACTTOHAVEINSPIREDSOMEOFTHEEARLYNAVIGATORSOFTHESIXTEENTHCENTURYTHISMYTHICALTALEOFWHICHTHESUBJECTWASAHISTORYOFTHEWARSOFTHEATHENIANSAGAINSTTHEISLANDOFATLANTISISSUPPOSEDTOBEFOUNDEDUPONANUNFINISHEDPOEMOFSOLONTOWHICHITWOULDHAVESTOODINTHESAMERELATIONASTHEWRITINGSOFTHELOGOGRAPHERSTOTHEPOEMSOFHOMERITWOULDHAVETOLDOFASTRUGGLEFORLIBERTYCPTIMINTENDEDTOREPRESENTTHECONFLICTOFPERSIAANDHELLASWEMAYJUDGEFROMTHENOBLECOMMENCEMENTOFTHETIMAEUSFROMTHEFRAGMENTOFTHECRITIASITSELFANDFROMTHETHIRDBOOKOFTHELAWSINWHATMANNERPLATOWOULDHAVETREATEDTHISHIGHARGUMENTWECANONLYGUESSWHYTHEGREATDESIGNWASABANDONEDPERHAPSBECAUSEPLATOBECAMESENSIBLEOFSOMEINCONGRUITYINAFICTITIOUSHISTORYORBECAUSEHEHADLOSTHISINTERESTINITORBECAUSEADVANCINGYEARSFORBADETHECOMPLETIONOFITANDWEMAYPLEASEOURSELVESWITHTHEFANCYTHATHADTHISIMAGINARYNARRATIVEEVERBEENFINISHEDWESHOULDHAVEFOUNDPLATOHIMSELFSYMPATHISINGWITHTHESTRUGGLEFORHELLENICINDEPENDENCECPLAWSSINGINGAHYMNOFTRIUMPHOVERMARATHONANDSALAMISPERHAPSMAKINGTHEREFLECTIONOFHERODOTUSWHEREHECONTEMPLATESTHEGROWTHOFTHEATHENIANEMPIREHOWBRAVEATHINGISFREEDOMOFSPEECHWHICHHASMADETHEATHENIANSSOFAREXCEEDEVERYOTHERSTATEOFHELLASINGREATNESSORMOREPROBABLYATTRIBUTINGTHEVICTORYTOTHEANCIENTGOODORDEROFATHENSANDTOTHEFAVOROFAPOLLOANDATHENECPINTRODTOCRITIASAGAINPLATOMAYBEREGARDEDASTHECAPTAINARHCHEGOZORLEADEROFAGOODLYBANDOFFOLLOWERSFORINTHEREPUBLICISTOBEFOUNDTHEORIGINALOFCICEROSDEREPUBLICAOFSTAUGUSTINESCITYOFGODOFTHEUTOPIAOFSIRTHOMASMOREANDOFTHENUMEROUSOTHERIMAGINARYSTATESWHICHAREFRAMEDUPONTHESAMEMODELTHEEXTENTTOWHICHARISTOTLEORTHEARISTOTELIANSCHOOLWEREINDEBTEDTOHIMINTHEPOLITICSHASBEENLITTLERECOGNISEDANDTHERECOGNITIONISTHEMORENECESSARYBECAUSEITISNOTMADEBYARISTOTLEHIMSELFTHETWOPHILOSOPHERSHADMOREINCOMMONTHANTHEYWERECONSCIOUSOFANDPROBABLYSOMEELEMENTSOFPLATOREMAINSTILLUNDETECTEDINARISTOTLEINENGLISHPHILOSOPHYTOOMANYAFFINITIESMAYBETRACEDNOTONLYINTHEWORKSOFTHECAMBRIDGEPLATONISTSBUTINGREATORIGINALWRITERSLIKEBERKELEYORCOLERIDGETOPLATOANDHISIDEASTHATTHEREISATRUTHHIGHERTHANEXPERIENCEOFWHICHTHEMINDBEARSWITNESSTOHERSELFISACONVICTIONWHICHINOUROWNGENERATIONHASBEENENTHUSIASTICALLYASSERTEDANDISPERHAPSGAININGGROUNDOFTHEGREEKAUTHORSWHOATTHERENAISSANCEBROUGHTANEWLIFEINTOTHEWORLDPLATOHASHADTHEGREATESTINFLUENCETHEREPUBLICOFPLATOISALSOTHEFIRSTTREATISEUPONEDUCATIONOFWHICHTHEWRITINGSOFMILTONANDLOCKEROUSSEAUJEANPAULANDGOETHEARETHELEGITIMATEDESCENDANTSLIKEDANTEORBUNYANHEHASAREVELATIONOFANOTHERLIFELIKEBACONHEISPROFOUNDLYIMPRESSEDWITHTHEUNITYOFKNOWLEDGEINTHEEARLYCHURCHHEEXERCISEDAREALINFLUENCEONTHEOLOGYANDATTHEREVIVALOFLITERATUREONPOLITICSEVENTHEFRAGMENTSOFHISWORDSWHENREPEATEDATSECONDHANDSYMPHAVEINALLAGESRAVISHEDTHEHEARTSOFMENWHOHAVESEENREFLECTEDINTHEMTHEIROWNHIGHERNATUREHEISTHEFATHEROFIDEALISMINPHILOSOPHYINPOLITICSINLITERATUREANDMANYOFTHELATESTCONCEPTIONSOFMODERNTHINKERSANDSTATESMENSUCHASTHEUNITYOFKNOWLEDGETHEREIGNOFLAWANDTHEEQUALITYOFTHESEXESHAVEBEENANTICIPATEDINADREAMBYHIMTHEARGUMENTOFTHEREPUBLICISTHESEARCHAFTERJUSTICETHENATUREOFWHICHISFIRSTHINTEDATBYCEPHALUSTHEJUSTANDBLAMELESSOLDMANTHENDISCUSSEDONTHEBASISOFPROVERBIALMORALITYBYSOCRATESANDPOLEMARCHUSTHENCARICATUREDBYTHRASYMACHUSANDPARTIALLYEXPLAINEDBYSOCRATESREDUCEDTOANABSTRACTIONBYGLAUCONANDADEIMANTUSANDHAVINGBECOMEINVISIBLEINTHEINDIVIDUALREAPPEARSATLENGTHINTHEIDEALSTATEWHICHISCONSTRUCTEDBYSOCRATESTHEFIRSTCAREOFTHERULERSISTOBEEDUCATIONOFWHICHANOUTLINEISDRAWNAFTERTHEOLDHELLENICMODELPROVIDINGONLYFORANIMPROVEDRELIGIONANDMORALITYANDMORESIMPLICITYINMUSICANDGYMNASTICAMANLIERSTRAINOFPOETRYANDGREATERHARMONYOFTHEINDIVIDUALANDTHESTATEWEARETHUSLEDONTOTHECONCEPTIONOFAHIGHERSTATEINWHICHNOMANCALLSANYTHINGHISOWNANDINWHICHTHEREISNEITHERMARRYINGNORGIVINGINMARRIAGEANDKINGSAREPHILOSOPHERSANDPHILOSOPHERSAREKINGSANDTHEREISANOTHERANDHIGHEREDUCATIONINTELLECTUALASWELLASMORALANDRELIGIOUSOFSCIENCEASWELLASOFARTANDNOTOFYOUTHONLYBUTOFTHEWHOLEOFLIFESUCHASTATEISHARDLYTOBEREALIZEDINTHISWORLDANDQUICKLYDEGENERATESTOTHEPERFECTIDEALSUCCEEDSTHEGOVERNMENTOFTHESOLDIERANDTHELOVEROFHONOURTHISAGAINDECLININGINTODEMOCRACYANDDEMOCRACYINTOTYRANNYINANIMAGINARYBUTREGULARORDERHAVINGNOTMUCHRESEMBLANCETOTHEACTUALFACTSWHENTHEWHEELHASCOMEFULLCIRCLEWEDONOTBEGINAGAINWITHANEWPERIODOFHUMANLIFEBUTWEHAVEPASSEDFROMTHEBESTTOTHEWORSTANDTHEREWEENDTHESUBJECTISTHENCHANGEDANDTHEOLDQUARRELOFPOETRYANDPHILOSOPHYWHICHHADBEENMORELIGHTLYTREATEDINTHEEARLIERBOOKSOFTHEREPUBLICISNOWRESUMEDANDFOUGHTOUTTOACONCLUSIONPOETRYISDISCOVEREDTOBEANIMITATIONTHRICEREMOVEDFROMTHETRUTHANDHOMERASWELLASTHEDRAMATICPOETSHAVINGBEENCONDEMNEDASANIMITATORISSENTINTOBANISHMENTALONGWITHTHEMANDTHEIDEAOFTHESTATEISSUPPLEMENTEDBYTHEREVELATIONOFAFUTURELIFETHEDIVISIONINTOBOOKSLIKEALLSIMILARDIVISIONSCPSIRGCLEWISINTHECLASSICALMUSEUMISPROBABLYLATERTHANTHEAGEOFPLATOTHENATURALDIVISIONSAREFIVEINNUMBERBOOKIANDTHEFIRSTHALFOFBOOKIIDOWNTOTHEPARAGRAPHBEGINNINGIHADALWAYSADMIREDTHEGENIUSOFGLAUCONANDADEIMANTUSWHICHISINTRODUCTORYTHEFIRSTBOOKCONTAININGAREFUTATIONOFTHEPOPULARANDSOPHISTICALNOTIONSOFJUSTICEANDCONCLUDINGLIKESOMEOFTHEEARLIERDIALOGUESWITHOUTARRIVINGATANYDEFINITERESULTTOTHISISAPPENDEDARESTATEMENTOFTHENATUREOFJUSTICEACCORDINGTOCOMMONOPINIONANDANANSWERISDEMANDEDTOTHEQUESTIONWHATISJUSTICESTRIPPEDOFAPPEARANCESTHESECONDDIVISIONINCLUDESTHEREMAINDEROFTHESECONDANDTHEWHOLEOFTHETHIRDANDFOURTHBOOKSWHICHAREMAINLYOCCUPIEDWITHTHECONSTRUCTIONOFTHEFIRSTSTATEANDTHEFIRSTEDUCATIONTHETHIRDDIVISIONCONSISTSOFTHEFIFTHSIXTHANDSEVENTHBOOKSINWHICHPHILOSOPHYRATHERTHANJUSTICEISTHESUBJECTOFENQUIRYANDTHESECONDSTATEISCONSTRUCTEDONPRINCIPLESOFCOMMUNISMANDRULEDBYPHILOSOPHERSANDTHECONTEMPLATIONOFTHEIDEAOFGOODTAKESTHEPLACEOFTHESOCIALANDPOLITICALVIRTUESINTHEEIGHTHANDNINTHBOOKSTHEPERVERSIONSOFSTATESANDOFTHEINDIVIDUALSWHOCORRESPONDTOTHEMAREREVIEWEDINSUCCESSIONANDTHENATUREOFPLEASUREANDTHEPRINCIPLEOFTYRANNYAREFURTHERANALYSEDINTHEINDIVIDUALMANTHETENTHBOOKISTHECONCLUSIONOFTHEWHOLEINWHICHTHERELATIONSOFPHILOSOPHYTOPOETRYAREFINALLYDETERMINEDANDTHEHAPPINESSOFTHECITIZENSINTHISLIFEWHICHHASNOWBEENASSUREDISCROWNEDBYTHEVISIONOFANOTHERORAMOREGENERALDIVISIONINTOTWOPARTSMAYBEADOPTEDTHEFIRSTBOOKSIIVCONTAININGTHEDESCRIPTIONOFASTATEFRAMEDGENERALLYINACCORDANCEWITHHELLENICNOTIONSOFRELIGIONANDMORALITYWHILEINTHESECONDBOOKSVXTHEHELLENICSTATEISTRANSFORMEDINTOANIDEALKINGDOMOFPHILOSOPHYOFWHICHALLOTHERGOVERNMENTSARETHEPERVERSIONSTHESETWOPOINTSOFVIEWAREREALLYOPPOSEDANDTHEOPPOSITIONISONLYVEILEDBYTHEGENIUSOFPLATOTHEREPUBLICLIKETHEPHAEDRUSSEEINTRODUCTIONTOPHAEDRUSISANIMPERFECTWHOLETHEHIGHERLIGHTOFPHILOSOPHYBREAKSTHROUGHTHEREGULARITYOFTHEHELLENICTEMPLEWHICHATLASTFADESAWAYINTOTHEHEAVENSWHETHERTHISIMPERFECTIONOFSTRUCTUREARISESFROMANENLARGEMENTOFTHEPLANORFROMTHEIMPERFECTRECONCILEMENTINTHEWRITERSOWNMINDOFTHESTRUGGLINGELEMENTSOFTHOUGHTWHICHARENOWFIRSTBROUGHTTOGETHERBYHIMORPERHAPSFROMTHECOMPOSITIONOFTHEWORKATDIFFERENTTIMESAREQUESTIONSLIKETHESIMILARQUESTIONABOUTTHEILIADANDTHEODYSSEYWHICHAREWORTHASKINGBUTWHICHCANNOTHAVEADISTINCTANSWERINTHEAGEOFPLATOTHEREWASNOREGULARMODEOFPUBLICATIONANDANAUTHORWOULDHAVETHELESSSCRUPLEINALTERINGORADDINGTOAWORKWHICHWASKNOWNONLYTOAFEWOFHISFRIENDSTHEREISNOABSURDITYINSUPPOSINGTHATHEMAYHAVELAIDHISLABOURSASIDEFORATIMEORTURNEDFROMONEWORKTOANOTHERANDSUCHINTERRUPTIONSWOULDBEMORELIKELYTOOCCURINTHECASEOFALONGTHANOFASHORTWRITINGINALLATTEMPTSTODETERMINETHECHRONOLOGICALORDEROFTHEPLATONICWRITINGSONINTERNALEVIDENCETHISUNCERTAINTYABOUTANYSINGLEDIALOGUEBEINGCOMPOSEDATONETIMEISADISTURBINGELEMENTWHICHMUSTBEADMITTEDTOAFFECTLONGERWORKSSUCHASTHEREPUBLICANDTHELAWSMORETHANSHORTERONESBUTONTHEOTHERHANDTHESEEMINGDISCREPANCIESOFTHEREPUBLICMAYONLYARISEOUTOFTHEDISCORDANTELEMENTSWHICHTHEPHILOSOPHERHASATTEMPTEDTOUNITEINASINGLEWHOLEPERHAPSWITHOUTBEINGHIMSELFABLETORECOGNISETHEINCONSISTENCYWHICHISOBVIOUSTOUSFORTHEREISAJUDGMENTOFAFTERAGESWHICHFEWGREATWRITERSHAVEEVERBEENABLETOANTICIPATEFORTHEMSELVESTHEYDONOTPERCEIVETHEWANTOFCONNEXIONINTHEIROWNWRITINGSORTHEGAPSINTHEIRSYSTEMSWHICHAREVISIBLEENOUGHTOTHOSEWHOCOMEAFTERTHEMINTHEBEGINNINGSOFLITERATUREANDPHILOSOPHYAMIDTHEFIRSTEFFORTSOFTHOUGHTANDLANGUAGEMOREINCONSISTENCIESOCCURTHANNOWWHENTHEPATHSOFSPECULATIONAREWELLWORNANDTHEMEANINGOFWORDSPRECISELYDEFINEDFORCONSISTENCYTOOISTHEGROWTHOFTIMEANDSOMEOFTHEGREATESTCREATIONSOFTHEHUMANMINDHAVEBEENWANTINGINUNITYTRIEDBYTHISTESTSEVERALOFTHEPLATONICDIALOGUESACCORDINGTOOURMODERNIDEASAPPEARTOBEDEFECTIVEBUTTHEDEFICIENCYISNOPROOFTHATTHEYWERECOMPOSEDATDIFFERENTTIMESORBYDIFFERENTHANDSANDTHESUPPOSITIONTHATTHEREPUBLICWASWRITTENUNINTERRUPTEDLYANDBYACONTINUOUSEFFORTISINSOMEDEGREECONFIRMEDBYTHENUMEROUSREFERENCESFROMONEPARTOFTHEWORKTOANOTHER",
                "this is a short test string to show that the cryptanalysis tool is unreliable with a short texts with different to normal distributions, and padded words (wqzfgqwzfgqzfwgqwzfgqfwzgqfwzgfqwzgfqzwgfqzgwfgq), it might sometimes choose the wrong keylength too"
        };

        String[] keys = {
                "plato",
                "securekey"
        };

        for (int i = 0; i < plaintexts.length; i++) {
            String ciphertext = VigenereCipher.encrypt(plaintexts[i], keys[i]);
            cryptanalysis.setCiphertext(ciphertext);
            cryptanalysis.cryptanalysis();
            cryptanalysis.decrypt();
            System.out.println("-----------------------------------------");
            System.out.printf("Actual Key: %s\nActual Key length: %d\nKey length: %d\nKey: %s\n", keys[i], keys[i].length(), cryptanalysis.key.length(), cryptanalysis.key);
//            System.out.printf("Plaintext:\n%s\n", cryptanalysis.plaintext);
            System.out.println("-----------------------------------------");
        }
    }
}
