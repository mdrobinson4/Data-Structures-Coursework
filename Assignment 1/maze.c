/*
    Author: Mark Douglas Ray Robinson
    Date: October 9, 2018
    Title: Assignment 1
    Description: Builds, draws, and solves maze
*/

#include "cell.h"
#include "maze.h"
#include "stack.h"
#include "queue.h"

extern void srandom(unsigned int);
extern long int random(void);

static CELL *getCELL(MAZE *m, int rows, int columns);
static CELL *getRandomNeighbor(MAZE *m, CELL *c);
static CELL *getNextNeighbor(MAZE *m , CELL *c);
static void removeWall(CELL *currCell, CELL *nCell);
static void decCellCount(MAZE *m);
static void fixNCount(MAZE *m, CELL *c);


struct maze {
  int rows;
  int columns;
  int cellCount;
  int visitedCells;
  CELL ***grid;
  STACK *stack;
  QUEUE *queue;
};

MAZE *newMAZE(int rows, int columns, unsigned int seed) {
  MAZE *n = malloc(sizeof(MAZE));
  n->stack = newSTACK();
  n->queue = newQUEUE();
  n->rows = rows;
  n->columns = columns;
  n->cellCount = rows * columns;
  n->visitedCells = 0;

  //create a one-dimensional array of row pointers
  n->grid = malloc(sizeof (CELL **) * rows);
  //make all the rows
  int i, j, nCount;
  for (i = 0; i < rows; ++i) {
    //create a single row
    n->grid[i] = malloc(sizeof(CELL *) * columns);
    //initialize the slots in the row
    for (j = 0; j < columns; ++j) {
      n->grid[i][j] = newCELL();
      nCount = 4;

      if ((i == 0))
        nCount -= 1;
      if ((j == 0))
        nCount -= 1;
      if ((i == rows - 1))
        nCount -= 1;
      if ((j == columns - 1))
        nCount -= 1;

      setNCount(n->grid[i][j], nCount);
      setPosition(n->grid[i][j], i, j);
     }
  }
  srandom(seed);
  return n;
}

void carveMAZE(MAZE *m) {
  CELL *currCell = getCELL(m, 0, 0);  // Mark initial cell the current cell
  CELL *nCell = getCELL(m, 0, 0);
  decCellCount(m);
  markVisited(currCell);  // Mark current cell as visited
  fixNCount(m, currCell);  // Decrease neighbor's neighbor count

  while (m->cellCount > 0) { // While there are unvisited cells
    if (getNCount(currCell) > 0) {  // If current cell has unvisited neighbors
      nCell = getRandomNeighbor(m, currCell); // Get reandom neighbor
      push(m->stack, currCell);
      removeWall(currCell, nCell);
      markVisited(nCell);
      fixNCount(m, nCell);  // Decrease neighbor's neighbor count
      decCellCount(m);  // Decrease (total) cell count
      currCell = nCell;
    }
    else
      currCell = pop(m->stack);
  }
}

void solveMAZE(MAZE *m) {
  int queueCount = 1, depth = 0, pendingDepthIncrease = 0;
  CELL *c = getCELL(m, 0, 0), *d = 0, *e = 0;

  markVisited(c);
  setValue(c, (depth % 10) + '0');
  enqueue(m->queue, c);

  while (sizeQUEUE(m->queue)) {
    d = dequeue(m->queue);
    queueCount -= 1;

    if (!queueCount) {
      depth += 1;
      pendingDepthIncrease = 1;
    }

    if ((getColumn(d) == m->columns - 1) && (getRow(d) == m->rows - 1)) {
      while (sizeQUEUE(m->queue))
        setValue(dequeue(m->queue), ' ');
      return;
    }

    for (int i = 0; i < getTotalNCount(d); i++) {
      e = getNextNeighbor(m, d);
      if (!visited(e)) {
        setValue(e, (depth % 10) + '0');
        markVisited(e);
        enqueue(m->queue, e);
        if (pendingDepthIncrease) {
          queueCount = sizeQUEUE(m->queue);
          pendingDepthIncrease = 0;
        }
      }
    }
  }
}

/*  Return next neighbor  */
static CELL *getNextNeighbor(MAZE *m , CELL *c) {
  CELL *nCell = 0;
  if (getRow(c) > 0) {
    nCell = getCELL(m, getRow(c) - 1, getColumn(c));
    if (!visited(nCell) && !checkWall(c, 't'))
      return nCell;
  }
  if (getColumn(c) > 0) {
    nCell = getCELL(m, getRow(c), getColumn(c) - 1);
    if (!visited(nCell) && !checkWall(nCell, 'r'))
      return nCell;
  }
  if (getColumn(c) < m->columns - 1) {
    nCell = getCELL(m, getRow(c), getColumn(c) + 1);
    if (!visited(nCell) && !checkWall(c, 'r'))
      return nCell;
  }
  if (getRow(c) < m->rows - 1) {
    nCell = getCELL(m, getRow(c) + 1, getColumn(c));
    if (!visited(nCell) && !checkWall(nCell, 't'))
      return nCell;
  }
  return c;
}

/*  Remove specified wall  */
static void removeWall(CELL *currCell, CELL *nCell) {
  if (getRow(currCell) == getRow(nCell)) {
    if (getColumn(currCell) > getColumn(nCell))
      breakWall(nCell, 'r');
    else
      breakWall(currCell, 'r');
  }

  else if (getColumn(currCell) == getColumn(nCell)) {
    if (getRow(currCell) > getRow(nCell))
      breakWall(currCell, 't');
    else
      breakWall(nCell, 't');
  }
}

  /*  Return cell */
static CELL *getCELL(MAZE *m, int rows, int columns) {
  return m->grid[rows][columns];
}

static CELL *getRandomNeighbor(MAZE *m , CELL *c) {
  CELL *temp[getNCount(c)];
  CELL *nCell = 0;
  int count = 0;

  if (getRow(c) > 0) {
    nCell = getCELL(m, getRow(c) - 1, getColumn(c));
    if (!visited(nCell))
      temp[count++] = nCell;
  }
  if (getColumn(c) > 0) {
    nCell = getCELL(m, getRow(c), getColumn(c) - 1);
    if (!visited(nCell))
      temp[count++] = nCell;
  }
  if (getColumn(c) < m->columns - 1) {
    nCell = getCELL(m, getRow(c), getColumn(c) + 1);
    if (!visited(nCell))
      temp[count++] = nCell;
  }
  if (getRow(c) < m->rows - 1) {
    nCell = getCELL(m, getRow(c) + 1, getColumn(c));
    if (!visited(nCell))
      temp[count++] = nCell;
  }
  return temp[random() % getNCount(c)];
}

void decCellCount(MAZE *m) {
  m->cellCount -= 1;
}

static void fixNCount(MAZE *m, CELL *c) {
  if (getRow(c) > 0)
    decNCount(getCELL(m, getRow(c) - 1, getColumn(c)));

  if (getRow(c) < m->rows - 1)
    decNCount(getCELL(m, getRow(c) + 1, getColumn(c)));

  if (getColumn(c) > 0)
    decNCount(getCELL(m, getRow(c), getColumn(c) - 1));

  if (getColumn(c) < m->columns - 1)
    decNCount(getCELL(m, getRow(c), getColumn(c) + 1));
}

void drawMAZE(MAZE *m, FILE *fp) {
  for (int j = 0; j < m->columns; j++) {
    fprintf(fp, "-");
    fprintf(fp, "---");
  }
  fprintf(fp, "-\n");

  for (int i = 0; i < m->rows; i++) {
    if (!i)
      fprintf(fp, " ");
    else
      fprintf(fp, "|");
    for (int j = 0; j < m->columns; j++) {
      fprintf(fp, " %c ", getValue(getCELL(m, i, j)));
      if (checkWall(getCELL(m, i, j), 'r'))
        (i == m->rows - 1 && j == m->columns - 1) ? fprintf(fp, " ") : fprintf(fp, "|");
      else
        fprintf(fp, " ");
    }
    fprintf(fp, "\n");
    for (int j = 0; j < m->columns; j++) {
      if (i + 1 < m->rows) {
        fprintf(fp, "-");
        if (checkWall(getCELL(m, i + 1, j), 't'))
          fprintf(fp, "---");
        else
          fprintf(fp, "   ");
      }
    }
    if (i + 1 < m->rows)
      fprintf(fp, "-\n");
  }

  for (int j = 0; j < m->columns; j++) {
    fprintf(fp, "-");
    fprintf(fp, "---");
  }
  fprintf(fp, "-\n");
}

void encode(MAZE *m, char *name) {
  FILE *fp = fopen(name, "w");
  CELL *c = 0;

  fprintf(fp, "%d %d %d\n", m->rows, m->columns, m->rows * m->columns);

  if (getValue(getCELL(m, 0, 0)) == '0')
    fprintf(fp, "%d\n", 1);
  else
    fprintf(fp, "%d\n", 0);

  for (int i = 0; i < m->rows; i++) {

    for (int j = 0; j < m->columns; j++) {
      c = getCELL(m, i, j);
      fprintf(fp, "%d %d %d %d %d %c\n", getRow(c), getColumn(c), checkWall(c, 't'), checkWall(c, 'r'), getTotalNCount(c), getValue(c));
    }
  }
  fclose(fp);
}

void decodeMAZE(MAZE *nMaze, FILE *fp) {
  int cellCount, row, column, topWall, rightWall, nCount, solveFlag = 0;
  unsigned char value;

  fscanf(fp, "%d", &cellCount);
  nMaze->cellCount = cellCount;

  fscanf(fp, "%d", &solveFlag);
  while (!feof(fp)) {

    fscanf(fp, "%d", &row);
    fscanf(fp, "%d", &column);
    fscanf(fp, "%d", &topWall);
    fscanf(fp, "%d", &rightWall);
    fscanf(fp, "%d", &nCount);
    fscanf(fp, "%c", &value);
    
    if (solveFlag)
      fscanf(fp, "%c", &value);

    setNCount(getCELL(nMaze, row, column), nCount);
    setValue(getCELL(nMaze, row, column), value);

    if (!rightWall)
      breakWall(getCELL(nMaze, row, column), 'r');
    if (!topWall)
      breakWall(getCELL(nMaze, row, column), 't');
  }
  fclose(fp);
}

void freeMAZE(MAZE *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->columns; j++) {
      free(m->grid[i][j]);
    }
    free(m->grid[i]);
  }
  free(m->grid);
  freeQUEUE(m->queue);
  freeSTACK(m->stack);
  free(m);
}
