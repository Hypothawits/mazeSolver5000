#pragma once
void checkFile(FILE* filePointer, char* fileName1);
void writeSolutionFile(MAZEDATA* mazeData, FILE* solutionFile);
void getMazeParam(FILE* mazeFile, MAZEDATA* mazeData);
void getMazeData(FILE* mazeFile, MAZEDATA* mData);
