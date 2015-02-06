#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define ROWS 10
#define COLS 10
#define LOG_FILE_NAME "battleship.log"
#define NUM_OF_SHIPS  5
#define HORIZONTAL    0
#define VERTICAL      1
#define PLAYER_ONE    0
#define PLAYER_TWO    1

//Representative characters

#define WATER         '~'
#define HIT           '*'
#define MISS          'm'
#define CARRIER       'c'
#define BATTLESHIP    'b'
#define CRUISER       'r'
#define SUBMARINE     's'
#define DESTROYER     'd'

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3


typedef enum
{
	FALSE, TRUE
} Boolean;

typedef enum
{
	CARRIER_L    = 5, 
	BATTLESHIP_L = 4,
	CRUISER_L    = 3, 
	SUBMARINE_L  = 3,
	DESTROYER_L  = 2
} ShipType;

/* STRUCT TYPES */

typedef struct watercraft
{
	char   symbol;
	short  length;
	char   *name;
} WaterCraft;

typedef struct stats 
{
	int        nums_hits;
	int        num_misses;
	int        totalShots;
	double     hitMissRatio;
} Stats;

typedef struct coordinate 
{
	int        row;
	int        column;
} Coordinate;

typedef struct cell 
{
	char       symbol; 
	Coordinate position;
} Cell;

void menu(void);
void init_board(Cell gameBoard[][COLS]);
void print_board(Cell gameBoard [][COLS], Boolean showPegs);
void drop_ship(Cell gameBoard[][COLS], WaterCraft ship, Coordinate position, int direction);
void manually_place(Cell gameBoard[][COLS], WaterCraft ship[]);
void randomly_place(Cell gameBoard[][COLS], WaterCraft ship[]);
void update_board(Cell gameBoard[][COLS], Coordinate target);
void get_bounds(Boolean cardinals[], int bound, int direction);
void systemMessage(char *message);

Boolean check_destroyed(short destroyed_ship[][NUM_OF_SHIPS], short player, char ship_char, FILE *stream);
Boolean viable_coord(Cell gameBoard[][COLS], Coordinate position, int direction, int length);
Boolean convert_to_pos(Coordinate position[], char *stringPosition, int length);
Boolean check_winner(Stats players[], int player);

Coordinate create_coord(int direction, int length);
Coordinate check_target(void);

short check_shot(Cell gameBoard[][COLS], Coordinate target);

int rand_num(int lowest, int highest);
#endif
