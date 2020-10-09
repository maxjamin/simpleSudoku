#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 9

/*	Struct: sudoku 
	Description: struct holds data for board structure
*/
typedef struct Box
{
	int value;
	int setPiece;
} Box;

typedef struct Sudoku
{
	Box *board;
	bool loadedBoard;
} Sudoku;

/*	Function: allocateSudoku
	Description: Allocate memory for a Sudoku struct and return 
	structure to called function
*/
Sudoku * allocateSudoku(int size)
{
	Sudoku *tempSudoku = calloc(1, sizeof(struct Sudoku)); 

	tempSudoku->board = calloc(size*size, sizeof(Box));
	tempSudoku->loadedBoard = false;

	return tempSudoku;
}
/*
*	Function:deallocateSudoku
	Description: Deallocate memory for board and return memory

*/
void deallocateSudoku(Sudoku *board)
{
	free(board->board);
	free(board);
}

/*
	Function: checkRow
	Description: check Row in table if value passed is currently in row	
*/
int checkRow(const Sudoku *board, int row, int value)
{
	int firstPlace = row * 9;
	int lastPlace = firstPlace+9;

	//printf("in checkRow\n");
	// check all values in row
	// if found return 0
	for(firstPlace; firstPlace < lastPlace; firstPlace++)
		if(board->board[firstPlace].value == value)
			return 1;

	return 0;
}
/*
	Function: checkColumn
	Description: check Column in table if value passed is currently in row	
*/
int checkColumn(const Sudoku *board, int column, int value)
{

	// check all values in column
	// if found return 0
	//printf("In checkColumn\n");
	int lastPlace = (SIZE*SIZE) - (SIZE-column);

	for(; column <= lastPlace; column+=SIZE)
	{
		//printf("%d\n",  column);
		if(board->board[column].value == value)
			return 1;
	}

	return 0;

}
/*
	Function:findBoxIndex
	Description: returns a value from 0-2 according to coordinate passed.
*/
int findBoxIndex(int coordinate)
{
	int cor=0;

	if(coordinate < 3) 
	{cor = 0; }
	else if(coordinate < 6)
	{cor = 1; }
	else
		cor=2;

	return cor;
}

/*
	Function:checkBox
	Description: check the size/3 box that the row or column passed to the function
*/
int checkBox(Sudoku *board, int x, int y, int value)
{
	//find the box that x,y cross
	int xCor;
	int yCor;
	xCor = findBoxIndex(x);
	yCor = findBoxIndex(y);

	//printf("in checkBox\n");
	//printf("Values %d x:%d  %d y:%d\n", x, xCor, y, yCor);

	int startIndex = yCor*SIZE*3 + xCor*3;
	int endIndex = startIndex+(9*2)+2;

	//printf("Start index: %d, %d\n", startIndex, endIndex);
	//printf("Starting index value: %d end:%d\n", board->board[startIndex], board->board[endIndex]);

	//check values for value parameter 
	for(int x= startIndex; x <= endIndex; x+=6)
	{	int temp = x+3;
		for(; x < temp; x++)
		{
			//printf("v: %d ", board->board[x]);
			if(board->board[x].value == value)
				return 1;
		}
		//printf("\n");
	}
	return 0;
}

/*
	Function: printBoard
	Description: prints the values of the board value for a passed
	Sudoku struct
*/
void printBoard(Sudoku *board)
{
	int totalSize = SIZE*SIZE;
	int tempSize = 0;
	int rowCounter = 1;


	printf("\n   0 1 2   3 4 5   6 7 8\n");
	printf(" 0-");
	for(Box *i= board->board; tempSize < totalSize; i++)
	{
		tempSize++;
		printf("%d ", *i);

		if(tempSize % (SIZE/3) == 0)
			printf("| ");
		//return if end of row and add row if divisible by Size/3
		if(tempSize % SIZE == 0)
		{

			if(tempSize != totalSize)
			{
				if(tempSize % (SIZE*3) == 0)
					printf("\n%.*s\n %d-", 23, "-------------------------------", rowCounter);
				else
					printf("\n %d-", rowCounter); 
			}
			else
				printf("\n\n");

			rowCounter++;
		}
	}
}
/*	Function: insertValue
*	Description: insertValue into the board data type of the passed struct
*/
int insertValue(Sudoku *board, int x, int y, int value)
{
	int tempPlace = x + (y*SIZE);

	//check that value in 0-9
	if((tempPlace > SIZE*SIZE || tempPlace < 0) || ( value > 9 || value < 0))
		return -1;

	
	board->board[tempPlace].value = value;
	return 0;
}

/*	Function: deleteValue
*	Description: delete Value into the board data
*/
int deleteValuesFromBoard(Sudoku *board, int x, int y)
{
	int tempPlace = x + (y*SIZE);

	//check that value in 0-9
	if((tempPlace > SIZE*SIZE || tempPlace < 0))
		return -1;

	
	board->board[tempPlace].value = 0;
	return 0;
}

/*	Function: insertValue
*	Description: insertValue into the board data type of the passed struct
	Last value is set value meaning the value can't be changed.
*/
int insertValueRealIndex(Sudoku *board, int place, int value, int setValue)
{

	//check that value in 0-9
	if((place > SIZE*SIZE || place < 0) || ( value > 9 || value < 0))
		return -1;

	if(value > 0)
	{
		board->board[place].setPiece = 1;
		board->board[place].value = value;
	}
	else
	{
		board->board[place].setPiece = 0;
		board->board[place].value = 0;
	}

	return 0;
}


/*	Function: addValuesFromFile
	Description: add value from file to sudoku struct passed
*/

int addValuesFromFile(char *fileName, Sudoku *board)
{
	int c;
	FILE *file;
	file = fopen(fileName, "r");

	if(!file) {
		perror("Error");
		return -1;
	}


	int boardCounter = 0;
 	while ((c = getc(file)) != EOF || boardCounter < 81)
    {
    	int a = (int)c - 48;;
    	if(a != -16)
    	{
    		//printf("A: %d \n", a);
    		//printf("Return %d \n", 
    		insertValueRealIndex(board, boardCounter, a, 1);
    		boardCounter++;
    	}
    	board->loadedBoard = true;
    }
    fclose(file);
    return 0;


}

int sudokuSolved(const Sudoku *board)
{
	for(int i = 0; i < SIZE*SIZE;i++)
	{
		if(board->board[i].value == 0)
			return 0;

	}
	return 1;
}

/*	Function: heuristic solve sudoku
*/
int heuristicSolveSudoku(Sudoku *board, int x)
{


	if(sudokuSolved(board))
		return true;

	//skip over set pieces 
	while((board->board[x].setPiece == 1))
		x++;

	int intRowValue = (double) x / (double) SIZE;
	int intColumnValue = x - (intRowValue*SIZE);

	for(int value=1; value < 10; value++)
	{
		if(	checkRow(board, intRowValue, value) == 0 &&
					checkColumn(board, intColumnValue, value) == 0 &&
					checkBox(board, intColumnValue, intRowValue, value) == 0)
		{
			//printf("Using value %d for (%d,%d)\n", board->board[x].value, intRowValue, intColumnValue);
			board->board[x].value = value;
			if (heuristicSolveSudoku(board, x+1))
				{ 
					return true; 
				}
		}
	}
	//if no possible values found possible return to parent node
	board->board[x].value = 0;
	return false;

}

int checkGame(Sudoku *board)
{
	//check rows 
	for(int row=0; row < SIZE; row++)
	{
		int rowsValues = 0;
		int firstPlace = row * 9;
		int lastPlace = firstPlace+9;

		for(firstPlace; firstPlace < lastPlace; firstPlace++)
			{
				if(rowsValues > 45)
					return -1; 
				if(board->board[firstPlace].setPiece)	
					rowsValues += board->board[firstPlace].value;
			}
	}		

	//check column
	for(int column=0; column < SIZE; column++)
	{
		int columnValues = 0;
		int lastPlace = (SIZE*SIZE) - (SIZE-column);
		for(int columnIncr = column; columnIncr <= lastPlace; columnIncr+=SIZE)
		{
			if(columnValues > 45)
				return -1;
			if(board->board[columnIncr].setPiece)
				columnValues += board->board[columnIncr].value;
		} 
	}

	
	//check boxes
	for(int xCor=0; xCor < 3; xCor++)
	{
		for(int yCor=0; yCor < 3; yCor++)
		{
			int startIndex = yCor*SIZE*3 + xCor*3;
			int endIndex = startIndex+(9*2)+2;
			int boxValues = 0;
			for(int x= startIndex; x <= endIndex; x+=6)
			{	int temp = x+3;
				for(; x < temp; x++)
				{
					if(boxValues > 45)
						return -1;
					if(board->board[x].setPiece)
						boxValues += board->board[x].value;
				}
			}
		}
	}
	return 0;

}

/*	Function: controlGame
	Description: allow user to control games through commands

*/
int controlGame(Sudoku *board, bool checkLoadedtable)
{
	char buffer[100];
	char fileName[100];
	char x[100];
	char y[100];
	char value[100];

	printf("Starting game of Sudoku enter help for more commands\n");

	do {
		char doubleCheck[] = "y";

		printf (":>");
		fflush (stdout);
		fgets(buffer, 100, stdin);

		if(strcmp (buffer, "help\n") == 0)
		{
			printf("commands: "
				"\n\t-help - prints all commands,"
				"\n\t-printTable - prints current table "
				"\n\t-addTable - add value to table, "
				"\n\t-addTableFromFile - enter a file and load a table into memory,"
				"\n\t-insertValue - insert a Value into the table with x,y format,"
				"\n\t-deleteValues - delete a Value into the table with x,y format,"
				"\n\t-solve - solve the sudoku table currently loaded"
				"\n");
		}
		else if(strcmp (buffer, "printTable\n") == 0)
		{	if(board->loadedBoard)
				printBoard(board); 
			else
				printf("No table loaded\n");
		}
		else if(strcmp (buffer, "insertValue\n") == 0)
		{	

			printf("Enter x value:\n");
			fgets(x, 100, stdin);
			printf("Enter y value:\n");
			fgets(y, 100, stdin);
			printf("Enter value:\n");
			fgets(value, 100, stdin);

			insertValue(board, atoi(x), atoi(y), atoi(value));
		}
		else if(strcmp(buffer, "checkTable\n") == 0)
		{
			checkGame(board);
		}
		else if(strcmp(buffer, "addTable\n") == 0)
		{
			char buf[0x100], *p;

			printf("Enter fileName:\n");
			fgets(buf, 100, stdin);

			if ((p = strchr(buf, '\n')) != NULL) { 
    			*p = '\0'; /* remove newline */
			}

			if(addValuesFromFile(buf, board) == 0)
				printf("File: %s has been added\n", buf);
		}
		else if(strcmp(buffer, "deleteValues\n") == 0)
		{
			printf("Enter x value:\n");
			fgets(x, 100, stdin);
			printf("Enter y value:\n");
			fgets(y, 100, stdin);

			deleteValuesFromBoard(board, atoi(x), atoi(y));
		}
		else if(strcmp(buffer, "solve\n") == 0)
		{
			printf("Are you sure?: y/n\n");
			scanf("%79s", buffer);
			if(!strcmp(doubleCheck, buffer))
				heuristicSolveSudoku(board, 0);

		}
		else if(strcmp(buffer, "solved?\n") == 0)
		{
			if(checkGame(board) == 0 && sudokuSolved(board))
				printf("Game solved\n");
			else
				printf("Game not Solved\n");
		}
		else if(strcmp (buffer, "exit\n") == 0)
		{
			return 0;
		}
		else
			printf("Command %s not known\n", buffer);


	} while (1);

}

/*	Function: main
	Description: driver function of the program

*/
int main()
{
	printf("Starting program...\n");

	Sudoku *sudokuBoard = allocateSudoku(SIZE);

	//addValuesFromFile("board.txt", sudokuBoard);
	controlGame(sudokuBoard, 0);

	deallocateSudoku(sudokuBoard);
}