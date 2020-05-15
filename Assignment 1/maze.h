/*
    Author: Mark Douglas Ray Robinson
    Date: September 14, 2018
    Title: Assignment 0
    Description: Circular dynamic array which is used to implement the queue data structure
*/

#ifndef __MAZE_INCLUDED__
#define __MAZE_INCLUDED__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct maze MAZE;

extern MAZE *newMAZE(int rows, int columns, unsigned int seed);
extern void carveMAZE(MAZE *m);
extern void drawMAZE(MAZE *m, FILE *fp);
extern void encode(MAZE *m, char *name);
extern void decodeMAZE(MAZE *nMaze, FILE *fp);
extern void solveMAZE(MAZE *m);
extern void freeMAZE(MAZE *m);

#endif
