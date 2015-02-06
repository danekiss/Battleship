/*******************************************************************************************
* Name: Dan Ekiss                                                                          *
* Class: Cpt_S 121                                                                         *
* Battleship                                                                               *
* Fall 2014                                                                                *
*                                                                                          *
*******************************************************************************************/
#include "battleship.h"

int main (void)
{
	Stats players[2] = {{0, 0, 0, 0.0}, {0, 0, 0, 0.0}};

	Cell player_one_board[ROWS][COLS];       /* Player one game board */
	Cell player_two_board[ROWS][COLS];       /* Player two game board */

	Coordinate target; // Target coordinates             
	Coordinate temp_target; 
	Coordinate initial_target;    
	Coordinate target_AI; // AI coordinates

	WaterCraft ship[NUM_OF_SHIPS] = {{'c', 5, "Carrier"}, {'b', 4, "Battleship"}, {'r', 3, "Cruiser"}, {'s', 3, "Submarine"}, {'d', 2, "Destroyer"}};
	                              
	                                 
	                                 
	                                 

	Boolean    rand_target = TRUE;                    //Choose a target randomly.

	Boolean    hit = FALSE;                    // True if there's been a hit.

	Boolean    flipped = TRUE;	                 

	Boolean    cardinals[4] = {TRUE, TRUE, TRUE, TRUE}; 

	Boolean    is_destroyed = FALSE;                    


	short destroyed_ship[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2},    
	                                   {5, 4, 3, 3, 2}};  // What part of the ship has been hit?

	short player  = 0;	           // Assign player values

	short shot    = 0;             // Remember temp value for hit ship 
	int   option  = 0,             // Random or manual
		  north   = 0,             
		  south = 0,           
		  east = 0,            
		  west = 0,   
		  i = 0,             
		  counter = 1;             /* i and counter are used as counters */

	char  ship_char = '\0';       /* temporary value to save character symbol of the ship */
	
	FILE *outStream = NULL;        /* stream to output file battleship.log */

	outStream = fopen (LOG_FILE_NAME, "w");

	srand ((unsigned int) time (NULL));

	menu ();
	systemMessage ("Press Enter please.\n");
	system ("cls");

	init_board (player_one_board);
	init_board (player_two_board);

	printf ("Place ships manually or randomly? Hit 1 or 2 respectively:\n");
	printf ("1. Manually\n");
	printf ("2. Randomly\n");
	printf ("Enter your choice: ");
	scanf ("%d", &option);
	
	switch (option) {
		case 1: manually_place (player_one_board, ship);
	            break;
		case 2: randomly_place (player_one_board, ship);
				break;
	}

	
	randomly_place (player_two_board, ship);
	printf ("AI's board has been generated.\n");

	//Randomly select who goes first.
	player = rand_num (0, 1);
	printf ("Player %d has chosen.\n", player + 1);
	systemMessage ("Press enter to continue.\n");
	system ("cls");

	
	while (TRUE) {

	// Output whose turn it is to battleship.log
	fprintf (outStream, "Player %d's turn.\n", player + 1);

	// Alternate players
	switch (player) {

		case PLAYER_ONE: 
				// Player 2's board:
				printf ("> Player 2's Board:\n");
				print_board (player_two_board, FALSE);
				printf ("It's player 1'S turm\n");

				// Enter target and tell player if it's a possible spot, and if it is a hit or a miss.
				do {
					target = check_target (); 
					shot = check_shot (player_two_board, target);
					
					
					if (shot == -1) 
						printf ("> Try inputting another target!\n");

				} while (shot == -1);

				// Save character
				ship_char = player_two_board[target.row][target.column].symbol;
				break;

			case PLAYER_TWO: 

				// Player 1's board:
				printf ("Player 1's board:\n");
				print_board (player_one_board, TRUE);
				printf ("It's the computer's turn.\n");

				//If ship is destroyed, go to "hit"
				if (is_destroyed) {
					is_destroyed = FALSE;
					hit = FALSE;
					rand_target = TRUE;
				}
				
				
				  //If AI chooses a target that is a hit, hit is true. Otherwise, false.
				
				if (hit) {
					
					target = target_AI;

					do {
						if (cardinals[NORTH]) {        
							target.row = north;
						} else if (cardinals[SOUTH]) { 
							target.row = south;
						} else if (cardinals[WEST]) {  
							target.column = west;
						} else if (cardinals[EAST]) {  
							target.column = east;
						} else if (!cardinals[NORTH] && !cardinals[SOUTH] && 
						           !cardinals[WEST]  && !cardinals[EAST]  && 
								   !is_destroyed) {
							
							target = initial_target;
							temp_target = target;

							north = target.row - counter;
							temp_target.row = north;

							// Is the value directly above a hit?
							if (check_shot (player_one_board, temp_target) != -1 && north >= 0) {
								cardinals[NORTH] = TRUE;
							}

							temp_target = target;
							south = target.row + counter;
							temp_target.row = south;

							// Is the value directly below a hit?
							if (check_shot (player_one_board, temp_target) != -1 && south <= 9) {
								cardinals[SOUTH] = TRUE;
							}

							temp_target = target;
							west = target.column - counter;
							temp_target.column = west;

							// Is the value directly to the left a hit?
							if (check_shot (player_one_board, temp_target) != -1 && west >= 0) {
								cardinals[WEST] = TRUE;
							}

							temp_target = target;
							east = target.column + counter;
							temp_target.column = east;

							// Is the value directly to the right a hit?
							if (check_shot (player_one_board, temp_target) != -1 && east <= 9) {
								cardinals[EAST] = TRUE;
							}
							counter++;

						} else  {
						
							hit = FALSE;
							rand_target = TRUE;
							break;
						}
						
						
						shot = check_shot (player_one_board, target);

					} while (shot == -1 && hit == TRUE);

					if (shot == 1 && rand_target == FALSE) {
						for (i = 0; i < 4; i++) {
							if (flipped == FALSE)
								cardinals[i] = FALSE;

							if (cardinals[i] == flipped) 
								flipped = FALSE;
						}
					} else {
						for (i = 0; i < 4; i++) {
							if (flipped == TRUE && cardinals[i] != FALSE) {
								cardinals[i] = FALSE;
								break;
							}
						}
					}

					
					flipped = TRUE;
				}

				if (rand_target) {	

					/* reinitializes important values */
					counter = 1;

					flipped = TRUE;

					for (i = 0; i < 4; i++)
					cardinals[i] = TRUE;

					// Targets a random area.
					do {
						target.row = rand_num (0, 9);

						target.column = rand_num (0, 9);

						//Hit or miss?
						shot = check_shot (player_one_board, target);
					} while (shot == -1);

					/* if it is a Hit, this saves the value of the original target */
					if (shot == 1) initial_target = target;
				}

				// If hit nested if
				if (shot == 1) {

					if (!cardinals[NORTH] && !cardinals[WEST] && !cardinals[SOUTH] && !cardinals[EAST] && !is_destroyed)
						 {target = initial_target;}

					rand_target = FALSE;
					hit = TRUE;
					target_AI = target;

					// Is the ship within the bounds?
					if (cardinals[NORTH] == TRUE) {  
						north = (target_AI.row - 1);

						get_bounds (cardinals, north, NORTH);

						temp_target = target;

						temp_target.row = north;
						if (check_shot (player_one_board, temp_target) == -1)
							cardinals[NORTH] = FALSE;
					}
					
					if (cardinals[SOUTH] == TRUE) {  

						south = target_AI.row + 1;

						get_bounds (cardinals, south, SOUTH);

						temp_target = target;

						temp_target.row = south;

						if (check_shot (player_one_board, temp_target) == -1)
							cardinals[SOUTH] = FALSE;
					}

					if (cardinals[WEST] == TRUE) {   
						west  = target_AI.column - 1;
						get_bounds (cardinals, west, WEST);
						temp_target = target;
						temp_target.column = west;
						if (check_shot (player_one_board, temp_target) == -1)
							cardinals[WEST] = FALSE;
					}

					if (cardinals[EAST] == TRUE) {   
						east  = target_AI.column + 1;
						get_bounds (cardinals, east, EAST);
						temp_target = target;
						temp_target.column = east;
						if (check_shot (player_one_board, temp_target) == -1)
							cardinals[EAST] = FALSE;
					}
				}

				//Save the character in the selected space.
				ship_char = player_one_board[target.row][target.column].symbol;
				break;
		}

		//Did the user hit or miss?
		if (shot == 1) { 
			printf ("%d, %d is a hit!\n", target.row, target.column);

			// Write hit coordinates to battleship.log
			fprintf (outStream, "%d, %d is a hit!\n", target.row, target.column);

			// Record correctly guessed hits
			players[player].nums_hits ++;

			// Is ship destroyed?
			if (player == 1)  
				is_destroyed = check_destroyed (destroyed_ship, !player, ship_char, outStream);
			else
				check_destroyed (destroyed_ship, !player, ship_char, outStream);

		} else {       
			printf ("> %d, %d is a miss!\n", target.row, target.column);

			//Write miss coordinates to battleship.log
			fprintf (outStream, "%d, %d is a miss!\n", target.row, target.column);
			players[player].num_misses++;
		}
		//Update game board for either player one or player two.
		if (player == 0) 
			update_board (player_two_board, target);
		else                
			update_board (player_one_board, target);

		// Who correctly guessed the most hits and won?
		if (check_winner (players, player)) {
			printf ("Player %d won!\n", player + 1);

			
			fprintf (outStream, "Player %d wins!\n", player + 1);
			break;
		}

		systemMessage ("Press enter to continue.\n");

		// Change players
		player = !player;	

		system ("cls");
	}

	
	players[0].totalShots = players[0].nums_hits + players[0].num_misses;
	players[0].hitMissRatio = ((double) players[0].nums_hits/(double) players[0].num_misses) * 100;

	players[1].totalShots = players[1].nums_hits  + players[1].num_misses;
	players[1].hitMissRatio = ((double) players[1].nums_hits/(double) players[1].num_misses) * 100;


	fprintf (outStream, "|^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	fprintf (outStream, "|                    Results                        \n");
	fprintf (outStream, "|							\n");
	fprintf (outStream, "|Player #1 had: %d hits                             \n", players[0].nums_hits );
	fprintf (outStream, "|               %d misses                           \n", players[0].num_misses);
	fprintf (outStream, "|               %d total # of shots                 \n", players[0].totalShots);
	fprintf (outStream, "|               %.2lf%% hit to miss ratio           \n", players[0].hitMissRatio);
	fprintf (outStream, "| Player #2 had: %d hits                            \n", players[1].nums_hits );
	fprintf (outStream, "|                %d misses                          \n", players[1].num_misses);
	fprintf (outStream, "|                %d total # of shots                \n", players[1].totalShots);
	fprintf (outStream, "|                %.2lf%% hit to miss ratio          \n", players[1].hitMissRatio);
	fprintf (outStream, "+^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");

	fclose (outStream);
	return 0;
}
