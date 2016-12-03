// Author: Shawn Manuel
// Date: 13/4/16
// This program takes in an input of characters and counts the frequency
// of each letter of the alphabet in that phrase. Also returns the
// total number of characters inputted and the number/proportion of 
// letters

// After testing, we found that character is one more than it should be

import java.io.FileReader;
import java.io.IOException;

public class Frequency {

	public static final int ALPHABET_SIZE = 26;
	public static final int PERCENT = 100;
	public static final int END_OF_FILE = -1;

	// Start main
	public static void main (String[] args){
		
		// Defining the variables to count each character type
		int charCount = 0;
		int upperCaseCount = 0;
		int lowerCaseCount = 0;
		int letterCount = 0;
		int[] frequency = new int[ALPHABET_SIZE];
	   	
		// Setting up the file as an object and defining path
		FileReader file = null;
      	String directory = "/Users/Shawn/Dropbox/"
        				+ "UNSW Units - Y3 S1/COMP1917/"
        				+ "Java/Something Awesome/src/";
		String fileName = "TheHungerGames.txt";
		
		// Surround the process of reading from the file in a
		// try-catch so that there is a fallback error message
		// if the specified file is not found
		try {
			file = new FileReader(directory + fileName);
        	
			int c;
			// While the input character exists
			// (or until it reaches end of file)
			while((c = file.read()) != END_OF_FILE) {
				char inputChar = (char) c;
				
				// Total number of characters increases by 1
			  	charCount++;
			  	
			  	// If the input character is between 'a' and 'z', increase
			  	// lower counter by 1
			  	if (inputChar >= 'a' && inputChar <= 'z'){
			     	lowerCaseCount++;
			     	frequency[inputChar - 'a'] += 1;
			    
			  	// If the character is between 'A' and 'Z', increase upper case
			  	// counter by 1
			  	} else if (inputChar >= 'A' && inputChar <= 'Z'){       
			     	upperCaseCount++;
			     	frequency[inputChar - 'A'] += 1;
			  	}        
			}
		} catch (IOException e) {
			System.out.println("File Not Found");
		}
	   
      // Combining the totals of upper case and lower case numbers
      letterCount = lowerCaseCount + upperCaseCount;
	   
      // printing out the number of characters vs number of letters
      System.out.println("Number of characters is :" + charCount);
      System.out.println("Number of letters is :" + letterCount);
	   
      // printing out the frequency and proportion of each letter
      int i = 0;
      float frequencyFloat;
      float proportion;
      while (i < ALPHABET_SIZE){
         frequencyFloat = frequency[i];
         proportion = (frequencyFloat/letterCount)*PERCENT;
         System.out.println( (char)('a' + i) + ":" + "\t"
	      						+ frequency[i] + "\t" 
	      						+ proportion);
         i++;
      }
   }
}
   
