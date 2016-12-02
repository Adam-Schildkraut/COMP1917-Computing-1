// Authors: Shawn, Matthew, Michael, Zihan
//
// Shawn: 0422429353
// Matthew: 0434643674
// Michael: 0430383141
// Zihan: 0401015021
//
// testGame.c
//
// Link to game.c: https://kobra.io/#/e/-KGzMvFlNcZD6946MHk1 (dead link?)
// Link to hopefully not dead game.c: https://kobra.io/#/e/-KHPbCBA3NsJxwLgFD24
// Link to todo list: https://kobra.io/#/e/-KHUqiu_nU8zX-BMVJpq

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Game.h"

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
#define NUM_DISCIPLINES 6

//#define assert(x) x

void testGetDiscipline(void);
static void testGetDiceValueOnce(int *dice);
void testGetDiceValue(void);
void testGetMostPublications(void);
void testGetTurnNumber(void);
void testGetWhoseTurn(void);
void testGetMostARCs(void);
void testGetCampus(void);
void testGetARC(void);
static void assertArcAction(Game g, action a, int result);
void testIsLegalAction(void);
void testGetKPIpoints(void);
void testgetARCs(void);
void testGetGO8s(void);
void testGetCampuses(void);
void testGetIPs(void);
void testGetPublications(void);
void testGetExchangeRate(void);
static int testStartingStudents(Game testGame);
void testGetStudents(void);

// ############################    main   #################################
int main(int argc, char *argv[]) {
   testGetDiscipline();
   testGetDiceValue();
   testGetMostPublications();
   testGetTurnNumber();
   testGetWhoseTurn();
   testGetMostARCs();
   testGetCampus();
   testGetARC();
   testGetExchangeRate();
   testIsLegalAction();
   testGetKPIpoints();
   testgetARCs();
   testGetGO8s();
   testGetCampuses();
   testGetIPs();
   testGetPublications();
   testGetStudents();
   return 0;
}

//###########################################################################

void testGetDiscipline() {
   
   // Setting up the game struct
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   
   // Testing the game exists
   Game g = newGame(disciplines, dice);
   assert(g != NULL);
   
   // Testing that the game has the fields for disciplines and
   // dice numbers for each of the 19 regions on the game map
   
   int i = 0;
   while (i < NUM_REGIONS){
      assert(getDiscipline(g,i) == disciplines[i]);
      ++i;
   }
   printf("Disciplines and dice tested and passed!\n");
   disposeGame(g);
}

//###########################################################################

static void testGetDiceValueOnce(int *dice) {
   Game testGame = NULL;
   int disciplines[] = DEFAULT_DISCIPLINES;
   testGame = newGame(disciplines, dice);
   assert(testGame != NULL);
   int i = 0;
   while (i < NUM_REGIONS) {
      assert(getDiceValue(testGame, i) == dice[i]);
      i++;
   }
   disposeGame(testGame);
}

//###########################################################################

void testGetDiceValue(void) {
   printf("Begin getDiceValue test.\n");
   {
      int dice[] = DEFAULT_DICE;
      testGetDiceValueOnce(dice);
   }
   {
      int dice[] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
      testGetDiceValueOnce(dice);
   }
   {
      int dice[] = {2,3,4,5,6,7,8,9,10,11,12,11,10,9,8,7,6,5,4};
      testGetDiceValueOnce(dice);
   }
   printf("All tests passed - You are awesome!\n");
}

//###########################################################################

#define B_START "LRLRLRRLRL"
void testGetMostARCs() {
   printf("Begin testGetMostArcs\n");
   Game testGame;
   {
      int disciplines[] = DEFAULT_DISCIPLINES;
      int dice[] = DEFAULT_DICE;
      testGame = newGame(disciplines, dice);
   }
   //Each player starts with enough students to make three arcs.
   //This should be enough to do the testing.
   
   //To start with, the value should be NO_ONE.
   printf("Testing noone.\n");
   assert(getMostARCs(testGame) == NO_ONE);
   
   throwDice(testGame, 8);
   //After player A builds an arc, the player with the msot should be A
   {
      printf("Testing first arc built.\n");
      action makeArc = { OBTAIN_ARC, "R", 0, 0 };
      makeAction(testGame, makeArc);
      assert(getMostARCs(testGame) == UNI_A);
   }
   printf("Testing turn change, will not announce again.\n");
   throwDice(testGame, 8);
   assert(getMostARCs(testGame) == UNI_A);
   
   //After player B builds an arc, it should still be player A
   //it should change after they build their second.
   
   printf("Testing B building two arcs to overtake A's 1.\n");
   {
      action makeArc = { OBTAIN_ARC, B_START "B", 0, 0 };
      makeAction(testGame, makeArc);
      assert(getMostARCs(testGame) == UNI_A);
   }
   {
      action makeArc = { OBTAIN_ARC, B_START "BR", 0, 0 };
      makeAction(testGame, makeArc);
      assert(getMostARCs(testGame) == UNI_B);
   }
   
   throwDice(testGame, 8);
   assert(getMostARCs(testGame) == UNI_B);
   
   //Player C building two arcs should change nothing
   printf("Testing C building two arcs to match B's.\n");
   {
      action makeArc = { OBTAIN_ARC, B_START "BRLR", 0, 0 };      // why B_START?
      makeAction(testGame, makeArc);
      assert(getMostARCs(testGame) == UNI_B);
   }
   {
      action makeArc = { OBTAIN_ARC, B_START "BRLRLL", 0, 0 };    // why B_START?
      makeAction(testGame, makeArc);
      assert(getMostARCs(testGame) == UNI_B);
   }
   
   throwDice(testGame, 8);
   
   assert(getMostARCs(testGame) == UNI_B);
   
   //Player A building two more should only change on the third.
   printf("Testing three way tie and A exceeding the rest again.\n");
   {
      action makeArc = { OBTAIN_ARC, "RL", 0, 0 };
      makeAction(testGame, makeArc);
      assert(getMostARCs(testGame) == UNI_B);
   }
   {
      action makeArc = { OBTAIN_ARC, "RLR", 0, 0 };
      makeAction(testGame, makeArc);
      assert(getMostARCs(testGame) == UNI_A);
   }
   disposeGame(testGame);
   //That should be enough to ensure it all works okay.
   printf("Tests passed. Good job.\n");
}
#undef B_START

//###########################################################################

void testGetTurnNumber(void) {
   printf("Begin getTurnNumber test.\n");
   Game testGame = NULL;
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   
   // test 1 - large automated test
   testGame = newGame(disciplines, dice);
   assert(testGame != NULL);
   
   assert(getTurnNumber(testGame) == -1);
   int turn = 0;
   // test an arbitrary large number of turns
   while (turn < 1000) {
      throwDice(testGame, 2 + turn % 11);
      assert(getTurnNumber(testGame) == turn);
      turn++;
   }
   
   disposeGame(testGame);
   
   // test 2 - manual test, make sure operations other than throwDice
   //     do not affect the turn number
   testGame = newGame(disciplines, dice);
   assert(testGame != NULL);
   
   assert(getTurnNumber(testGame) == -1);
   
   throwDice(testGame, 7);
   assert(getTurnNumber(testGame) == 0);
   {
      // make an arc
      action getArc = { OBTAIN_ARC, "L", 0, 0 };
      makeAction(testGame, getArc);
      assert(getTurnNumber(testGame) == 0);
   }
   throwDice(testGame, 4);
   // call getTurnNumber multiple times in a row
   assert(getTurnNumber(testGame) == 1);
   assert(getTurnNumber(testGame) == 1);
   assert(getTurnNumber(testGame) == 1);
   {
      action pass = { PASS, "", 0, 0 };
      makeAction(testGame, pass);
      assert(getTurnNumber(testGame) == 1);
   }
   // go 2 turns without a call to getTurnNumber
   // first turn nothing happens, second turn somebody
   // passes and then makes an arc and a campus
   throwDice(testGame, 7);
   throwDice(testGame, 11);
   {
      action pass = { PASS, "", 0, 0 };
      makeAction(testGame, pass);
      action makeArc = { OBTAIN_ARC, "LR", 0, 0 };
      makeAction(testGame, makeArc);
      action makeCampus = { BUILD_CAMPUS, "LR", 0, 0 };
      makeAction(testGame, makeCampus);
      assert(getTurnNumber(testGame) == 3);
   }
   throwDice(testGame, 6);
   assert(getTurnNumber(testGame) == 4);
   {
      action getPublication = { OBTAIN_PUBLICATION, "", 0, 0 };
      makeAction(testGame, getPublication);
      assert(getTurnNumber(testGame) == 4);
   }
   throwDice(testGame, 2);
   assert(getTurnNumber(testGame) == 5);
   
   disposeGame(testGame);
   
   printf("All tests passed - You are awesome!\n");
}

//###########################################################################
static void assertArcAction(Game g, action a, int result) {
   // tests both the arc with the given path, and the arc going the other way
   // by appending 'B'. Only works if the path length is not the max length
   assert(strlen(a.destination) + 1 < PATH_LIMIT);
   assert(a.actionCode == OBTAIN_ARC);
   if (result) {
      assert(isLegalAction(g, a));
      strcat(a.destination, "B");
      assert(isLegalAction(g, a));
   } else {
      assert(!isLegalAction(g, a));
      strcat(a.destination, "B");
      assert(!isLegalAction(g, a));
   }
}
#define B_START "LRLRLRRLRL"
void testIsLegalAction() {
   //For the sake of testing, the entire board is 8s. This way, everyone
   //will get plenty of resources. Or at least, B will. Player B is my
   //favourite player, and also not the first player. Player B starts
   //with spots that make ARC materials.
   printf("Begin isLegalAction test.\n");
   
   // first test game
   {
      Game testGame;
      {
         int disciplines[] = DEFAULT_DISCIPLINES;
         int dice[] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
         testGame = newGame(disciplines, dice);
      }
      //To start, it should be perform any action at all, since the game
      //has yet to begin.
      {
         action startTest = { PASS, "R", 4, 5 };
         
         printf("Testing pre-start legality.\n");
         while (startTest.actionCode <= RETRAIN_STUDENTS)
         {
            assert(!isLegalAction(testGame, startTest));
            ++startTest.actionCode;
         }
      }
      
      //Now we start the game.
      throwDice(testGame, 8);
      
      //It's player A's turn, and they want to build an ARC near player B's
      //spot! That's not legal at all. They also want to build one and a
      //university in the middle of the map. Also illegal.
      //Then they want to immediately convert their starting spot to a G08
      //and build an ARC in the sea. How very illegal!
      {
         action teleportTest = { OBTAIN_ARC, "RRLRL", 0, 0 };
         printf("Testing valid path legality.\n");
         assert(!isLegalAction(testGame, teleportTest));
      }
      {
         action middleTest = { BUILD_CAMPUS, "RLRLR", 4, 5 };
         assert(!isLegalAction(testGame, middleTest));
         middleTest.actionCode = OBTAIN_ARC;
         assert(!isLegalAction(testGame, middleTest));
      }
      {
         action pylonsTest = { BUILD_GO8, "", 0, 0 };
         printf("Testing resource requirements GO8.\n");
         assert(!isLegalAction(testGame, pylonsTest));
      }
      {
         action returnToSea = { OBTAIN_ARC, "RRRRRR", 0, 0 };
         printf("Testing boundaries.\n");
         assert(!isLegalAction(testGame, returnToSea));
      }
      
      //Player A has pissed off the GM, so now it's player B's turn.
      //Player B is much nicer, and just wants to do the right things.
      //They also test the legality of everything they do.
      //They should have the three initial red and blue units, plus two
      //more of each, so they should be able to make five paths this turn.
      throwDice(testGame, 8);
      printf("Testing constructing a path.\n");
      {
         action makeArc = { OBTAIN_ARC, B_START "B", 0 , 0 };
         assertArcAction(testGame, makeArc, 1);
         makeAction(testGame, makeArc);
         assertArcAction(testGame, makeArc, 0);
      }
      {
         action makeArc = { OBTAIN_ARC, B_START "BR", 0 , 0 };
         assertArcAction(testGame, makeArc, 1);
         makeAction(testGame, makeArc);
         assertArcAction(testGame, makeArc, 0);
      }
      {
         action makeArc = { OBTAIN_ARC, B_START "BRL", 0 , 0 };
         assertArcAction(testGame, makeArc, 1);
         makeAction(testGame, makeArc);
         assertArcAction(testGame, makeArc, 0);
      }
      {
         action makeArc = { OBTAIN_ARC, B_START "BRLR", 0 , 0 };
         assertArcAction(testGame, makeArc, 1);
         makeAction(testGame, makeArc);
         assertArcAction(testGame, makeArc, 0);
      }
      {
         action makeArc = { OBTAIN_ARC, B_START "BRLRL", 0 , 0 };
         assertArcAction(testGame, makeArc, 1);
         makeAction(testGame, makeArc);
         assertArcAction(testGame, makeArc, 0);
      }
      
      //Player B is a goodie two shoes, and wants to know if they can build
      //a path through C's settlement. They can't, but this won't tell us
      //that, because they don't have enough resources.
      {
         printf("Testing resource requirements ARC maybe? This action is"
                "illegal for multiple reasons.\n");
         action makeArc = { OBTAIN_ARC, B_START "BRLRLL", 0, 0 };
         assert(!isLegalAction(testGame, makeArc));
      }
      
      throwDice(testGame, 8);
      //That's it for B. It's now C's turn. C wants to build a path over
      //B's path, gets frustrated they can't, and starts a business spinoff
      //after taking a couple of tries to realise they can't make papers or
      //patents directly.
      {
         printf("Testing occupied path.\n");
         action cAction = { OBTAIN_ARC, B_START "BRLRL", 0, 0};
         assert(!isLegalAction(testGame, cAction));
         printf("Testing actions that are always illegal?\n");
         cAction.actionCode = OBTAIN_IP_PATENT;
         assert(!isLegalAction(testGame, cAction));
         cAction.actionCode = OBTAIN_PUBLICATION;
         assert(!isLegalAction(testGame, cAction));
         printf("Testing start spinoff.\n");
         cAction.actionCode = START_SPINOFF;
         assert(isLegalAction(testGame, cAction));
      }
      
      throwDice(testGame, 8);
      //It's A's turn. They're retraining their units into THDs. This is
      //technically legal, but it's better not to ask questions.
      //They have 3BPS, 3?, 5MTV, 5MJ, and 1 M$. I think.
      {
         printf("Testing retraining.\n");
         action retraining = { RETRAIN_STUDENTS, "",
            STUDENT_BPS, STUDENT_THD };
         while (retraining.disciplineFrom < STUDENT_MMONEY) {
            assert(isLegalAction(testGame, retraining));
            ++retraining.disciplineFrom;
         }
         assert(!isLegalAction(testGame, retraining));
         printf("You can't retrain THDs :(\n");
         retraining.disciplineFrom = STUDENT_THD;
         retraining.disciplineTo = STUDENT_MMONEY;
         assert(!isLegalAction(testGame, retraining));
      }
      
      throwDice(testGame, 8);
      //Back to B. This time, they really are going to find out they can't
      //pass someone else's settlement, and then build a campus somewhere
      //they can accumulate funds for a GO8.
      {
         printf("Testing path obstructed by settlement.\n");
         action makeArc = { OBTAIN_ARC, B_START "BRLRLL", 0, 0 };
         assert(!isLegalAction(testGame, makeArc));
         strcat(makeArc.destination, "B");
         assert(!isLegalAction(testGame, makeArc));
      }
      //Testing building another two arcs, but this time from an odd
      //specified direction.
      printf("Testing two more arcs.\n");
      {
         action makeArc = { OBTAIN_ARC, B_START "BLLBR", 0 , 0 };
         assert(isLegalAction(testGame, makeArc));
         makeAction(testGame, makeArc);
      }
      {
         action makeArc = { OBTAIN_ARC, B_START "BLLB", 0 , 0 };
         assert(isLegalAction(testGame, makeArc));
         makeAction(testGame, makeArc);
      }
      {
         printf("Testing making a campus too close to an existing one.\n");
         action makeCampus = { BUILD_CAMPUS, B_START "B", 0, 0 };
         assert(!isLegalAction(testGame, makeCampus));
      }
      {
         printf("Testing making a campus.\n");
         action makeCampus = { BUILD_CAMPUS, B_START "BLL", 0, 0 };
         assert(isLegalAction(testGame, makeCampus));
         makeAction(testGame, makeCampus);
      }
      throwDice(testGame, 8);
      //You should always be able to pass your turn, but I don't think you
      //should require it, so I haven't yet.
      {
         action pass = { PASS, "", 0, 0 };
         assert(isLegalAction(testGame, pass));
         //makeAction(testGame, pass);
      }
      throwDice(testGame, 8);
      throwDice(testGame, 8);
      
      //B's turn again, nobody else did anything, almost done testing.
      //They make a GO8, but first time they forget to specify a path
      //and try to make one on player A's one.
      {
         printf("Testing can't upgrade someone else's campus.\n");
         action makeGO8 = { BUILD_GO8, "", 0, 0 };
         assert(!isLegalAction(testGame, makeGO8));
      }
      {
         printf("Testing can make own campus into GO8.\n");
         action makeGO8 = { BUILD_GO8, B_START "BLL", 0, 0 };
         assert(isLegalAction(testGame, makeGO8));
         makeAction(testGame, makeGO8);
      }
      throwDice(testGame, 8);
      printf("The setup for the next test will fail automatically if you assert legality"
             " in makeAction. So will some others before or later though.\n");
      {
         action makeGO8 = { BUILD_GO8, "RL", 0, 0 };
         int i = 0;
         while (i < 3) {
            makeAction(testGame, makeGO8);
            strcat(makeGO8.destination,"R");
            makeAction(testGame, makeGO8);
            strcat(makeGO8.destination,"L");
            ++i;
         }
         makeAction(testGame, makeGO8);
      }
      throwDice(testGame, 8);
      {
         int i = 0;
         while (i < 150) {
            throwDice(testGame, 8);
            ++i;
         }
         action go8Retraining = { RETRAIN_STUDENTS, "", STUDENT_MTV, STUDENT_MMONEY };
         while (isLegalAction(testGame, go8Retraining)) {
            makeAction(testGame, go8Retraining);
         }
      }
      printf("Testing too many GO8s\n");
      {
         action pylonsTest = { BUILD_GO8, "", 0, 0 };
         assert(!isLegalAction(testGame, pylonsTest));
      }
      disposeGame(testGame);
   }
   
   // second test game
   // test Go8s more
   // test building ARCs off of Go8s
   {
      // initialise game
      int disciplines[NUM_REGIONS];
      int dice[NUM_REGIONS];
      int i = 0;
      while (i < NUM_REGIONS) {
         disciplines[i] = STUDENT_BPS;
         dice[i] = 6;
         i++;
      }
      Game testGame = newGame(disciplines, dice);
      assert(testGame != NULL);
      
      // give everyone lots of resources
      i = 0;
      while (i < 3000) {
         // firstly give everyone lots of STUDENT_BPS
         throwDice(testGame, 6);
         i++;
      }
      i = 0;
      while (i < 3) {
         // then retrain some of the STUDENT_BPS to other disciplines
         throwDice(testGame, 6);
         int j = 0;
         while (j < 100) {
            action retrain = { RETRAIN_STUDENTS, "", STUDENT_BPS, -1 };
            retrain.disciplineTo = STUDENT_BQN;
            makeAction(testGame, retrain);
            retrain.disciplineTo = STUDENT_MJ;
            makeAction(testGame, retrain);
            retrain.disciplineTo = STUDENT_MTV;
            makeAction(testGame, retrain);
            retrain.disciplineTo = STUDENT_MMONEY;
            makeAction(testGame, retrain);
            j++;
         }
         i++;
      }
      
      // check the resource giving worked
      {
         // start with {0, 3, 3, 1, 1, 1, 1},
         // dice thrown 3003 times of two campuses getting STUDENT_BPS,
         // and 100 conversions to each of the other disciplines at
         // exchange rate of 3 to 1
         int students[] = { 0, 4809, 103, 101, 101, 101 };
         int disc = 0;
         while (disc < NUM_DISCIPLINES) {
            assert(getStudents(testGame, UNI_A, disc) == students[disc]);
            assert(getStudents(testGame, UNI_B, disc) == students[disc]);
            assert(getStudents(testGame, UNI_C, disc) == students[disc]);
            disc++;
         }
      }
      
      // A's turn
      throwDice(testGame, 9);
      assert(getWhoseTurn(testGame) == UNI_A);
      {
         // A tries to build a Go8
         action buildGo8 = { BUILD_GO8, "", 0, 0 };
         assert(isLegalAction(testGame, buildGo8));
         makeAction(testGame, buildGo8);
         // check you can't make a Go8 on a Go8
         printf("testing you can't build a Go8 on a Go8\n");
         assert(!isLegalAction(testGame, buildGo8));
      }
      {
         // check you can't make a Go8 on someone else's
         char *p = "RRLRL";
         action buildGo8 = { BUILD_GO8, "", 0, 0 };
         strcpy(buildGo8.destination, p);
         assert(getCampus(testGame, p) == UNI_B);
         printf("testing you can't build a Go8 on someone else's campus\n");
         assert(!isLegalAction(testGame, buildGo8));
      }
      {
         // A tries to build an arc off of the Go8
         action buildArc = { OBTAIN_ARC, "R", 0, 0 };
         action buildArcBack = { OBTAIN_ARC, "RB", 0, 0 };
         printf("testing that you can build an ARC off a Go8\n");
         assert(isLegalAction(testGame, buildArc));
         assert(isLegalAction(testGame, buildArcBack));
         makeAction(testGame, buildArc);
      }
      
      disposeGame(testGame);
   }
   
   printf("All tests passed. 99%% of the game should be working for all"
          "of these to actually be able to pass. Well done.\n");
}
#undef B_START

//###########################################################################
void testGetMostPublications(void) {
   printf("Begin getMostPublications test.\n");
   Game testGame = NULL;
   action getPub = { OBTAIN_PUBLICATION, "", 0, 0 };
   int i;
   
   // initialise game
   int disciplines[NUM_REGIONS];
   int dice[NUM_REGIONS];
   i = 0;
   while (i < NUM_REGIONS) {
      disciplines[i] = STUDENT_BPS;
      dice[i] = 6;
      i++;
   }
   testGame = newGame(disciplines, dice);
   assert(testGame != NULL);
   
   // give everyone lots of resources
   i = 0;
   while (i < 3000) {
      // firstly give everyone lots of STUDENT_BPS
      throwDice(testGame, 6);
      i++;
   }
   i = 0;
   while (i < 3) {
      // then retrain some of the STUDENT_BPS to other disciplines
      throwDice(testGame, 6);
      int j = 0;
      while (j < 100) {
         action retrain = { RETRAIN_STUDENTS, "", STUDENT_BPS, -1 };
         retrain.disciplineTo = STUDENT_BQN;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MJ;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MTV;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MMONEY;
         makeAction(testGame, retrain);
         j++;
      }
      i++;
   }
   
   // Terra Nullis
   assert(getMostPublications(testGame) == NO_ONE);
   
   // uni A
   throwDice(testGame, 4);
   assert(getMostPublications(testGame) == NO_ONE);
   
   // uni B
   throwDice(testGame, 9);
   assert(getMostPublications(testGame) == NO_ONE);
   
   // uni C
   throwDice(testGame, 11);
   assert(getMostPublications(testGame) == NO_ONE);
   
   // uni A
   throwDice(testGame, 12);
   assert(getMostPublications(testGame) == NO_ONE);
   
   // uni B gains 1 publication
   throwDice(testGame, 11);
   assert(getMostPublications(testGame) == NO_ONE);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_B);
   
   // uni C
   throwDice(testGame, 11);
   assert(getMostPublications(testGame) == UNI_B);
   
   // uni A gains 2 publications
   throwDice(testGame, 8);
   assert(getMostPublications(testGame) == UNI_B);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_B);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_A);
   
   // uni B
   throwDice(testGame, 3);
   assert(getMostPublications(testGame) == UNI_A);
   
   // uni C gains 2 publications in a row
   // *without* a call to getMostPublications
   // (in case someone has a really messed up implementation somehow...)
   throwDice(testGame, 7);
   assert(getMostPublications(testGame) == UNI_A);
   makeAction(testGame, getPub);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_A);
   
   // uni A
   throwDice(testGame, 12);
   assert(getMostPublications(testGame) == UNI_A);
   
   // uni B gains 4 publications
   // *without* a call to getMostPublications in between
   throwDice(testGame, 10);
   assert(getMostPublications(testGame) == UNI_A);
   makeAction(testGame, getPub);
   makeAction(testGame, getPub);
   makeAction(testGame, getPub);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_B);
   
   // uni C gains 1 publication
   throwDice(testGame, 6);
   assert(getMostPublications(testGame) == UNI_B);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_B);
   
   // uni A
   throwDice(testGame, 7);
   assert(getMostPublications(testGame) == UNI_B);
   
   // uni B
   throwDice(testGame, 9);
   assert(getMostPublications(testGame) == UNI_B);
   
   // uni C gains 4 publications
   throwDice(testGame, 3);
   assert(getMostPublications(testGame) == UNI_B);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_B);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_B);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_C);
   makeAction(testGame, getPub);
   assert(getMostPublications(testGame) == UNI_C);
   
   // seems legit
   
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
}

//###########################################################################
void testGetWhoseTurn(void) {
   printf("Begin getWhoseTurn test.\n");
   Game testGame = NULL;
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   testGame = newGame(disciplines,dice);
   assert(testGame != NULL);
   
   // Terra nullis
   assert(getWhoseTurn(testGame) == NO_ONE);
   
   throwDice(testGame, 8);
   assert(getWhoseTurn(testGame) == UNI_A);
   
   throwDice(testGame, 8);
   assert(getWhoseTurn(testGame) == UNI_B);
   
   throwDice(testGame, 8);
   assert(getWhoseTurn(testGame) == UNI_C);
   
   throwDice(testGame, 8);
   assert(getWhoseTurn(testGame) == UNI_A);
   
   disposeGame(testGame);
   printf("All tests passed! You are awesome!\n");
}

//###########################################################################
void testGetCampus(void) {
   printf("Begin testGetCampus test.\n");
   Game testGame = NULL;
   int i;
   
   // initialise game
   int disciplines[NUM_REGIONS];
   int dice[NUM_REGIONS];
   i = 0;
   while (i < NUM_REGIONS) {
      disciplines[i] = STUDENT_BPS;
      dice[i] = 5;
      ++i;
   }
   testGame = newGame(disciplines, dice);
   
   // test Terra Nullis game state
   
   // test inital 6 campus locations
   assert(getCampus(testGame, "") == UNI_A);
   assert(getCampus(testGame, "L") == NO_ONE);
   assert(getCampus(testGame, "R") == NO_ONE);
   
   assert(getCampus(testGame, "LRLRL") == UNI_C);
   assert(getCampus(testGame, "LRLRLB") == NO_ONE);
   assert(getCampus(testGame, "LRLRLR") == NO_ONE);
   
   assert(getCampus(testGame, "RRLRL") == UNI_B);
   assert(getCampus(testGame, "RRLR") == NO_ONE);
   assert(getCampus(testGame, "RRLRLL") == NO_ONE);
   
   assert(getCampus(testGame, "RLRLRLRRLR") == UNI_C);
   assert(getCampus(testGame, "RLRLRLRRLRB") == NO_ONE);
   assert(getCampus(testGame, "RLRLRLRRLRR") == NO_ONE);
   
   assert(getCampus(testGame, "RLRLRLRLRLL") == UNI_A);
   assert(getCampus(testGame, "RLRLRLRLRL") == NO_ONE);
   assert(getCampus(testGame, "RLRLRLRLRLLL") == NO_ONE);
   
   assert(getCampus(testGame, "RLLRRLLRRLLL") == UNI_B);
   assert(getCampus(testGame, "RLLRRLLRRLLLL") == NO_ONE);
   assert(getCampus(testGame, "RLLRRLLRRLLLB") == NO_ONE);
   
   // test some cyclic paths
   assert(getCampus(testGame, "LRRRRRRRRRRR") == UNI_A);
   assert(getCampus(testGame, "LRRRRRRRRRRRR") == NO_ONE);
   assert(getCampus(testGame, "LRRRRRRRRRRRRR") == NO_ONE);
   assert(getCampus(testGame, "LRRRRRRRRRR") == NO_ONE);
   assert(getCampus(testGame, "LRRRRRRRRRRRBRLRLLLLLLL") == UNI_B);
   assert(getCampus(testGame, "LRRRRRRRRRRRBRLRLLLLLLLL") == NO_ONE);
   assert(getCampus(testGame, "LRRRRRRRRRRRBRLRLLLLLLLLL") == NO_ONE);
   
   // initial state seems legit (y)
   // okay now to build campuses, apparently people are assuming that
   // makeAction won't do any sanity checking. So since the tests will
   // break if makeAction sanity checks anyway, there's probably no
   // point in me prematurely 'fixing' it to not break. So for now I'll
   // just also assume makeAction doesn't do sanity checking
   
   // player A's turn
   throwDice(testGame, 8);
   {
      // A builds a campus
      action buildCampus = { BUILD_CAMPUS, "LR", 0, 0 };
      makeAction(testGame, buildCampus);
      assert(getCampus(testGame, "LR") == UNI_A);
      assert(getCampus(testGame, "L") == NO_ONE);
      assert(getCampus(testGame, "LRR") == NO_ONE);
      assert(getCampus(testGame, "LRL") == NO_ONE);
      // test going a different route
      assert(getCampus(testGame, "RLLL") == UNI_A);
      assert(getCampus(testGame, "RLLLB") == NO_ONE);
      assert(getCampus(testGame, "RLLLL") == NO_ONE);
      assert(getCampus(testGame, "RLLLR") == NO_ONE);
      // make sure A's original campus is all good
      assert(getCampus(testGame, "") == UNI_A);
   }
   {
      // A builds another campus
      action buildCampus = { BUILD_CAMPUS, "RRLLLR", 0, 0 };
      makeAction(testGame, buildCampus);
      assert(getCampus(testGame, "RLRL") == UNI_A);
      assert(getCampus(testGame, "RRLLLR") == UNI_A);
      // at this point I'm going to stop testing NO_ONE points
      // because it's probs correct by now... I mean... what are the chances
      // anyway, test that the other two campuses owned by A are still all good
      assert(getCampus(testGame, "RLLLLL") == UNI_A);
      assert(getCampus(testGame, "RLRLLLLR") == UNI_A);
   }
   
   // player B's turn
   throwDice(testGame, 8);
   {
      // B builds 2 campuses
      action buildCampus = { BUILD_CAMPUS, "", 0, 0 };
      strcpy(buildCampus.destination, "RRL");
      makeAction(testGame, buildCampus);
      strcpy(buildCampus.destination, "RRLLRL");
      makeAction(testGame, buildCampus);
      // check that all of B's campuses exist
      assert(getCampus(testGame, "RRLRL") == UNI_B);
      assert(getCampus(testGame, "RRLRLBR") == UNI_B);
      assert(getCampus(testGame, "RRLRLBRRRL") == UNI_B);
      // check A's campusus for good measure
      assert(getCampus(testGame, "") == UNI_A);
      assert(getCampus(testGame, "LR") == UNI_A);
      assert(getCampus(testGame, "RLRL") == UNI_A);
   }
   
   // at this point, things are probably pretty legit
   // only important thing left to check is that C can build campuses
   // player C's turn
   throwDice(testGame, 8);
   {
      action buildCampus = { BUILD_CAMPUS, "", 0, 0 };
      // test
      assert(getCampus(testGame, "LRRRLLLLRLR") == UNI_C);
      assert(getCampus(testGame, "RRLRLLRL") == NO_ONE);
      assert(getCampus(testGame, "LRLRR") == NO_ONE);
      // C builds a campuses (using a roundabout route)
      strcpy(buildCampus.destination, "RLRLRLRRRL");
      makeAction(testGame, buildCampus);
      // test
      assert(getCampus(testGame, "LRRRLLLLRLR") == UNI_C);
      assert(getCampus(testGame, "RRLRLLRL") == UNI_C);
      assert(getCampus(testGame, "LRLRR") == NO_ONE);
      // C builds another campus (using another round about route)
      strcpy(buildCampus.destination, "LRRLL");
      makeAction(testGame, buildCampus);
      // test
      assert(getCampus(testGame, "LRRRLLLLRLR") == UNI_C);
      assert(getCampus(testGame, "RRLRLLRL") == UNI_C);
      assert(getCampus(testGame, "LRLRR") == UNI_C);
   }
   
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void testGetARC() {
   printf("Begin testGetArc test.\n");
   Game testGame;
   {
      int disciplines[] = DEFAULT_DISCIPLINES;
      int dice[] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
      testGame = newGame(disciplines, dice);
   }
   //Paths to each initial starting position such that R and B are both
   //valid moves.
   {
      char* startingPaths[6] = { "RB", "RLRLRLRLRLLLB",
         "RRLRLLB", "LRLRLRRLRL",
         "RRLRLLRLRLLB","LRLRL"} ;
      //Rather than being thorough, which is hard, I will be targeted.
      //First, the map should have no edges already placed. A common
      //mistake would be to replicate the web game and have an edge to
      //the left and/or an edge to the right of each starting zone.
      printf("Testing initialisation.\n");
      int i = 0;
      while (i < 6) {
         char path[PATH_LIMIT];
         
         //Dirty way to concatenate strings to new string.
         sprintf(path, "%s%s", startingPaths[i], "R");
         assert(getARC(testGame, path) == VACANT_ARC);
         sprintf(path, "%s%s", startingPaths[i], "B");
         assert(getARC(testGame, path) == VACANT_ARC);
         ++i;
      }
      
      throwDice(testGame, 8);
      
      //After A makes three paths, none of the rest should have changed.
      //But it should also change the ones that A built over.
      printf("Testing making three paths.\n");
      {
         action makeARC = { OBTAIN_ARC, "", 0, 0 };
         strcat(makeARC.destination, startingPaths[0]);
         i = 0;
         while (i < 3) {
            strcat(makeARC.destination, "R");
            assert(getARC(testGame, makeARC.destination) == VACANT_ARC);
            makeAction(testGame, makeARC);
            assert(getARC(testGame, makeARC.destination) == ARC_A);
            ++i;
         }
      }
      
      //Copy pasting is bad but...
      i = 1;
      while (i < 6) {
         char path[PATH_LIMIT];
         
         //Dirty way to concatenate strings to new string.
         sprintf(path, "%s%s", startingPaths[i], "R");
         assert(getARC(testGame, path) == VACANT_ARC);
         sprintf(path, "%s%s", startingPaths[i], "B");
         assert(getARC(testGame, path) == VACANT_ARC);
         ++i;
      }
      
      throwDice(testGame, 8);
      
      //Player B making their paths should be their own path...
      //Again, copy pasting is bad don't do it...
      printf("Testing player two making three paths.\n");
      {
         action makeARC = { OBTAIN_ARC, "", 0, 0 };
         strcat(makeARC.destination, startingPaths[3]);
         i = 0;
         while (i < 3) {
            strcat(makeARC.destination, "R");
            assert(getARC(testGame, makeARC.destination) == VACANT_ARC);
            makeAction(testGame, makeARC);
            assert(getARC(testGame, makeARC.destination) == ARC_B);
            ++i;
         }
      }
      
      throwDice(testGame, 8);
      //Finally, when player C inspects everyone's paths, they should
      //all still belong to the right players.
      printf("Testing examining other people's paths.\n");
      {
         char path[PATH_LIMIT];
         sprintf(path, "%s", startingPaths[0]);
         i = 0;
         while (i < 3) {
            strcat(path, "R");
            assert(getARC(testGame, path) == ARC_A);
            ++i;
         }
         sprintf(path, "%s", startingPaths[3]);
         i = 0;
         while (i < 3) {
            strcat(path, "R");
            assert(getARC(testGame, path) == ARC_B);
            ++i;
         }
      }
   }
   disposeGame(testGame);
   printf("All tests passed, nice going.\n");
}

//###########################################################################
#define B_START "LRLRLRRLRL"
#define C_START "RRLRLLRLRL"
void testGetKPIpoints(void) {
   // This function tests KPI points based on the KPI allocation rules:
   // Normal Campus = 10
   // GO8 = 20
   // ARC Grant = 2
   // IP = 10
   // Most publications = 10
   // Most ARC grants = 10
   printf("Begin testKPIpoints test.\n");
   Game testGame = NULL;
   int i;
   
   // initialise game
   int disciplines[NUM_REGIONS];
   int dice[NUM_REGIONS];
   i = 0;
   while (i < NUM_REGIONS) {
      disciplines[i] = STUDENT_BPS;
      dice[i] = 6;
      ++i;
   }
   testGame = newGame(disciplines, dice);
   
   // give everyone lots of resources
   i = 0;
   while (i < 3000) {
      // firstly give everyone lots of STUDENT_BPS
      throwDice(testGame, 6);
      i++;
   }
   i = 0;
   while (i < 3) {
      // then retrain some of the STUDENT_BPS to other disciplines
      throwDice(testGame, 6);
      int j = 0;
      while (j < 100) {
         action retrain = { RETRAIN_STUDENTS, "", STUDENT_BPS, -1 };
         retrain.disciplineTo = STUDENT_BQN;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MJ;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MTV;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MMONEY;
         makeAction(testGame, retrain);
         j++;
      }
      i++;
   }
   
   // Terra nullis condition
   // At the start of the game, each player has 2 campuses but no ARC grants
   assert(getKPIpoints (testGame, UNI_A) == 20);
   assert(getKPIpoints (testGame, UNI_B) == 20);
   assert(getKPIpoints (testGame, UNI_C) == 20);
   
   // Start game with UNI_A at turn 0
   throwDice(testGame, 6);
   // UNI_A decides to build one arc grants
   {
      action makeARC = { OBTAIN_ARC, "L", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getKPIpoints (testGame, UNI_A) == 32);
      // 2*campus + 1*arc + 10 for most arcs
      assert(getKPIpoints (testGame, UNI_B) == 20);
      assert(getKPIpoints (testGame, UNI_C) == 20);
   }
   
   
   // now it's player B's turn, player B will make 2 arc grants
   throwDice(testGame, 6);
   // making the first arc grant and checking that A will still
   // have the most arcs the longest
   {
      action makeARC = { OBTAIN_ARC, B_START "R", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getKPIpoints (testGame, UNI_A) == 32);
      // 2*campus + 1*arc + 10 for most arcs longer than B
      assert(getKPIpoints (testGame, UNI_B) == 22);
      // 2*campus + 1*arc
      assert(getKPIpoints (testGame, UNI_C) == 20);
   }
   // making the second arc grant which now exceeds A
   {
      action makeARC = { OBTAIN_ARC, B_START "RR", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getKPIpoints (testGame, UNI_A) == 22);
      // 2*campus + 1*arc
      assert(getKPIpoints (testGame, UNI_B) == 34);
      // 2*campus + 2*arc + 10 for most arcs now
      assert(getKPIpoints (testGame, UNI_C) == 20);
   }
   // UNI_B also wants to build a campus
   {
      action buildCampus = { BUILD_CAMPUS, B_START "RR", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getKPIpoints (testGame, UNI_A) == 22);
      // 2*campus + 1*arc
      assert(getKPIpoints (testGame, UNI_B) == 44);
      // 3*campus + 2*arc + 10 for most arcs now
      assert(getKPIpoints (testGame, UNI_C) == 20);
   }
   
   // now it's player C's turn, player C will make 3 arc grants
   throwDice(testGame, 6);
   // making the first arc grant and checking that A will still
   // have the most arcs the longest
   {
      action makeARC = { OBTAIN_ARC, C_START "L", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getKPIpoints (testGame, UNI_A) == 22);
      // 2*campus + 1*arc
      assert(getKPIpoints (testGame, UNI_B) == 44);
      // 3*campus + 2*arc + 10 for most arcs
      assert(getKPIpoints (testGame, UNI_C) == 22);
      // 2*campus + 1*arc
   }
   // making the second arc grant which now exceeds A
   {
      action makeARC = { OBTAIN_ARC, C_START "LL", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getKPIpoints (testGame, UNI_A) == 22);
      // 2*campus + 1*arc
      assert(getKPIpoints (testGame, UNI_B) == 44);
      // 3*campus + 2*arc + 10 for most arcs longer than C
      assert(getKPIpoints (testGame, UNI_C) == 24);
      // 2*campus + 2*arc
   }
   // making the third arc grant which now exceeds B
   {
      action makeARC = { OBTAIN_ARC, C_START "LLL", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getKPIpoints (testGame, UNI_A) == 22);
      // 2*campus + 1*arc
      assert(getKPIpoints (testGame, UNI_B) == 34);
      // 3*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_C) == 36);
      // 2*campus + 3*arc + 10 for most arcs now
   }
   
   // now back to player A's turn. Player A will make a campus and
   // get a publication
   throwDice(testGame, 6);
   // UNI_A will make the next arc grant
   {
      action makeARC = { OBTAIN_ARC, "LR", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getKPIpoints (testGame, UNI_A) == 24);
      // 2*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_B) == 34);
      // 3*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_C) == 36);
      // 2*campus + 3*arc + 10 for most arcs now
   }
   // UNI_A also wants to build a campus
   {
      action buildCampus = { BUILD_CAMPUS, "LR", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getKPIpoints (testGame, UNI_A) == 34);
      // 3*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_B) == 34);
      // 3*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_C) == 36);
      // 2*campus + 3*arc + 10 for most arcs now
   }
   // UNI_A getting a publication
   {
      action getPub = { OBTAIN_PUBLICATION, "", 0, 0 };
      makeAction(testGame, getPub);
      assert(getKPIpoints (testGame, UNI_A) == 44);
      // 3*campus + 2*arc + most pubs
      assert(getKPIpoints (testGame, UNI_B) == 34);
      // 3*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_C) == 36);
      // 2*campus + 3*arc + 10 for most arcs now
   }
   
   // now player B's turn. Player B will make a GO8 and
   // get 2 publications
   throwDice(testGame, 6);
   // Converting to GO8
   {
      action buildGO8 = { BUILD_GO8, B_START "RR", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getKPIpoints (testGame, UNI_A) == 44);
      // 3*campus + 2*arc + most pubs
      assert(getKPIpoints (testGame, UNI_B) == 44);
      // 2*campus + 2*arc + 1*GO8
      assert(getKPIpoints (testGame, UNI_C) == 36);
      // 2*campus + 3*arc + 10 for most arcs now
   }
   // Getting 1st pub
   {
      action getPub = { OBTAIN_PUBLICATION, "", 0, 0 };
      makeAction(testGame, getPub);
      assert(getKPIpoints (testGame, UNI_A) == 44);
      // 3*campus + 2*arc + most pubs still
      assert(getKPIpoints (testGame, UNI_B) == 44);
      // 2*campus + 2*arc + 1*GO8
      assert(getKPIpoints (testGame, UNI_C) == 36);
      // 2*campus + 3*arc + 10 for most arcs now
   }
   // Getting 2nd pub and exceeding A
   {
      action getPub = { OBTAIN_PUBLICATION, "", 0, 0 };
      makeAction(testGame, getPub);
      assert(getKPIpoints (testGame, UNI_A) == 34);
      // 3*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_B) == 54);
      // 2*campus + 2*arc + 1*GO8 + most pubs now
      assert(getKPIpoints (testGame, UNI_C) == 36);
      // 2*campus + 3*arc + 10 for most arcs now
   }
   
   // now player C's turn. Player C will get a patent
   // get 2 publications
   throwDice(testGame, 6);
   // getting patent
   {
      action getPatent = { OBTAIN_IP_PATENT, "", 0, 0 };
      makeAction(testGame, getPatent);
      assert(getKPIpoints (testGame, UNI_A) == 34);
      // 3*campus + 2*arc
      assert(getKPIpoints (testGame, UNI_B) == 54);
      // 2*campus + 2*arc + 1*GO8 + most pubs now
      assert(getKPIpoints (testGame, UNI_C) == 46);
      // 2*campus + 3*arc + 1*Patent + 10 for most arcs now
   }
   
   // That should test all the factors that make up KPI points
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
   
}
#undef C_START
#undef B_START

//###########################################################################
#define B_START "LRLRLRRLRL"
#define C_START "RRLRLLRLRL"
void testgetARCs(void) {
   
   printf("Begin testgetARCs test.\n");
   Game testGame = NULL;
   int i;
   
   // initialise game
   int disciplines[NUM_REGIONS];
   int dice[NUM_REGIONS];
   i = 0;
   while (i < NUM_REGIONS) {
      disciplines[i] = STUDENT_BPS;
      dice[i] = 6;
      ++i;
   }
   testGame = newGame(disciplines, dice);
   
   // Terra Nullis condition
   // At the start, no one has any arcs
   assert(getARCs (testGame, UNI_A) == 0);
   assert(getARCs (testGame, UNI_B) == 0);
   assert(getARCs (testGame, UNI_C) == 0);
   
   // Starting the game with UNI_A
   throwDice(testGame, 6);
   // UNI_A will make one arc
   {
      action makeARC = { OBTAIN_ARC, "L", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getARCs (testGame, UNI_A) == 1);
      assert(getARCs (testGame, UNI_B) == 0);
      assert(getARCs (testGame, UNI_C) == 0);
   }
   
   // Now it is UNI_B's turn and they will make 2 arcs
   throwDice(testGame, 6);
   // Making the first arc
   {
      action makeARC = { OBTAIN_ARC, B_START "B", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getARCs (testGame, UNI_A) == 1);
      assert(getARCs (testGame, UNI_B) == 1);
      assert(getARCs (testGame, UNI_C) == 0);
   }
   // Making the second one
   {
      action makeARC = { OBTAIN_ARC, B_START "BL", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getARCs (testGame, UNI_A) == 1);
      assert(getARCs (testGame, UNI_B) == 2);
      assert(getARCs (testGame, UNI_C) == 0);
   }
   
   // Now make it UNI_C's turn, who will make 3 arcs
   throwDice(testGame, 6);
   // Making the first arc
   {
      action makeARC = { OBTAIN_ARC, C_START "L", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getARCs (testGame, UNI_A) == 1);
      assert(getARCs (testGame, UNI_B) == 2);
      assert(getARCs (testGame, UNI_C) == 1);
   }
   // Making the second one
   {
      action makeARC = { OBTAIN_ARC, C_START "LL", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getARCs (testGame, UNI_A) == 1);
      assert(getARCs (testGame, UNI_B) == 2);
      assert(getARCs (testGame, UNI_C) == 2);
   }
   // And the third one
   {
      action makeARC = { OBTAIN_ARC, C_START "LLL", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getARCs (testGame, UNI_A) == 1);
      assert(getARCs (testGame, UNI_B) == 2);
      assert(getARCs (testGame, UNI_C) == 3);
   }
   
   // Now back to player A to make another arc
   throwDice(testGame, 6);
   // UNI_A will make one arc
   {
      action makeARC = { OBTAIN_ARC, "LR", 0 , 0 };
      makeAction(testGame, makeARC);
      assert(getARCs (testGame, UNI_A) == 2);
      assert(getARCs (testGame, UNI_B) == 2);
      assert(getARCs (testGame, UNI_C) == 3);
   }
   
   // That's pretty much all you can test there for now
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
}
#undef C_START
#undef B_START

//###########################################################################
#define B_START "LRLRLRRLRL"
#define C_START "RRLRLLRLRL"
void testGetGO8s(void) {
   
   printf("Begin testGetGO8s test.\n");
   Game testGame = NULL;
   int i;
   
   // initialise game
   int disciplines[NUM_REGIONS];
   int dice[NUM_REGIONS];
   i = 0;
   while (i < NUM_REGIONS) {
      disciplines[i] = STUDENT_BPS;
      dice[i] = 6;
      ++i;
   }
   testGame = newGame(disciplines, dice);
   
   // give everyone lots of resources
   i = 0;
   while (i < 3000) {
      // firstly give everyone lots of STUDENT_BPS
      throwDice(testGame, 6);
      i++;
   }
   i = 0;
   while (i < 3) {
      // then retrain some of the STUDENT_BPS to other disciplines
      throwDice(testGame, 6);
      int j = 0;
      while (j < 100) {
         action retrain = { RETRAIN_STUDENTS, "", STUDENT_BPS, -1 };
         retrain.disciplineTo = STUDENT_BQN;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MJ;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MTV;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MMONEY;
         makeAction(testGame, retrain);
         j++;
      }
      i++;
   }
   
   // Terra Nullis condition
   // At the start, no one has any arcs
   assert(getGO8s (testGame, UNI_A) == 0);
   assert(getGO8s (testGame, UNI_B) == 0);
   assert(getGO8s (testGame, UNI_C) == 0);
   
   // Start with Player A's turn. Player A will make 2 arc grants and
   // build a campus
   throwDice(testGame, 6);
   // Making first arc
   {
      action makeARC = { OBTAIN_ARC, "L", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, "LR", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, "LR", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getGO8s (testGame, UNI_A) == 0);
      assert(getGO8s (testGame, UNI_B) == 0);
      assert(getGO8s (testGame, UNI_C) == 0);
   }
   {
      action buildGO8 = { BUILD_GO8, "LR", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getGO8s (testGame, UNI_A) == 1);
      assert(getGO8s (testGame, UNI_B) == 0);
      assert(getGO8s (testGame, UNI_C) == 0);
   }
   
   // Now go to player B's turn. Player B will build 4 arcs, with a
   // campus at each secone one, and then convert them into GO8s
   // after creating each campus
   throwDice(testGame, 6);
   // Making first arc
   {
      action makeARC = { OBTAIN_ARC, B_START "B", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, B_START "BL", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, B_START "BL", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getGO8s (testGame, UNI_A) == 1);
      assert(getGO8s (testGame, UNI_B) == 0);
      assert(getGO8s (testGame, UNI_C) == 0);
   }
   // Now converting it to GO8
   {
      action buildGO8 = { BUILD_GO8, B_START "BL", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getGO8s (testGame, UNI_A) == 1);
      assert(getGO8s (testGame, UNI_B) == 1);
      assert(getGO8s (testGame, UNI_C) == 0);
   }
   // Making next 2 arcs
   {
      action makeARC = { OBTAIN_ARC, B_START "BLR", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, B_START "BLRL", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, B_START "BLRL", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getGO8s (testGame, UNI_A) == 1);
      assert(getGO8s (testGame, UNI_B) == 1);
      assert(getGO8s (testGame, UNI_C) == 0);
      
      action buildGO8 = { BUILD_GO8, B_START "BLRL", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getGO8s (testGame, UNI_A) == 1);
      assert(getGO8s (testGame, UNI_B) == 2);
      assert(getGO8s (testGame, UNI_C) == 0);
   }
   
   // Now on to C's turn. C will make 3 arcs, and then a campus
   // then convert it to a GO8
   throwDice(testGame, 6);
   {
      action makeARC = { OBTAIN_ARC, C_START "L", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, C_START "LL", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, C_START "LL", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getGO8s (testGame, UNI_A) == 1);
      assert(getGO8s (testGame, UNI_B) == 2);
      assert(getGO8s (testGame, UNI_C) == 0);
      
      action buildGO8 = { BUILD_GO8, C_START "LL", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getGO8s (testGame, UNI_A) == 1);
      assert(getGO8s (testGame, UNI_B) == 2);
      assert(getGO8s (testGame, UNI_C) == 1);
   }
   
   // Finally, converting A's primary campus into a GO8
   throwDice(testGame, 6);
   {
      action buildGO8 = { BUILD_GO8, "", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getGO8s (testGame, UNI_A) == 2);
      assert(getGO8s (testGame, UNI_B) == 2);
      assert(getGO8s (testGame, UNI_C) == 1);
   }
   
   // That's all the tests
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
}
#undef C_START
#undef B_START

//###########################################################################
#define B_START "LRLRLRRLRL"
#define C_START "RRLRLLRLRL"
void testGetCampuses(void) {
   
   printf("Begin testGetCampuses test.\n");
   Game testGame = NULL;
   int i;
   
   // initialise game
   int disciplines[NUM_REGIONS];
   int dice[NUM_REGIONS];
   i = 0;
   while (i < NUM_REGIONS) {
      disciplines[i] = STUDENT_BPS;
      dice[i] = 6;
      ++i;
   }
   testGame = newGame(disciplines, dice);
   
   // give everyone lots of resources
   i = 0;
   while (i < 3000) {
      // firstly give everyone lots of STUDENT_BPS
      throwDice(testGame, 6);
      i++;
   }
   i = 0;
   while (i < 3) {
      // then retrain some of the STUDENT_BPS to other disciplines
      throwDice(testGame, 6);
      int j = 0;
      while (j < 100) {
         action retrain = { RETRAIN_STUDENTS, "", STUDENT_BPS, -1 };
         retrain.disciplineTo = STUDENT_BQN;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MJ;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MTV;
         makeAction(testGame, retrain);
         retrain.disciplineTo = STUDENT_MMONEY;
         makeAction(testGame, retrain);
         j++;
      }
      i++;
   }
   
   // Terra Nullis condition
   // At the start, everyone has 2 campuses
   assert(getCampuses (testGame, UNI_A) == 2);
   assert(getCampuses (testGame, UNI_B) == 2);
   assert(getCampuses (testGame, UNI_C) == 2);
   
   // Start with Player A's turn. Player A will make 2 arc grants and
   // build a campus
   throwDice(testGame, 6);
   // Making first arc
   {
      action makeARC = { OBTAIN_ARC, "L", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, "LR", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, "LR", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getCampuses (testGame, UNI_A) == 3);
      assert(getCampuses (testGame, UNI_B) == 2);
      assert(getCampuses (testGame, UNI_C) == 2);
   }
   {
      action buildGO8 = { BUILD_GO8, "LR", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getCampuses (testGame, UNI_A) == 2);
      assert(getCampuses (testGame, UNI_B) == 2);
      assert(getCampuses (testGame, UNI_C) == 2);
   }
   
   // Now go to player B's turn. Player B will build 4 arcs, with a
   // campus at each secone one, and then convert them into GO8s
   // after creating each campus
   throwDice(testGame, 6);
   // Making first arc
   {
      action makeARC = { OBTAIN_ARC, B_START "B", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, B_START "BL", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, B_START "BL", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getCampuses (testGame, UNI_A) == 2);
      assert(getCampuses (testGame, UNI_B) == 3);
      assert(getCampuses (testGame, UNI_C) == 2);
   }
   // Making next 2 arcs
   {
      action makeARC = { OBTAIN_ARC, B_START "BLR", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, B_START "BLRL", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, B_START "BLRL", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getCampuses (testGame, UNI_A) == 2);
      assert(getCampuses (testGame, UNI_B) == 4);
      assert(getCampuses (testGame, UNI_C) == 2);
      
      action buildGO8 = { BUILD_GO8, B_START "BLRL", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getCampuses (testGame, UNI_A) == 2);
      assert(getCampuses (testGame, UNI_B) == 3);
      assert(getCampuses (testGame, UNI_C) == 2);
   }
   
   // Now on to C's turn. C will make 3 arcs, and then a campus
   // then convert it to a GO8
   throwDice(testGame, 6);
   {
      action makeARC = { OBTAIN_ARC, C_START "L", 0 , 0 };
      makeAction(testGame, makeARC);
   }
   // Second arc, then campus then convert to GO8
   {
      action makeARC = { OBTAIN_ARC, C_START "LL", 0 , 0 };
      makeAction(testGame, makeARC);
      
      action buildCampus = { BUILD_CAMPUS, C_START "LL", 0 , 0 };
      makeAction(testGame, buildCampus);
      assert(getCampuses (testGame, UNI_A) == 2);
      assert(getCampuses (testGame, UNI_B) == 3);
      assert(getCampuses (testGame, UNI_C) == 3);
      
      action buildGO8 = { BUILD_GO8, C_START "LL", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getCampuses (testGame, UNI_A) == 2);
      assert(getCampuses (testGame, UNI_B) == 3);
      assert(getCampuses (testGame, UNI_C) == 2);
   }
   
   // Finally, converting A's primary campus into a GO8
   throwDice(testGame, 6);
   {
      action buildGO8 = { BUILD_GO8, "", 0 , 0 };
      makeAction(testGame, buildGO8);
      assert(getCampuses (testGame, UNI_A) == 1);
      assert(getCampuses (testGame, UNI_B) == 3);
      assert(getCampuses (testGame, UNI_C) == 2);
   }
   
   // That's all the tests
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
}
#undef C_START
#undef B_START

//###########################################################################
void testGetIPs(void) {
   
   printf("Begin testGetIPs test.\n");
   Game testGame = NULL;
   action getIP = { OBTAIN_IP_PATENT, "", 0, 0 };
   
   // initialise game
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   testGame = newGame(disciplines, dice);
   
   // Terra Nullis condition
   // At the start, no one has any patents
   assert(getIPs (testGame, UNI_A) == 0);
   assert(getIPs (testGame, UNI_B) == 0);
   assert(getIPs (testGame, UNI_C) == 0);
   
   // Start the game with player A and get them a couple of patents
   throwDice(testGame, 6);
   {
      makeAction(testGame, getIP);
      assert(getIPs (testGame, UNI_A) == 1);
      assert(getIPs (testGame, UNI_B) == 0);
      assert(getIPs (testGame, UNI_C) == 0);
   }
   // Get another one
   {
      makeAction(testGame, getIP);
      assert(getIPs (testGame, UNI_A) == 2);
      assert(getIPs (testGame, UNI_B) == 0);
      assert(getIPs (testGame, UNI_C) == 0);
   }
   
   // Now on to player B's turn getting 2 patents
   throwDice(testGame, 6);
   {
      makeAction(testGame, getIP);
      assert(getIPs (testGame, UNI_A) == 2);
      assert(getIPs (testGame, UNI_B) == 1);
      assert(getIPs (testGame, UNI_C) == 0);
   }
   // Get another one
   {
      makeAction(testGame, getIP);
      assert(getIPs (testGame, UNI_A) == 2);
      assert(getIPs (testGame, UNI_B) == 2);
      assert(getIPs (testGame, UNI_C) == 0);
   }
   
   // Now testing for player C
   throwDice(testGame, 6);
   {
      makeAction(testGame, getIP);
      assert(getIPs (testGame, UNI_A) == 2);
      assert(getIPs (testGame, UNI_B) == 2);
      assert(getIPs (testGame, UNI_C) == 1);
   }
   // Get another one
   {
      makeAction(testGame, getIP);
      assert(getIPs (testGame, UNI_A) == 2);
      assert(getIPs (testGame, UNI_B) == 2);
      assert(getIPs (testGame, UNI_C) == 2);
   }
   
   // Now testing back for player A
   throwDice(testGame, 6);
   {
      makeAction(testGame, getIP);
      assert(getIPs (testGame, UNI_A) == 3);
      assert(getIPs (testGame, UNI_B) == 2);
      assert(getIPs (testGame, UNI_C) == 2);
   }
   
   // That's all the steps to test
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
   
}

//###########################################################################
void testGetPublications(void) {
   
   printf("Begin testGetPublications test.\n");
   Game testGame = NULL;
   action getPub = { OBTAIN_PUBLICATION, "", 0, 0 };
   
   // initialise game
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   testGame = newGame(disciplines, dice);
   
   // Terra Nullis condition
   // At the start, no one has any publications
   assert(getPublications (testGame, UNI_A) == 0);
   assert(getPublications (testGame, UNI_B) == 0);
   assert(getPublications (testGame, UNI_C) == 0);
   
   // Start the game with player A and get them a couple of pubs
   throwDice(testGame, 6);
   {
      makeAction(testGame, getPub);
      assert(getPublications (testGame, UNI_A) == 1);
      assert(getPublications (testGame, UNI_B) == 0);
      assert(getPublications (testGame, UNI_C) == 0);
   }
   // Get another one
   {
      makeAction(testGame, getPub);
      assert(getPublications (testGame, UNI_A) == 2);
      assert(getPublications (testGame, UNI_B) == 0);
      assert(getPublications (testGame, UNI_C) == 0);
   }
   
   // Now on to player B's turn getting 2 pubs
   throwDice(testGame, 6);
   {
      makeAction(testGame, getPub);
      assert(getPublications (testGame, UNI_A) == 2);
      assert(getPublications (testGame, UNI_B) == 1);
      assert(getPublications (testGame, UNI_C) == 0);
   }
   // Get another one
   {
      makeAction(testGame, getPub);
      assert(getPublications (testGame, UNI_A) == 2);
      assert(getPublications (testGame, UNI_B) == 2);
      assert(getPublications (testGame, UNI_C) == 0);
   }
   
   // Now testing for player C
   throwDice(testGame, 6);
   {
      makeAction(testGame, getPub);
      assert(getPublications (testGame, UNI_A) == 2);
      assert(getPublications (testGame, UNI_B) == 2);
      assert(getPublications (testGame, UNI_C) == 1);
   }
   // Get another one
   {
      makeAction(testGame, getPub);
      assert(getPublications (testGame, UNI_A) == 2);
      assert(getPublications (testGame, UNI_B) == 2);
      assert(getPublications (testGame, UNI_C) == 2);
   }
   
   // Now testing back for player A
   throwDice(testGame, 6);
   {
      makeAction(testGame, getPub);
      assert(getPublications (testGame, UNI_A) == 3);
      assert(getPublications (testGame, UNI_B) == 2);
      assert(getPublications (testGame, UNI_C) == 2);
   }
   
   // That's all the steps to test
   disposeGame(testGame);
   printf("All tests passed - You are awesome!\n");
   
}

//###########################################################################
void testGetExchangeRate() {
   printf("Begin testGetExchangeRate test.\n");
   Game testGame;
   {
      int disciplines[] = DEFAULT_DISCIPLINES;
      int dice[] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
      testGame = newGame(disciplines, dice);
   }
   
   //To start with, all exchange rates should be 3 for all players.
   printf("Testing initial state.\n");
   int i = UNI_A;
   while (i <= UNI_C) {
      int j;
      throwDice(testGame, 8);
      j = STUDENT_BPS;
      while (j <= STUDENT_MMONEY) {
         int k = STUDENT_THD;
         while (k <= STUDENT_MMONEY) {
            assert(getExchangeRate(testGame, i, j, k) == 3);
            ++k;
         }
         ++j;
      }
      ++i;
   }
   
   throwDice(testGame, 8);
   
   //Each player has to build campuses next to the retraining centers
   //to get the bonuses. I am going to assume that makeAction doesn't
   //care at all about legality, and have A build one next to each.
   {
      printf("Testing each retraining center.\n");
      //All retraining centers in order of student ID and with the
      //second node being right from the specified node.
      char* retCenters[5] = {"RRLRLLRLRLL","LRLRLRRL","LRLRLRRLRLRRRB",
         "R","LRRB"};
      
      i = STUDENT_BPS;
      while (i <= STUDENT_MMONEY) {
         action buildCampus = { BUILD_CAMPUS, "",
            0, 0 };
         strcat(buildCampus.destination, retCenters[i-STUDENT_BPS]);
         makeAction(testGame, buildCampus);
         //Contrary to common sense, each retraining center determines
         //who you can retrain more cheaply from rather than to.
         //Also, none of this should change other players.
         int j = STUDENT_THD;
         while (j <= STUDENT_MMONEY) {
            assert(getExchangeRate(testGame, UNI_A, i, j) == 2);
            assert(getExchangeRate(testGame, UNI_B, i, j) == 3);
            assert(getExchangeRate(testGame, UNI_C, i, j) == 3);
            if (j != STUDENT_THD) {
               if (j > i) {
                  assert(getExchangeRate(testGame, UNI_A, j, i) == 3);
               } else {
                  assert(getExchangeRate(testGame, UNI_A, j, i) == 2);
               }
               assert(getExchangeRate(testGame, UNI_B, j, i) == 3);
               assert(getExchangeRate(testGame, UNI_C, j, i) == 3);
            }
            ++j;
         }
         ++i;
      }
      
      throwDice(testGame, 8);
      
      //Player B should be able to get the bonuses if they build theirs
      //on the other slots. I mean, if that were legal. But we don't
      //care about that, right?
      printf("Testing for player B.\n");
      i = STUDENT_BPS;
      while (i <= STUDENT_MMONEY) {
         action buildCampus = { BUILD_CAMPUS, "",
            0, 0 };
         sprintf(buildCampus.destination, "%s%s",
                 retCenters[i-STUDENT_BPS], "R");
         makeAction(testGame, buildCampus);
         //Now, player B should have the bonuses too, while C is left
         //sad that there are no bonuses left for them :(
         int j = STUDENT_THD;
         while (j <= STUDENT_MMONEY) {
            assert(getExchangeRate(testGame, UNI_B, i, j) == 2);
            assert(getExchangeRate(testGame, UNI_C, i, j) == 3);
            if (j != STUDENT_THD) {
               if (j > i) {
                  assert(getExchangeRate(testGame, UNI_B, j, i) == 3);
               } else if (j != STUDENT_THD) {
                  assert(getExchangeRate(testGame, UNI_B, j, i) == 2);
               }
               assert(getExchangeRate(testGame, UNI_C, j, i) == 3);
            }
            ++j;
         }
         ++i;
      }
   }
   
   //That should be everything?
   printf("Tests have passed, keep on chugging!\n");
   disposeGame(testGame);
}

//###########################################################################
void testGetStudents() {
   printf("Begin testGetStudents test.\n");
   Game testGame;
   {
      int disciplines[] = DEFAULT_DISCIPLINES;
      int dice[] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
      testGame = newGame(disciplines, dice);
   }
   
   //To start with, each student should have the number of students in
   //the defined array.
   printf("Testing starting students.\n");
   {
      assert(testStartingStudents(testGame));
      //After throwing a number other than 8 or 7, the numbers should
      //not change.
      printf("Testing no change.\n");
      throwDice(testGame, 6);
      assert(testStartingStudents(testGame));
   }
   
   //With the given board, throwing an 8 should increase A and C's
   //student counts for MTV and MJ by one, and B's BQN and BPS by
   //one.
   printf("Testing adding students.\n");
   throwDice(testGame, 8);
   {
      int acStudents[6] = STARTING_STUDENTS;
      int bStudents[6] = STARTING_STUDENTS;
      ++acStudents[STUDENT_MTV];
      ++acStudents[STUDENT_MJ];
      ++bStudents[STUDENT_BQN];
      ++bStudents[STUDENT_BPS];
      int i = STUDENT_THD;
      while (i <= STUDENT_MMONEY) {
         assert(getStudents(testGame, UNI_A, i) == acStudents[i]);
         assert(getStudents(testGame, UNI_B, i) == bStudents[i]);
         assert(getStudents(testGame, UNI_C, i) == acStudents[i]);
         ++i;
      }
      
      //Throwing a 7 should turn all MTVs and MMONEY into THDs.
      printf("Testing 7.\n");
      throwDice(testGame, 7);
      acStudents[STUDENT_THD] += (acStudents[STUDENT_MTV] +
                                  acStudents[STUDENT_MMONEY]);
      bStudents[STUDENT_THD] += (bStudents[STUDENT_MTV] +
                                 bStudents[STUDENT_MMONEY]);
      acStudents[STUDENT_MTV] = 0;
      acStudents[STUDENT_MMONEY] = 0;
      bStudents[STUDENT_MTV] = 0;
      bStudents[STUDENT_MMONEY] = 0;
      
      i = STUDENT_THD;
      while (i <= STUDENT_MMONEY) {
         assert(getStudents(testGame, UNI_A, i) == acStudents[i]);
         assert(getStudents(testGame, UNI_B, i) == bStudents[i]);
         assert(getStudents(testGame, UNI_C, i) == acStudents[i]);
         ++i;
      }
      //It's C's turn. By retraining to a THD, C will have one more
      //THD and three less BPS.
      printf("Testing retraining.\n");
      {
         action retrain = { RETRAIN_STUDENTS, "",
            STUDENT_BPS, STUDENT_THD };
         makeAction(testGame, retrain);
      }
      
      acStudents[STUDENT_BPS] -= 3;
      ++acStudents[STUDENT_THD];
      
      i = STUDENT_THD;
      while (i <= STUDENT_MMONEY) {
         assert(getStudents(testGame, UNI_C, i) == acStudents[i]);
         ++i;
      }
   }
   disposeGame(testGame);
   
   // construction cost tests
   printf("Testing construction costs\n");
   
   {
      int disciplines[] = DEFAULT_DISCIPLINES;
      // This map is *MODIFIED* version of the default map
      disciplines[7] = STUDENT_MMONEY;
      int dice[] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
      testGame = newGame(disciplines, dice);
   }
   
   throwDice(testGame, 6);
   // Uni A tests building 2 arcs and a campus
   {
      // initial state
      int students[] = {0, 3, 3, 1, 1, 1};
      int discipline = 0;
      while (discipline < NUM_DISCIPLINES) {
         assert(getStudents(testGame, UNI_A, discipline) == students[discipline]);
         discipline++;
      }
      // build an arc
      {
         action makeArc = { OBTAIN_ARC, "L", 0, 0 };
         makeAction(testGame, makeArc);
         students[STUDENT_BPS]--;
         students[STUDENT_BQN]--;
      }
      discipline = 0;
      while (discipline < NUM_DISCIPLINES) {
         assert(getStudents(testGame, UNI_A, discipline) == students[discipline]);
         discipline++;
      }
      // build another arc
      {
         action makeArc = { OBTAIN_ARC, "LR", 0, 0 };
         makeAction(testGame, makeArc);
         students[STUDENT_BPS]--;
         students[STUDENT_BQN]--;
      }
      discipline = 0;
      while (discipline < NUM_DISCIPLINES) {
         assert(getStudents(testGame, UNI_A, discipline) == students[discipline]);
         discipline++;
      }
      // build a campus
      {
         action makeCampus = { BUILD_CAMPUS, "L", 0, 0 };
         makeAction(testGame, makeCampus);
         students[STUDENT_BPS]--;
         students[STUDENT_BQN]--;
         students[STUDENT_MJ]--;
         students[STUDENT_MTV]--;
      }
      discipline = 0;
      while (discipline < NUM_DISCIPLINES) {
         assert(getStudents(testGame, UNI_A, discipline) == students[discipline]);
         discipline++;
      }
   }
   
   // Uni B tests getting a publication
   throwDice(testGame, 6);
   {
      action getPub = { OBTAIN_PUBLICATION, "", 0, 0 };
      makeAction(testGame, getPub);
      assert(getStudents(testGame, UNI_B, STUDENT_THD) == 0);
      assert(getStudents(testGame, UNI_B, STUDENT_BPS) == 3);
      assert(getStudents(testGame, UNI_B, STUDENT_BQN) == 3);
      assert(getStudents(testGame, UNI_B, STUDENT_MJ) == 0);
      assert(getStudents(testGame, UNI_B, STUDENT_MTV) == 0);
      assert(getStudents(testGame, UNI_B, STUDENT_MMONEY) == 0);
   }
   
   // Uni C tests getting a patent
   throwDice(testGame, 6);
   {
      action getPatent = { OBTAIN_IP_PATENT, "", 0, 0 };
      makeAction(testGame, getPatent);
      assert(getStudents(testGame, UNI_C, STUDENT_THD) == 0);
      assert(getStudents(testGame, UNI_C, STUDENT_BPS) == 3);
      assert(getStudents(testGame, UNI_C, STUDENT_BQN) == 3);
      assert(getStudents(testGame, UNI_C, STUDENT_MJ) == 0);
      assert(getStudents(testGame, UNI_C, STUDENT_MTV) == 0);
      assert(getStudents(testGame, UNI_C, STUDENT_MMONEY) == 0);
   }
   
   // Recall that this game is played on a modified map (see the
   // discipline[] array above)
   // Also recall that we already built a campus for Uni A
   // Pass 3 times to give Uni A more resources
   throwDice(testGame, 8);
   throwDice(testGame, 8);
   throwDice(testGame, 8);
   throwDice(testGame, 8);
   // Uni A's turn again, Uni A now builds a Go8
   {
      assert(getStudents(testGame, UNI_A, STUDENT_THD) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_BPS) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_BQN) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_MJ) == 4);
      assert(getStudents(testGame, UNI_A, STUDENT_MTV) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_MMONEY) == 9);
      
      action getGo8 = { BUILD_GO8, "LR", 0, 0 };
      makeAction(testGame, getGo8);
      
      assert(getStudents(testGame, UNI_A, STUDENT_THD) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_BPS) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_BQN) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_MJ) == 2);
      assert(getStudents(testGame, UNI_A, STUDENT_MTV) == 0);
      assert(getStudents(testGame, UNI_A, STUDENT_MMONEY) == 6);
   }
   
   disposeGame(testGame);
   
   printf("Tests passed. Yay.\n");
   
}
//--------------------------------------------------------------------

static int testStartingStudents(Game testGame) {
   int startingStudents[6] = STARTING_STUDENTS;
   int r = 1;
   
   int i = UNI_A;
   
   //This would be so much nicer with multiple return locations and
   //returning within a loop.
   while (r && (i <= UNI_C)) {
      int j = STUDENT_THD;
      while (r && (j <= STUDENT_MMONEY)) {
         r = r && (getStudents(testGame, i, j) == startingStudents[j]);
         ++j;
      }
      ++i;
   }
   return r;
}
//###########################################################################

/* functions:
 
 // A
 Game newGame (int discipline[], int dice[]);;
 
 // B
 void disposeGame (Game g);
 
 // C
 void throwDice (Game g, int diceScore);
 
 // D
 void makeAction (Game g, action a);
 
 // 1 - Shawn (done)
 int getDiscipline (Game g, int regionID);
 
 // 2 - Michael (done)
 int getDiceValue (Game g, int RegionID);
 
 // 3 - Matt (done)
 int getMostARCs (Game g);
 
 // 4 - Michael (done)
 int getMostPublications (Game g);
 
 // 5 - Michael (done)
 int getTurnNumber (Game g);
 
 // 6 - Zihan (done)
 int getWhoseTurn (Game g);
 
 // 7 - Michael (done)
 int getCampus(Game g, path pathToVertex);
 
 // 8 - Matt (done)
 int getARC(Game g, path pathToEdge);
 
 // 9 - Matt (done)
 int isLegalAction (Game g, action a);
 
 // 10 - Shawn (done)
 int getKPIpoints (Game g, int player);
 
 //11 - Shawn (done)
 int getARCs (Game g, int player);
 
 //12 - Shawn (done)
 int getGO8s (Game g, int player);
 
 //13 - Shawn (done)
 int getCampuses (Game g, int player);
 
 //14 - Shawn (done)
 int getIPs (Game g, int player);
 
 //15 - Shawn (done)
 int getPublications (Game g, int player);
 
 //16 - Matt (done)
 //     Michael additional tests (done)
 int getStudents (Game g, int player, int discipline);
 
 //17 - Matt (done)
 int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo);
 
 */



//*************************************************************************
/*
 Here is the code already in Game.h:
 
 *  Game.h
 *  1917 v1.07
 *   edit log -
 *    1.07 badly-drawn map region and dice values comment
 *    1.06 expanded comments on newGame(), explain regionID
 *    1.05 expanded the comments explaining isLegalAction()
 *    1.04 expanded path description, starting direction
 *    1.03 edited comment for getmostpubs() 30-apr-14
 *    1.02 abstract ADT version of Game
 *    1.00 concrete game type version
 *  Do not alter this file
 *
 *  Created by Richard Buckland on 20/04/14.
 *  Licensed under Creative Commons SA-BY-NC 3.0.
 *
 
 
 #define NUM_UNIS 3
 
 // player ID of each university
 #define NO_ONE 0
 #define UNI_A 1
 #define UNI_B 2
 #define UNI_C 3
 
 // contents of an ARC
 #define VACANT_ARC 0
 #define ARC_A 1
 #define ARC_B 2
 #define ARC_C 3
 
 // contents of a VERTEX
 #define VACANT_VERTEX 0
 #define CAMPUS_A 1
 #define CAMPUS_B 2
 #define CAMPUS_C 3
 #define GO8_A 4
 #define GO8_B 5
 #define GO8_C 6
 
 // action codes
 #define PASS 0
 #define BUILD_CAMPUS 1
 #define BUILD_GO8 2
 #define OBTAIN_ARC 3
 #define START_SPINOFF 4
 #define OBTAIN_PUBLICATION 5
 #define OBTAIN_IP_PATENT 6
 #define RETRAIN_STUDENTS 7
 
 // disciplines
 #define STUDENT_THD 0
 #define STUDENT_BPS 1
 #define STUDENT_BQN 2
 #define STUDENT_MJ  3
 #define STUDENT_MTV 4
 #define STUDENT_MMONEY 5
 
 #define NUM_REGIONS 19
 #define PATH_LIMIT 150
 
 #define TRUE 1
 #define FALSE 0
 
 typedef struct _game * Game;
 
 // your team designs this type not us
 // store in this struct all the things you might want to know about
 // the game so you can write the interface functions in this header
 // eg you might want to store the current turn number (so i've put
 // it in for you as an example but take it out if you don't want it)
 
 // in your Game.c include the details of the data
 // you want to store in the _game struct eg
 
 // typedef struct _game {
 //   int currentTurn;
 //   ... more stuff in here
 // } game;
 
 // a path is a sequence of L=left R=right B=back steps
 // starting from the initial campus of player 1 / A at the top
 // of the map facing inwards represented as a string of length
 // PATH_LIMIT or less (including the terminating 0).
 // a path can specify a vertex (the vertex at the end of the path)
 // and a path can specify an ARC (the last ARC in the path)
 // it is fine and legal for a path to not be the shortest path
 // to the destination, it can even contain loops if you like.
 // The length of a path (including the terminating 0) must never
 // exceed PATH_LIMIT.  Every edge in the path must lie on the
 // island, paths cannot include edges which are in the sea.
 typedef char path[PATH_LIMIT];
 
 // actions are what the player AI returns.  They say the one thing
 // the AI wants to do next.  In the playGame logic you'll ask
 // a player for their next action, then you'll check it is a legal
 // action (using isLegalAction()), then you'll perform that action
 // for them (using makeAction()), then you'll ask the same player
 // for another action and repeat this over and over again until they
 // return PASS at which time you'll throw the dice (using
 // throwDice()) and advance the game to the next player.  And repeat.
 typedef struct _action {
 int actionCode;  // see #defines above
 path destination; // if the action operates on a vertex or ARC this
 // specifies *which* vertex or path.  unused
 // otherwise
 int disciplineFrom;  // used for the retrain students action
 int disciplineTo;    // used for the retrain students action
 } action;
 
 **** Functions which change the game aka SETTERS **** */
// make a new game, given the disciplines produced by each
// region, and the value on the dice discs in each region.
// note: each array must be NUM_REGIONS long
// eg if you are using my sample game struct above this function
// would need to set the field currentTurn to -1.  (because the turn
// number is -1 at the start of the game)
// the ordering of the regions is column by column left to right,
// going from the top of each column to the bottom before moving
// to the next column to the right.
//
// so to create the default game as shown on the badly drawn map:
//
/*
 #define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
 STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
 STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
 STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
 STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
 #define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}
 
 //
 //   int disciplines[] = DEFAULT_DISCIPLINES;
 //   int dice[] = DEFAULT_DICE;
 //   Game g = newGame (disciplines, dice);
 Game newGame (int discipline[], int dice[]);
 
 // free all the memory malloced for the game
 void disposeGame (Game g);
 
 // make the specified action for the current player and update the
 // game state accordingly.
 // The function may assume that the action requested is legal.
 // START_SPINOFF is not a legal action here
 void makeAction (Game g, action a);
 
 // advance the game to the next turn,
 // assuming that the dice has just been rolled and produced diceScore
 // the game starts in turn -1 (we call this state "Terra Nullis") and
 // moves to turn 0 as soon as the first dice is thrown.
 void throwDice (Game g, int diceScore);
 
 **** Functions which GET data about the game aka GETTERS ****
 
 // what type of students are produced by the specified region?
 // regionID is the index of the region in the newGame arrays (above)
 // see discipline codes above
 int getDiscipline (Game g, int regionID);
 
 // what dice value produces students in the specified region?
 // 2..12
 int getDiceValue (Game g, int regionID);
 
 // which university currently has the prestige award for the most ARCs?
 // this is NO_ONE until the first arc is purchased after the game
 // has started.
 int getMostARCs (Game g);
 
 // which university currently has the prestige award for the most pubs?
 // this is NO_ONE until the first publication is made.
 int getMostPublications (Game g);
 
 // return the current turn number of the game -1,0,1, ..
 int getTurnNumber (Game g);
 
 // return the player id of the player whose turn it is
 // the result of this function is NO_ONE during Terra Nullis
 int getWhoseTurn (Game g);
 
 // return the contents of the given vertex (ie campus code or
 // VACANT_VERTEX)
 int getCampus(Game g, path pathToVertex);
 
 // the contents of the given edge (ie ARC code or vacent ARC)
 int getARC(Game g, path pathToEdge);
 
 // returns TRUE if it is legal for the current
 // player to make the specified action, FALSE otherwise.
 //
 // "legal" means everything is legal:
 //   * that the action code is a valid action code which is legal to
 //     be made at this time
 //   * that any path is well formed and legal ie consisting only of
 //     the legal direction characters and of a legal length,
 //     and which does not leave the island into the sea at any stage.
 //   * that disciplines mentioned in any retraining actions are valid
 //     discipline numbers, and that the university has sufficient
 //     students of the correct type to perform the retraining
 //
 // eg when placing a campus consider such things as:
 //   * is the path a well formed legal path
 //   * does it lead to a vacent vertex?
 //   * under the rules of the game are they allowed to place a
 //     campus at that vertex?  (eg is it adjacent to one of their ARCs?)
 //   * does the player have the 4 specific students required to pay for
 //     that campus?
 // It is not legal to make any action during Terra Nullis ie
 // before the game has started.
 // It is not legal for a player to make the moves OBTAIN_PUBLICATION
 // or OBTAIN_IP_PATENT (they can make the move START_SPINOFF)
 // you can assume that any pths passed in are NULL terminated strings.
 int isLegalAction (Game g, action a);
 
 // --- get data about a specified player ---
 
 // return the number of KPI points the specified player currently has
 int getKPIpoints (Game g, int player);
 
 // return the number of ARC grants the specified player currently has
 int getARCs (Game g, int player);
 
 // return the number of GO8 campuses the specified player currently has
 int getGO8s (Game g, int player);
 
 // return the number of normal Campuses the specified player currently has
 int getCampuses (Game g, int player);
 
 // return the number of IP Patents the specified player currently has
 int getIPs (Game g, int player);
 
 // return the number of Publications the specified player currently has
 int getPublications (Game g, int player);
 
 // return the number of students of the specified discipline type
 // the specified player currently has
 int getStudents (Game g, int player, int discipline);
 
 // return how many students of discipline type disciplineFrom
 // the specified player would need to retrain in order to get one
 // student of discipline type disciplineTo.  This will depend
 // on what retraining centers, if any, they have a campus at.
 int getExchangeRate (Game g, int player,
 int disciplineFrom, int disciplineTo);
 
 */