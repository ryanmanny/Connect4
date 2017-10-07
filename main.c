/*************************************************************************
Name: Ryan Manny
Date: 12/9/2016
Program: Connect 4 Bonus Project
Program Description: Allows two users to play a fun game of Connect 4
					 Features non-monochrome graphics and "physics engine"
*************************************************************************/

#include "connect4.h"

int main(void)
{
	int turn = 0;
	Cell board[NUM_ROWS][NUM_COLS] = { NULL };
	Winner winner = NOT_OVER;
	Bool ai_player;

	srand((unsigned int) time(NULL));

	ai_player = ask_ai();

	init_board(board, NUM_ROWS, NUM_COLS);

	print_board(board, NUM_ROWS, NUM_COLS); // these two lines fix a bug where the colors codes are printed for some reason on the first call
	system("cls");
	
	print_board(board, NUM_ROWS, NUM_COLS);

	while ((winner = game_over(board, NUM_ROWS, NUM_COLS)) == NOT_OVER) //function checks to see if 4 are indeed connected. If not game continues
	{
		update(board, NUM_ROWS, NUM_COLS, turn % 2, ai_player); // completes entire turn, including placing piece and updating gravity
		turn++; //swaps players
	}

	print_win_screen(board, winner);
}