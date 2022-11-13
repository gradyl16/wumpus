/*
	Program 1: Wumpus, version 2 (dynamically allocated array with superbats and arrows)
	CS 211, UIC, Fall 2022
	System: Replit
	Author: Dale Reed and Ellen Kidane

	Hunt the Wumpus is a classic text-based adventure game by Gregory Yob from 1972.
	The Wumpus lives in a completely dark cave of 20 rooms.  Each room has 3 tunnels leading to other rooms.
	This program implements this game, with all arrays being dynamically allocated.

	Running the program looks like:

		You are in room 4. You hear rustling of bat wings. 

		1. Enter your move (or 'D' for directions): d

			 ______18______
            /      |       \
           /      _9__      \
          /      /    \      \
		 /      /      \      \
		17     8        10     19       
		| \   / \      /  \   / |    
		|  \ /   \    /    \ /  |    
		|   7     1---2     11  |       
		|   |    /     \    |   |      
		|   6----5     3---12   |       
		|   |     \   /     |   |      
		|   \       4      /    |      
		|    \      |     /     |      
		\     15---14---13     /       
		 \   /            \   /
          \ /              \ /
          16---------------20

		Hunt the Wumpus:                                             
		The Wumpus lives in a completely dark cave of 20 rooms. Each 
		room has 3 tunnels leading to other rooms.                   
																	
		Hazards:                                                     
		1. Two rooms have bottomless pits in them.  If you go there you fall and die.   
		2. Two other rooms have super-bats.  If you go there, the bats grab you and     
		fly you to some random room, which could be troublesome.  Then those bats go 
		to a new room different from where they came from and from the other bats.   
		3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    
		is too heavy for bats to lift.  Usually he is asleep.  Two things wake       
			him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   
			will move into the lowest-numbered adjacent room anytime you shoot an arrow.
			When you move into the Wumpus' room, then he wakes and moves if he is in an 
			odd-numbered room, but stays still otherwise.  After that, if he is in your 
			room, he snaps your neck and you die!                                       
																						
		Moves:                                                                          
		On each move you can do the following, where input can be upper or lower-case:  
		1. Move into an adjacent room.  To move enter 'M' followed by a space and       
		then a room number.                                                          
		2. Shoot your guided arrow through a list of up to three adjacent rooms, which  
		you specify.  Your arrow ends up in the final room.                          
		To shoot enter 'S' followed by the number of rooms (1..3), and then the      
		list of the desired number (up to 3) of adjacent room numbers, separated     
		by spaces. If an arrow can't go a direction because there is no connecting   
		tunnel, it ricochets and moves to the lowest-numbered adjacent room and      
		continues doing this until it has traveled the designated number of rooms.   
		If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  
		automatically pick up the arrow if you go through a room with the arrow in   
		it.                                                                          
		3. Enter 'R' to reset the player and hazard locations, useful for testing.      
		4. Enter 'C' to cheat and display current board positions.                      
		5. Enter 'D' to display this set of instructions.                               
		6. Enter 'P' to print the maze room layout.                                     
		7. Enter 'X' to exit the game.                                                  
																						
		Good luck!                                                                      
		

		You are in room 4. You hear rustling of bat wings. 

		1. Enter your move (or 'D' for directions): P

			 ______18______
            /      |       \
           /      _9__      \
          /      /    \      \
		 /      /      \      \
		17     8        10     19
		| \   / \      /  \   / |
		|  \ /   \    /    \ /  |
		|   7     1---2     11  |
		|   |    /     \    |   |
		|   6----5     3---12   |
		|   |     \   /     |   |
		|   \       4      /    |
		|    \      |     /     |
		\     15---14---13     /
		 \   /            \   /
          \ /              \ /
          16---------------20       

		You are in room 4. You hear rustling of bat wings. 

		1. Enter your move (or 'D' for directions): c
		Cheating! Game elements are in the following rooms: 
		Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  
		4     7     18    16    14    13    10 

		You are in room 4. You hear rustling of bat wings. 

		1. Enter your move (or 'D' for directions): m 3
		You are in room 3. 

		2. Enter your move (or 'D' for directions): M 12
		You are in room 12. You hear rustling of bat wings. 

		3. Enter your move (or 'D' for directions): s
		Sorry, you can't shoot an arrow you don't have.  Go find it.
		You are in room 12. You hear rustling of bat wings. 

		4. Enter your move (or 'D' for directions): m 13
		Woah... you're flying! 
		You've just been transported by bats to room 2.
		You are in room 2. You hear rustling of bat wings. 

		5. Enter your move (or 'D' for directions): m 10
		Congratulations, you found the arrow and can once again shoot.
		You are in room 10. 

		6. Enter your move (or 'D' for directions): s
		Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: 3 9 8 7
		Wumpus has just been pierced by your deadly arrow! 
		Congratulations on your victory, you awesome hunter you.

		Exiting Program ...
*/

#include <stdio.h>
#include <stdbool.h>	// for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

// Used to more conveniently pass all game information between functions.
struct GameInfo {
	int moveNumber;  // Counts up from 1, incrementing for each move
	int playerRoom;  // Room 1..20 the player currently is in
	int wumpusRoom;  // Room 1..20 the Wumpus is in
	int pitRoom1;    // Room 1..20 the first pit is in
	int pitRoom2;    // Room 1..20 the second pit is in
	int batsRoom1;   // Room 1..20 the first set of bats are in
	int batsRoom2;   // Room 1..20 the second set of bats are in
	int arrowRoom;   // -1 if arrow is with player, else room number 1..20
}; 

// Function prototype needed to allow calls in any order between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *playerIsAlive, int **rooms);

//--------------------------------------------------------------------------------
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
}


//--------------------------------------------------------------------------------
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
// Return true if randomValue is already in array
int alreadyFound(int randomValue,		// New number we're checking
                 int *randomNumbers,	// Set of numbers previously found
                 int limit)				// How many numbers previously found
{
    int returnValue = false;
    
    // compare random value against all previously found values
    for( int i = 0; i<limit; i++) {
        if( randomValue == randomNumbers[i]) {
            returnValue = true;   // It is already there
            break;
        }
    }
    
    return returnValue;
} // end alreadyFound(...)


//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int *randomNumbers,	// Array of random numbers
                     int size)              // Size of random numbers array
{
    int randomValue = -1;
    
    for( int i = 0; i<size; i++) {
        do {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
} 

//--------------------------------------------------------------------------------
// Set up the cave room connections.  The rooms 2D array (int **) is created within
// this function.  Because the function changes what it points to, it is itself a
// reference parameter, making its type: int * **
void setRoomConnections( int * **rooms)	// Array of room connections, pass-by-reference
{
	// Initialize cave room connections
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

	// First we allocate space for the 20 room pointers (each an int *)
	*rooms = malloc(sizeof(int *) * 21);
	// Then for each of the 20 room pointers, we allocate space to store the 3 adjacent rooms
	for(int i=0; i<21; i++) {
		(*rooms)[i] = malloc(sizeof(int) * 3);
	}

	// Now we can store the room connections.
    // Room 0 is unused, to help avoid off-by-one indexing issues
	(*rooms)[0][0] = 0; (*rooms)[0][1] = 0; (*rooms)[0][2] = 0;			// Room 0 (unused)
	(*rooms)[1][0] = 2; (*rooms)[1][1] = 5; (*rooms)[1][2] = 8;			// Room 1
	(*rooms)[2][0] = 1; (*rooms)[2][1] = 3; (*rooms)[2][2] = 10;		// Room 2
	(*rooms)[3][0] = 2; (*rooms)[3][1] = 4; (*rooms)[3][2] = 12;		// Room 3
	(*rooms)[4][0] = 3; (*rooms)[4][1] = 5; (*rooms)[4][2] = 14;		// Room 4
	(*rooms)[5][0] = 1; (*rooms)[5][1] = 4; (*rooms)[5][2] = 6;			// Room 5
	(*rooms)[6][0] = 5; (*rooms)[6][1] = 7; (*rooms)[6][2] = 15;		// Room 6
	(*rooms)[7][0] = 6; (*rooms)[7][1] = 8; (*rooms)[7][2] = 17;		// Room 7
	(*rooms)[8][0] = 1; (*rooms)[8][1] = 7; (*rooms)[8][2] = 9;			// Room 8
	(*rooms)[9][0] = 8; (*rooms)[9][1] = 10; (*rooms)[9][2] = 18;		// Room 9
	(*rooms)[10][0] = 2; (*rooms)[10][1] = 9; (*rooms)[10][2] = 11;		// Room 10
	(*rooms)[11][0] = 10; (*rooms)[11][1] = 12; (*rooms)[11][2] = 19;	// Room 11
	(*rooms)[12][0] = 3; (*rooms)[12][1] = 11; (*rooms)[12][2] = 13;	// Room 12
	(*rooms)[13][0] = 12; (*rooms)[13][1] = 14; (*rooms)[13][2] = 20;	// Room 13
	(*rooms)[14][0] = 4; (*rooms)[14][1] = 13; (*rooms)[14][2] = 15;	// Room 14
	(*rooms)[15][0] = 6; (*rooms)[15][1] = 14; (*rooms)[15][2] = 16;	// Room 15
	(*rooms)[16][0] = 15; (*rooms)[16][1] = 17; (*rooms)[16][2] = 20;	// Room 16
	(*rooms)[17][0] = 7; (*rooms)[17][1] = 16; (*rooms)[17][2] = 18;	// Room 17
	(*rooms)[18][0] = 9; (*rooms)[18][1] = 17; (*rooms)[18][2] = 19;	// Room 18
	(*rooms)[19][0] = 11; (*rooms)[19][1] = 18; (*rooms)[19][2] = 20;	// Room 19
	(*rooms)[20][0] = 13; (*rooms)[20][1] = 16; (*rooms)[20][2] = 19;	// Room 20
} // end setRoomConnections(...)


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(struct GameInfo *gameInfo)   // All other game settings variables
{
    // Array of 6 unique values 1..20, to be used in initializing cave hazards locations
    int *randomNumbers;
	randomNumbers = malloc(sizeof(int) * 7);
    
    // Create a set of unique random values 1..20 to be used as room locations.
    setUniqueValues(randomNumbers, 7);
    // Use the above set of unique random numbers to set initial room locations of 
    // game components, which should each be in separate rooms.
	// Order of assignment is: player, Wumpus, pit1, pit2, bat1, bat2, arrow
	gameInfo->playerRoom = randomNumbers[0];
	gameInfo->wumpusRoom = randomNumbers[1];
	gameInfo->pitRoom1 = randomNumbers[2];   gameInfo->pitRoom2 = randomNumbers[3];
	gameInfo->batsRoom1 = randomNumbers[4];  gameInfo->batsRoom2 = randomNumbers[5];
    gameInfo->arrowRoom = randomNumbers[6];
    
    gameInfo->moveNumber = 1;

	// Free up the space from randomNumbers
	free(randomNumbers);
}// end initializeGame(...)


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[ 3],     // Array of adjacent tunnels
                    int nextRoom)        // Desired room to move to
{
    return( tunnels[0] == nextRoom ||
            tunnels[1] == nextRoom ||
            tunnels[2] == nextRoom
          );
}


//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(struct GameInfo gameInfo)
{
    printf( "Cheating! Game elements are in the following rooms: \n"
    		"Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
    		"%4d %5d %6d %5d %5d %5d %5d \n\n", 
			gameInfo.playerRoom,
			gameInfo.wumpusRoom,
			gameInfo.pitRoom1,
			gameInfo.pitRoom2,
			gameInfo.batsRoom1,
			gameInfo.batsRoom2,
			gameInfo.arrowRoom
		);
}


//--------------------------------------------------------------------------------
// Display room number and hazards detected.  The order of checking for hazards
// is critical to matching expected output.
void displayRoomInfo(int **rooms, struct GameInfo gameInfo)
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.playerRoom;
    printf("You are in room %d. ", currentRoom);
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = rooms[currentRoom][0];
    int room2 = rooms[currentRoom][1];
    int room3 = rooms[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if( room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
        printf("You smell a stench. ");
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if( room1 == pit1Room || room1 == pit2Room ||
        room2 == pit1Room || room2 == pit2Room ||
        room3 == pit1Room || room3 == pit2Room
      ) {
        printf("You feel a draft. ");
    }

	// Display hazard detection message if bats are in an adjacent room
    int batsRoom1 = gameInfo.batsRoom1;
    int batsRoom2 = gameInfo.batsRoom2;
    if( room1 == batsRoom1 || room1 == batsRoom2 ||
        room2 == batsRoom1 || room2 == batsRoom2 ||
        room3 == batsRoom1 || room3 == batsRoom2
      ) {
        printf("You hear rustling of bat wings. ");
    }
    
    printf("\n\n");
}//end displayRoomInfo(...)


//--------------------------------------------------------------------------------
// When the player just moved into a room with bats, the bats transport the player to
//   a random room that is not where they just were, and is not a room that already
//   has bats in it, but it could be a room with the Wumpus or a pit.  The bats are then
//   placed in a random room that is not where the player is, is not where the other
//   bats are, and is not their original room.
void inRoomWithBats(
        struct GameInfo *gameInfo,    // Hazards location and game info
        bool *playerIsAlive,   // player starts as alive, but this could change from true to false
        int **rooms)        // Gets passed on to check for death by pit or Wumpus
//   which 75% of the time just moves the Wumpus
{
    int randomNewRoom = -1;    // Used to transport player to a new room
    
    // player is transported by bats and dropped off in some random room.
    // Find new random room location that is not where we started
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS + 1;
    } while (randomNewRoom == gameInfo->playerRoom);
    
    // Move the player to the new room.  Keep track of which room they came from, to help us
    // know which set of bats to adjust afterwards
    int playerOldRoom = gameInfo->playerRoom;
    gameInfo->playerRoom = randomNewRoom;
    printf("Woah... you're flying! \n");
    printf("You've just been transported by bats to room %d.\n", gameInfo->playerRoom);
    
    // Find new random room location that is not where the player is now, and is not
    //   where the other bats are, and is not where the bats came from.
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS + 1;
    } while (randomNewRoom == gameInfo->playerRoom ||
             randomNewRoom == gameInfo->batsRoom1  ||
             randomNewRoom == gameInfo->batsRoom2 );
    
    // Move the bats to this newly selected room.  Figure out which set of bats to move by
    // comparing their current location to the room the player came from.
    if( playerOldRoom == gameInfo->batsRoom1) {
        // Change batsRoom1, since that is where the player came from
        gameInfo->batsRoom1 = randomNewRoom;
    }
    else {
        // Change batsRoom2, since that is where the player came from
        gameInfo->batsRoom2 = randomNewRoom;
    }
    
    // Handle pit, Wumpus, and bats hazards in player's new room if present.  The
    // checkForHazards(..) function at the end again checks for bats, thus handling
    // the low-probability event that bats drop you in a room that itself already
    // has bats in it, leading to multiple sequential bats flights.
    int playerRoom = gameInfo->playerRoom;
    if( playerRoom == gameInfo->batsRoom1 || playerRoom == gameInfo->batsRoom2 ||
        playerRoom == gameInfo->pitRoom1 || playerRoom == gameInfo->pitRoom2 ||
        playerRoom == gameInfo->wumpusRoom) {
        checkForHazards(gameInfo, playerIsAlive, rooms);
    }
}//end inRoomWithBats(...)


//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the player dies.
// If the player just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
void checkForHazards(
         struct GameInfo *gameInfo,	// Hazards location and game info
         bool *playerIsAlive,		// player is alive, but could die depending on the hazards
         int **rooms)				// Cave network of rooms
{
    // Retrieve the room the player is in
    int playerRoom = gameInfo->playerRoom;
      
    // Check for the Wumpus
    if( playerRoom == gameInfo->wumpusRoom) {
        // To make it easier to test, in this version of the program the Wumpus always
        // moves if it is currently in an odd-numbered room, and it moves into the
        // lowest-numbered adjacent room.  In the version that is more fun to play
        // (but harder to test), the Wumpus has a 75% chance of moving, and a 25%
        // chance of staying and killing you.  The "more fun" code is commented out below.
        
        // Wumpus is there. 75% change of Wumpus moving, 25% chance of it killing you
        // Generate a random number 1..100
        // if(  (rand() % 100) < 75) {
        if( gameInfo->wumpusRoom %2 == 1) {
            // You got lucky and the Wumpus moves away
            printf( "You hear a slithering sound, as the Wumpus slips away. \n"
					"Whew, that was close! \n");
            // Choose a random adjacent room for the Wumpus to go into
            // gameInfo.wumpusRoom = rooms[ playerRoom][ rand() % 3];
            gameInfo->wumpusRoom = rooms[ playerRoom][ 0];  // Choose the lowest-numbered adjacent room
        }
        else {
            // Wumpus kills you
            printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
					"It is over.\n");
            *playerIsAlive = false;
            return;
        }
    } //end if( playerRoom...

	// Check whether there is a pit
    if( playerRoom == gameInfo->pitRoom1 || playerRoom == gameInfo->pitRoom2) {
        // player falls into pit
        printf("Aaaaaaaaahhhhhh....   \n");
        printf("    You fall into a pit and die. \n");
        *playerIsAlive = false;
        return;
    }

	// Check for bats, which if present move you to a new room.  The inRoomWithBats(..) function
    // at the end again checks for hazards.
    if( gameInfo->playerRoom == gameInfo->batsRoom1 || gameInfo->playerRoom == gameInfo->batsRoom2) {
        inRoomWithBats(gameInfo, playerIsAlive, rooms);
    }
}//end checkForHazards(...)


//--------------------------------------------------------------------------------
// User shot arrow, providing up to 3 rooms for the arrow to fly through
void shootArrow(int **rooms,          			// Array of connected rooms
                struct GameInfo *gameInfo,      // Locations of hazards, user, Wumpus and arrow
                bool *playerIsAlive,     		// Whether or not player is alive
                bool *wumpusIsAlive)     		// Whether or not Wumpus is alive
{
    int *roomsForArrowTravel;			// Array of up to 3 rooms for arrow to travel through
    int howManyRoomsForArrow = 0;		// How many rooms the user supplies for arrow to travel through
    
    // Ask user to enter number of rooms for arrow to travel through
    printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
    scanf("%d", &howManyRoomsForArrow);
    if( howManyRoomsForArrow > 3) {
        printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
        howManyRoomsForArrow = 3;
    }

	// Dynamically allocate space for roomsForArrowTravel based on howManyRoomsForArrow.
    // Storing the rooms is not strictly necessary, as we could just process them one
    // at a time as they are read in.
	roomsForArrowTravel = malloc(sizeof(int) * howManyRoomsForArrow);

	// Store room numbers into the array
    for( int i=0; i<howManyRoomsForArrow; i++) {
        scanf("%d", &roomsForArrowTravel[ i]);
    }
    
    // Move the arrow through those rooms, ensuring they are adjacent
    gameInfo->arrowRoom = gameInfo->playerRoom;  // arrow starts in player room
    for( int i = 0; i < howManyRoomsForArrow; i++) {
        // Ensure room is adjacent before traveling through it
        if( roomIsAdjacent( rooms[ gameInfo->arrowRoom], roomsForArrowTravel[i]) ) {
            // Room was adjacent, so move arrow there
            gameInfo->arrowRoom = roomsForArrowTravel[i];
            // Kill Wumpus if it is there
            if( gameInfo->arrowRoom == gameInfo->wumpusRoom) {
                *wumpusIsAlive = false;
                printf( "Wumpus has just been pierced by your deadly arrow! \n"
                		"Congratulations on your victory, you awesome hunter you.\n");
                return;
            }
            // Kill player if path went to a second room and then came right back
            if( gameInfo->arrowRoom == gameInfo->playerRoom) {
                *playerIsAlive = false;
                printf( "You just shot yourself.  \n"
                		"Maybe Darwin was right.  You're dead.\n");
                return;
            }
        }
        else {
            // Error in user input, as indicated room is not adjacent.
			printf("Room %d is not adjacent.  Arrow ricochets...\n", roomsForArrowTravel[i]);
            // Move arrow to lowest numbered adjacent rooms
            gameInfo->arrowRoom = rooms[ gameInfo->arrowRoom][ 0];
            // The game is more interesting if you move the arrow to a random adjacent room, but
            // this feature (line below) was removed to make it easier to test your program.
            // gameInfo.arrowRoom = rooms[gameInfo.arrowRoom][rand() % 3];
            
            // If it richochets into room where user is, user dies
            if( gameInfo->arrowRoom == gameInfo->playerRoom) {
                *playerIsAlive = false;
                printf( "You just shot yourself, and are dying.\n"
                		"It didn't take long, you're dead.\n");
                return;
            }
            
            // If it ricochets into room where Wumpus is, Wumpus dies
            if( gameInfo->arrowRoom == gameInfo->wumpusRoom) {
                *wumpusIsAlive = false;
                printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                		"Accidental victory, but still you win!\n");
                return;
            }
        }//end else
        
    }//end for( int i = 0; i < howManyRoomsForArrow
    
    // Since arrow was shot, Wumpus hears it.  Choose a random adjacent room for the Wumpus 
    // to go into.  The line below has the Wumpus go to a random room, but this has been 
    // disabled for testing, instead having the Wumpus move to the lowest-numbered adjacent room.
    // gameInfo.wumpusRoom = rooms[ gameInfo.wumpusRoom][ rand() % 3];
    gameInfo->wumpusRoom = rooms[ gameInfo->wumpusRoom][ 0];

	// Free up the space for roomsForArrowTravel
	free(roomsForArrowTravel);
}//end shootArrow(...)


//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the player's location,
// useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo)
{
    printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n");
	// In the scanf below note that we put the space at the beginning of the scanf so  
    // that any newline left over from a previous input is not read in and used as 
	// the next move. Another option is having getchar() after the scanf() statement.
	scanf(" %d %d %d %d %d %d %d",  &theGameInfo->playerRoom, &theGameInfo->wumpusRoom, 
                        			&theGameInfo->pitRoom1, &theGameInfo->pitRoom2,
									&theGameInfo->batsRoom1, &theGameInfo->batsRoom2,
									&theGameInfo->arrowRoom);
    printf("\n");
}


//--------------------------------------------------------------------------------
int main(void) {
	int **rooms;           		// 3 tunnel index values for the 20 rooms. Indexed 0..20, 
    							//    but we will ignore 0 and only use 1..20.  This approach
                                //    helps prevent off-by-one indexing errors.
    struct GameInfo gameInfo;   // Used to more easily pass game info variables around
    bool playerIsAlive = true;  // Used in checking for end of game
    bool wumpusIsAlive = true;  // Used in checking for end of game
    char typeOfMove;            // Used to handle user input letter
    int nextRoom;               // User input of destination room number, used on a 'M' type move
    
    // Seed the random number generator.  Change seed to time(0) to change layout each time.
    // srand(time(0));
    srand(1);

	// Set the room connection values (allocated using malloc)
	setRoomConnections(&rooms);
    
    // Set random initial values for player, Wumpus, bats and pits
    initializeGame(&gameInfo);

	// Main playing loop.  Break when player dies, or player kills Wumpus
    while( playerIsAlive && wumpusIsAlive) {
        
		// Display current room information: Room number, hazards detected
		displayRoomInfo(rooms, gameInfo);
		
		// Prompt for and handle move
		printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
        // Note the extra space in the scanf below between the opening quote " and the %c.  
        //    This skips leading white space in the input so that the newline left over from
        //    a previous move is not read in and used as the current move.  An alternative 
        //    is using getchar() after the scanf() statement to read the input-buffered
        //    return character.
		scanf(" %c", &typeOfMove);							
		typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
		
		// Check all types of user input and handle them.  This uses if-else-if code
		// rather than switch-case, so that we can more easily take advantage of break 
        // and continue.
		if( typeOfMove == 'D') {
			displayCave();
			displayInstructions();
			continue;       // Loop back up to reprompt for the same move number
		}
		else if( typeOfMove == 'P') {
			// To assist with play, display the cave layout
			displayCave();
			continue;       // Loop back up to reprompt for the same move number
		}
		else if( typeOfMove == 'M') {
			// Move to an adjacent room,
			scanf(" %d", &nextRoom);
			
			if( roomIsAdjacent( rooms[ gameInfo.playerRoom], nextRoom)) {
				gameInfo.playerRoom = nextRoom;        // move to a new room
				
				// Check if pit or wumpus is present in this new room
				checkForHazards(&gameInfo, &playerIsAlive, rooms);

				// Pick up arrow if it is in this new room
				if( gameInfo.arrowRoom == gameInfo.playerRoom) {
					printf("Congratulations, you found the arrow and can once again shoot.\n");
					gameInfo.arrowRoom = -1;   // -1 indicates player has arrow
				}
			}
			else {
				printf("Invalid move.  Please retry. \n");
				continue;        // Doesn't count as a move, so retry same move.
			}
		}
		else if( typeOfMove == 'S') {
			// arrowRoom value of -1 means it is with the player, which must be true for arrow to be shot
			if( gameInfo.arrowRoom == -1) {
				shootArrow(rooms, &gameInfo, &playerIsAlive, &wumpusIsAlive);
			}
			else {
				printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
			}
		}
		else if( typeOfMove == 'C') {
			// Display Cheat information
			displayCheatInfo(gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
		else if( typeOfMove == 'R') {
			// Specify resetting the hazards and player positions, useful for testing
			resetPositions( &gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
		else if( typeOfMove == 'X') {
			// Exit program
			playerIsAlive = false;     // Indicate player is dead as a way to exit playing loop
			break;
		}

		// Increment the move number
		gameInfo.moveNumber = gameInfo.moveNumber + 1;
    } //end while( playerIsAlive && wumpusIsAlive)

	// Before we exit the game, free the space used to store room connections.  This is 
	// done in the opposite order of the mallocs.  This is not strictly necessary in 
    // this context, because all the memory will be freed once we exit the program.
    // First free the space used to store the adjacent rooms for each room
	for(int i=0; i<21; i++) {
		free( rooms[i] );
	}
	// Next free the space used for the 20 room pointers
	free(rooms);

    printf("\nExiting Program ...\n");
	return 0;
} // end main(..)
