//Authors: Shawn Manuel
//Date: 2/5/16
//This program converts text written in Rot13 code to normal English

import java.util.Scanner;

public class Rot13 {

	public static final int ROT = 13;
	public static final int ESC = 27;	
	
	public static void main(String[] args) {

		testEncode ();
		//Defined text as the input character variable
		String text;

		//Initializing text value with a scanner for first input character
		System.out.println("Enter a string to rotate ");
		Scanner scanner = new Scanner(System.in);
		text = scanner.nextLine();
		System.out.print("\n");

		//Continue decoding/encoding for any number of text inputs
		for (int i = 0; i < text.length(); i++){
			char decoded = encode(text.charAt(i));;
			System.out.print(decoded);
		}
		// Java does not have a null character to show the end of
		// a string which c has, so we need to use text.length()
		// to determine how long the string is and use a for loop
		// to iterate through the string up to the length 
		
		scanner.close();
	}
	
	//Function to check whether the letter is a Lowercase or Uppercase
	
	static boolean isLowerCase (char letter){
		return (letter >= 'a') && (letter <= 'z');
	}

	static boolean isUpperCase (char letter){
		return (letter >= 'A') && (letter <= 'Z');
	}
	
	//The actual decoding or encoding function
	static char encode(char cipherText){
		char plainText;
		boolean lowerCaseToBeEncoded = isLowerCase(cipherText);
		boolean upperCaseToBeEncoded = isUpperCase(cipherText);
		//If the input is a lowercase or uppercase letter, ROT(13) is taken
		//away, but if the decoded letter is before 'a' or 'A' respectively,
		//then 2*ROT is added to bring it back within the alphabet range.
		if (lowerCaseToBeEncoded){
		   plainText = (char) (cipherText - ROT);
		   if (plainText < 'a'){
		      plainText += 2*ROT;
		   }
		} else if (upperCaseToBeEncoded){
		   plainText = (char) (cipherText - ROT);
		   if (plainText < 'A'){
		      plainText += 2*ROT;
		   }
		} else {
		   plainText = cipherText;
		}
	
		return plainText;
	}

	//Asserts that the encoding/decoding works
	static void testEncode(){
		assert (encode ('a') == 'n');
		assert (encode ('n') == 'a');
		assert (encode ('A') == 'N');
		assert (encode ('N') == 'A');
		assert (encode ('m') == 'z');
		assert (encode ('z') == 'm');
		assert (encode ('M') == 'Z');
		assert (encode ('Z') == 'M');

		System.out.println("All tests passed. You are Awesome!");
	}	
}



