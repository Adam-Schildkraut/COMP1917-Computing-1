// Author: Shawn Manuel
// Date: 8/5/16
// This program simulates the parsing activity from mandelbrot in Java

public class Parsing {
	public static final int DECIMAL_BASE = 10;
	
	public static void main(String[] args) {
		String message = "http://localhost:1917/tile_x1.25_y0.56_z8.bmp";
		triordinate data = extract (message);
		System.out.println("x, y and z are: " + data.x + ", " + data.y + " and " + data.z);

	}
	
	//Defining the struct triordinate as the (x,y) coordinates and zoom
	//z stored from the url
	static class triordinate{
		double x;
		double y;
		int z;
	}
	
	//Function 1
	//This function takes in a url to be used for calling a web page
	//for the mandelbrot server and converts the string of x,y and z
	//variables as the actual numbers in the struct variable field
	static triordinate extract (String message) {
		// Define the returned struct 'data' as triordinate type
		triordinate data = new triordinate();

		// Define counter i to use to cycle through the message
		int i = 0;
		int length = message.length();

		// Define check for whether the coordinate has been found
		boolean xFound = false;
		boolean yFound = false;
		boolean zFound = false;

		// Cycle through the url looking for x,y and z letters
		while (i < length){
		   
		   // If it finds an x or y, input the address to the string
		   // following that letter into the myAtoD function and store
		   // as x and y
		   if (message.charAt(i) == 'x' && xFound == false){
		      // Feeding in the address to the string after the x character
		      data.x = myAtoD(message, i+1);
		      xFound = true;
		   } else if (message.charAt(i) == 'y' && yFound == false){
		      data.y = myAtoD(message, i+1);
		      yFound = true;
		      // When it finds z, input the following string into myAtoI
		   } else if (message.charAt(i) == 'z' && zFound == false){
		      data.z = myAtoI(message, i+1);
		      zFound = true;
		   }
		   
		   i++;
		}
		return data;
	}
	
	//Function 2:
	//This function converts a string of a signed decimal number into
	//an double type by taking in a message and also the index of the
	//message with the first number to convert to an double
	static double myAtoD (String message, int index){
		// Set a variable as the double type of the number
		double myAtoD = 0;
	
		// Check whether the number is to be negative
		int isNegative = 0;
		if (message.charAt(index) == '-'){
		   isNegative = 1;
		   index++;
		}
	
		// Assume the string is initially is not a decimal
		int decimalCount = 0;
	
		while ((index) < message.length() &&
			 ((message.charAt(index) >= '0' && 
			   message.charAt(index) <= '9') || 
			   message.charAt(index) == '.')){
		   // Check if the string has reached a decimal point making
		   // the number following a
		   if (message.charAt(index) == '.'){
		      decimalCount = 1;
		      index++;
		   }
		   
		   // Two conditions for whether it's a decimal digit or not
		   if (decimalCount >= 1){
		      // Define the multiple of 10 to divide the number to
		      // achieve the next decimal place
		      double decimalPlace = myPow(DECIMAL_BASE,decimalCount);
		      // Adding the required decimal place
		      myAtoD = myAtoD + (message.charAt(index) - '0')/decimalPlace;
		      decimalCount++;
		   } else {
		      // If it is not a decimal, increase base of previous
		      // numbers by 10 and add the current integer
		      myAtoD = DECIMAL_BASE*myAtoD + (message.charAt(index) - '0');
		   }
		   
		   // Increment i
		   index++;
		}
	
		// Make number negative if there was a '-' sign at the start
		if (isNegative == 1){
		   myAtoD = 0 - myAtoD;
		}
		return myAtoD;
	}
	
	//Function 3:
	//This function converts a string of a signed integer number into
	//an int type by taking in a message and also the index of the
	//message with the first number to convert to an int
	static int myAtoI (String message, int index){
		// Set a variable as the long type of the number
		int myAtoI = 0;
			
		// Check whether the number is to be negative
		int isNegative = 0;
		if (message.charAt(index) == '-'){
		   isNegative = 1;
		   index++;
		}
	
		while ((index) < message.length() &&
			 ((message.charAt(index) >= '0') && 
			  (message.charAt(index) <= '9'))){
			
		   // Increase base of previous numbers by 10 and add
		   // the current integer
		   myAtoI = DECIMAL_BASE*myAtoI + (message.charAt(index) - '0');
		   
		   // Increment i
		   index++;
		}
	
		// Make number negative if there was a '-' sign at the start
		if (isNegative == 1){
		   myAtoI = 0 - myAtoI;
		}
	
		return myAtoI;
	}

	//FUNCTION 4:
	//THis function finds the result of raising an input 'base' to the power
	//of the input 'index'
	static double myPow(int base, int index){
		double myPow = 1;
		int i = 0;
		if (index > 0){
		   while (i < index){
		      myPow *= base;
		      i++;
		   }
		} else {
		   while (index < i){
		      myPow /= base;
		      index++;
		   }
		}
		return myPow;
	}

}

// What i needed to do
// instead of passing in the address of message, passed in the message itself 
// and the index of the message where x was found and then the myAtoD would start
// at that index in the message and extract the number until it reached a 
// non-number in the message
