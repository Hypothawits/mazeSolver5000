#pragma once
/*
enum BOOL:
	Defines True and False types to make code easier to read.
*/
typedef enum
{	
	False = 0,
	True = 1
}BOOL;

/*
struct myCOORD:
	Struct for holding X, Y coordinate data of size Int.	
*/
typedef struct myCOORD
{	

	int X;
	int Y;
}myCOORD;

/*
struct WALLS:
	Defines a type for the direction of walls
	around a position in the maze.
*/
typedef struct WALLS
{	

	//Struct type for holding a '4 bit binary number'
	//where b0 b1 b2 b3
	//	=	U  L  D  R      
	BOOL U; BOOL L; BOOL D;  BOOL R;
}WALLS;

/*
struct MAZEDATA:
	Stores all the the data related to the Maze.
*/
typedef struct MAZEDATA
{	

	//Maze data
	int**	maze;		//Stores the Maze structure data
	BOOL**	mazeVisit;	//Keeps track of where in the maze has been visited

	//Start and End locations
	myCOORD	endMaze;
	myCOORD	startMaze;

	//MAze Size
	myCOORD	sizeMaze;

	//A Coordinate Array. So each step of the path will have an x and y value
	myCOORD	pathArray[500000];	//assumes that maze path will take less than 1 Million 

	//Current position in the maze							
	myCOORD	position;

	//number of steps taken
	int		stepNumber;

}MAZEDATA;

void quitError(char* errorMsg);
void create2DArray(MAZEDATA* mData);