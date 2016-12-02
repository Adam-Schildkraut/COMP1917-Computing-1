/*
 *  Mr Pass.  Brain the size of a planet!
 *
 *  Proundly Created by Richard Buckland
 *  Share Freely Creative Commons SA-BY-NC 3.0.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "Game.h"
#include "mechanicalTurk.h"

#define A_START ""
#define A_OPP "RLRLRLRLRLL"   // A's other campus opposite A_START
#define B_START "LRLRLRRLRL"  // The further B campus from A_START
#define B_OPP "RRLRL"         // B's other campus from B_START
#define C_START "RRLRLLRLRL"  // The further C campus from A_START
#define C_OPP "LRLRL"

static int hasSpinoffResource(Game g, int player);
static int hasArcResource(Game g, int player);
static int hasCampusResource(Game g, int player);

static int canRetrainForAction(Game g, int actionCode, action* outAction);
static int coordToPath(int x, int y, int dir, path buffer);
static int canSpamBuild(Game g, action* outAction);

// Static function to check whether we have the resources to start a spinoff
static int hasSpinoffResource(Game g, int player){
   int hasResource = (getStudents(g, player, STUDENT_MJ) > 0) &&
   (getStudents(g, player, STUDENT_MTV) > 0) &&
   (getStudents(g, player, STUDENT_MMONEY) > 0);
   return hasResource;
}

// Static function to check whether we have the resources to obtain arc
static int hasArcResource(Game g, int player){
   int hasResource = (getStudents(g, player, STUDENT_BQN) > 0) &&
   (getStudents(g, player, STUDENT_BPS) > 0);
   return hasResource;
}

// Static function to check whether we have the resources to obtain arc
static int hasCampusResource(Game g, int player){
   int hasResource = (getStudents(g, player, STUDENT_BQN) > 0) &&
   (getStudents(g, player, STUDENT_BPS) > 0) &&
   (getStudents(g, player, STUDENT_MJ) > 0) &&
   (getStudents(g, player, STUDENT_MTV) > 0);
   return hasResource;
}

action decideAction (Game g) {
   
   // This turk aims to just start a business spin-off when it has
   // the resources and pass otherwise
   
   // To start a business spinoff, you need 1 STUDENT_MJ ,
   // 1 STUDENT_MTV and 1 STUDENT_MMONEY
   
   // Also will aim to build 2 arcs at the start of the game and
   // a campus at a location that will give the best students
   // according to the following scores
   // THD = 0, BPS & BQN = 5, MJ = 3, MTV = 2, M$ = 1
   int studentScores[] = {0, 5, 5, 3, 2, 1};
   
   // Figuring out which player we are
   int player = getWhoseTurn(g);
   
   // Create action for starting spinoff or passing after the end
   action nextAction = {0,"",0,0};
   
   // Creating intelligence for making arcs for when starting the game
   {
      // Checking we have resources for an arc
      int canARC = hasArcResource(g, player);
      if (canARC == TRUE){
         nextAction.actionCode = OBTAIN_ARC;
         if (player == UNI_A){
            // setting a target path for that player to get to
            // a vertex with 3 regions
            path targetPath;
            
            // defining paths of the closest 3-region vertex that is
            // twp arcs away from starting campus
            path path1 = A_START "RL";
            path path2 = A_OPP "LL";
            
            // also specifying the region ID's at those vertices
            int path1Regions[] = {3, 7, 8};
            int path2Regions[] = {10, 11, 15};
            
            // initialize counter to cycle through the 3 regions at
            // the specified path closest to tripple region
            int a = 0;
            int path1Score = 0;
            int path2Score = 0;
            while (a < 3){
               int student1 = getDiscipline(g, path1Regions[a]);
               int student2 = getDiscipline(g, path2Regions[a]);
               
               path1Score += studentScores[student1];
               path2Score += studentScores[student2];
               a++;
            }
            
            int validPath;
            int i;
            // assign target path to be the path with the highest score
            if (path1Score >= path2Score){
               i = strlen(A_START);
               validPath = getCampus(g, A_START) == UNI_A || getARC(g, A_START) == UNI_A;
               strcpy(targetPath, path1);
            } else {
               i = strlen(A_OPP);
               validPath = getCampus(g, A_OPP) == UNI_A || getARC(g, A_OPP) == UNI_A;
               strcpy(targetPath, path2);
            }
            
            // check that the initial point of the path has a campus
            // of the current player
            
            // cycling through each character in targetPath cumulatively
            // with the previous characters to form sub strings to test
            // whether there are arcs along the sub string to allow
            // the next one to be legal
            while (i < strlen(targetPath) && validPath == TRUE){
               // building the substring
               sprintf(nextAction.destination,"%s",targetPath);
               nextAction.destination[i+1] = '\0';
               
               // checking whether the current substring is a legal path
               // with having arcs along it
               validPath = (getARC(g, nextAction.destination) == UNI_A);
               i++;
            }
         } else if (player == UNI_B){
            // setting a target path for that player to get to
            // a vertex with 3 regions
            path targetPath;
            
            // defining paths of the closest 3-region vertex that is
            // twp arcs away from starting campus
            path path1 = B_START "BL";
            path path2 = B_OPP "LL";
            
            // also specifying the region ID's at those vertices
            int path1Regions[] = {0, 1, 4};
            int path2Regions[] = {14, 17, 18};
            
            // initialize counter to cycle through the 3 regions at
            // the specified path closest to tripple region
            int a = 0;
            int path1Score = 0;
            int path2Score = 0;
            while (a < 3){
               int student1 = getDiscipline(g, path1Regions[a]);
               int student2 = getDiscipline(g, path2Regions[a]);
               
               path1Score += studentScores[student1];
               path2Score += studentScores[student2];
               a++;
            }
            
            int validPath;
            int i;
            // assign target path to be the path with the highest score
            if (path1Score >= path2Score){
               i = strlen(B_START);
               validPath = getCampus(g, B_START) == UNI_B || getARC(g, B_START) == UNI_B;
               strcpy(targetPath, path1);
            } else {
               i = strlen(B_OPP);
               validPath = getCampus(g, B_OPP) == UNI_B || getARC(g, B_OPP) == UNI_B;
               strcpy(targetPath, path2);
            }
            
            // check that the initial point of the path has a campus
            // of the current player
            
            // cycling through each character in targetPath cumulatively
            // with the previous characters to form sub strings to test
            // whether there are arcs along the sub string to allow
            // the next one to be legal
            while (i < strlen(targetPath) && validPath == TRUE){
               // building the substring
               sprintf(nextAction.destination,"%s",targetPath);
               nextAction.destination[i+1] = '\0';
               
               // checking whether the current substring is a legal path
               // with having arcs along it
               validPath = (getARC(g, nextAction.destination) == UNI_B);
               i++;
            }
         } else if (player == UNI_C) {
            // setting a target path for that player to get to
            // a vertex with 3 regions
            path targetPath;
            
            // defining paths of the closest 3-region vertex that is
            // twp arcs away from starting campus
            path path1 = C_START "LL";
            path path2 = C_OPP "BL";
            
            // also specifying the region ID's at those vertices
            int path1Regions[] = {2, 5, 6};
            int path2Regions[] = {12, 13, 16};
            
            // initialize counter to cycle through the 3 regions at
            // the specified path closest to tripple region
            int a = 0;
            int path1Score = 0;
            int path2Score = 0;
            while (a < 3){
               int student1 = getDiscipline(g, path1Regions[a]);
               int student2 = getDiscipline(g, path2Regions[a]);
               
               path1Score += studentScores[student1];
               path2Score += studentScores[student2];
               a++;
            }
            
            int validPath;
            int i;
            // assign target path to be the path with the highest score
            if (path1Score >= path2Score){
               i = strlen(C_START);
               validPath = getCampus(g, C_START) == UNI_C || getARC(g, C_START) == UNI_C;
               strcpy(targetPath, path1);
            } else {
               i = strlen(C_OPP);
               validPath = getCampus(g, C_OPP) == UNI_C || getARC(g, C_OPP) == UNI_C;
               strcpy(targetPath, path2);
            }
            
            // check that the initial point of the path has a campus
            // of the current player
            
            // cycling through each character in targetPath cumulatively
            // with the previous characters to form sub strings to test
            // whether there are arcs along the sub string to allow
            // the next one to be legal
            while (i < strlen(targetPath) && validPath == TRUE){
               // building the substring
               sprintf(nextAction.destination,"%s",targetPath);
               nextAction.destination[i+1] = '\0';
               
               // checking whether the current substring is a legal path
               // with having arcs along it
               validPath = (getARC(g, nextAction.destination) == UNI_C);
               i++;
            }
         }
      }
      //printf("%s\n",nextAction.destination);
      
   }
   
   
   // The next action is for starting a spinoff after trying to
   // make an arc or campus
   {
      // Checking we have the initial resources to start a spinoff
      int canSpinoff = hasSpinoffResource(g, player);
      
      // Starting a spinoff while we have the resources
      if(hasArcResource(g, player) && getARC(g, nextAction.destination) == NO_ONE) {
         nextAction.actionCode = OBTAIN_ARC;
      } else if(hasCampusResource(g, player) && getCampus(g, nextAction.destination) == NO_ONE) {
         nextAction.actionCode = BUILD_CAMPUS;
      } else if (canSpinoff) {
         nextAction.actionCode = START_SPINOFF;
      } else if (canSpamBuild(g,&nextAction)) {
         //Same as before.
      } else if (canRetrainForAction(g, START_SPINOFF, &nextAction) &&
                 (getCampuses(g, player)+getGO8s(g,player) > 3)) {
         //Don't need to do anything here? This will have already changed
         //nextAction's action code to retrain.
      } else if ((nextAction.disciplineTo == STUDENT_MJ) &&
                 (nextAction.disciplineFrom >= STUDENT_MTV)) {
         nextAction.actionCode = RETRAIN_STUDENTS;
      }  else {
         nextAction.actionCode = PASS;
      }
      if (!isLegalAction(g,nextAction)) {
         nextAction.actionCode = PASS;
      }
   }
   
   return nextAction;
}

//Takes in a game and an action code, and if you have enough students
//to perform this action strictly after retraining some, writes the next retrain
//action to outAction, and returns a truthy value. Otherwise, it will return
//0, but will still write to the otherwise unused retraining
//discipline values if and only if you can retrain to get closer to the goal
//but do not have enough students to make it there this turn, in case you
//want to try retraining to get closer anyway. It, however, will not write
//to the action code. It will not edit the path under any circumstance.

//Will return 0 either if you won't have enough or if you don't need to
//retrain. Truthy return values mean that outAction should be returned
//by decideAction, false means that you should not try to retrain for
//actionCode this turn.

//Will always retrain from best ratio first. This doesn't matter much but
//it was easy to write and I wrote it to do this before I considered whether
//it mattered. Oh well.
static int canRetrainForAction(Game g, int actionCode, action* outAction) {
   int costs[4][5] = {{1,1,1,1,0},{0,0,2,0,3},{1,1,0,0,0},{0,0,1,1,1}};
   
   if ((actionCode >= BUILD_CAMPUS) && (actionCode <= START_SPINOFF)) {
      int self = getWhoseTurn(g);
      int* thisCost = costs[actionCode-1];
      int ownedStudents[5];
      int sourceStudent = 0;
      int targetStudent = 0;
      
      //Total Deficit is number of total students short of the requirements
      //irrespective of which requirement, totalSurplus is the total number
      //that can be gained by retraining suprlus students to the requirements
      int totalDeficit = 0;
      int totalSurplus = 0;
      
      int bestRatio = INT_MAX;
      
      int i = 0;
      while (i < 5) {
         ownedStudents[i] = getStudents(g, self, i+STUDENT_BPS);
         //printf("Have %d of student %d\n",ownedStudents[i],i+1);
         if ((ownedStudents[i] < thisCost[i])) {
            totalDeficit += thisCost[i] - ownedStudents[i];
            if (!targetStudent) {
               //printf("Target set to %d!\n",i+1);
               targetStudent = i+STUDENT_BPS;
            }
         }
         ++i;
      }
      //printf("%d\n",targetStudent);
      if (targetStudent) {
         //Now reverse iterating, so that "risky" students have higher
         //priority for retraining.
         while (i > 0) {
            --i;
            int thisRatio = getExchangeRate(g, self, i+STUDENT_BPS, targetStudent);
            //printf("Ratio is %d for %d\n",thisRatio,i+1);
            if (ownedStudents[i] >= (thisCost[i] + thisRatio)) {
               totalSurplus += (ownedStudents[i] - thisCost[i]) / thisRatio;
               if (thisRatio < bestRatio) {
                  //printf("Source set to %d!\n",i+1);
                  sourceStudent = i+STUDENT_BPS;
                  bestRatio = thisRatio;
               }
            }
         }
         if (sourceStudent) {
            outAction->disciplineFrom = sourceStudent;
            outAction->disciplineTo = targetStudent;
            if (totalSurplus >= totalDeficit) {
               outAction->actionCode = RETRAIN_STUDENTS;
            } else {
               actionCode = 0;
            }
         } else {
            actionCode = 0;
         }
      } else {
         actionCode = 0;
      }
   } else {
      actionCode = 0;
   }
   return actionCode;
}

static int coordToPath(int x, int y, int dir, path buffer) {
   //In this function, the reverse of traverse path, the only valid dirs
   //are U, R, and X, to specify if you want the U or R edge of the coord,
   //or the vertex. Will return false if dir is invalid, or if X or Y
   //are outside of the valid play area, or if dir is.
   
   int valid = (y <= 10) && (y >= 0) && (x <= 5) && (x >= 0) &&
   ((x + y) > 1) && (((5-x) + y) > 1) && ((x + (10-y)) > 1) &&
   (((5-x) + (10-y)) > 1);
   if (valid) {
      buffer[0] = '\0';
      //Begin by approaching the correct Y coord.
      sprintf(buffer,"%s","RLRLRLRLRL");
      buffer[10-y] = '\0';
      
      int form = y%2;
      char* xExtensions[2][6] = {{"BLLR","BLL","","L",  "LLRR","LLRRL"},
         {"RRLL","R",  "","BRR","BRRL","BRRLLRR"}};
      
      char* uExtensions[2][6] = {{"R","B","B","L","B","L"},
         {"B","R","B","B","L","B"}};
      
      char* rExtensions[2][6] = {{"B",   "n/a","L",  "n/a","L","n/a"},
         {"n/a","B",   "n/a","L","n/a","n/a"}};
      
      strcat(buffer,xExtensions[form][x]);
      
      if (dir == 'R') {
         if (rExtensions[form][x][0] == 'n') {
            valid = 0;
         } else {
            strcat(buffer,rExtensions[form][x]);
         }
      } else if (dir == 'U') {
         strcat(buffer,uExtensions[form][x]);
         ++y;
         valid = valid && (y <= 10) && (y >= 0) && (x <= 5) && (x >= 0) &&
         ((x + y) > 1) && (((5-x) + y) > 1) && ((x + (10-y)) > 1) &&
         (((5-x) + (10-y)) > 1);
      } else if (dir != 'X') {
         valid = 0;
      }
   }
   return valid;
}
#define sqrMag(x,y) ((x)*(x)+(y)*(y))
static int canSpamBuild(Game g, action* outAction) {
   int whyNoMultipleReturnPaths = 0;
   int player = getWhoseTurn(g);
   
   if (hasCampusResource(g, player)) {
      //If a campus can be built anywhere, it's assumed it should be.
      action legalTest = { BUILD_CAMPUS, "", 0, 0 };
      int x = 0;
      while ((x <= 5) && !whyNoMultipleReturnPaths) {
         int y = 0;
         while ((y <= 10) && !whyNoMultipleReturnPaths) {
            if (coordToPath(x,y,'X',legalTest.destination) &&
                isLegalAction(g, legalTest)) {
               outAction->actionCode = BUILD_CAMPUS;
               strcpy(outAction->destination, legalTest.destination);
               whyNoMultipleReturnPaths = 1;
            }
            ++y;
         }
         ++x;
      }
      
      if (!whyNoMultipleReturnPaths) {
         //Current version will build as close to the middle as possible
         //Possible improvements exist but this will at least get some more
         //resources. If the desert's always in the middle though ISTG...
         //if (hasArcResource(g, player)) {
         action legalTest = { OBTAIN_ARC, "", 0, 0 };
         float lastX = 0;
         float lastY = 0;
         int x = 0;
         while (x <= 5) {
            int y = 0;
            while (y <= 10){
               if ((coordToPath(x,y,'U',legalTest.destination) &&
                    isLegalAction(g, legalTest))) {
                  //The only place in the entire function where a value
                  //being stored about whether something is already found
                  //is useful yet.
                  if ((!whyNoMultipleReturnPaths) ||
                      (sqrMag(x*2-5.f,y+0.5f-5.f) < sqrMag(lastX*2-5.f,lastY-5.f))) {
                     lastX = x;
                     lastY = y + 0.5f;
                     outAction->actionCode = OBTAIN_ARC;
                     strcpy(outAction->destination, legalTest.destination);
                     whyNoMultipleReturnPaths = 1;
                  }
               } else if ((coordToPath(x,y,'R',legalTest.destination) &&
                           isLegalAction(g, legalTest))) {
                  //The only place in the entire function where a value
                  //being stored about whether something is already found
                  //is useful yet.
                  if ((!whyNoMultipleReturnPaths) ||
                      (sqrMag((x+0.5f)*2-5.f,y-5.f) < sqrMag(lastX*2-5.f,lastY-5.f))) {
                     lastX = x+0.5f;
                     lastY = y;
                     outAction->actionCode = OBTAIN_ARC;
                     strcpy(outAction->destination, legalTest.destination);
                     whyNoMultipleReturnPaths = 1;
                  }
               }
               ++y;
            }
            ++x;
         }
         //} else if (canRetrainForAction(g, OBTAIN_ARC, outAction)) {
         //   whyNoMultipleReturnPaths = 1;
         //}
      }
   } else if (canRetrainForAction(g, BUILD_CAMPUS, outAction)) {
      whyNoMultipleReturnPaths = 1;
   }
   
   
   return whyNoMultipleReturnPaths;
}
