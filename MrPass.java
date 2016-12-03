
public class MrPass extends PlayGame {
	
	static action decideAction (Game g) {
		   
		// This turk aims to just start a business spin-off when it has
		// the resources and pass otherwise

		// To start a business spinoff, you need 1 STUDENT_MJ ,
		// 1 STUDENT_MTV and 1 STUDENT_MMONEY

		// Figuring out which player we are
		int player = getWhoseTurn(g);

		// Create action for starting spinoff or passing after the end
		int actionCode = -1;
		String path = new String();
		int disciplineFrom = 0;
		int disciplineTo = 0;

		// Checking we have the initial resources to start a spinoff
		boolean canSpinoff = hasSpinoffResource(g, player);

		// Starting a spinoff while we have the resources
		if (canSpinoff == true){
			actionCode = START_SPINOFF;
			//canSpinoff = hasSpinoffResource(g, player);
		} else {

			// Ending the turn with pass when we don't have enough resources
			// to start a spinoff for that turn
			actionCode = PASS;
		}
		
		action nextAction = new action(actionCode,path,disciplineFrom,disciplineTo);
		return nextAction;
	}

	// Static function to check whether we have the resources to start a spinoff
	static boolean hasSpinoffResource(Game g, int player){
		boolean hasResource = (getStudents(g, player, STUDENT_MJ) > 0) &&
				(getStudents(g, player, STUDENT_MTV) > 0) &&
				(getStudents(g, player, STUDENT_MMONEY) > 0);
		return hasResource;
	}
}
