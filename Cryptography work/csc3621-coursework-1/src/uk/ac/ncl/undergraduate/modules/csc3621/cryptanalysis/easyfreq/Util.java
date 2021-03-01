package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Scanner;

/**
 * This class contains generic methods that can be used throughout the
 * implementation in all contexts.
 *
 * @author Changyu Dong
 * @author Roberto Metere
 * @author Zameer Parvez
 */
public class Util {

    /**
     * Scanner wrapping standard input.
     *
     * This scanner shall not be closed (it closes automatically); if you close
     * it accidentally, or because of IDE complaint, you will close System.in as
     * well, and you do not probably want to do that).
     */
    static final Scanner reader = new Scanner(System.in);

    /**
     * Integer value for 'A', used as an offset to convert between characters
     * and indexes.
     *
     * We assume the convention that A, B, ..., Z correspond to 0, 1, ..., 25
     * respectively.
     *
     * 'A' is the decimal constant 65 (0x41).
     */
    public static final int OFFSET = 'A';

    /**
     * This method reads the whole content of a file in a String. It does not
     * support large files (they will likely work slowly).
     *
     * @param path the file path to read from
     * @return the content of the file
     * @throws IOException something goes wrong creating the file and/or reading
     * from it
     */
    public static String readFileToBuffer(String path) throws IOException {
        byte[] encoded;

        encoded = Files.readAllBytes(Paths.get(path));
        return new String(encoded);
    }

    /**
     * Print a string to file.
     *
     * @param buffer the content to write into the file
     * @param path the file path
     * @throws IOException something goes wrong creating the file and/or writing
     */
    public static void printBufferToFile(String buffer, String path) throws IOException {
        PrintWriter writer;

        writer = new PrintWriter(path, "UTF-8");
        writer.print(buffer);
        writer.close();
    }

    /**
     * You can use this method to find the index in the frequency table of the
     * given character.
     *
     * It assumes the convention that A, B, ..., Z correspond to 0, 1, ..., 25
     * respectively.
     *
     * @param letter the letter to convert
     * @return the index corresponding to the given letter
     */
    public static int charToIndex(char letter) {
        if (!isValidLetter(letter)) {
            throw new IllegalArgumentException("Input charater must be an alphabetic character");
        }

        letter = Character.toUpperCase(letter);

        return letter - OFFSET;
    }

    /**
     * You can use this method to find the character given an index of the
     * frequency table.
     *
     * It assumes the convention that A, B, ..., Z correspond to 0, 1, ..., 25
     * respectively.
     *
     * @param index the index to convert
     * @return the character corresponding to the given index
     */
    public static char indexToChar(int index) {
        if (index < 0 || index > 25) {
            throw new IllegalArgumentException("Input index must be between 0 and 25");
        }

        return (char) (index + OFFSET);
    }

    /**
     * A letter is valid if it belongs to the English alphabet.
     *
     * @param letter the letter to check
     * @return <code>true</code> if the letter belongs to the English alphabet.
     */
    public static boolean isValidLetter(char letter) {
        return Character.toString(letter).matches("[A-Za-z]");
    }

}
