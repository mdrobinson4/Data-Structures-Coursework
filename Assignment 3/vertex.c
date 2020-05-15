#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "real.h"
#include "vertex.h"

struct vertex {
  void *value;
  VERTEX *parent;
  int rank;
  int index;
};

VERTEX *newVERTEX(void *value) {
  VERTEX *v = malloc(sizeof(VERTEX));
  assert(v != 0);
  v->value = value;
  v->parent = v;
  v->rank = 0;
  v->index = 0;
  return v;
}

VERTEX *getRep(VERTEX *v) {
  if (v != v->parent)
    v->parent = getRep(v->parent);
  return v->parent;
}

int link(VERTEX *r1, VERTEX *r2) {
  if (r1->rank > r2->rank) {
    r2->parent = r1;
    return r1->index;
  }
  if (r2->rank > r1->rank) {
    r1->parent = r2;
    return r2->index;
  }
  if (r1->index < r2->index) {
    r2->parent = r1;
    r1->rank += 1;
    return r1->index;
  }
  if (r1->index > r2->index) {
    r1->parent = r2;
    r2->rank += 1;
    return r2->index;
  }
  return r1->index;
}

VERTEX *getValue(VERTEX *v) {
  return v->value;
}

int getIndex(VERTEX *v) {
  return v->index;
}

int getRank(VERTEX *v) {
  return v->rank;
}

VERTEX *getParent(VERTEX *v) {
  return v->parent;
}

void setValue(VERTEX *v, void *value) {
  v->value = value;
}

void setIndex(VERTEX *v, int index) {
  v->index = index;
}

void setRank(VERTEX *v, int rank) {
  v->rank = rank;
}
