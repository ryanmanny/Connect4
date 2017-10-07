#ifndef CONNECT4_H
#define CONNECT4_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define NUM_ROWS 6
#define NUM_COLS 7
#define GRAVITY 30 //higher number means lower gravity

//this defines the colors that are used in the programs
#define PINK	"\x1B[31;1m" //This says pink because RED was already defined as a key word
#define BLUE	"\x1B[34;1m" //Player 2's color is internally known as Black but their pieces print out as BLUE because it's cooler
#define RESET	"\x1B[0m"

typedef enum boolean
{
	FALSE, TRUE
} Bool;

typedef enum turn
{
	P1, P2
} Turn; //get turnt

typedef struct location
{
	int row;
	int col;
} Location;

typedef enum color
{
	EMPTY, RED, BLACK
} Color;

typedef struct cell
{
	Location position;
	Color color;
	Bool isOccupied;
	Bool gravity;
} Cell;

typedef struct possible_cell
{
	int row;
	int col;
	Bool full;
} Poss;

typedef enum winner
{
	NOT_OVER, P1WIN, P2WIN, TIE
} Winner;

void init_board(Cell board[][NUM_COLS], int rows, int cols);
void print_board(Cell board[][NUM_COLS], int rows, int cols);
void print_gravity(Cell board[][NUM_COLS], int rows, int cols);
Bool gravity_update(Cell board[][NUM_COLS], int rows, int cols);
Bool check_gravity(Cell board[][NUM_COLS], int row, int col);
void update(Cell board[][NUM_COLS], int rows, int cols, Turn turn, Bool p2_ai);
void place_cell(Cell board[][NUM_COLS], int rows, int cols, Turn turn, Bool p2_ai);
Winner game_over(Cell board[][NUM_COLS], int rows, int cols);
Bool full_board(Cell board[][NUM_COLS], int rows, int cols);
void print_win_screen(Cell board[][NUM_COLS], Winner winner);
Bool ask_ai(void);
int ai(Cell board[][NUM_COLS]);
int ai_random(Cell board[][NUM_COLS]);
void clear_buffer(void);

#endif