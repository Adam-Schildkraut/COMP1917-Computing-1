// Author: Shawn Manuel
// Date: 8/5/16
// chessboard.java
// create a 512x512 PNG of a chessboard, with user specified size (in       // pixels)
// of the black and white squares on the board.  bottom right square must be white.

// Created by Richard Buckland on 14/04/11, edited 5/4/14
// Licensed under Creative Commons BY 3.0.
//

import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

public class Chessboard {
	public static final int SIZE = 512;
	
	public static void main(String[] args) {
		// create buffered image object img to store image information
		BufferedImage img = new BufferedImage(SIZE, SIZE, BufferedImage.TYPE_INT_ARGB);
		
		// output file object
		File f = null;
		String directory = "/Users/Shawn/Dropbox/"
        				+ "UNSW Units - Y3 S1/COMP1917/"
        				+ "Java/Something Awesome/src/";
		String fileName = "chess.png";
		
		// Getting the checker square size (in pixels) from the user
		System.out.println ("Enter square size (must be a factor of 512): ");
		Scanner scanner = new Scanner(System.in);
		int squareSize = scanner.nextInt();
		assert (SIZE % squareSize == 0);
		scanner.close();
		
		// create chessboard pixel by pixel
		int squaresPerRow = (SIZE/squareSize);
		int squaresPerCol = (SIZE/squareSize);
		int colourIntensity; 
		int i = 0;  // counter for squares per row
		int j = 0;  // counter for squares per column
		int x = 0;  // counter for pixels per row
		int y = 0;  // counter for pixels per column
		
		// The following table is used for displaying all iteraters for debugging
		// System.out.println("i  j  x  y");
		for (j = 0; j < squaresPerCol; j++) {
			for (i = 0; i < squaresPerRow; i++) {
				// y starts at the first pixel of every square in multiples 
				// of the length of the square in # of pixels
				for (y = j*squareSize; y < (j+1)*squareSize; y++) {
					for (x = i*squareSize; x < (i+1)*squareSize; x++) {
		                // Check the position of the square for whether
		                // to make it black if the i & j coordinates are
		                // both even/odd or white otherwise
		                if ((i+j)%2 == 0) {
		                   colourIntensity = 255;
		                } else {
		                   colourIntensity = 0;
		                }
		                
		                // Specifying the intensities of each r, g, b 
		                // property of a pixel as well as the brightness a
		                int a = 255;
						int r = colourIntensity;
						int g = colourIntensity;
						int b = colourIntensity;
						
						// Specifying bits of pixel
						int p = (a<<24) | (r<<16) | (g<<8) | b;
						
						img.setRGB(x, y, p);
		                
						// Following is for debugging
						// System.out.println(i + "  " + j + "  " + x + "  " + y);
						
		    		}
		    	} 
		    }
		}
						
		// writing image
		try {
			f = new File(directory + fileName);
			ImageIO.write(img, "png", f);
		} catch (IOException e) {
			System.out.println("Error: " + e);
		}
	}

}

