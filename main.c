#include <stdio.h>
#include <Windows.h>
#include <GL/GL.h>
#include "GL/freeglut.h"

#include "main.h"
#include "fileIO.h"
#include "mazeSolver.h"
#include "renderMaze.h"

/*
Name: main.c
Contains the main program loop and additional functions
for handling errors and creating the maze data arrays.
*/

int main(int argc, char *argv[])
{
	printf("Program Started\n");
	//Input Variable Storage
	char*	guiArg = " ";			
	char*	mazeFileName = " ";		
	char*	solutionFileName = " ";	
	char	displayMaze = 1;		//Bool for displaying the maze (note: true = 0, false = 1)

	//create File pointers
	FILE *mazeFile = 0;
	FILE *solutionFile = 0;

	//Create an instance of MAZEDATA
	MAZEDATA mazeData;

	/*
		Check the number of arguments passed
		and allocation to correct variable.
	*/
	switch (argc)
	{
	case 3:	//Only file names passed
		mazeFileName = argv[1];
		solutionFileName = argv[2];
		break;

	case 4:	//files and gui indicator passed
		guiArg = argv[1];
		mazeFileName = argv[2];
		solutionFileName = argv[3];
		//Test gui arg = "-d"
		displayMaze = strcmp(guiArg, "-d");
		break;

	default://Error in the number of arguments passed
		printf("Error: Invalid Number of Arguments Passed\n");
		quitError("Press Enter to Exit Program.");
		break;
	}

	/*
		Open Files and check if successful
	*/
	printf("Opening Files... \n");
	fopen_s(&mazeFile, mazeFileName, "r");
	fopen_s(&solutionFile, solutionFileName, "w");

	checkFile(mazeFile, mazeFileName);
	checkFile(solutionFile, solutionFileName);
	printf("Files Opened Successfully \n");

	
	/*
		Read parameters and structure data
		from maze.txt file, into the created array.
	*/
	printf("Getting Data from File...\n");
	getMazeParam(mazeFile, &mazeData);
	printf("Got Maze Parameters...\n");

	create2DArray(&mazeData);
	printf("Created maze Array...\n");

	getMazeData(mazeFile, &mazeData);
	printf("Got Maze Data\n");

	fclose(mazeFile);

	/*
		Solve the Maze with Recursion
	*/
	printf("Started Maze Solver...\n");
	mazeData.stepNumber = 0;	//Initial Step Number
	mazeData.position = mazeData.startMaze;	//Initial location
	
	char pathFound = pathFinder(&mazeData);
	printf("Finished Maze Solver\n");

	/*
		If the solution was found, write solution to the file.
		If not, quit the program.
	*/
	if (pathFound)
	{	
		printf("Writing Solution File...\n");
		writeSolutionFile(&mazeData, solutionFile);
		printf("Solution File has been Written\n");
		fclose(solutionFile);
	}
	else
	{
		quitError("No Path found, Maze not solved!");
	}
	

	/*
		Print out the maze parameters, if a solution was found,
		and the number of steps of the solution.
	*/
	printf("\nSize:	%d-%d\nStart:	%d-%d\nEnd:	%d-%d\n",
		mazeData.sizeMaze.X, mazeData.sizeMaze.Y,
		mazeData.startMaze.X, mazeData.startMaze.Y,
		mazeData.endMaze.X, mazeData.endMaze.Y);

	if (pathFound)
	{
		printf("\nMaze Route Found!");
		printf("\nNumber of Steps:  %d", mazeData.stepNumber);
	}
	else
	{
		printf("\nNo Maze Route Found!");
	}

	
	/*
		Check if the maze should be displayed, then 
		begins the set up.
	*/
	if (displayMaze == 0) //False = 1 
	{
		printf("\nDisplaying Maze");
		printf("\nUse WASD keys to move around the maze! \nQ and E zooms in and out! \nR resets the view to defualt!");
		//Set the default tile size
		float tSize = 2.0f;
		//Run Set up for OpenGL
		setUpRender(argc, argv, tSize, &mazeData, mazeFileName);
	}

	/*
		free memory back to the operating system
	*/
	free(mazeData.maze);
	free(mazeData.mazeVisit);

	printf("\n__Program Exit__\n");
	char x = getchar();	//Wait for key-press before exiting
	return(0);
}

/*
void create2DArray(MAZEDATA* mData):
	Creates 2 double pointer arrays, one for the maze data (type int)
	one for storing where the maze solver has been in the maze (type char)
	Creates a double pointer and Creates a 2D array of given size.
	create the Number of Columns, with data size int
*/
void create2DArray(MAZEDATA* mData)
{
	//Create temporary pointers
	int**  newMazeArray;		//To hold the maze structurer
	BOOL** newVisitArray;		//visited locations in the maze

	//Allocate Memory using malloc to the pointers
	//Allocates memory for the number of columns.
	newMazeArray = malloc(mData->sizeMaze.X * sizeof(int));
	newVisitArray = malloc(mData->sizeMaze.X * sizeof(BOOL));

	//Loop through the Number of Columns, allocating memory for the number of Rows
	int i;
	for (i = 0; i < mData->sizeMaze.X; i++)
	{
		//Create the 2 arrays
		newMazeArray[i] = malloc(mData->sizeMaze.Y * sizeof(int));
		newVisitArray[i] = malloc(mData->sizeMaze.Y * sizeof(BOOL));
	}

	//Move arrays to the data struct
	mData->maze = newMazeArray;
	mData->mazeVisit = newVisitArray;

	return;
}

/*
void quitError(char* errorMsg):
	Used to Display an error message and exit the program.
	This function accepts a single string for the message.
*/
void quitError(char* errorMsg)
{
	printf(errorMsg);	//Display the Error Message
	char x = getchar();	//Wait for key-press before exiting 
	exit(-1);			//Exit with error
}