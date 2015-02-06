#include "battleship.h"

void menu (void) {
	printf ("Welcome to the game of Battleship!\n");
	printf ("Battleship Rules:\n");

	printf ("The game involves two players.\n");
	 // Tell user character representations for the different ships.
	printf ("Carriers [c] occupy 5 spaces,  battleships [b] occupy 4 spaces, cruisers (r) occupy 3 spaces, submarines (s) occupy 3 spaces, destroyers (d) occupy 2 spaces \n");
	printf ("The computer randomly chooses who goes first.\n");
	printf ("Each player has to guess to coordinates of the other player's ships.\n");
	printf ("of the opposing player's game board; hit and miss\n");
	printf ("First player to guess the location of all ships wins\n\n");
}


void init_board (Cell board[][COLS]) {
	int i = 0, j = 0;

	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++) {
			board[i][j].symbol          = WATER;
			board[i][j].position.row    = i;
			board[i][j].position.column = j;
		}
}

void print_board (Cell board [][COLS], Boolean showPegs) {
	int i = 0, j = 0;

	printf ("  0 1 2 3 4 5 6 7 8 9\n");

	for (i = 0; i < ROWS; i++) {
		printf ("%d ", i);

		for (j = 0; j < COLS; j++) {
			if (showPegs == TRUE) 
				printf ("%c ", board [i][j].symbol);
			else {

			switch (board [i][j].symbol) {
				case HIT:   printf ("%c ", HIT);   break;
				case MISS:  printf ("%c ", MISS);  break;
				case WATER: printf ("%c ", WATER); break;
				default: printf ("%c ", WATER); break;
					
				}
			}
		}
		
		putchar('\n');
	}
}

// Puts ships on the board.
void drop_ship (Cell board[][COLS], WaterCraft ship, Coordinate position, int direction) {
	int i = ship.length - 1;

	for(i = 0; i < ship.length; i++) {
		if (direction == HORIZONTAL) 
			board [position.row][position.column + i].symbol = ship.symbol;
		else 
			board [position.row + i][position.column].symbol = ship.symbol;
	}
}

void manually_place (Cell board[][COLS], WaterCraft ship[]) {
	char       stringPosition[11] = "";

	int        i = 0, j = 0;

	Coordinate position[5];
	Boolean    is_valid = FALSE;

	fflush (stdin);

	for (i = 0; i < NUM_OF_SHIPS; i++) {

		while (TRUE) {
			system("cls");
			print_board(board, TRUE);
			printf("Please enter the %d blocks to place the %s across (10 total numbers in x,y format):\n", ship[i].length, ship[i].name);
			scanf("%s", stringPosition);

			// convert_to_pos returns false if it failed.
			if(convert_to_pos (position, stringPosition, ship[i].length)) {

				is_valid = TRUE;

				for (j = 0; j < ship[i].length; j++) {

					if (board[position[j].row][position[j].column].symbol == WATER) {
						board[position[j].row][position[j].column].symbol = ship[i].symbol;
					} else {
						is_valid = FALSE;
						printf ("> Invalid input!\n");

						if (j != 0)
							while (j >= 0) {
								board[position[j].row][position[j].column].symbol = WATER;
								j--;
							}

						break;
					}
				}
			} else {
				is_valid = FALSE;
				printf ("Error: Try another input\n");
			}

			if (is_valid == TRUE) break;
		}

	}
}

void randomly_place (Cell board[][COLS], WaterCraft ship[]) {
	Coordinate position;
	int direction = -1;
	int i = 0;

	for (i = 0; i < NUM_OF_SHIPS; i++) {
		while (TRUE) {
			direction = rand_num (0, 1); /* 0 -> horizontal, 1 -> vertical */
			position = create_coord (direction, ship[i].length);

			if (viable_coord (board, position, direction, ship[i].length)) break;
		}

		drop_ship (board, ship[i], position, direction);
	}
}

//Is hit or miss?
void update_board (Cell board[][COLS], Coordinate target) {
	switch (board[target.row][target.column].symbol) {
		
		case WATER: 
			board[target.row][target.column].symbol = MISS;
			break;

		
		case CARRIER: 
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			board[target.row][target.column].symbol = HIT;
			break;

		case HIT:
		case MISS:
		default:
			break;
	}	
}

void get_bounds (Boolean cardinals[], int bound, int direction) {
	switch (direction) {
		case NORTH: 
			if (bound < 0) 
				cardinals[0] = FALSE;
			else            
				cardinals[0] = TRUE;
			break;

		case SOUTH:
			if (bound > 9) 
				cardinals[1] = FALSE;
			else            
				cardinals[1] = TRUE;
			break;

		case WEST:
			if (bound < 0)  
				cardinals[2] = FALSE;
			else            
				cardinals[2] = TRUE;
			break;

		case EAST:
			if (bound > 9)  
				cardinals[3] = FALSE;
			else            
				cardinals[3] = TRUE;	
			break;
	}
}


void systemMessage (char *message) {
	char ch = '\0';

	do {
		printf ("%s", message);
	} while((ch = _getch()) != '\r');
}


Boolean check_destroyed(short destroyed_ship[][NUM_OF_SHIPS], short player, char ship_char, FILE *stream) {
	Boolean sunked = FALSE;

	switch (ship_char) {
		case CARRIER:    
			if (--destroyed_ship[player][0] == 0) 
			{
				printf("Player %d got their carrier destroyed!\n", player + 1);

				
				fprintf(stream, "Player %d got their carrier destroyed\n", player + 1);

				sunked = TRUE;
			}
			break;

		case BATTLESHIP: 
			if (--destroyed_ship[player][1] == 0)
			{
				printf("Player %d got their battleship destroyed!\n", player + 1);

			
				fprintf(stream, "Player %d got their battleship destroyed!\n", player + 1);

				sunked = TRUE;
			}
			break;

		case CRUISER:    
			if (--destroyed_ship[player][2] == 0) {
				printf ("> Player %d had their cruiser destroyed!\n", player + 1);

				fprintf (stream, "Player %d had their cruiser destroyed!\n", player + 1);

				sunked = TRUE;
			}
			break;

		case SUBMARINE:  
			if (--destroyed_ship[player][3] == 0)
			{
				printf ("Player %d had their submarine destroyed!\n", player + 1);

				fprintf (stream, "Player %d had their submarine destroyed!\n", player + 1);

				sunked = TRUE;
			}
			break;

		case DESTROYER:  
			if (--destroyed_ship[player][4] == 0) {
				printf ("Player %d had their destroyer destroyed!\n", player + 1);

				fprintf (stream, "Player %d had their destroyer destroyed!\n", player + 1);

				sunked = TRUE;
			}
			break;
	}

	return sunked;
}

Boolean viable_coord (Cell board[][COLS], Coordinate position, 
				         int direction, int length) 
{
	int i = length - 1;
	Boolean is_valid = TRUE;

	for (i = 0; is_valid && i < length; i++) {
		if (direction == HORIZONTAL) {
			if (board [position.row][position.column + i].symbol != WATER &&
				(position.column + i) < COLS)
				is_valid = FALSE;
		} else { 
			if (board [position.row + i][position.column].symbol != WATER &&
				(position.row + i) < ROWS)
				is_valid = FALSE;
		}
	}

	return is_valid;
}


Boolean convert_to_pos (Coordinate position[], char *stringPosition, int length) 
{
Boolean flag = TRUE;

	char temp = '\0';

	int i = 0, j = 0, k = 1;

	// Validate string length and if digit.
	if (strlen (stringPosition)/2 == length) {
		for (i = 0; i < length && flag; i++) {
			if (isdigit (stringPosition[j]) && isdigit (stringPosition[k])) {
				position[i].row = stringPosition[j] - '0';
				position[i].column = stringPosition[k] - '0'; 

				j += 2;
				k += 2;
			} else
			{
				flag = FALSE;
			}
		}
	} else 
	{
		flag = FALSE;
	}

	return flag;
}

Boolean check_winner (Stats players[], int player) 
{
	Boolean isWin = FALSE;

	if (players[player].nums_hits == 17) 
		isWin = TRUE;

	return isWin;
}

Coordinate create_coord (int direction, int length) {
Coordinate position;

	if (direction == HORIZONTAL)
	{
		position.row    = rand_num (0, ROWS);
		position.column = rand_num (0, COLS - length);
	} else 
	{ 
		position.row    = rand_num (0, ROWS - length);
		position.column = rand_num (0, COLS);
	}

	return position;
}

Coordinate check_target (void) {
	Coordinate target;

	fflush (stdin);

	printf ("Please enter coordinates with a space in between.\n");
	scanf ("%d %d", &target.row, &target.column);

	return target;
}

short check_shot (Cell board[][COLS], Coordinate target) {
	int hit = -2;

	switch (board[target.row][target.column].symbol) {
		case WATER: 
			hit = 0;
			break;
		case CARRIER: 
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			hit = 1;
			break;

		case HIT:
		case MISS:
		default:
			hit = -1;
			break;
	}	

	return hit;
}

int rand_num (int lowest, int highest) {
	if (lowest == 0)
		return rand () % ++highest;
	
	if (lowest > 0)
		return rand () % ++highest + lowest;
}
