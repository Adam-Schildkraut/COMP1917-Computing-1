
public class PixelColor {
	// FUNCTION:
	// When the point is definitely in the set (steps = 256) or definitely
	// out (steps = 1) then we want the pixel to be redIntensityck, but any number
	// of steps in between will have an increasing intensity until it
	// reaches 256, where it will go from bright white to redIntensityck to create
	// contrast.

	static char stepsToBlue (int steps){
	   char blueIntensity = 0;
	   if (steps == 256){
	      blueIntensity = 1;      // black for definitely in
	   } else if (steps < 16) {
	      blueIntensity = (char) (32 + (steps%192));
	   } else if (steps < 32) {
	      blueIntensity = (char) (32 + (steps%192));
	   } else if (steps < 64) {
	      blueIntensity = (char) (32 + (steps%192));
	   } else if (steps < 128) {
	      blueIntensity = (char) (32 + (steps%192));
	   } else {
	      blueIntensity = (char) (32 + (steps%192));
	   }
	   return blueIntensity;
	}

	static char stepsToRed (int steps){
	   char redIntensity = 1;
	   if (steps == 256){
	      redIntensity = 0;      // black for definitely in
	   } else if (steps < 16) {
	      redIntensity = (char) ((steps%192));
	   } else if (steps < 32) {
	      redIntensity = (char) ((steps%192));
	   } else if (steps < 64) {
	      redIntensity = (char) ((steps%192));
	   } else if (steps < 128) {
	      redIntensity = (char) ((steps%192));
	   } else {
	      redIntensity = (char) ((steps));
	   }
	   return redIntensity;
	}

	static char stepsToGreen (int steps){
	   char greenIntensity = 0;
	   if (steps == 256){
	      greenIntensity = 1;      // black for definitely in
	   } else if (steps < 16) {
	      greenIntensity = (char) (32 + (steps%192));
	   } else if (steps < 32) {
	      greenIntensity = (char) (32 + (steps%192));
	   } else if (steps < 64) {
	      greenIntensity = (char) (32 + (steps%192));
	   } else if (steps < 128) {
	      greenIntensity = (char) (32 + (steps%192));
	   } else {
	      greenIntensity = (char) ((steps)%192);
	   }
	   return greenIntensity;
	}
}
