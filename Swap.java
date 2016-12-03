//Authors: Shawn Manuel
//Date: 02/5/16
//This program inputs two integers as x and y and swaps the values
//stored in each variable.

import java.util.Scanner;

public class Swap {

	public static void main(String[] args) {
		IntAddress x = new IntAddress();
		IntAddress y = new IntAddress();
		
		//This takes in the input of x and y
		System.out.println("Enter values for X and Y "); 
		Scanner scanner = new Scanner(System.in);
		x.value = scanner.nextInt();
		y.value = scanner.nextInt();
		   
		//This prints the current values of x and y
		System.out.println("X is currently " + x.value + ", and "
						 + "Y is currently " + y.value);
		
		// The actual swap
		System.out.println("Swapping variable now");
		swap (x,y);
		   
		//This prints the values of x and y after the swap
		System.out.println("X is currently " + x.value + ", and "
						 + "Y is currently " + y.value);
		scanner.close();
	}

	//This function swaps the values stored in the two inputs by assigning
	//the first value to a temporary variable and overwrites the first value
	//with the second value before overwriting the second value with the
	//first.
	static void swap (IntAddress first, IntAddress second){
		//Defines temporary value to store the first integer
		int temp = first.value;
		first.value = second.value;
		second.value = temp;
	}	
	
}

// A separate class to hold the address of the variable
class IntAddress {
	int value;
}
