#include "connect4.h"

//Initializes all board squares in array to empty
void init_board(Cell board[][NUM_COLS], int rows, int cols)
{
	int row_index = 0, col_index = 0;

	for (; row_index < rows; ++row_index)
	{
		for (col_index = 0; col_index < cols; ++col_index)
		{
			board[row_index][col_index].color = EMPTY;
			board[row_index][col_index].isOccupied = FALSE;
			board[row_index][col_index].gravity = FALSE;
			board[row_index][col_index].position.row = row_index;
			board[row_index][col_index].position.col = col_index;
		}
	}
}

//Runs through every cell and prints them
void print_board(Cell board[][NUM_COLS], int rows, int cols)
{
	int row_index = 0, col_index = 0, index_index = 0;
	
	printf("  Connect 4\n");
	printf("  "); // Adds the leading corner area to the graph
	for (; index_index < NUM_COLS; index_index++)
		printf("%d ", index_index + 1); // Counts out the column indexes
	printf("\n");
	for (; row_index < rows; ++row_index)
	{
		printf("%d ", row_index + 1); // Adds row index before row
		for (col_index = 0; col_index < cols; ++col_index)
		{
			if (board[row_index][col_index].isOccupied == FALSE) //space looks less confusing than O
				printf("  ");
			else if (board[row_index][col_index].color == BLACK)
				printf(BLUE "B " RESET);
			else if (board[row_index][col_index].color == RED)
				printf(PINK "R " RESET);
		}
		printf("\n");
	}
	//debug
	//print_gravity(board, rows, cols);
}

//For early debug purposes. Not called in program
void print_gravity(Cell board[][NUM_COLS], int rows, int cols)
{
	int row_index = 0, col_index = 0, index_index = 0;

	printf("  ");
	for (; index_index < NUM_COLS; index_index++)
		printf("%d ", index_index + 1);
	printf("\n");
	for (; row_index < rows; ++row_index)
	{
		printf("%d ", row_index + 1);
		for (col_index = 0; col_index < cols; ++col_index)
		{
			if (board[row_index][col_index].gravity)
				printf("G ");
			else
				printf("N ");
		}
		printf("\n");
	}
}

//Uses gravity to update existing squares and lets update know if gravity is finished updating (to prevent infinite loop)
Bool gravity_update(Cell board[][NUM_COLS], int rows, int cols)
{
	int row_index = 0, col_index = 0;
	Bool gravity_was_updated = FALSE; // this return lets the update function know if any pieces needed updating. As a result this function is called one extra time, however
	Color previous_color = '\0';

	for (col_index = 0; col_index < cols; ++col_index)
	{
		for (row_index = 0; row_index < rows; ++row_index)
		{
			if (board[row_index][col_index].gravity == TRUE)
			{
				previous_color = board[row_index][col_index].color;

				board[row_index][col_index].isOccupied = FALSE;
				board[row_index][col_index].color = EMPTY;
				board[row_index][col_index].gravity = FALSE;
				
				board[row_index + 1][col_index].isOccupied = TRUE;
				board[row_index + 1][col_index].color = previous_color;
				board[row_index + 1][col_index].gravity = check_gravity(board, row_index + 1, col_index);

				gravity_was_updated = TRUE;
				return gravity_was_updated; //Only allows one square update per call to allow for graphics to update appropriately
			}
		}
	}
	return gravity_was_updated;
}

//Looks at cell beneath and decides whether gravity applies
Bool check_gravity(Cell board[][NUM_COLS], int row, int col)
{
	Bool gravity = FALSE;
	if (row != NUM_ROWS - 1) // if the piece is not on the bottom of the board
	{
		if (board[row + 1][col].isOccupied == TRUE) // if there's a piece below, stop the piece
			gravity = FALSE;
		if (board[row + 1][col].isOccupied == FALSE) // if there's nothing below, piece falls down
			gravity = TRUE;
	}
	else if (row == NUM_ROWS - 1) // if the piece has reached the bottom
		gravity = FALSE; // stop the piece so it doesn't fall through the bottom
	else // just in case
		gravity = FALSE;
	return gravity; // DON'T DELETE THIS LINE AGAIN WHAT WAS I THINKING
}

//Goes through the entire process of one turn, including placing piece and updating the gravity
void update(Cell board[][NUM_COLS], int rows, int cols, Turn turn, Bool p2_ai)
{
	place_cell(board, rows, cols, turn, p2_ai);
	system("cls");
	print_board(board, rows, cols);
	_sleep(GRAVITY);

	while (gravity_update(board, rows, cols)) //this loop acts as the graphics engine basically. pretty cool
	{
		system("cls");
		print_board(board, rows, cols);
		_sleep(GRAVITY); //pauses between each update to give the illusion that the pieces are actually falling.
	}
}

//Allows user to drop piece in any free column
void place_cell(Cell board[][NUM_COLS], int rows, int cols, Turn turn, Bool p2_ai)
{
	unsigned int column = 0;
	Bool success = FALSE;

	if (turn == P1)
		printf(PINK "Player 1's turn\n" RESET);
	
	if (turn == P2)
		printf(BLUE "Player 2's turn\n" RESET);

	if (turn == P1 || !p2_ai)
	{
		printf("Enter the column to drop disc: ");
		do {
			success = scanf("%d", &column);
			if (success == FALSE)
				clear_buffer();
			if (success == TRUE)
				column--;
		} while (column >= NUM_COLS || board[0][column].isOccupied == TRUE);
	}
	else if (p2_ai && turn == P2)
	{
		column = ai_random(board, NUM_ROWS, NUM_COLS); //Could put the do while on the outside of the function to maintain symmetry
		//column = ai(board, NUM_ROWS, NUM_COLS); //Not working properly
	}

	board[0][column].isOccupied = TRUE; //places piece
	board[0][column].gravity = check_gravity(board, 0, column); //checks if piece is floating or not

	if (turn == P1)
	{
		//debug
		//printf("Red\n");
		board[0][column].color = RED;
	}
	if (turn == P2)
	{
		//debug
		//printf("Black\n");
		board[0][column].color = BLACK;
	}
}

//Uses some sort of chain logic to trace around entire board and see if game is over. If all cells full but no winner, returns tie
//There is a much more efficient way to do this I imagine, which would probably involve copying the array and overwriting cell contents from unsuccessful searches with empty
//Also add a feature that records the 4 winning tiles and changes them to green on the final print
Winner game_over (Cell board[][NUM_COLS], int rows, int cols)
{
	int col_index = 0, row_index = 0;
	Bool is_winner = FALSE;

	for (col_index = 0; col_index < cols; col_index++)
	{
		for (row_index = 0; row_index < rows; row_index++)
		{
			if (board[row_index][col_index].isOccupied == TRUE)
			{
				is_winner = search_max_row(board, row_index, col_index);
				if (is_winner && board[row_index][col_index].color == RED)
					return P1WIN;
				if (is_winner && board[row_index][col_index].color == BLACK)
					return P2WIN;
			}
		}
	}

	//now checks if board is full AFTER the winner check to make sure the last cell placed didn't create a winner
	if (full_board(board, rows, cols))
		return TIE;

	return NOT_OVER;
}

//Searches board in horizontal, vertical and diagonal directions to see if indeed 4 have been connected
Bool search_max_row(Cell board[][NUM_COLS], int row, int col)
{
	Color search_color = board[row][col].color;

	//Traces down. Since the game_over function scans from top-left to bottom-right, if a winning possibility wasn't found tracing down it won't be found tracing up
	if (row <= 2) // Max row (value) = 5, Max column (value) = 6
	{
		if (board[row + 1][col].color == search_color)
			if (board[row + 2][col].color == search_color)//recursion could clean this up but this is probably actually more efficient
				if (board[row + 3][col].color == search_color)
				{
					//debug
					printf("Winner found down\n");
					return TRUE;
				}
	}

	//Traces right based on the same principle as tracing down, b/c parent function scans left to right
	if (col <= 3) // Max row (value) = 5, Max column (value) = 6
	{
		if (board[row][col + 1].color == search_color)
			if (board[row][col + 2].color == search_color)
				if (board[row][col + 3].color == search_color)
				{
					//debug
					printf("Winner found right\n");
					return TRUE;
				}
	}

	//Now the diagonal searching. This is a bit tougher but thankfully only needs to search in the left-down and right-down directions because of previously outlined principles
	if (row <= 2 && col <= 3 ) // Max row (value) = 5, Max column (value) = 6
	{
		if (board[row + 1][col + 1].color == search_color)
			if (board[row + 2][col + 2].color == search_color)
				if (board[row + 3][col + 3].color == search_color)
				{
					//debug
					printf("Winner found diagonal right\n");
					return TRUE;
				}
	}

	if (row <= 2 && col >= 3) // Max row (value) = 5, Max column (value) = 6
	{
		if (board[row + 1][col - 1].color == search_color)
			if (board[row + 2][col - 2].color == search_color)
				if (board[row + 3][col - 3].color == search_color)
				{
					//debug
					printf("Winner found diagonal left\n");
					return TRUE;
				}
	}
	return FALSE;
}

//Checks to see if all of the spaces on the board are full
Bool full_board(Cell board[][NUM_COLS], int rows, int cols)
{
	int row_index = 0, col_index = 0, num_filled = 0;

	for (; row_index < rows; ++row_index)
	{
		for (col_index = 0; col_index < cols; ++col_index)
		{
			if (board[row_index][col_index].isOccupied == TRUE)
				num_filled++;
		}
	}
	if (num_filled == NUM_ROWS * NUM_COLS)
		return TRUE;
	else
		return FALSE;
}

//Once winner is known, winner displayed, game ends
void print_win_screen (Cell board[][NUM_COLS], Winner winner)
{
	system("cls");
	print_board(board, NUM_ROWS, NUM_COLS);
	if (winner == P1WIN)
	printf(PINK "Player 1 Wins! " RESET "Good work!\n");
	else if (winner == P2WIN)
	printf(BLUE "Player 2 Wins! " RESET "Good work!\n");
	else if (winner == TIE)
	printf("You two tied. Try again next time.\n");
}

//Asks user if they want to play 1 or 2 player game. Returns result to main
Bool ask_ai(void)
{
	int ai = FALSE;

	printf("How many players will be playing (1 or 2)?\n"); //Add three player mode? Definitely possible
	do {
		scanf(" %d", &ai);
	} while (ai != 1 && ai != 2);

	_sleep(500);
	clear_buffer();

	if (ai == TRUE)
	{
		printf("One player mode selected.");
		return TRUE;
	}
	else
	{
		printf("Two player mode selected.");
		return FALSE;
	}
}

//Just randomly drops a piece in a random column. Not very fun
int ai_random(Cell board[][NUM_COLS])
{
	int column = 0;
	do
	{
		column = rand() % 7;
	} while (board[0][column].isOccupied == TRUE);
	return column;
}

//Prototype advanced AI system. Needs more time
//Finds ideal column using modified check_max_row function and returns
int ai(Cell board[][NUM_COLS])
{
	Color search_color = BLACK; //Only P2 can be the AI
	int best_column = rand() % 7, col = 0, check_col = 0, row = 0, check_row = 0, num_links = 0, best_links = 0, best_links_now = 0;
	Poss possible_cells[7];

	//Finds the seven cells that dropping a cell in each of the columns would result in

	for (col = 0; col < NUM_COLS; col++)
	{
		for (row = NUM_ROWS - 1; row >= 0; row--)
		{
			if (board[row][col].isOccupied == TRUE)
			{
				if (row == 0)
					possible_cells[col].full = TRUE;
				else
				{
					possible_cells[col].full = FALSE;
					possible_cells[col].row = row + 1;
					possible_cells[col].col = col; //A little redundant
				}
				row = -1;
			}
			else if (row == 0)
			{
				possible_cells[col].full = FALSE;
				possible_cells[col].row = 5;
				possible_cells[col].col = col;
			}
		}
	}

	for (col = 0; col < NUM_COLS; col++)
	{
		if (possible_cells[col].full == FALSE)
		{

			//Scans up and down
			best_links_now = 0;
			num_links = 0;
			check_row = possible_cells[col].row - 3;
			
			if (check_row < 0)
				check_row = 0;
			while (check_row >=  0 && check_row <= 5 && check_row <= possible_cells[col].row + 3)
			{
				if (board[row][col].color == search_color)
					++num_links;
				++check_row;
			}

			if (num_links > best_links_now)
				best_links_now = num_links;
			//End up and down

			//Scans left and right
			best_links_now = 0;
			num_links = 0;
			check_col = possible_cells[col].col - 3;

			if (check_col < 0)
				check_col = 0;
			while (check_col >= 0 && check_col <= 6 && check_col <= possible_cells[col].row + 3)
			{
				if (board[possible_cells[col].row][check_col].color == search_color)
					++num_links;
				++check_col;
			}

			if (num_links > best_links_now)
				best_links_now = num_links;
			//End left and right

			////Scans up right and down left diagonally
			//best_links_now = 0;
			//num_links = 0;
			//check_col = possible_cells[col].col - 3;
			//check_row = possible_cells[col].row - 3;

			//if (check_col < 0)
			//{
			//	check_row -= check_col; //pushes the diagonal starting point over. I hope this works
			//	check_col = 0;
			//}
			//if (check_row < 0)
			//{
			//	check_col -= check_row;
			//	check_row = 0;
			//}

			//while (check_col >= 0 && check_col <= 6 && check_col <= possible_cells[col].row + 3)
			//{
			//	while (check_row >= 0 && check_row <= 5 && check_row <= possible_cells[col].row + 3)
			//	{
			//		if (board[possible_cells[col].row][check_col].color == search_color)
			//			++num_links;
			//		++check_col;
			//		++check_row;
			//	}
			//}

			//if (num_links > best_links_now)
			//	best_links_now = num_links;
			////End positive diagonal

			////Scans up right and down left diagonally
			//best_links_now = 0;
			//num_links = 0;
			//check_col = possible_cells[col].col + 3;
			//check_row = possible_cells[col].row - 3;

			//if (check_col > 6)
			//{
			//	check_row -= (6 - possible_cells[col].col); //pushes the diagonal starting point over. I hope this works
			//	check_col = 6;
			//}
			//if (check_row < 0)
			//{
			//	check_col += possible_cells[col].row;
			//	check_row = 0;
			//}

			//while (check_col >= 0 && check_col <= 6 && check_col <= possible_cells[col].row + 3)
			//{
			//	while (check_row >= 0 && check_row <= 5 && check_row <= possible_cells[col].row + 3)
			//	{
			//		if (board[possible_cells[col].row][check_col].color == search_color)
			//			++num_links;
			//		++check_col;
			//		++check_row;
			//	}
			//}

			//if (num_links > best_links_now)
			//	best_links_now = num_links;
			////End negative diagonal

			//After the check, sees if that column is indeed the best
			if (best_links_now > best_links)
			{
				best_links = best_links_now;
				best_column = col;
			}
		}
	}
	return best_column;
}

//Clears input buffer. Doesn't really work I don't think, since inputting something crazy in scanf still softlocks
void clear_buffer(void)
{
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}