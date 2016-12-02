// Authors: Shawn, Matthew, Michael, Zihan
//
// Shawn: 0422429353
// Matthew: 0434643674
// Michael: 0430383141
// Zihan: 0401015021
//
//  game.c
//  C
//
// Link to testGame.c: https://kobra.io/#/e/-KGLx4Pogc1_VQcIVEno
// Link to runGame.c: https://kobra.io/#/e/-KHJcZ3lztoGH5nm7OHw
// Link to todo list: https://kobra.io/#/e/-KHUqiu_nU8zX-BMVJpq

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Game.h"
#define NUM_DISCIPLINES 6
#define NUM_ROWS 11
#define NUM_COLUMNS 6
#define TERRA_NULLIS -1
#define STARTING_STUDENTS {0, 3, 3, 1, 1, 1}
// change this to 0 to turn off debugging output
#define DEBUG_MODE 0

typedef struct _player {
   // An array of each student type, with index of each type
   // of student corresponding to the discipline code
   int students[NUM_DISCIPLINES];
   // Number of campuses the player owns, to be increased when a new one is
   // built but decreased if it's converted to a go8. (Recalculate entirely?)
   int campuses;
   int go8s;
   // Number of arcs owned, only ever increases.
   int arcs;
   // Number of publications or patents arising from a spin-off
   int publications;
   int patents;
} player;

typedef struct _vertex {
   //Stores the arc north of the vertex.
   int arcV;
   //Stores the arc east of the vertex.
   int arcH;
   //Stores the campus on the vertex.
   int campus;
} vertex;

typedef struct _game {
   // Stores the turn number
   // currentTurn % 3 + 1 = current player's turn
   int currentTurn;
   
   // The first player to reach the current highest number of arcs or
   // publications respectively.
   int mostPublications;
   int mostARCs;
   
   // Stores the discipline of student produced by each region
   int discipline[NUM_REGIONS];
   
   // The dice number for each region that will produce students
   // of the corresponding discipline
   int dice[NUM_REGIONS];
   
   // Struct of players, one for each uni A, B and C
   // Stores
   player players[NUM_UNIS];
   
   // An 2D array of structs that indicate whether an arc or campus
   // exists at that index
   // 0,0 is the bottom left, array is column major.
   vertex vertices[NUM_COLUMNS][NUM_ROWS];
   
} game;

static int isCampusLegal(Game g, path p);
static int isArcLegal(Game g, path p);
static int traverseMap(path pathTo, int *i, int *j, int *dir);

// A - Michael (done)
Game newGame (int discipline[], int dice[]) {
   if (DEBUG_MODE) {
      // debugging output
      printf("newGame() called, discipline[] = {%d", discipline[0]);
      int region = 1;
      while (region < NUM_REGIONS) {
         printf(",%d", discipline[region]);
         region++;
      }
      printf("}, dice[] = {%d", dice[0]);
      region = 1;
      while (region < NUM_REGIONS) {
         printf(",%d", dice[region]);
         region++;
      }
      printf("}\n");
   }
   // Created a pointer to the struct 'game' and allocates
   // space in memory for that struct
   Game g = (Game) malloc(sizeof(game));
   
   assert(g);
   
   // initialize non-region, non-player, non-vertex stuff
   g->currentTurn = TERRA_NULLIS;
   g->mostPublications = NO_ONE;
   g->mostARCs = NO_ONE;
   
   // initialize regions
   int regionID = 0;
   while (regionID < NUM_REGIONS) {
      g->discipline[regionID] = discipline[regionID];
      g->dice[regionID] = dice[regionID];
      regionID++;
   }
   
   // initialize players
   int playerID = 0;
   while (playerID < NUM_UNIS) {
      g->players[playerID].students[STUDENT_THD] = 0;
      g->players[playerID].students[STUDENT_BPS] = 3;
      g->players[playerID].students[STUDENT_BQN] = 3;
      g->players[playerID].students[STUDENT_MJ] = 1;
      g->players[playerID].students[STUDENT_MTV] = 1;
      g->players[playerID].students[STUDENT_MMONEY] = 1;
      g->players[playerID].campuses = 2;
      g->players[playerID].go8s = 0;
      g->players[playerID].arcs = 0;
      g->players[playerID].publications = 0;
      g->players[playerID].patents = 0;
      playerID++;
   }
   
   // initialize vertices
   // firstly initialize everything to 0...
   int x = 0;
   while (x < NUM_COLUMNS) {
      int y = 0;
      while (y < NUM_ROWS) {
         g->vertices[x][y].arcV = VACANT_ARC;
         g->vertices[x][y].arcH = VACANT_ARC;
         g->vertices[x][y].campus = VACANT_VERTEX;
         y++;
      }
      x++;
   }
   // then initialize the starting campuses
   g->vertices[2][10].campus = CAMPUS_A;
   g->vertices[3][0].campus = CAMPUS_A;
   g->vertices[0][7].campus = CAMPUS_B;
   g->vertices[5][3].campus = CAMPUS_B;
   g->vertices[0][2].campus = CAMPUS_C;
   g->vertices[5][8].campus = CAMPUS_C;
   
   return g;
}

// B - Shawn (done)
void disposeGame (Game g){
   if (DEBUG_MODE) {
      printf("disposeGame() called\n");
   }
   free(g);
}

// C - Michael (done)
void throwDice (Game g, int diceScore){
   if (DEBUG_MODE) {
      printf("throwDice() called, diceScore = %d\n", diceScore);
   }
   g->currentTurn++;
   
   // need to check for campuses at regions corresponding to dice score
   // give each player a student for each region with the player's
   // campus at a vertex
   
   // hardcoded arrays for the region x and y coordinates
   // stores the x and y coordinates of the vertex to the bottom left of
   // the region
   int regionX[] = { 0,0,0, 1,1,1,1, 2,2,2,2,2, 3,3,3,3, 4,4,4 };
   int regionY[] = { 6,4,2, 7,5,3,1, 8,6,4,2,0, 7,5,3,1, 6,4,2 };
   
   // the campuses adjacent to a region with position (x, y) are
   // the campuses (x', y') such that x' = x or x+1 and y' = y, y+1 or y+2
   int regionID = 0;
   while (regionID < NUM_REGIONS) {
      if (g->dice[regionID] == diceScore) {
         // give resources to the adjacent campuses
         int studentType = g->discipline[regionID];
         int campusX = regionX[regionID];
         while (campusX <= regionX[regionID] + 1) {
            int campusY = regionY[regionID];
            while (campusY <= regionY[regionID] + 2) {
               int campusType = g->vertices[campusX][campusY].campus;
               // case bash it, you can do it without case bashing
               // but it's probably clearer this way
               // (even though it's more verbose)
               if (campusType == 1) {
                  g->players[0].students[studentType] += 1;
               } else if (campusType == 2) {
                  g->players[1].students[studentType] += 1;
               } else if (campusType == 3) {
                  g->players[2].students[studentType] += 1;
               } else if (campusType == 4) {
                  g->players[0].students[studentType] += 2;
               } else if (campusType == 5) {
                  g->players[1].students[studentType] += 2;
               } else if (campusType == 6) {
                  g->players[2].students[studentType] += 2;
               }
               campusY++;
            }
            campusX++;
         }
      }
      regionID++;
   }
   
   // if a 7 was rolled, convert all MTV and MMONEY to THD
   if (diceScore == 7) {
      int playerID = 0;
      while (playerID < NUM_UNIS) {
         g->players[playerID].students[STUDENT_THD] += g->players[playerID].students[STUDENT_MTV];
         g->players[playerID].students[STUDENT_MTV] = 0;
         g->players[playerID].students[STUDENT_THD] += g->players[playerID].students[STUDENT_MMONEY];
         g->players[playerID].students[STUDENT_MMONEY] = 0;
         playerID++;
      }
   }
}

// D - Michael (done)
void makeAction (Game g, action a) {
   if (DEBUG_MODE) {
      printf("makeAction() called, a = { %d, \"%s\", %d, %d }\n",
             a.actionCode, a.destination, a.disciplineFrom, a.disciplineTo);
   }
   assert(a.actionCode != START_SPINOFF);
   assert(getWhoseTurn(g) != NO_ONE);
   
   // get some information common to multiple actions
   int who = getWhoseTurn(g);
   int x = 0, y = 0, dir = 0;
   
   if (a.actionCode == PASS) {
      // lol
      //printf("dude why would I want this?\n");
   } else if (a.actionCode == BUILD_CAMPUS) {
      assert(traverseMap(a.destination, &x, &y, &dir));
      g->vertices[x][y].campus = who;
      g->players[who - 1].campuses++;
      // remove students
      g->players[who - 1].students[STUDENT_BPS]--;
      g->players[who - 1].students[STUDENT_BQN]--;
      g->players[who - 1].students[STUDENT_MJ]--;
      g->players[who - 1].students[STUDENT_MTV]--;
   } else if (a.actionCode == BUILD_GO8) {
      assert(traverseMap(a.destination, &x, &y, &dir));
      g->vertices[x][y].campus = who + 3;
      g->players[who - 1].campuses--;
      g->players[who - 1].go8s++;
      // remove students
      g->players[who - 1].students[STUDENT_MJ] -= 2;
      g->players[who - 1].students[STUDENT_MMONEY] -= 3;
   } else if (a.actionCode == OBTAIN_ARC) {
      assert(traverseMap(a.destination, &x, &y, &dir));
      assert(strlen(a.destination) > 0);
      
      // get the correct arc based on direction
      // add arc to map
      if (dir == 'U') {
         g->vertices[x][y].arcV = who;
      } else if (dir == 'D') {
         g->vertices[x][y-1].arcV = who;
      } else if (dir == 'L') {
         g->vertices[x-1][y].arcH = who;
      } else if (dir == 'R') {
         g->vertices[x][y].arcH = who;
      }
      
      // add arc to player
      g->players[who - 1].arcs++;
      // remove students
      g->players[who - 1].students[STUDENT_BPS]--;
      g->players[who - 1].students[STUDENT_BQN]--;
      // update most arcs
      if (g->mostARCs == 0 ||
          g->players[who - 1].arcs > g->players[g->mostARCs - 1].arcs) {
         g->mostARCs = who;
      }
      
   } else if (a.actionCode == OBTAIN_PUBLICATION) {
      g->players[who - 1].publications++;
      // oh no the line is over 80 characters what ever will we do...
      if (g->mostPublications == 0 ||
          g->players[who - 1].publications > g->players[g->mostPublications - 1].publications) {
         g->mostPublications = who;
      }
      
      // remove students
      g->players[who - 1].students[STUDENT_MJ]--;
      g->players[who - 1].students[STUDENT_MTV]--;
      g->players[who - 1].students[STUDENT_MMONEY]--;
   } else if (a.actionCode == OBTAIN_IP_PATENT) {
      g->players[who - 1].patents++;
      
      // remove students
      g->players[who - 1].students[STUDENT_MJ]--;
      g->players[who - 1].students[STUDENT_MTV]--;
      g->players[who - 1].students[STUDENT_MMONEY]--;
   } else if (a.actionCode == RETRAIN_STUDENTS) {
      int rate = getExchangeRate(g, who, a.disciplineFrom, a.disciplineTo);
      g->players[who - 1].students[a.disciplineFrom] -= rate;
      g->players[who - 1].students[a.disciplineTo] += 1;
   }
}

// 1 - Shawn (done)
int getDiscipline (Game g, int regionID){
   if (DEBUG_MODE) {
      printf("getDiscipline() called, regionID = %d\n", regionID);
   }
   int discipline = g->discipline[regionID];
   return discipline;
}

// 2 - Shawn (done)
int getDiceValue (Game g, int regionID){
   if (DEBUG_MODE) {
      printf("getDiceValue() called, regionID = %d\n", regionID);
   }
   int diceValue = g->dice[regionID];
   return diceValue;
}

// 3 - Shawn (done)
int getMostARCs (Game g){
   if (DEBUG_MODE) {
      printf("getMostARCs() called\n");
   }
   int mostARCs = g->mostARCs;
   return mostARCs;
}

// 4 - Shawn (done)
int getMostPublications (Game g){
   if (DEBUG_MODE) {
      printf("getMostPublications() called\n");
   }
   int mostPublications = g->mostPublications;
   return mostPublications;
}

// 5 - Shawn (done)
int getTurnNumber (Game g){
   if (DEBUG_MODE) {
      printf("getTurnNumber() called\n");
   }
   int turnNumber = g->currentTurn;
   return turnNumber;
}

// 6 - Shawn (done)
int getWhoseTurn (Game g){
   if (DEBUG_MODE) {
      printf("getWhoseTurn() called\n");
   }
   int whoseTurn = 0;
   if (getTurnNumber(g) == TERRA_NULLIS) {
      whoseTurn = NO_ONE;
   } else {
      whoseTurn = getTurnNumber(g) % 3 + UNI_A;
   }
   return whoseTurn;
}

// 7 - Matt (done)
int getCampus(Game g, path pathToVertex){
   if (DEBUG_MODE) {
      printf("getCampus() called, pathToVertex = \"%s\"\n", pathToVertex);
   }
   int x, y, dir;
   int campus = -1;
   // follows path to determine index in array stores the vertex, returns that
   if (traverseMap(pathToVertex, &x, &y, &dir)) {
      // then using the row and columns index:
      campus = g->vertices[x][y].campus;
   } else {
      printf("You returned to the sea.\n");
   }
   return campus;
}

// 8 - Matt (done)
int getARC(Game g, path pathToEdge){
   if (DEBUG_MODE) {
      printf("getARC() called, pathToEdge = \"%s\"\n", pathToEdge);
   }
   int x, y, dir;
   int arc = -1;
   // follows path to determine index in array stores the vertex, returns that
   if(traverseMap(pathToEdge, &x, &y, &dir)) {
      // then using the row and columns index:
      if (dir == 'U') {
         arc = g->vertices[x][y].arcV;
      } else if (dir == 'D') {
         arc = g->vertices[x][y-1].arcV;
      } else if (dir == 'R') {
         arc = g->vertices[x][y].arcH;
      } else if (dir == 'L') {
         arc = g->vertices[x-1][y].arcH;
      } else {
         printf("You can't specify an edge without a path.\n");
      }
   } else {
      printf("You returned to the sea.\n");
   }
   return arc;
}

// 9 - (notShawn) Matt's now (done)
int isLegalAction (Game g, action a){
   if (DEBUG_MODE) {
      printf("isLegalAction() called, a = { %d, \"%s\", %d, %d }\n",
             a.actionCode, a.destination, a.disciplineFrom, a.disciplineTo);
   }
   //hell
   //Look at my lovely shiny code for handling actions. It's like some
   //kind of /switch/ that handles a bunch of different /cases/ to
   //determine what needs to be done. If only there were some kind of
   ///switch/ syntax that's there specifically to handle situations where
   //you have a bunch of different /cases/ to handle.
   
   int isLegal;
   
   int whoseTurn = getWhoseTurn(g);
   if (whoseTurn != NO_ONE) {
      if (a.actionCode == PASS) {
         //I wish you could return from more than one place in the code.
         //Sigh.
         
         isLegal = 1;
      } else if (a.actionCode == BUILD_CAMPUS) {
         //Writing this got so out of hand I made it it's own function.
         isLegal = isCampusLegal(g, a.destination);
      } else if (a.actionCode == BUILD_GO8) {
         //Much simpler. If they have the resources, and the path is legal,
         //and the specified campus is a standard campus belonging to them.
         int x, y, dir;
         isLegal = (g->players[whoseTurn - 1].students[STUDENT_MJ] >= 2) &&
         (g->players[whoseTurn - 1].students[STUDENT_MMONEY] >= 3) &&
         traverseMap(a.destination,&x,&y,&dir) &&
         (g->vertices[x][y].campus == whoseTurn) &&
         ((getGO8s(g,UNI_A) + getGO8s(g,UNI_B) + getGO8s(g,UNI_C)) < 8);
      } else if (a.actionCode == OBTAIN_ARC) {
         //I'm breaking this out into it's own function preemptively.
         isLegal = isArcLegal(g, a.destination);
      } else if (a.actionCode == START_SPINOFF) {
         //Well, if you have the resources, nothing's stopping you.
         //Also I don't have to explicitly handle the others two related
         //ones because the, as one might call the syntax for a case statement,
         //default, perhaps, is to set isLegal to 0.
         isLegal = (g->players[whoseTurn - 1].students[STUDENT_MJ] >= 1) &&
         (g->players[whoseTurn - 1].students[STUDENT_MMONEY] >= 1) &&
         (g->players[whoseTurn - 1].students[STUDENT_MTV] >= 1);
      } else if (a.actionCode == RETRAIN_STUDENTS) {
         //IF you are retraining a valid student other than THDs to a valid
         //student type that exists and you have enough, then you can.
         isLegal = (a.disciplineFrom > STUDENT_THD) &&
         (a.disciplineFrom <= STUDENT_MMONEY) &&
         (a.disciplineTo >= STUDENT_THD) &&
         (a.disciplineTo <= STUDENT_MMONEY) &&
         (g->players[whoseTurn - 1].students[a.disciplineFrom] >=
          getExchangeRate(g,whoseTurn,a.disciplineFrom,a.disciplineTo));
      } else {
         isLegal = 0;
      }
   } else {
      isLegal = 0;
   }
   return isLegal;
}

static int isCampusLegal(Game g, path p) {
   //more hell
   int x, y, dir;
   int form;
   int isLegal = 0; //to save writing 500 elses
   int whoseTurn = getWhoseTurn(g);
   
   //If they have the resources.
   if ((g->players[whoseTurn - 1].students[STUDENT_BPS] >= 1) &&
       (g->players[whoseTurn - 1].students[STUDENT_BQN] >= 1) &&
       (g->players[whoseTurn - 1].students[STUDENT_MJ ] >= 1) &&
       (g->players[whoseTurn - 1].students[STUDENT_MTV] >= 1)) {
      //If the path is legal...
      if (traverseMap(p,&x,&y,&dir)) {
         //And there isn't already a campus here.
         if (g->vertices[x][y].campus == VACANT_VERTEX) {
            //Still also needs a neighbouring road
            if ((g->vertices[x][y].arcH == whoseTurn) ||
                (g->vertices[x][y].arcV == whoseTurn) ||
                ((x > 0) && (g->vertices[x-1][y].arcH == whoseTurn)) ||
                ((y > 0) && (g->vertices[x][y-1].arcV == whoseTurn))) {
               //AND the there are no campuses on adjacent nodes, which
               //would make it too close to build one, BUT making sure
               //we only consider the adjacent campus that is actually
               //connected by a valid ARC position.
               form = ((x + y) % 2) * 2 - 1;
               if (((y == 10) || (g->vertices[x][y+1].campus == VACANT_VERTEX)) &&
                   ((y ==  0) || (g->vertices[x][y-1].campus == VACANT_VERTEX)) &&
                   ((form ==  1) || (x == 10) || (g->vertices[x+1][y].campus == VACANT_VERTEX)) &&
                   ((form == -1) || (x ==  0) || (g->vertices[x-1][y].campus == VACANT_VERTEX))) {
                  //Then, finally, the campus is legal.
                  isLegal = 1;
               }
            }
         }
      }
   }
   return isLegal;
}

static int isArcLegal(Game g, path p) {
   int x, y, dir;
   int isLegal = 0; //to save writing 500 elses
   int who = getWhoseTurn(g);
   
   //If they have the resources.
   if ((g->players[who - 1].students[STUDENT_BPS] >= 1) &&
       (g->players[who - 1].students[STUDENT_BQN] >= 1)) {
      //If the path is legal and actually points to an edge.
      if (traverseMap(p,&x,&y,&dir) && (dir != '\0')) {
         // extended path strings for adjacent ARCs
         char adj[4][PATH_LIMIT + 2];
         sprintf(adj[0], "%sL", p);
         sprintf(adj[1], "%sR", p);
         sprintf(adj[2], "%sBL", p);
         sprintf(adj[3], "%sBR", p);
         
         // extended path strings for the two campuses at the end of the ARC
         char ends[2][PATH_LIMIT+1];
         sprintf(ends[0], "%s", p);
         sprintf(ends[1], "%sB", p);
         
         //If the arc in question is vacant AND
         if (getARC(g, p) == NO_ONE) {
            //If they own a campus at either end of the edge.
            if (getCampus(g, ends[0]) == who || getCampus(g, ends[0]) == who + 3 ||
                getCampus(g, ends[1]) == who || getCampus(g, ends[1]) == who + 3) {
               isLegal = 1;
            }
            //OR if they own a path connected to either end of the edge
            //AND nobody owns a campus at that end of the edge.
            if ((traverseMap(adj[0], &x, &y, &dir) && getARC(g, adj[0]) == who) ||
                (traverseMap(adj[1], &x, &y, &dir) && getARC(g, adj[1]) == who)) {
               if (getCampus(g, ends[0]) == NO_ONE) {
                  isLegal = 1;
               }
            }
            if ((traverseMap(adj[2], &x, &y, &dir) && getARC(g, adj[2]) == who) ||
                (traverseMap(adj[3], &x, &y, &dir) && getARC(g, adj[3]) == who)) {
               if (getCampus(g, ends[1]) == NO_ONE) {
                  isLegal = 1;
               }
            }
         }
      }
   }
   return isLegal;
}

// 10 - Zihan (done)
int getKPIpoints (Game g, int player){
   if (DEBUG_MODE) {
      printf("getKPIpoints() called, player = %d\n", player);
   }
   //calculates based on number of campuses, number of GO8s, arcs, most arcs,
   //most publications,
   int points = 0;
   
   points += (getCampuses(g, player) * 10);
   points += (getGO8s(g, player) * 20);
   points += (getARCs(g, player) * 2);
   points += (getIPs(g, player) * 10);
   
   if(getMostARCs(g) == player){
      points += 10;
   }
   
   if(getMostPublications(g) == player){
      points += 10;
   }
   
   return points;
}

// 11 - Shawn (done)
int getARCs (Game g, int player){
   if (DEBUG_MODE) {
      printf("getARCs() called, player = %d\n", player);
   }
   int arcs = g->players[player-UNI_A].arcs;
   return arcs;
}

// 12 - Shawn (done)
int getGO8s (Game g, int player){
   if (DEBUG_MODE) {
      printf("getGO8s() called, player = %d\n", player);
   }
   int go8s = g->players[player-UNI_A].go8s;
   return go8s;
}

// 13 - Shawn (done)
int getCampuses (Game g, int player){
   if (DEBUG_MODE) {
      printf("getCampuses() called, player = %d\n", player);
   }
   int campuses = g->players[player-UNI_A].campuses;
   return campuses;
}

// 14 - Shawn (done)
int getIPs (Game g, int player){
   if (DEBUG_MODE) {
      printf("getIPs() called, player = %d\n", player);
   }
   int patents = g->players[player-UNI_A].patents;
   return patents;
}

// 15 - Shawn (done)
int getPublications (Game g, int player){
   if (DEBUG_MODE) {
      printf("getPublications() called, player = %d\n", player);
   }
   int publications = g->players[player-UNI_A].publications;
   return publications;
}

// 16 - Shawn (done)
int getStudents (Game g, int player, int discipline){
   if (DEBUG_MODE) {
      printf("getStudents() called, player = %d, discipline = %d\n", player, discipline);
   }
   int students = g->players[player-UNI_A].students[discipline];
   return students;
}

// 17 - Matt (done)
int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo){
   if (DEBUG_MODE) {
      printf("getExchangeRate() called, player = %d, disciplineFrom = %d, disciplineTo = %d\n",
             player, disciplineFrom, disciplineTo);
   }
   // since retraining centres are constant*, uses vertex array to see if player
   // has a campus at a retraining center, and if so, return 2, else return 3
   // *but I'm not allowed to use that keyword and it's not as useful as you'd
   // think to the optimiser anyway :,(
   int retCenters[5][2][2] = { { {1,1}, {1,2} },
      { {5,4}, {5,5} },
      { {4,1}, {4,2} },
      { {1,9}, {2,9} },
      { {3,9}, {4,9} } };
   int hasRetCenter = 0;
   int exchangeRate = 3;
   if (disciplineFrom >= STUDENT_BPS && disciplineFrom <= STUDENT_MMONEY) {
      int i = 0;
      while (i < 2) {
         int* coord = retCenters[disciplineFrom-STUDENT_BPS][i];
         int campus = g->vertices[coord[0]][coord[1]].campus;
         hasRetCenter = hasRetCenter ||
         (campus == player) || (campus == player+3);
         ++i;
      }
      if (hasRetCenter) {
         exchangeRate = 2;
      }
   }
   return exchangeRate;
}

static int traverseMap(path pathTo, int *i, int *j, int *dir) {
   //0 is uppy, 1 is downy, 2 is sideways.
   int dirFrom = 0;
   int x = 2;
   int y = 10;
   int k = 0;
   int sanity = 1;
   
   
   //printf("Testing %s\n",pathTo);
   while (pathTo[k] != '\0') {
      int pathDir = pathTo[k];
      //R is -1, L is 1, and B is 0
      if (pathDir == 'R') {
         pathDir = -1;
      } else if (pathDir == 'L') {
         pathDir = 1;
      } else {
         pathDir = 0;
      }
      //Form A is -1, Form B is 1.
      int form = ((x + y) % 2) * 2 - 1;
      //+1 is counterclockwise in form A, and clockwise in form B
      //right is counterclockwise, left is clockwise, back is no change
      //multiplying pathDir by form will orient it correctly
      //the loop prior to this line was written backwards
      int dirTo = (form * pathDir + dirFrom + 3) % 3;
      dirFrom = (dirTo * 2 + 1) % 3;
      //printf("%d\n",pathTo[k]);
      //printf("Testing %s\n",pathTo);
      //printf("Step %d, Gone from (%d,%d) to ",k,x,y);
      if (dirTo == 0) {
         ++y;
      } else if (dirTo == 1) {
         --y;
      } else {
         x -= form;
      }
      //printf("(%d,%d)\n",x,y);
      //If path so far has been valid, test if the new position is
      //inside bounding rectangle.
      sanity = sanity && (y <= 10) && (y >= 0) && (x <= 5) && (x >= 0) &&
      //If inside bounding rectangle, test if inside ocean inside
      //bounding rectangle
      ((x + y) > 1) && (((5-x) + y) > 1) && ((x + (10-y)) > 1) &&
      (((5-x) + (10-y)) > 1);
      //Also test if in ocean within bounding rectangle.
      
      ++k;
   }
   //Return x and y through their pointers.
   *i = x;
   *j = y;
   //Work out the direction from the point specified that the edge specified
   //is located. Note that an empty string will result in the sanity check
   //passing, and a valid path to a vertex, but the edge specified will be
   //invalid.
   if (pathTo[0] != '\0') {
      if (dirFrom == 0) {
         *dir = 'U';
      } else if (dirFrom == 1) {
         *dir = 'D';
      }
      else {
         if ((x + y) % 2) {
            *dir = 'L';
         } else {
            *dir = 'R';
         }
      }
   } else {
      *dir = '\0';
   }
   return sanity;
}