// Author: Shawn Manuel
// Date: 3/4/16
// This is a Java program that aims to imitate the leap year
// program that I made in C, which asks the user to input a 
// and will print whether it is a leap year or not.

import java.util.Scanner;

public class LeapYear{
	
	// Define magic numbers for testing conditions of the
	// year being a leap year
	public static final int TEST1 = 4;
	public static final int TEST2 = 100;
	public static final int TEST3 = 400;
	
	// Main
	public static void main(String[] args){
      
      testLeapYear();
	
		// Ask the user for an input year
		System.out.println("Please enter a year");
		
		// Create a scanner object to take in input
		Scanner scannerYearInput = new Scanner(System.in);
		
		// Storing the next line as an integer
		int year = scannerYearInput.nextInt();
		
		// Verifying what was stored in year
		System.out.println("The year you entered is " + year);
		
		// Printing out whether it is a leap year or not
		if (isLeapYear(year) == true){
			System.out.println("It is a leap year");
		} else {
			System.out.println("It is not a leap year");
		}
		scannerYearInput.close();
	}
	
	// Function:
   // This function takes in the year as an input and checks whether it
   // is a leap year or not and then returns true or false accordingly
   //       - Input: year
	//   		- Output: isItLeapYear
	private static boolean isLeapYear(int year){
		
		// Define a variable to store the true/false result of the
		// year being a leap year
		boolean isItLeapYear;
		
		// First check if divisibly by 4 but not by 100, then 
		// a leap year
		if ( (year % TEST1) == 0 && (year % TEST2) != 0 ){
			isItLeapYear = true;
		// Otherwise if divisible by 400, then it is a leap year
		} else if ((year % TEST3) == 0 ){
			isItLeapYear = true;
		// If not, then it is not a leap year
		} else {
			isItLeapYear = false;
		}
			
		return isItLeapYear;
	}
   
   // Function to test that leap year works using known outcomes
   private static void testLeapYear(){
      assert isLeapYear(4000) == true;
      assert isLeapYear(4004) == true;
      assert isLeapYear(1999) == false;
      assert isLeapYear(1900) == false;
      assert isLeapYear(2000) == true;
      assert isLeapYear(1904) == true;
      assert isLeapYear(156837) == false;
      System.out.println("Test Works!\n");
   }
   
}
