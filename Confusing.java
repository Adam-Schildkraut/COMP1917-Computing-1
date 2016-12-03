/*
Test if a point is inside a triangle.
Julian Saknussemm

Given Three points of a triangle, and another arbitrary point
this program determines if that point lies inside the triangle.

This is determined by satisfying the following rule:
A point P(x,y) is inside triangle A(x0,y0), B(x1,y1), C(x2,y2) if:
- P is on the same side of the line AB as C
- P is on the same side of the line BC as A
- P is on the same side of the line AC as B

A special case exits for a vertical line (inf gradient) when
testing the side of the line
*/

import java.util.Scanner;

public class Confusing {
	public static final int TRUE = 1;
	public static final int FALSE = 0;
	
	// Main
	public static void main(String[] args) {
		double x0,y0,x1,y1,x2,y2,px,py;
		int CheckInputs = 0;
		Scanner scanner = new Scanner(System.in);
		// CheckInputs tracks the cumulative number of inputs from the scanf
		// function below to test whether there are 8 inputs
		     
		// Get input of the coordinates of the three vertices of the triangle
		System.out.print("Enter Triangle Vertex A (x,y): "); 
		x0 = scanner.nextDouble(); CheckInputs++;
		y0 = scanner.nextDouble(); CheckInputs++;
		   
		System.out.print("Enter Triangle Vertex B (x,y): "); 
		x1 = scanner.nextDouble(); CheckInputs++;
		y1 = scanner.nextDouble(); CheckInputs++;
		   
		System.out.print("Enter Triangle Vertex C (x,y): "); 
		x2 = scanner.nextDouble(); CheckInputs++;
		y2 = scanner.nextDouble(); CheckInputs++;

		// Get the coordinates of the test point
		System.out.print("Enter Test Point (x,y): "); 
		px = scanner.nextDouble(); CheckInputs++;
		py = scanner.nextDouble(); CheckInputs++;
		
		scanner.close();
		   
		if( CheckInputs != 8 ) {
		    // print error
			System.out.println("Please enter the correctly formatted inputs!\n");
		} else {
			// print answer
			boolean InsideTriangle = test3(x0,y0,x1,y1,x2,y2,px,py);
		    if (InsideTriangle == true) {
		    	System.out.println("Point (" + px + "," + py + ") is inside the triangle");
		    } else { 
		    	System.out.println("Point (" + px + "," + py + ") is outside the triangle");
		    }
	    }

	}
	
	// FUNCTION 1
	// Check that the two points (px,py) and (lx,ly) are on the
	// same side of the line with gradient m and intercept b
	static boolean test1( double px, double py, double m,
		        double b, double lx, double ly ) {
		
		// two points lie on the same side of a line
	   boolean sameSide = test2( px, py, m, b ) == test2( lx, ly, m, b );
	   
	   return sameSide;
	}

	// FUNCTION 2
	// Checks the position of the test point (px,py) relative to the line
	// with gradient m and intercept b.
	// Classifies whether it is over, under or on the line.
	static int test2( double px, double py, double m, double b ) {
	   int position;

	   if ( py < m * px + b ) {
	      position = -1; 	// point is under the line
	   } else if ( py == m * px + b ) {
	      position = 0; 	   // point is on the line
	   } else {
	      position = 1; 	   // point is over the line
	   }
	   return position;
	}

	// FUNCTION 3
	// Creates a line from the 3 combination of 2 points and uses test2 
	// function to check whether the test point and the third point are
	// on the same side of the line of the first 2 points
	static boolean test3(double x0, double y0, double x1, double y1, 
	          double x2, double y2, double px, double py){

	   // There are 3 points (0, 1 and 2) so m01 would be referring to the
	   // gradient of the line between points 0 and 1
	   double m01 = (y1-y0)/(x1-x0);
	   double m02 = (y2-y0)/(x2-x0);
	   double m12 = (y2-y1)/(x2-x1);
	   
	   // b is the y-intercept and found to be y when substituting x=0 into:
	   // y - y1 = m( x - x1 )
	   // Therefore y-int = m(0-x1) + y1
	   double b01 = m01 * (0-x1) + y1;
	   double b02 = m02 * (0-x2) + y2;
	   double b12 = m12 * (0-x2) + y2;

	   
	   // Defining boundaries of the triangle
	   // These variables are 1 if the test point and the third point are 
	   // on the same side of the line
	   boolean line1, line2, line3;
	   
	   // vertical line checks
	   if( x0 == x1 ) {
	      // Check if the input x-value (px) and the third x-value (x2) are
	      // on the same side of the vertical line (x0=x1)
	      line1 = ( (px <= x0) == (x2 <= x0) );
	   } else {
	      // Run it normally in test 1
	      line1 = test1(px, py, m01, b01, x2, y2);
	   }
	 
	   // Repeating first check for line between point 1 and point 2
	   if( x1 == x2 ) {
	      line2 = ( (px <= x2) == (x0 <= x2) );
	   } else {
	      line2 = test1(px, py, m12, b12, x0, y0);
	   }

	   // Repeating first check for line between point 2 and point 0
	   if( x2 == x0 ) {
	      line3 = ( (px <= x0 ) == (x1 <= x0) );
	   } else { 
	      line3 = test1(px, py, m02, b02, x1, y1);
	   }

	   // Point is inside the line when all line boundary checks are true
	   boolean InsideTriangle = (line1 == true) && (line2 == true) && (line3 == true);

	   return InsideTriangle;

	}

}
