/*
    Author: Mark Douglas Ray Robinson
    Date: September 14, 2018
    Title: Assignment 0
    Description: Circular dynamic array which is used to implement the queue data structure
*/

#ifndef __CELL_INCLUDED__
#define __CELL_INCLUDED__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct cell CELL;

extern CELL *newCELL(void);
extern void setPosition(CELL *n, int x, int y);
extern int visited(CELL *n);
extern void markVisited(CELL *n);
extern void setNCount(CELL *n, int count);
extern int nVisited(CELL *n);
extern int getRow(CELL *n);
extern int getColumn(CELL *);
extern int getNCount(CELL *n);
extern void breakWall(CELL *c, char choice);
extern void decNCount(CELL *c);
extern int checkWall(CELL *c, char choice);
extern unsigned char getValue(CELL *c);
extern int getTotalNCount(CELL *c);
extern void setValue(CELL *c, unsigned char value);

#endif
