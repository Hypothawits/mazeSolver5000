#include <stdio.h>
#include <Windows.h>
#include <GL/GL.h>
#include "GL/freeglut.h"
#include "main.h"
#include "mazeSolver.h"

/*
Name: renderMaze.c
Contains render functions for displaying the
maze using openGL.
*/

//Global Variables for Rendering the Maze in OpenGL
int**	renderMazeData;		

//Incrementers
int		i, j;				 

myCOORD* renderPathArray;	
myCOORD renderMazeSize;		
WALLS	wall;				

float	origin_x = -1.0;		
float	origin_y = -1.0;	

float	pos_x, pos_y;
float	tileSize;			
float	defaultTileSize;	
float	translation = 0.2f;
int		numberSteps;

//Scaling offsets
float	xOffset = 0;
float	yOffset = 0;

//Tile size
float	xWidth;
float	yWidth;
//Window Size
int		windowHeight;
int		windowWitdh;

/*
void keyPress(unsigned char key):
	Handle keyboard input.
	The input argument is the char input from the keyboard. The switch-case is used to test the input.
	the wasd keys are used for lateral and horizontal movement of the maze. The q and e keys are
	used for zooming in and out. The r key is used to reset the view to default.
	movements are do by an X and Y offset. Zooming is done by scaling the size of the drawn tile.
*/
void keyPress(unsigned char key)
{
	//Get and resolve keyboard input
	switch (key)
	{
	case 'q':
		//Zoom In
		tileSize = tileSize + defaultTileSize*0.1;
		break;

	case 'e':
		//Zoom Out
		tileSize = tileSize - defaultTileSize*0.1;
		break;

	case 'w':
		//pan Up
		yOffset -= translation;
		break;

	case 's':
		//pan down
		yOffset += translation;
		break;

	case 'd':
		//pan right
		xOffset -= translation;
		break;

	case 'a':
		//pan left
		xOffset += translation;
		break;

	case 'r':
		//reset view
		xOffset = 0;
		yOffset = 0;
		tileSize = defaultTileSize;
		break;

	default:
		//Any other key, do nothing
		break;
	}

	//Force Re-rendering of Window
	glutPostRedisplay();
	return;
}


/*
void renderMaze():
	Convert maze data to graphic.
	Takes a position, WALL and tileSize inputs and converts it into a rendered tile on the window.
	The window is a grid that has coordinates (-1,-1) in the lower left and (1,1) in the upper right.
	The center of the window is (0,0).
	The if statements below check if a wall is present and
	draws the appropriate wall, in the correct location.
*/
void renderMaze()
{	//Draw walls
	if (wall.D)
	{	//Draw Down Wall
		glRectd(pos_x + xOffset, pos_y + yOffset, pos_x + xOffset + tileSize, pos_y + yOffset + yWidth);
	}
	if (wall.L)
	{	//Draw Left Wall
		glRectd(pos_x + xOffset, pos_y + yOffset, pos_x + xOffset + xWidth, pos_y + yOffset + tileSize);
	}
	if (wall.R)
	{	//Draw Right Wall
		glRectd(pos_x + xOffset + tileSize, pos_y + yOffset, pos_x + xOffset + tileSize - xWidth, pos_y + yOffset + tileSize);
	}
	if (wall.U)
	{	//Draw Top Wall
		glRectd(pos_x + xOffset, pos_y + yOffset + tileSize, pos_x + xOffset + tileSize, pos_y + yOffset + tileSize - yWidth);
	}
}

/*
void renderSoultionPath():
	Draw the solution to the maze, drawn first so that it in under the maze itself.
	Sets the colour of the start and end points to green and red. The colour of the
	path is set to white.
	Each step of the path is a filled coloured tile. The function loops through and draws
	all of the steps. For each step, converts the position in the maze into a position
	in the window (based on the origin and tile size).
*/
void renderSoultionPath()
{	//Loop through each step of the solution
	for (i = 0; i <= numberSteps; i++)
	{
		//If the starting position
		if (i == 0)
		{	//Set tile colour to Green
			glColor3f(0, 1, 0);
		}
		//If the end position
		else if (i == numberSteps)
		{	//Set tile colour to Red
			glColor3f(1, 0, 0);
		}
		else
		{	//Set the tile colour to White
			glColor3f(1, 1, 1);
		}
		//Plus 1 to pos_x/y as dots rendered 1 tile too low and too 'left'
		//Calculate x,y positions.
		pos_x = origin_x + tileSize * (renderPathArray[i].X + 1);
		pos_y = origin_y + tileSize * (renderMazeSize.Y - renderPathArray[i].Y + 1) - tileSize;		//Invert j (y coordinate) because of render system, -tile size is because it builds tile upwards, to offset by 1 tile

		//Draw the tile
		glRectd(pos_x + xOffset, pos_y + yOffset, pos_x + xOffset - (tileSize), pos_y + yOffset - (tileSize));
	}
}

/*
void renderScene():
	Main Render Function, Clears the scene for the next image (for zooming and translations)
	Gets the window size (for scaling the width of the maze walls)
	Calls the function for drawing the solution path.
	Sets the colour of the maze walls. Loops through each maze tile,converts it to a
	wall struct, calculates the position in the window and draws it to the window.
	At the end, swap the render buffers to show the newly drawn window to the user.
*/
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Variables
	//Get size of the window
	windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	windowWitdh = glutGet(GLUT_WINDOW_WIDTH);

	//set width to be 1px 
	xWidth = 1.01f / (float)windowWitdh;
	yWidth = 1.01f / (float)windowHeight;

	//Draw each step of maze solution
	renderSoultionPath();

	//Draw each maze tile
	glColor3f(1, 0.3, 1);	//Set maze wall colour
	for (j = 0; j < renderMazeSize.Y; j++)
	{
		for (i = 0; i < renderMazeSize.X; i++)
		{
			wall = toBinary(renderMazeData[i][j]);

			pos_x = origin_x + tileSize * (i);
			pos_y = origin_y + tileSize * (renderMazeSize.Y - j) - tileSize;		//Invert j (y coordinate) becuase of render system, -tile size is becuase it builds tile upwards, to offset by 1 tile

			glColor3f(1, pos_x, pos_y); //Colour Fun! Uncomment for rainbow mazes
			renderMaze();
		}
	}

	//Show the render, move buffer to display
	glutSwapBuffers();
}


/*
void setUpRender(int argc, char* argv[], float tSize, MAZEDATA* mData, char* windowName):
	Set up window and other params for openGL
	Set up for the Render loop, as variables can't be passed into it,
	needed values are moved to global variables. General OpenGL setup
	for the window size, name, position and render type. Keyboard
	input functions and render functions are also passed to the OpenGL framework.
	The Main render loop is then called.
*/
void setUpRender(int argc, char* argv[], float tSize, MAZEDATA* mData, char* windowName)
{
	//Set globals to values, to 'pass' them into render function
	tileSize = tSize / mData->sizeMaze.X;	//Divide grind into X number of tiles with size a fraction of 1 <- is this English?
	defaultTileSize = tileSize;						//Make a copy of tileSize for reseting to default scale.
	//Move required maze data to global variables
	renderMazeData = mData->maze;
	renderPathArray = mData->pathArray;
	renderMazeSize = mData->sizeMaze;
	numberSteps = mData->stepNumber;

	//Set the colour of the maze wall
	glColor3f(0.5, 0.5, 0.9);
	//Set up OpenGL
	glutInit(&argc, argv);
	//Window position
	glutInitWindowPosition(5, 5);
	//Window size, Set to same aspect ration as maze
	glutInitWindowSize(500 * (mData->sizeMaze.X / mData->sizeMaze.Y), 500);
	//Display properties, double buffer and rgb
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//Name window
	glutCreateWindow(windowName);
	//Point to render Function
	glutDisplayFunc(renderScene);
	//Point to keyboard handler
	glutKeyboardFunc(keyPress);
	//Start Render Loop
	glutMainLoop();
}