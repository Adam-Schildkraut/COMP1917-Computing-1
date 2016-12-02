// runGame.c
// Shawn, Matthew, Michael, Zihan

// Link to todo list: http s://kobra.io/#/e/-KHUqiu_nU8zX-BMVJpq

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "Game.h"
#include "mechanicalTurk.h"

#define DEFAULT_DISCIPLINES { \
STUDENT_BQN, \
STUDENT_MMONEY, \
STUDENT_MJ, \
STUDENT_MMONEY, \
STUDENT_MJ, \
STUDENT_BPS, \
STUDENT_MTV, \
STUDENT_MTV, \
STUDENT_BPS, \
STUDENT_MTV, \
STUDENT_BQN, \
STUDENT_MJ, \
STUDENT_BQN, \
STUDENT_THD, \
STUDENT_MJ, \
STUDENT_MMONEY, \
STUDENT_MTV, \
STUDENT_BQN, \
STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}
#define STARTING_STUDENTS {0, 3, 3, 1, 1, 1}
#define SIDES_ON_DICE 6
#define MAX_KPI 150
#define WAIT_TIME (500*1000*1000)

int rollDice(int numRolls, int diceSides);
action getMove (Game g);
action getUserMove (Game g);
void printMap(Game g);
int coordToPath(int x, int y, int dir, path buffer);
void wait(int len);

int main (int argc, char *argv[]){
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   
   srand(time(NULL));
   Game g = newGame(disciplines, dice);
   
   int winner = NO_ONE;
   
   while (winner == NO_ONE){
      int diceValue = rollDice(2,SIDES_ON_DICE);
      
      throwDice(g, diceValue);
      printf("\nDice value %d thrown!\n", diceValue);
      
      int who = getWhoseTurn(g);
      printf("Turn %d, UNI %c\n",
             getTurnNumber(g), 'A' + (char) who - UNI_A);
      
      int turnOver = FALSE;
      while (turnOver == FALSE){
         printf("player %d is currently on %d points\n", who, getKPIpoints(g, who));
         action move = getMove(g);
         assert(isLegalAction(g,move));
         
         // Check for probabilities for spinoff outcomes
         // 1,2 -> patent
         // 3,4,5,6 -> publication
         if (move.actionCode == START_SPINOFF){
            if(rollDice(1, SIDES_ON_DICE) <= 2) {
               printf("Player %d got an IP Patent!\n", who);
               move.actionCode = OBTAIN_IP_PATENT;
            } else {
               printf("Player %d got a Publication!\n", who);
               move.actionCode = OBTAIN_PUBLICATION;
            }
         }
         
         makeAction(g, move);
         if(move.actionCode == PASS) {
            turnOver = TRUE;
         }
         if(getKPIpoints(g, getWhoseTurn(g)) >= MAX_KPI){
            winner = getWhoseTurn(g);
            turnOver = TRUE;
         }
      }
   }
   char winnerChar = 'A' + winner - UNI_A;
   printf("Uni %c won!\n", winnerChar);
   return EXIT_SUCCESS;
}

int rollDice(int numRolls, int diceSides) {
   int diceRollCount = 0;
   int diceValue = 0;
   
   while (diceRollCount < numRolls) {
      diceValue = diceValue + (rand() % diceSides + 1);
      diceRollCount++;
   }
   return diceValue;
}

action getMove (Game g){
   action move;
   if (getWhoseTurn(g) == UNI_B || getWhoseTurn(g) == UNI_C){
      move = decideAction(g);
      
      if (!isLegalAction(g, move)) {
         printf("Uni %c made an invalid action! It will pass!\n", (char)(getWhoseTurn(g) - 1 + 'A'));
         move.actionCode = PASS;
      }
      else {
         char *actionDisplay[] = {
            "passed",
            "built a campus",
            "built a Go8",
            "obtained an arc",
            "started a spinoff",
            "",
            "",
            "retrained students"
         };
         printf("Uni %c %s\n", (char)(getWhoseTurn(g) - 1 + 'A'), actionDisplay[move.actionCode]);
      }
      fflush(stdout);
      wait(WAIT_TIME);
   } else {
      move = getUserMove(g);
   }
   return move;
}

action getUserMove (Game g){
   action userAction;
   userAction.actionCode = -1;
   // getting action code from user
   printf("Enter action number: \n"
          "PASS 0 \n"
          "BUILD_CAMPUS 1 \n"
          "BUILD_GO8 2 \n"
          "OBTAIN_ARC 3 \n"
          "START_SPINOFF 4 \n"
          "OBTAIN_PUBLICATION 5 (can't do this) \n"
          "OBTAIN_IP_PATENT 6 (can't do this either) \n"
          "RETRAIN_STUDENTS 7 \n"
          "GET_STATS 8 \n"
          "CALL_AI 9 \n");
   
   scanf("%d", &userAction.actionCode);
   
   while (userAction.actionCode == 8){
      int who = getWhoseTurn(g);
      printf("#THDs   is %d \n", getStudents(g,who,STUDENT_THD));
      printf("#BPSs   is %d \n", getStudents(g,who,STUDENT_BPS));
      printf("#BQNs   is %d \n", getStudents(g,who,STUDENT_BQN));
      printf("#MJs    is %d \n", getStudents(g,who,STUDENT_MJ));
      printf("#MTVs   is %d \n", getStudents(g,who,STUDENT_MTV));
      printf("#M$s    is %d \n", getStudents(g,who,STUDENT_MMONEY));
      printf("\n");
      printf("#ARCs   is %d \n", getARCs(g, who));
      printf("#Campus is %d \n", getCampuses(g, who));
      printf("#GO8s   is %d \n", getGO8s(g, who));
      printf("#Pubs   is %d \n", getPublications(g, who));
      printf("#IPs    is %d \n", getIPs(g, who));
      printMap(g);
      scanf("%d", &userAction.actionCode);
   }
   
   if (userAction.actionCode == OBTAIN_ARC ||
       userAction.actionCode == BUILD_CAMPUS ||
       userAction.actionCode == BUILD_GO8) {
      // get user to specify path
      char path[PATH_LIMIT+1];
      printf("Enter path (Eg. LRLRRL): ");
      scanf("%s", path);
      assert(strlen(path) <= PATH_LIMIT);
      strcpy(userAction.destination,path);
   }
   
   if (userAction.actionCode == RETRAIN_STUDENTS){
      printf("Enter discipline to train from: ");
      scanf("%d",&userAction.disciplineFrom);
      printf("Enter discipline to train to: ");
      scanf("%d",&userAction.disciplineTo);
   }
   
   if (userAction.actionCode == 9) {
      userAction = decideAction(g);
   }
   
   if (!isLegalAction(g, userAction)){
      printf("You are not being legal! You shall pass!\n");
      userAction.actionCode = PASS;
   }
   
   return userAction;
}

int coordToPath(int x, int y, int dir, path buffer) {
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

void printMap(Game g) {
   char* campuses[] = {"\x1b[37mo\x1b[37m",
      "\x1b[31mo\x1b[37m","\x1b[32mo\x1b[37m","\x1b[34mo\x1b[37m",
      "\x1b[31mO\x1b[37m","\x1b[32mO","\x1b[34mO\x1b[37m"};
   char* hArcs[] = {"\x1b[37m---\x1b[37m",
      "\x1b[31m---\x1b[37m","\x1b[32m---\x1b[37m","\x1b[34m---\x1b[37m"};
   char* vArcs[] = {"\x1b[37m|\x1b[37m",
      "\x1b[31m|\x1b[37m","\x1b[32m|\x1b[37m","\x1b[34m|\x1b[37m"};
   int dicePos[][2] = {
      {0, 8}, {0, 6}, {0, 4},
      {1, 9}, {1, 7}, {1, 5}, {1, 3},
      {2, 10}, {2, 8}, {2, 6}, {2, 4}, {2, 2},
      {3, 9}, {3, 7}, {3, 5}, {3, 3},
      {4, 8}, {4, 6}, {4, 4},
   };
   char *discDisplay[] = {"THD", "BPS", "BQN", "MJ ", "MTV", "M$ "};
   
   int x;
   int y = 10;
   path buffer;
   while (y >= 0) {
      x = 0;
      while (x <= 5) {
         if (coordToPath(x,y,'X',buffer)) {
            printf("%s",campuses[getCampus(g,buffer)]);
         } else {
            printf(" ");
         }
         if (coordToPath(x,y,'R',buffer)) {
            printf("%s",hArcs[getARC(g,buffer)]);
         } else {
            printf("   ");
         }
         ++x;
      }
      printf("\n");
      x = 0;
      while (x <= 5) {
         if (coordToPath(x,y-1,'U',buffer)) {
            printf("%s",vArcs[getARC(g,buffer)]);
         } else {
            printf(" ");
         }
         
         // print out dice or discipline information
         int done = 0, region = 0;
         while (!done && region < NUM_REGIONS) {
            if (x == dicePos[region][0] && y == dicePos[region][1]) {
               printf("%2d ", getDiceValue(g, region));
               done = 1;
            } else if (x == dicePos[region][0] && y == dicePos[region][1] - 1) {
               printf("%s", discDisplay[getDiscipline(g, region)]);
               done = 1;
            }
            region++;
         }
         if (!done) {
            printf("   ");
         }
         
         ++x;
      }
      printf("\n");
      --y;
   }
   printf("\x1b[37m");
}

void wait(int len) {
   int i = 0, j = 1;
   while (i < len) {
      j = j*2+1;
      i++;
   }
}
