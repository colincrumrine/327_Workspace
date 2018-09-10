#include <stdio.h>
#include <stdbool.h>

/*Recursive algorithm used to visit all open knight's tours on a 5 x 5 board. 
int i: used as variable to tack current row
int j: used as variable to tack current column
currTour[5][5]: current tour of visited tiles in order they were visited
int tile: Count of tiles visited on the current tour.
*/
void knightsTour(int i, int j, int currTour[5][5], int tile);
/* Prints currTour my mapping visited values and values stored in numbered chess board into a 1D array before printing. */
void printArr(int currTour[5][5]);
/* Checks to see if current value of i or j is out of the threshold of the 2D array. Either i,j > 4 or i,j < 0 will result in a 0, or false return. */
int isValidPos(int i, int j);

/* Total amount of tours printed. */
int totalTours = 1;
/* List of moves that can be made in the i direction. */
int iMove[] = {2, 1, -1, -2, -2, -1, 1, 2};
/* List of moves that can be made in the j direction. */
int jMove[] = {1, 2, 2, 1, -1 , -2, -2, - 1};
/* Chessboard layout with numbered positions used for printing in printArr(). */
int board[5][5] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};

int main(int argc, char *argv[])
{
	/* Array to store the current tour the knight is taking. All values are initially 0, or un-visited. */
	int currTour[5][5] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	/* Row position that each recursive call of knightsTour() starts with. */
	int startI;
	/* Row position that each recursive call of knightsTour() starts with. */
	int startJ;
	for(startI = 0; startI < 5; startI++)
	{
		for(startJ = 0; startJ < 5; startJ++)
		{
			/* Tile visit count. Stops recursive call when 25 tiles have been visited. */
			int tile = 1;
			knightsTour(startI,startJ,currTour,tile);
		}
	}
	return 0;
}

void knightsTour(int i, int j, int currTour[5][5], int tile)
{
/* Update current tour board with the number  */
currTour[i][j] = tile;

/* If the tile count is 25 (therefore at the end of the current tour), print the tour set the last tile to unvisited and return. */
if(tile == 25)
{
	printArr(currTour);
	currTour[i][j] = 0;
	return;
}
		/* For loop iterates thorugh all 8 moves the knight can make and checks to see if the new tile placement would be valid.
			If it is a valid placement and the tile has yet to be visited, go there and increase the tile count by 1.*/
		int k;
		for (k = 0; k < 8; k++)
        {
            int newI = i + iMove[k];
            int newJ = j + jMove[k];

            if (isValidPos(newI, newJ) == 1 && currTour[newI][newJ] == 0)
			{
				knightsTour(newI, newJ, currTour, tile + 1);
			}
                
        }

        currTour[i][j] = 0;
}
void printArr(int currTour[5][5])
{
	/* Prints number of total tours printed for ease of use. :D */
	printf("(%d.) ", totalTours);
	totalTours++;
	int outTour[25];
	
	/* Maps positions of the visited 2D array (currTour) with the numbered board (board) into the 1D array (outTour) to be printed. */
	int i, j;
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			outTour[currTour[i][j] - 1] = board[i][j];
		}
	}
	
	/* Prints the 1D array in specified format. */
	int k;
	for(k = 0; k < 25; k++)
	{
		
		if(k == 24)
		{
			printf("%d", outTour[k]);
		}else
		{
			printf("%d, ", outTour[k]);
		}
	}
	printf("\n");
}

int isValidPos(int i, int j)
{	
	/* Checks to see if the potential move the night could make will be outside the boundaries of the 2D array. */
	if(i < 0 || i > 4 || j < 0 || j > 4)
	{
		return 0;
	}
	return 1;
}