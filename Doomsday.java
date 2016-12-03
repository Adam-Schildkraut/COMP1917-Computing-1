// Author: Shawn Manuel
// Date: 10/4/16
// This program aims to calculate the day of the week for a given input Date
// depending on the doomsday of that year and whether it is a leap year

// import java.util.Scanner;

public class Doomsday {

	public static final int DAYS_PER_WEEK = 7;
	public static final int TRUE = 1;
	public static final int FALSE = 0;
	
	// Define start of array at 1 since Jan corresponds to month = 1
	public static final int ARRAY_START = 1;
	
	// Assigning numerical numbers to represent days of the week
	public static final int THURSDAY = 0;
	public static final int FRIDAY = 1;
	public static final int SATURDAY = 2;
	public static final int SUNDAY = 3;
	public static final int MONDAY = 4;
	public static final int TUESDAY = 5;
	public static final int WEDNESDAY = 6;
	
	// Defines the Doomsday date reference for each month
	public static final int DD_JAN = 3;
	public static final int DD_FEB = 0;
	public static final int LEAP_YEAR_ADJUST = 1;   // LEAP_YEAR_ADJUST adjusts the 
	public static final int DD_MAR = 0;				// leap year ref for JAN and FEB 
	public static final int DD_APR = 4;				// if it is aleap year
	public static final int DD_MAY = 9;
	public static final int DD_JUN = 6;
	public static final int DD_JUL = 11;
	public static final int DD_AUG = 8;
	public static final int DD_SEP = 5;
	public static final int DD_OCT = 10;
	public static final int DD_NOV = 7;
	public static final int DD_DEC = 12;
	
	public static void main (String[] args) {
		testDoomsday();
	}
	
	// Function 1
	// This function calculates the number corresponding to the day of the week 
	// given the day and month and what the doomsday of that year is
   
	public static int dayOfWeek (int doomsday, int leapYear, int month, int day) {
		
		// Define an array that holds the doomsday reference for each month
		int[] doomsdayRef = {DD_JAN, DD_FEB, DD_MAR, DD_APR, DD_MAY, DD_JUN,
							 DD_JUL, DD_AUG, DD_SEP, DD_OCT, DD_NOV, DD_DEC};
							      
		// Adjust doomsday reference only for Jan or Feb (month 1 or 2) if it is
		// a leap year.
		int leapYearAdjust = 0;
		if (month == 1 || month == 2) {
			leapYearAdjust = LEAP_YEAR_ADJUST*leapYear;
		}							 
		
		// Find the number of days past the doomsday as well as adjusting for leap year
		int dayDifference = day - (doomsdayRef[month - ARRAY_START] + leapYearAdjust);
		
		// If day is less than doomsday reference, make this a positive number between 
		// 0 and 7 because mod only works on positive numbers
		while (dayDifference < 0) {
			dayDifference += DAYS_PER_WEEK;
		}
		
		// Combine all the 'days difference' information starting from the year's
		// doomsday and adding the number of days past the doomsday reference 
		// Then modded by 7 to get a day of the week corresponding to the defined 
		// day of the week.
		int weekDay = (doomsday + dayDifference) % DAYS_PER_WEEK;	 
		
		return weekDay;
	}
	
	static void testDoomsday(){
		System.out.println("Testing the program based on all the assertions");
		
		assert(dayOfWeek (THURSDAY,  FALSE,  4,  4) == THURSDAY);
		assert(dayOfWeek (FRIDAY,    FALSE,  6,  6) == FRIDAY);
		assert(dayOfWeek (MONDAY,    FALSE,  8,  8) == MONDAY);
		assert(dayOfWeek (WEDNESDAY, FALSE, 10, 10) == WEDNESDAY);
		assert(dayOfWeek (FRIDAY,    FALSE, 12, 12) == FRIDAY);
		assert(dayOfWeek (THURSDAY,  FALSE,  9,  5) == THURSDAY);
		assert(dayOfWeek (FRIDAY,    FALSE,  5,  9) == FRIDAY);
		assert(dayOfWeek (SUNDAY,    FALSE,  7, 11) == SUNDAY);
		assert(dayOfWeek (TUESDAY,   FALSE, 11,  7) == TUESDAY);
		assert(dayOfWeek (WEDNESDAY, FALSE,  3,  7) == WEDNESDAY);
		assert(dayOfWeek (THURSDAY,  FALSE,  4,  5) == FRIDAY);
		assert(dayOfWeek (SATURDAY,  FALSE,  6,  5) == FRIDAY);
		assert(dayOfWeek (MONDAY,    FALSE,  8,  9) == TUESDAY);
		assert(dayOfWeek (WEDNESDAY, FALSE, 10,  9) == TUESDAY);
		assert(dayOfWeek (FRIDAY,    FALSE, 12, 20) == SATURDAY);
		assert(dayOfWeek (THURSDAY,  FALSE,  9,  9) == MONDAY);
		assert(dayOfWeek (FRIDAY,    FALSE,  5,  5) == MONDAY);
		assert(dayOfWeek (SUNDAY,    FALSE,  7,  7) == WEDNESDAY);	
		assert(dayOfWeek (TUESDAY,   FALSE, 11, 11) == SATURDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  3, 30) == SATURDAY);
	   	assert(dayOfWeek (TUESDAY,   FALSE,  2, 28) == TUESDAY);	
	   	assert(dayOfWeek (TUESDAY,   FALSE,  2, 27) == MONDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  1,  3) == THURSDAY);
	   	assert(dayOfWeek (MONDAY, 	  TRUE,	 1,  4) == MONDAY);
	   	assert(dayOfWeek (MONDAY,	  TRUE,	 2,  8) == MONDAY);
	   	assert(dayOfWeek (MONDAY,	  TRUE,	 3,  7) == MONDAY);
	   	assert(dayOfWeek (WEDNESDAY, FALSE,  4, 30) == MONDAY);
	   	assert(dayOfWeek (WEDNESDAY, FALSE,  4,  1) == SUNDAY);
	   	assert(dayOfWeek (MONDAY,	  FALSE,  7, 26) == TUESDAY);
	   	assert(dayOfWeek (WEDNESDAY, FALSE,  11,30) == FRIDAY);
	   	assert(dayOfWeek (FRIDAY,	  FALSE,  5, 29) == THURSDAY);
	   	assert(dayOfWeek (FRIDAY,	  FALSE,  5,  1) == THURSDAY);
	   	assert(dayOfWeek (SUNDAY,	  FALSE,  5,  4) == TUESDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  2, 10) == SUNDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  1, 26) == SATURDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  3, 29) == FRIDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  4, 25) == THURSDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  11,28) == THURSDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  12,25) == WEDNESDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  11,11) == MONDAY);
	   	assert(dayOfWeek (WEDNESDAY, TRUE,   1, 23) == MONDAY);
	   	assert(dayOfWeek (WEDNESDAY, TRUE,   1,  1) == SUNDAY);
	   	assert(dayOfWeek (WEDNESDAY, TRUE,   4,  6) == FRIDAY);
	   	assert(dayOfWeek (WEDNESDAY, TRUE,   4,  7) == SATURDAY);
	   	assert(dayOfWeek (WEDNESDAY, TRUE,   7, 20) == FRIDAY);
	   	assert(dayOfWeek (WEDNESDAY, TRUE,   11, 6) == TUESDAY);
	   	assert(dayOfWeek (THURSDAY,  TRUE,   2, 28) == WEDNESDAY);
	   	assert(dayOfWeek (THURSDAY,  FALSE,  2, 28) == THURSDAY);
	   	
	   	System.out.println("All tests passed! You are awesome!");
	}
}































