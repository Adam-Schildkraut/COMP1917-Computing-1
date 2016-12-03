import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class Mandelbrot extends Parsing{

	public static final int SET_LIMIT = 2;
	public static final int ESCAPE_STEPS_LIMIT = 256;
	public static final int PIXELS_DOWN = 512;
	public static final int PIXELS_ACROSS = 512;
	public static final int ZOOM_INDEX = 2;
	
	
	public static void main(String[] args) {
		
		//data.x = -0.7478;
		//data.y = 0.09;
		//data.z = 17;
		
		// Call the extract function
		String message = "http://localhost:1917/tile_x-0.7467_y0.09_z17.bmp";
		triordinate data = Parsing.extract(message);
		System.out.println("x, y and z are: " + data.x + ", " + data.y + " and " + data.z);
		
		// image dimensions
		int width = PIXELS_ACROSS;
		int height = PIXELS_DOWN;
		
		// create buffered image object img
		BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		
		// file object
		File f = null;
		String directory = "/Users/Shawn/Dropbox/" + "UNSW Units - Y3 S1/COMP1917/"
        				+ "Java/Something Awesome/src/";
		String fileName = "Mandelbrot.png";
		
		// Calculating pixel length as 2^(-zoom)
		int zoom = data.z;
		double pixelLength = myPow(ZOOM_INDEX,-zoom);
		   
		// x and y are the span of the cartesian grid that the image will be
		// mapped on
		double xRange = pixelLength*PIXELS_ACROSS;
		double yRange = pixelLength*PIXELS_DOWN;
		   
		// Define x and y to be the coordinates for a given pixel
		double x;
		double y;
		
		// create random image pixel by pixel
		for (int j = 0; j < height; j++){
			for (int i = 0; i < width; i++){
				
				// The graph will be centered around (dat.x,dat.y) and
	            // we need to have the pixel coordinate going from the
	            // centre of the pixel
	            x = data.x - xRange/2 + (i + 0.5)*pixelLength;
	            y = data.y + yRange/2 - (j + 0.5)*pixelLength;
	            
				int a = 255;
				int r = PixelColor.stepsToRed(escapeSteps(x,y));
				int g = PixelColor.stepsToGreen(escapeSteps(x,y));
				int b = PixelColor.stepsToBlue(escapeSteps(x,y));
				
				// Specifying bits of pixel
				int p = (a<<24) | (r<<16) | (g<<8) | b;
				
				img.setRGB(i, j, p);
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

	// FUNCTION 2:
	// This returns the number of steps it takes a given coordinate to
	// escape the mandelbrot set. The condition for the mandelbrot set is
	// the modulus of the coordinate (x + yi) < 2
	static int escapeSteps(double x, double y){
		// Defining the struct complex number for the coordinate c = (x,y)
		ComplexNo c = new ComplexNo();
		c.re = x;
		c.im = y;
		   
		// Defining the struct complex number for initial point P0 = (0,0)
		ComplexNo p = new ComplexNo();
		p.re = 0;
		p.im = 0;
		   
		// Initialising value for modP, the squre modulus of the point P
		double modP = (p.re*p.re) + (p.im*p.im);
		   
		// Initialising the number of steps to escape
		int steps = 0;
		   
		// Check if squared modulus is less than SET_LIMIT squared
		while (modP < SET_LIMIT*SET_LIMIT && steps < ESCAPE_STEPS_LIMIT){
		   p = nextPoint(p,c);
		   modP = (p.re*p.re) + (p.im*p.im);
		   steps++;
		}
		return steps;
		  
	}
	
	// FUNCTION 3:
	// Returns the complex number which is the next term of mandelbrot
	// formula when given the previous point and the coordinate c.
	// This is: p2 = (p1)^2 + c
	static ComplexNo nextPoint (ComplexNo p1, ComplexNo c) {
	   ComplexNo p2 = new ComplexNo();
	   p2.re = (p1.re*p1.re) - (p1.im*p1.im) + c.re;
	   p2.im = 2*p1.re*p1.im + c.im;
	   return p2;
	}
	
	// FUNCTION 4:
	// THis function finds the result of raising an input 'base' to the power
	// of the input 'index'
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

//Defining the struct triordinate as the (x,y) coordinates and zoom
//z stored from the url
class triordinate {
	double x;      // The x coordinate of the tile
	double y;      // The y coordinate of the tile
	int z;         // Zoom
}

//Defining the complex number data type to represent points in set.
//Define type name 'struct _ComplexNo' with a shorter name 'ComplexNo'
class ComplexNo {
	double re;	  // These are fields within the struct that correspond
	double im;	  // to the real and imaginary parts of a complex number
};
