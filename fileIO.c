#include <stdio.h>
#include "main.h"

/*
Name: fileIO.c
Contains Functions for reading and writing to files.
*/

/*
void checkFile(FILE* filePointer, char* fileName1):
	Check if the file opened successfully.
	The function takes a file pointer and the file name as arguments.
	If the FILE pointer is Zero or NULL then the FILE failed to open.
	If the FILE failed, the program Exits with an Error message.
	IF the file opened, a success message is shown and the program continues.
*/
void checkFile(FILE* filePointer, char* fileName1)
{
	//Check if the filePointer is non-Zero
	if ((filePointer == NULL) || (filePointer == 0))
	{	//Error has occurred
		printf("Error, Failed to open: %s\n", fileName1);	//display Error message
		quitError(" ");
	}
	else
	{	//No Error
		printf("Opened File: %s\n", fileName1);	//DIsplay success message
	}
	return;
}

/*
void writeSolutionFile(MAZEDATA* mData, FILE* solutionFile):
	Writes the number of steps to the solution file, then the movement
	steps of solution path.	Loops through all locations along the path,
	comparing the current step with the next to determine the direction.
	Once direction is determined, writes it to the solution file.
*/
void writeSolutionFile(MAZEDATA* mData, FILE* solutionFile)
{
	//write number of moves required to solve maze to solutionFile
	fprintf_s(solutionFile, "%d", mData->stepNumber);
	fprintf_s(solutionFile, "\n");			//add new line

	int i;	//Increment
	//Loop through each step of the path.
	for (i = 0; i < mData->stepNumber; i++)
	{
		//Determine the direction of movement by calculating the x,y difference between consecitive steps.
		char xMove = mData->pathArray[i + 1].X - mData->pathArray[i].X;
		char yMove = mData->pathArray[i + 1].Y - mData->pathArray[i].Y;

		if (xMove)
		{	//if xMove is non-zero; Move was in x direction
			switch (xMove)	//switch case for finding direction on selected axis
			{
			case 1:		//Case: moved Right (R)
				fprintf_s(solutionFile, "R ");
				break;

			case -1:	//Case: moved Left (L)
				fprintf_s(solutionFile, "L ");
				break;

			default:	//Case: Error invalid move
				printf("Error Writing Solution\n");
				quitError("Invalid Move Taken\n");
				break;
			}
		}
		else
		{	//if yMove is non-zero; Move was in y direction
			switch (yMove)	//switch case for finding direction on selected axis
			{
			case 1:		//Case: moved Down (D)
				fprintf_s(solutionFile, "D ");
				break;

			case -1:	//Case: moved Up (U)
				fprintf_s(solutionFile, "U ");
				break;

			default:	//Case: Error invalid move
				printf("Error Writing Solution\n");
				quitError("Invalid Move Taken\n");
				break;
			}
		}
	}
	return;	//No Error
}

/*
void getMazeParam(FILE* mazeFile, MAZEDATA* mazeData):
	Load in the 6 maze parameters from the maze.txt file.
	Check that 6 maze parameters where found and that
	they make logical sense.
*/
void getMazeParam(FILE* mazeFile, MAZEDATA* mData)
{	//read in maze size, start and end location, store XY values in Coordinate Struct
	int foundValues;	//Store the number of successful values read

	foundValues = fscanf_s(mazeFile, "%d %d %d %d %d %d ",
		&mData->sizeMaze.Y, &mData->sizeMaze.X,
		&mData->startMaze.Y, &mData->startMaze.X,
		&mData->endMaze.Y, &mData->endMaze.X);

	//Check if all values successfully read in, should have 6 values
	if (foundValues != 6)
	{
		quitError("Error reading in Size, starting location and End location");
	}
	else
	{	//check that values make sense
		if (mData->sizeMaze.Y == 0 || mData->sizeMaze.X == 0)
		{
			quitError("Error: non-real maze size!");
		}
		//Check if start or end outside maze
		if (mData->startMaze.Y >= (mData->sizeMaze.Y) ||
			mData->startMaze.X >= (mData->sizeMaze.X))
		{
			quitError("Error: No maze solution, start outside maze.");
		}
		if (mData->endMaze.Y >= (mData->sizeMaze.Y) ||
			mData->endMaze.X >= (mData->sizeMaze.X))
		{
			quitError("Error: No maze solution, end outside maze.");
		}
	}
}


/*
void getMazeData(FILE* mazeFile, MAZEDATA* mData):
	Gets the maze structure data from the maze.txt file.
	Integers are read 1 at a time and loaded into the
	2D array. At the same time, the location is set to
	unvisited in the 'visited array'. Each integer loaded
	is to be a valid maze value. The total number read is
	checked if the expected to ensure all numbers are loaded.
*/
void getMazeData(FILE* mazeFile, MAZEDATA* mData)
{	//Gets the Maze values out of the Text File

	//Incrementors
	int i, j;
	char readSuccess;	//number of values read in by fscanf
	int valueCount = 0;	//Total number of values read in
	int mazeValue;		//Value of maze at position (i,j)

						//Increment through each value of maze data
	for (j = 0; j < mData->sizeMaze.Y; j++)
	{
		for (i = 0; i < mData->sizeMaze.X; i++)
		{
			readSuccess = fscanf_s(mazeFile, "%d", &mazeValue);	//Pointer increments on each function call
			valueCount += readSuccess;							//Add number read in to the total

																//maze value cannot be larger than 15,therefore check and return error if larger.
			if (mazeValue <= 15)
			{	//Maze value within range, add to array
				mData->maze[i][j] = mazeValue;						//Stores 
				mData->mazeVisit[i][j] = False;			//Set to not visited
			}
			else
			{	//Maze value out of range, throw Error
				printf("Error Loading Maze Data\n");
				quitError("Error: Invalid Maze Data in File\n");
			}
		}
	}
	if (valueCount != mData->sizeMaze.X*mData->sizeMaze.Y)
	{	//if Value Count doesn't match the number in the maze, give error 
		printf("Error Loading Maze Data\n");
		quitError("Error: Insufficient Maze Data in File\n");
	}

	return;		//return with no error
}