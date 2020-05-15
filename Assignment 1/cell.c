/*
    Author: Mark Douglas Ray Robinson
    Date: October 9, 2018
    Title: Assignment 1
    Description: CELL class with necessary get and set functions
*/


#include "cell.h"

struct cell {
  int row;
  int column;
  int nCount;
  int visited;
  int rightWall;
  int topWall;
  unsigned char value;
  int totalNCount;
};

CELL *newCELL(void) {
  CELL *c = malloc(sizeof(CELL));
  c->row = 0;
  c->column = 0;
  c->visited = 0;
  c->rightWall = 1;
  c->topWall = 1;
  c->nCount = 0;  // Neighbor count
  c->value = ' ';
  c->totalNCount = 0;
  return c;
}

void setPosition(CELL *c, int row, int column) {
  c->row = row;
  c->column = column;
}

int visited(CELL *c) {
  return c->visited;
}

void markVisited(CELL *c) {
  c->visited = 1;
}

void setNCount(CELL *c, int count) {
  c->nCount = count;
  c->totalNCount = count;
}

int getNCount(CELL *c) {
  return c->nCount;
}

int getTotalNCount(CELL *c) {
  return c->totalNCount;
}

void decNCount(CELL *c) {
  c->nCount -= 1;
}

int getRow(CELL *c) {
  return c->row;
}

int getColumn(CELL *c) {
  return c->column;
}

void breakWall(CELL *c, char choice) {
  if (choice == 't')
    c->topWall = 0;
  else
    c->rightWall = 0;
}

int checkWall(CELL *c, char choice) {
  if (choice == 't')
    return c->topWall;
  else
    return c->rightWall;
}

unsigned char getValue(CELL *c) {
  return c->value;
}

void setValue(CELL *c, unsigned char value) {
  c->value = value;
}
