#include "edge.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "real.h"

struct edge {
  int v1;
  int v2;
  int weight;
  int index;
};

struct vertex {
  int vertex;
  DA *neighbors;
  DA *weights;
}

EDGE *newEDGE(VERTEX v1, VERTEX v2, int weight) {
  EDGE *e = malloc(sizeof(EDGE));
  assert(e != 0);
  e->v1 = v1;
  e->v2 = v2;
  e->weight = weight;
  e->index = 0;
  return e;
}

int compareEDGE(EDGE *e1, EDGE *e2) {
  // edge is already in the tree
  if ((e1->v1 == e2->v1) && (e1->v2 == e2->v2))
    return 0;
  else if ((e1->v2 == e2->v1) && (e1->v1 == e2->v2))
    return 0;

  if (e1->weight > e2->weight) {
    return 1;
  }
  else if (e1->weight < e2->weight) {
    return -1;
  }
  else if (e1->v1 < e2->v1)
    return -1;
  else if (e1->v2 > e2->v2)
    return 1;
  return 0;
}

int compareVERTEX(EDGE *e1, EDGE *e2) {
  // The sum of the vertices in the first edge is greater than the second edges'
  if ((sumVERTEX(e1)) > (sumVERTEX(e2))) {
    return 1;
  }
  // The sum of the vertices in the first edge is greater than the second edges'
  else if ((sumVERTEX(e1)) < (sumVERTEX(e2))) {
    return -1;
  }
    // The sum of the vertices in the first and second edge are equal
  else if ((sumVERTEX(e1)) == (sumVERTEX(e2))) {
    if (minVERTEX(e1) > minVERTEX(e2))
      return 1;
    else if (minVERTEX(e1) < minVERTEX(e2))
      return -1;
    // The vertices are all the same
    else
      return 0;
  }
  return 0;
}

int sumVERTEX(EDGE *e) {
  return e->v1 + e->v2;
}

int minVERTEX(EDGE *e) {
  if (e->v1 > e->v2) {
    return 1;
  }
  else if (e->v1 > e->v2) {
    return -1;
  }
  else {
    return 0;
  }
}

int getV1(EDGE *e) {
  return e->v1;
}

int getV2(EDGE *e) {
  return e->v2;
}

int getWeight(EDGE *e) {
  return e->weight;
}

void setEDGEindex(EDGE *e, int index) {
  e->index = index;
}

int getEDGEindex(EDGE *e) {
  return e->index;
}
