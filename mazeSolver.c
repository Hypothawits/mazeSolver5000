#include <stdio.h>
#include "main.h"
#include "mazeSolver.h"

/*
Name: mazeSolver.c
Contains Functions for solving the maze.
*/

/*
WALLS toBinary(int decimal):
	Convert the decimal value into a 4bit structure for storing locations of walls.
	There are only 15 valid tiles, with int values 0->14, any value outside this range
	will throw an error.
*/
WALLS toBinary(int decimal) {
	//Create variable for WALLS
	WALLS walls;

	//Convert the decimal into WALLS
	switch (decimal)
	{
	case 0:
		walls.U = False;
		walls.L = False;
		walls.D = False;
		walls.R = False;
		return(walls);
	case 1:
		walls.U = False;
		walls.L = False;
		walls.D = False;
		walls.R = True;
		return(walls);
	case 2:
		walls.U = False;
		walls.L = False;
		walls.D = True;
		walls.R = False;
		return(walls);
	case 3:
		walls.U = False;
		walls.L = False;
		walls.D = True;
		walls.R = True;
		return(walls);
	case 4:
		walls.U = False;
		walls.L = True;
		walls.D = False;
		walls.R = False;
		return(walls);
	case 5:
		walls.U = False;
		walls.L = True;
		walls.D = False;
		walls.R = True;
		return(walls);
	case 6:
		walls.U = False;
		walls.L = True;
		walls.D = True;
		walls.R = False;
		return(walls);
	case 7:
		walls.U = False;
		walls.L = True;
		walls.D = True;
		walls.R = True;
		return(walls);
	case 8:
		walls.U = True;
		walls.L = False;
		walls.D = False;
		walls.R = False;
		return(walls);
	case 9:
		walls.U = True;
		walls.L = False;
		walls.D = False;
		walls.R = True;
		return(walls);
	case 10:
		walls.U = True;
		walls.L = False;
		walls.D = True;
		walls.R = False;
		return(walls);
	case 11:
		walls.U = True;
		walls.L = False;
		walls.D = True;
		walls.R = True;
		return(walls);
	case 12:
		walls.U = True;
		walls.L = True;
		walls.D = False;
		walls.R = False;
		return(walls);
	case 13:
		walls.U = True;
		walls.L = True;
		walls.D = False;
		walls.R = True;
		return(walls);
	case 14:
		walls.U = True;
		walls.L = True;
		walls.D = True;
		walls.R = False;
		return(walls);
	case 15:
		walls.U = True;
		walls.L = True;
		walls.D = True;
		walls.R = True;
		quitError("Error: Trapped in Maze", decimal);
		return(walls);

	default:	//all walls (value 15) should never enter into this position
		walls.U = True;
		walls.L = True;
		walls.D = True;
		walls.R = True;
		quitError("\nError: Invalid Wall! Value:%d", decimal);
		return(walls);
	}
}

/*
char moveMaze(MAZEDATA* mData, char direction):
	Prepare and move to next level of recursion.
	The function adds the current position to the pathArray (At the current step number).
	The current number of steps is then incremented.
	The direction of movement within the maze is passed into the function, this
	is used to increment the x or y coordinate accordingly.
	Then calling the next level of the recursion.
*/
char moveMaze(MAZEDATA* mData, char direction)
{
	//Add current pos to pathArray
	mData->pathArray[mData->stepNumber].X = mData->position.X;
	mData->pathArray[mData->stepNumber].Y = mData->position.Y;

	//increment stepNumber
	mData->stepNumber += 1;

	//
	switch (direction)
	{
	case('U'):	//move pos up
		mData->position.Y--;
		break;

	case('D'):	//move pos down
		mData->position.Y++;
		break;

	case('R'):	//move pos right
		mData->position.X++;
		break;

	case('L'):	//move pos left
		mData->position.X--;
		break;

	default:
		//if passed an invalid movement direction
		quitError("Error: Invalid Direction");
		break;
	}

	//Enter next level of recursion
	char pathFound = pathFinder(mData);

	//Return to previous level of Recursion
	return(pathFound);
}

/*
char pathFinder(MAZEDATA* mData):
	The recursive function, takes the MAZEDATA as an input.
	The function checks if the End has been reached. If the End is found, it returns up the recursions.
	The function checks if it has crossed it's own path. If it has, it returns to previous recursion.

	To Determine the next step to take, the value of the current tile is converted into a usable format.
	Update the current location visited to True.
	Determine the direction of previous movement, used to avoid moving back on itself.
	Check the location of walls, if the path was found, and if it came from the direction.
	These checks are used to determine the direction the solver should move. The solver prioritizes
	moving Right and then Down.

	If none of those movement directions reach the end, then a dead-end may have been found,
	so move up to previous level of recursion and try next direction. To move to previous recursion
	decrement the step number and load the previous position from the pathArray.
*/
char pathFinder(MAZEDATA* mData)
{
	//char 'Boolean" for when path is found (local to each instance of function call)
	char pathFound = False;

	//Check if current position is the End location (then End)
	if (mData->position.X == mData->endMaze.X && mData->position.Y == mData->endMaze.Y)
	{	//End has been reached

		//Add current pos to pathArray
		mData->pathArray[mData->stepNumber].X = mData->position.X;
		mData->pathArray[mData->stepNumber].Y = mData->position.Y;
		// Return pathFound true
		return(True);
	}
	else
	{
		//Check if current position is on current path, then check movement directions and enter next level of recursion
		if (mData->mazeVisit[mData->position.X][mData->position.Y])	//if True then has visited
		{	// path overlaps path already taken, reverse and try another route.

			// Don't add new position to pathArray
			// decrement stepNumber
			mData->stepNumber -= 1;
			// set pos to previous position from pathArray
			mData->position.X = mData->pathArray[mData->stepNumber].X;
			mData->position.Y = mData->pathArray[mData->stepNumber].Y;

			// Return pathFound False
			return(False);
		}
		else
		{	//Current Location is not on path already taken, look for new direction to move in.
			//Check which directions are free to move in, and where the path was found in a previous direction and is returning from recursion

			//Convert to "binary" values, where 1 indicates a wall in that direction.
			WALLS walls = toBinary(mData->maze[mData->position.X][mData->position.Y]);

			//Set current position to 'visited = True'
			mData->mazeVisit[mData->position.X][mData->position.Y] = True;

			//Calculate difference to get previous move, to avoid going back on self
			char deltaX, deltaY;
			if (mData->stepNumber == 0)
			{	//If first location, no movement
				deltaX = 0;
				deltaY = 0;
			}
			else
			{	//Determine movement through maze
				deltaX = mData->position.X - mData->pathArray[mData->stepNumber - 1].X;	//X direction last taken, R=1, L=-1
				deltaY = mData->position.Y - mData->pathArray[mData->stepNumber - 1].Y;	//Y direction last taken, D=1, U=-1
			}

			//Check if direction free & path not found & hasn't come from that direction
			if (walls.R == 0 && !pathFound && deltaX != -1)
			{	//Can move in Right direction
				pathFound = moveMaze(mData, 'R');
			}

			if (walls.D == 0 && !pathFound && deltaY != -1)
			{	//Can move in Down direction
				pathFound = moveMaze(mData, 'D');
			}

			if (walls.L == 0 && !pathFound && deltaX != 1)
			{	//Can move in Left direction
				pathFound = moveMaze(mData, 'L');
			}

			if (walls.U == 0 && !pathFound && deltaY != 1)
			{	//Can move in Up direction
				pathFound = moveMaze(mData, 'U');
			}

			if (pathFound == 0)
			{	//if Path still not found, then possible dead-end encountered and so
				//return 0 to upper level and reset all appropriate values

				// Don't add new position to pathArray
				// decrement stepNumber
				mData->stepNumber -= 1;
				// set pos to previous position (from pathArray)
				mData->position.X = mData->pathArray[mData->stepNumber].X;
				mData->position.Y = mData->pathArray[mData->stepNumber].Y;

				// Return pathFound False
				return(False);
			}
			else
			{	//Path has been found, return 1!
				return(True);
			}
		}
	}
}