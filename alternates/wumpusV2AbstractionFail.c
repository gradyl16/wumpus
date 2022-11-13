/*
	Program 2: Wumpus, version 2
	CS 211, UIC, Fall 2022
	System: Replit
	Authors: Dylen Greenenwald, Dale Reed, Ellen Kidane

	Hunt the Wumpus is a classic text-based adventure game by Gregory Yob from 1972.
	The Wumpus lives in a completely dark cave of 20 rooms.  Each room has 3 tunnels leading to other rooms.
*/

#include <stdio.h>
#include <stdbool.h>  // for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()

// Global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

int **Rooms;

//--------------------------------------------------------------------------------
// Previous Rooms init code below
//--------------------------------------------------------------------------------
// Room connection values are set as global constants, and don't need to be passed.
// The rooms array is indexed from 0..22, but we don't use the 0th entry and 
// only use 1..20.  This approach helps prevent off-by-one indexing errors.
//const int Rooms[ 21][ 3] = {
      // Room 0 is unused, to help avoid off-by-one indexing errors.
                            //{0,0,0},
      //                    Room 1     Room 2      Room 3      Room 4      Room 5
      /* Rooms 1-5   */     //{2,5,8},  {1,3,10},   {2,4,12},   {3,5,14},    {1,4,6},
      //                    Room 6     Room 7      Room 8      Room 9     Room 10
      /* Rooms 6-10  */    //{5,7,15},  {6,8,17},    {1,7,9},  {8,10,18},   {2,9,11},
      //                   Room 11    Room 12     Room 13     Room 14     Room 15
      /* Rooms 11-15 */  //{10,12,19}, {3,11,13}, {12,14,20},  {4,13,15},  {6,14,16},
      //                   Room 16    Room 17     Room 18     Room 19     Room 20
      /* Rooms 16-20 */  //{15,17,20}, {7,16,18},  {9,17,19}, {11,18,20}, {13,16,19},
//};
//--------------------------------------------------------------------------------

// Used to more conveniently pass all game information between functions.
struct GameInfo
{
    int moveNumber;  // Counts up from 1, incrementing for each move
    int personRoom;  // Room 1..20 the person currently is in
    int wumpusRoom;  // Room 1..20 the Wumpus is in
    int pitRoom1;    // Room 1..20 the first pit is in
    int pitRoom2;    // Room 1..20 the second pit is in
    int batsRoom1;   // Room 1..20 the first set of super bats are in
    int batsRoom2;   // Room 1..20 the second set of super bats are in
    int arrowRoom;   // Room 1..20 the arrow is in
};

// Function prototype needed to allow calls in any order between
// functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *personIsAlive);

//--------------------------------------------------------------------------------
// Return true if randomValue is already in array.
int alreadyFound(int randomValue,    // New number we're checking
                 int randomRooms[],  // Set of numbers previously found
                 int limit)          // How many numbers previously found
{
    int returnValue = false;
    
    // Compare random value against all previously found values
    int i;
    for(i = 0; i < limit; i++ )
    {
        if(randomValue == randomRooms[i])
        {
          returnValue = true;   // It is already there
          break;
        }
    }
    
    return returnValue;
}// end alreadyFound(...)


//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20.
void setUniqueValues(int randomRooms[],  // Array of random numbers
                     int size)           // Size of random numbers array
{
    int randomValue = -1;
    
    for( int i = 0; i < size; i++ )
    {
        do
        {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while(alreadyFound(randomValue, randomRooms, i));
      
        randomRooms[i] = randomValue;
    }
}// end setUniqueValues(...)


//--------------------------------------------------------------------------------
// Assigns the person, wumpus, pits, bats and arrow to distinct random rooms.
void initializeGame(struct GameInfo *gameInfo)  // All game settings variables
{
    // Allocates space for 21 rooms in the Wumpus cave with room 0 being unused
    Rooms = (int**) malloc(sizeof(int*) * NUMBER_OF_ROOMS + 1);

    // Loops through rooms to allocate space for 3 connections to each room
    int i;
    for(i = 0; i < NUMBER_OF_ROOMS + 1; i++)
    {
        Rooms[i] = (int*) malloc(sizeof(int) * 3);
    }
  
    // Allocates space for 6 unique values 1..20, to be used in initializing cave hazards locations
    int *randomRooms = (int*) malloc(sizeof(int) * 7);
    
    // Initializes cave room connections
    //       ______18______
    //      /      |       \
    //     /      _9__      \
    //    /      /    \      \
    //   /      /      \      \
    //  17     8        10    19
    // |  \   / \      /  \   / |
    // |   \ /   \    /    \ /  |
    // |    7     1---2     11  |
    // |    |    /     \    |   |
    // |    6----5     3---12   |
    // |    |     \   /     |   |
    // |    \       4      /    |
    // |     \      |     /     |
    //  \     15---14---13     /
    //   \   /            \   /
    //    \ /              \ /
    //    16---------------20
    Rooms[0][0] = 0;   Rooms[0][1] = 0;   Rooms[0][2] = 0;    // Room 0 (unused) prevents indexing errors
    Rooms[1][0] = 2;   Rooms[1][1] = 5;   Rooms[1][2] = 8;    // Room 1 connections
    Rooms[2][0] = 1;   Rooms[2][1] = 3;   Rooms[2][2] = 10;   // Room 2 connections
    Rooms[3][0] = 2;   Rooms[3][1] = 4;   Rooms[3][2] = 12;   // Room 3 connections
    Rooms[4][0] = 3;   Rooms[4][1] = 5;   Rooms[4][2] = 14;   // Room 4 connections
    Rooms[5][0] = 1;   Rooms[5][1] = 4;   Rooms[5][2] = 6;    // Room 5 connections
    Rooms[6][0] = 5;   Rooms[6][1] = 7;   Rooms[6][2] = 15;   // Room 6 connections
    Rooms[7][0] = 6;   Rooms[7][1] = 8;   Rooms[7][2] = 17;   // Room 7 connections
    Rooms[8][0] = 1;   Rooms[8][1] = 7;   Rooms[8][2] = 9;    // Room 8 connections
    Rooms[9][0] = 8;   Rooms[9][1] = 10;  Rooms[9][2] = 18;   // Room 9 connections
    Rooms[10][0] = 2;  Rooms[10][1] = 9;  Rooms[10][2] = 11;  // Room 10 connections
    Rooms[11][0] = 10; Rooms[11][1] = 12; Rooms[11][2] = 19;  // Room 11 connections
    Rooms[12][0] = 3;  Rooms[12][1] = 11; Rooms[12][2] = 13;  // Room 12 connections
    Rooms[13][0] = 12; Rooms[13][1] = 14; Rooms[13][2] = 20;  // Room 13 connections
    Rooms[14][0] = 4;  Rooms[14][1] = 13; Rooms[14][2] = 15;  // Room 14 connections
    Rooms[15][0] = 6;  Rooms[15][1] = 14; Rooms[15][2] = 16;  // Room 15 connections
    Rooms[16][0] = 15; Rooms[16][1] = 17; Rooms[16][2] = 20;  // Room 16 connections
    Rooms[17][0] = 7;  Rooms[17][1] = 16; Rooms[17][2] = 18;  // Room 17 connections
    Rooms[18][0] = 9;  Rooms[18][1] = 17; Rooms[18][2] = 19;  // Room 18 connections
    Rooms[19][0] = 11; Rooms[19][1] = 18; Rooms[19][2] = 20;  // Room 19 connections
    Rooms[20][0] = 13; Rooms[20][1] = 16; Rooms[20][2] = 19;  // Room 20 connections
      
    // Select some unique random values 1..20 to be used for the person room,
    // the Wumpus room, 2 pit rooms, 2 bat rooms and the arrow room
    setUniqueValues(randomRooms, 7);
  
    // Initializes the move number to 1 and uses the unique random numbers to set
    // initial locations of hazards, which should be non-overlapping.
    gameInfo->moveNumber = 1;
    gameInfo->personRoom = randomRooms[0];
    gameInfo->wumpusRoom = randomRooms[1];
    gameInfo->pitRoom1 = randomRooms[2];   
    gameInfo->pitRoom2 = randomRooms[3];
    gameInfo->batsRoom1 = randomRooms[4];
    gameInfo->batsRoom2 = randomRooms[5];
    gameInfo->arrowRoom = randomRooms[6];
}// end initializeGame(...)


//--------------------------------------------------------------------------------
// Display room number and hazards detected.
void displayRoomInfo(struct GameInfo gameInfo)
{
    // Retrieve person's current room number and display it
    int currentRoom = gameInfo.personRoom;
    printf("You are in room %d. ", currentRoom);
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = Rooms[currentRoom][0];
    int room2 = Rooms[currentRoom][1];
    int room3 = Rooms[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if(room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom)
    {
        printf("You smell a stench. ");
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if(room1 == pit1Room || room1 == pit2Room ||
       room2 == pit1Room || room2 == pit2Room ||
       room3 == pit1Room || room3 == pit2Room
      )
    {
        printf("You feel a draft. ");
    }

    // Display hazard detection message if a set of bats are in an adjacent room
    int bats1Room = gameInfo.batsRoom1;
    int bats2Room = gameInfo.batsRoom2;
    if(room1 == bats1Room || room1 == bats2Room ||
       room2 == bats1Room || room2 == bats2Room ||
       room3 == bats1Room || room3 == bats2Room
      )
    {
        printf("You hear rustling of bat wings. ");
    }
    
    printf("\n\n");
}//end displayRoomInfo(...)


//-------------------------------------------------------------------------
// Returns true if nextPlayerRoom is adjacent to current room, else returns false.
int roomIsAdjacent(int tunnels[3],      // Array of adjacent tunnels
                   int nextPlayerRoom)  // Desired room to move to
{
    return(tunnels[0] == nextPlayerRoom ||
           tunnels[1] == nextPlayerRoom ||
           tunnels[2] == nextPlayerRoom
          );
}// end roomIsAdjacent(...)


//-----------------------------------------------------------------------
// Bats transport person to a random room in the cave, then bats fly to a
// different random room that is also not the same room as their original
// room or the other bats' room.
void inRoomWithBats(struct GameInfo *gameInfo)
{
    // Makes copy of previous person location
    int personRoom = gameInfo->personRoom;
    // Bats transport person to random room
    gameInfo->personRoom = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20

    // Makes copy of previous bat locations
    int batsOldRoom1 = gameInfo->batsRoom1;
    int batsOldRoom2 = gameInfo->batsRoom2;
    // Bats fly to a new location
    if(personRoom == gameInfo->batsRoom1)
    {
        do
        {
            gameInfo->batsRoom1 = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while(gameInfo->batsRoom1 == batsOldRoom1 ||
                gameInfo->batsRoom1 == batsOldRoom2 ||
                gameInfo->batsRoom1 == personRoom);
    }
    else if(personRoom == gameInfo->batsRoom2)
    {
        do
        {
            gameInfo->batsRoom2 = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while(gameInfo->batsRoom2 == batsOldRoom1 ||
                gameInfo->batsRoom2 == batsOldRoom2 ||
                gameInfo->batsRoom2 == personRoom);
    }
}// end inRoomWithBats(...)


//--------------------------------------------------------------------------------
// If the person just moved into a room with a pit, the person dies.
// If the person just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
// If the person just moved into a room with bats, the person is transported to a
// new room and the bats fly to a different room from the person, the other bats,
// and their original room.
// If the person just moved into the room with the arrow, they pick it up.
void checkForHazards(struct GameInfo *gameInfo,  // Hazards location and game info
                     bool *personIsAlive)        // Person is alive, but could die depending on the hazards
{
    // Retrieve the room the person is in
    int personRoom = gameInfo->personRoom;

    // Check for the Wumpus
    if(personRoom == gameInfo->wumpusRoom) 
    {
        // To make it easier to test, in this version of the program the Wumpus always
        // moves if it is currently in an odd-numbered room, and it moves into the
        // lowest-numbered adjacent room.  In the version that is more fun to play
        // (but harder to test), the Wumpus has a 75% chance of moving, and a 25%
        // chance of staying and killing you.  The "more fun" code is commented out below.
        
        // Wumpus is there. 75% change of Wumpus moving, 25% chance of it killing you
        // Generate a random number 1..100
        // if(  (rand() % 100) < 75) {
        if(gameInfo->wumpusRoom % 2 == 1)
        {
            // You got lucky and the Wumpus moves away
            printf("You hear a slithering sound, as the Wumpus slips away. \n"
					         "Whew, that was close! \n");
            // Choose a random adjacent room for the Wumpus to go into
            // gameInfo.wumpusRoom = rooms[personRoom][rand() % 3];
            gameInfo->wumpusRoom = Rooms[personRoom][0];  // Choose the lowest-numbered adjacent room
        }
        else
        {
            // Wumpus kills you
            printf("You briefly feel a slimy tentacled arm as your neck is snapped. \n"
					         "It is over.\n");
            *personIsAlive = false;
            return;
        }
    }
  
    // Check whether there is a pit
    if(personRoom == gameInfo->pitRoom1 || personRoom == gameInfo->pitRoom2)
    {
      // Person falls into pit
      printf("Aaaaaaaaahhhhhh....   \n");
      printf("    You fall into a pit and die. \n");
      *personIsAlive = false;
      return;
    }

    // Check whether there are super bats
    if(personRoom == gameInfo->batsRoom1 || personRoom == gameInfo->batsRoom2)
    { 
      inRoomWithBats(gameInfo);
      
      // Message when bats transport person
	    printf("Woah... you're flying! \n");
      printf("You've just been transported by bats to room %d.\n", gameInfo->personRoom);
    }

    // Check for the arrow
    if(personRoom == gameInfo->arrowRoom)
    {
      printf("Congratulations, you found the arrow and can once again shoot.\n");
      gameInfo->arrowRoom = -1;
    } 
}//end checkForHazards(...)


//--------------------------------------------------------------------------------
// Sets flags based on whether or not the person or the wumpus remain alive after
// the arrow travels through a given room
void checkLifeStatus(struct GameInfo *gameInfo,  // Hazards and game info
                  bool *personIsAlive,        // Person is alive, but could die if arrow hits
                  bool *wumpusIsAlive)        // Wumpus is alive, but could die if arrow hits
{
  // Wumpus is shot
  if(gameInfo->arrowRoom == gameInfo->wumpusRoom)
  {
    *wumpusIsAlive = false;
  }
  // Person is shot
  else if(gameInfo->arrowRoom == gameInfo->personRoom)
  {
    *personIsAlive = false;
  }
}// end checkLifeStatus(...)


//--------------------------------------------------------------------------------
// Shoots an arrow across up to 3 adjacent rooms.
// If the player or Wumpus is it, they die.
// If nobody dies, the arrow is left in the last room it traveled through.
void checkForKill(struct GameInfo *gameInfo,
                int *nextArrowRoom,
                bool *personIsAlive,
                bool *wumpusIsAlive)
{
    // Set initial current arrow room equal to the room the player shoots it from
    gameInfo->arrowRoom = gameInfo->personRoom;
    int arrowRoom = gameInfo->arrowRoom;         // for legibility

    // Check if the room to shoot through next is adjacent to the arrow's current room
    if(roomIsAdjacent(Rooms[arrowRoom], nextArrowRoom))
    {
        // If so, set arrow room equal to user selection
        gameInfo->arrowRoom = *nextArrowRoom;

        // Check if either the person or wumpus died, displays appropriate messages if so
        checkLifeStatus(gameInfo, personIsAlive, wumpusIsAlive);
        if(!wumpusIsAlive)
        {
            // Arrow killed Wumpus
	          printf("Wumpus has just been pierced by your deadly arrow! \n"
                   "Congratulations on your victory, you awesome hunter you.\n");
            return;
        }
        else if(!personIsAlive)
        {
            // Arrow killed person
	          printf("You just shot yourself.  \n"
                   "Maybe Darwin was right.  You're dead.\n");
            return;
        }
    }
    else
    {
        // Arrow ricochets to the lowest numbered adjacent room
        gameInfo->arrowRoom = Rooms[arrowRoom][0];
	      printf("Room %d is not adjacent.  Arrow ricochets...\n", *nextArrowRoom);
            
        checkLifeStatus(gameInfo, personIsAlive, wumpusIsAlive);
        if(!wumpusIsAlive)
        {
            // Arrow ricochets, and kills Wumpus
	          printf("Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                   "Accidental victory, but still you win!\n");
            return;
        }
        else if(!personIsAlive)
        {
            // Arrow ricochets, and kills person
            printf("You just shot yourself, and are dying.\n"
                   "It didn't take long, you're dead.\n");
            return;
        }
    }
}// end checkForKill(...)
//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the person's location,
// useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo)
{
  printf("Enter the room locations (1..20) for person, wumpus, pit1, pit2, bats1, bats2, and arrow: \n");
	// In the scanf below note that we put the space at the beginning of the scanf so  
  // that any newline left over from a previous input is not read in and used as 
	// the next move. Another option is having getchar() after the scanf() statement.
	scanf(" %d %d %d %d %d %d %d", &theGameInfo->personRoom, &theGameInfo->wumpusRoom, 
                                 &theGameInfo->pitRoom1, &theGameInfo->pitRoom2,
                                 &theGameInfo->batsRoom1, &theGameInfo->batsRoom2,
                                 &theGameInfo->arrowRoom);
  printf("\n");
}// end resetPositions(...)


//--------------------------------------------------------------------------------
// Display where everything is in the cave.
void displayCheatInfo(struct GameInfo gameInfo)
{
    printf("Cheating! Game elements are in the following rooms: \n"
    		   "Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
    		   "%4d %5d %6d %5d %5d %5d %5d \n\n", 
			      gameInfo.personRoom,
			      gameInfo.wumpusRoom,
			      gameInfo.pitRoom1,
			      gameInfo.pitRoom2,
            gameInfo.batsRoom1,
            gameInfo.batsRoom2,
            gameInfo.arrowRoom
		      );
}// end displayCheatInfo(...)


//--------------------------------------------------------------------------------
// Displays the layout of the Wumpus' cave.
void displayCave()
{
    printf( "\n"
			"       ______18______             \n"
    		"      /      |       \\           \n"
    		"     /      _9__      \\          \n"
    		"    /      /    \\      \\        \n"
    		"   /      /      \\      \\       \n"
    		"  17     8        10     19       \n"
    		"  | \\   / \\      /  \\   / |    \n"
    		"  |  \\ /   \\    /    \\ /  |    \n"
    		"  |   7     1---2     11  |       \n"
    		"  |   |    /     \\    |   |      \n"
    		"  |   6----5     3---12   |       \n"
    		"  |   |     \\   /     |   |      \n"
    		"  |   \\       4      /    |      \n"
    		"  |    \\      |     /     |      \n"
    		"  \\     15---14---13     /       \n"
    		"   \\   /            \\   /       \n"
    		"    \\ /              \\ /        \n"
    		"    16---------------20           \n"
    		"\n");
}// end displayCave()


//--------------------------------------------------------------------------------
// Displays the set of instructions that teaches user how to play the game.
void displayInstructions()
{
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
        	"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
        	"2. Two other rooms have super-bats.  If you go there, the bats grab you and     \n"
        	"   fly you to some random room, which could be troublesome.  Then those bats go \n"
        	"   to a new room different from where they came from and from the other bats.   \n"
    		"3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    \n"
        	"   is too heavy for bats to lift.  Usually he is asleep.  Two things wake       \n"
        	"    him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   \n"
        	"    will move into the lowest-numbered adjacent room anytime you shoot an arrow.\n"
        	"    When you move into the Wumpus' room, then he wakes and moves if he is in an \n"
        	"    odd-numbered room, but stays still otherwise.  After that, if he is in your \n"
        	"    room, he snaps your neck and you die!                                       \n"
        	"                                                                                \n"
        	"Moves:                                                                          \n"
        	"On each move you can do the following, where input can be upper or lower-case:  \n"
        	"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
        	"   then a room number.                                                          \n"
        	"2. Shoot your guided arrow through a list of up to three adjacent rooms, which  \n"
        	"   you specify.  Your arrow ends up in the final room.                          \n"
        	"   To shoot enter 'S' followed by the number of rooms (1..3), and then the      \n"
        	"   list of the desired number (up to 3) of adjacent room numbers, separated     \n"
        	"   by spaces. If an arrow can't go a direction because there is no connecting   \n"
        	"   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      \n"
        	"   continues doing this until it has traveled the designated number of rooms.   \n"
        	"   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  \n"
        	"   automatically pick up the arrow if you go through a room with the arrow in   \n"
        	"   it.                                                                          \n"
        	"3. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
        	"4. Enter 'C' to cheat and display current board positions.                      \n"
        	"5. Enter 'D' to display this set of instructions.                               \n"
        	"6. Enter 'P' to print the maze room layout.                                     \n"
        	"7. Enter 'X' to exit the game.                                                  \n"
        	"                                                                                \n"
        	"Good luck!                                                                      \n"
        	" \n\n");
}//end displayInstructions()


//--------------------------------------------------------------------------------
int main(void)
{
    struct GameInfo gameInfo;   // Used to more easily pass game info variables around
    bool personIsAlive = true;  // Used in checking for end of game
    bool wumpusIsAlive = true;  // Used in checking for end of game
    char typeOfMove;            // Used to handle user input letter
    int nextPlayerRoom;         // User input of destination room number, used on a 'M' type move
    int arrowDisplacement;      // User input of arrow distance (in rooms), used on an 'S' type move
    int nextArrowRoom;          // User input of next room number, used on an an 'S' type move
    
    // Seed the random number generator.  Change seed to change output each time.
    // srand(time(0)); <-- Other seeds could be used for better pseudorandomness
    srand(1);
    
    // Set random initial values for person, Wumpus, bats and pits
    initializeGame(&gameInfo);

    // Main playing loop; Break when person dies, or person kills Wumpus.
    while (personIsAlive && wumpusIsAlive)
    {
        // Display current room information: Room number, hazards detected
        displayRoomInfo(gameInfo);
		
        // Prompt for and handle move
        printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
        // Note the extra space in the scanf below between the opening quote " and the %c.  
        //    This skips leading white space in the input so that the newline left over from
        //    a previous move is not read in and used as the current move.  An alternative 
        //    is using getchar() after the scanf() statement.
        scanf(" %c", &typeOfMove);	
				
        typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking

        // Check all types of user input and handle them.  This uses if-else-if code
        // rather than switch-case so that we can take advantage of break and continue.
        if(typeOfMove == 'M')
        {
			      // Move to an adjacent room
            // Note the extra space in the scanf below between the opening quote " and the %c.  
            //    This skips leading white space in the input so that the newline left over from
            //    a previous move is not read in and used as the current move.  An alternative 
            //    is using getchar() after the scanf() statement.
			      scanf(" %d", &nextPlayerRoom);
			
			      if(roomIsAdjacent(Rooms[gameInfo.personRoom], nextPlayerRoom))
            {
				        gameInfo.personRoom = nextPlayerRoom;        // move to a new room
				        // Check if pit or wumpus is present in this new room
				        checkForHazards(&gameInfo, &personIsAlive);
			      }
			      else 
            {
				        printf("Invalid move.  Please retry. \n");
				        continue;  // Doesn't count as a move, so retry same move.
			      }
		    }
        // How can this code be modulated (converted to a function)?
        // P.S. That's why the whitespace is uneven -- I plan to change this section.
        else if(typeOfMove == 'S')
        {
            // Check if arrow is with the person
            if(gameInfo.arrowRoom == -1)
            {
                if(arrowDisplacement > 3)
                {
                    // User enters more than 3 rooms to shoot arrow through
	                  printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
                    arrowDisplacement = 3;
                }
                // Prompt user to enter input
	              printf("Enter the number of rooms (1..3) into which you want to shoot, "
                       "followed by the rooms themselves: ");
                scanf(" %d %d", &arrowDisplacement, &nextArrowRoom);

                int i;
                for(i = 0; i < arrowDisplacement; i++)
                {
                    checkForKill(&gameInfo, &nextArrowRoom, &personIsAlive, &wumpusIsAlive);
                    // Read next user room choice
                    scanf(" %d", &nextArrowRoom);
                }
              // Wumpus moves to lowest numbered adjacent room after arrow is shot
              gameInfo.wumpusRoom = Rooms[gameInfo.wumpusRoom][0];
            }
            else
            {
              // Attempt to shoot arrow you don't have
	            printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
            }
        }
		    else if(typeOfMove == 'R')
        {
			      // Specify resetting the hazards and person positions, useful for testing
			      resetPositions( &gameInfo);
			      continue;        // Doesn't count as a move, so retry same move.
        }
		    else if(typeOfMove == 'C')
        {
			      // Display Cheat information
			      displayCheatInfo(gameInfo);
			      continue;        // Doesn't count as a move, so retry same move.
        }
		    else if(typeOfMove == 'D')
        {
            // Display the instructions for the game; usually the first move selected by the player.
            // Placed down here for sake of code being analagous to instruction set; difference in
            // efficiency is negligible.
			      displayCave();
			      displayInstructions();
			      continue;       // Loop back up to reprompt for the same move
		    }
		    else if(typeOfMove == 'P')
        {
			      // To assist with play, display the cave layout
			      displayCave();
			      continue;       // Loop back up to reprompt for the same move
		    }
		    else if(typeOfMove == 'X')
        {
			      // Exit program
			      personIsAlive = false;   // Indicate person is dead as a way to exit playing loop
			      break;
		    }

		    // Increment the move number
		    gameInfo.moveNumber++;
        
    }//end while(personIsAlive && wumpusIsAlive)
    
    printf("\nExiting Program ...\n");
	
	return 0;
}