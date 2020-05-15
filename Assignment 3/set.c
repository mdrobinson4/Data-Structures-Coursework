#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include "da.h"
#include "real.h"

static int link(SET *s1, SET *s2);
static SET *getRep(SET *s);

struct set {
  DA *store;
  void *value;
  SET *rep;
  SET *parent;
  int rank;
  int index;
  void (*display)(void *, FILE *);
  void (*free)(void *);
};

SET *newSET(void) {
  SET *s = malloc(sizeof(SET));
  assert(s != 0);
  s->store = newDA();
  s->display = 0;
  s->free = 0;
  s->rep = s;
  s->parent = s;
  s->rank = 0;
  s->index = 0;
  return s;
}

void setSETdisplay(SET *s,void (*d)(void *,FILE *)) {
  setDAdisplay(s->store, d);
  s->display = d;
}

void setSETfree(SET *s,void (*d)(void *)) {
  setDAfree(s->store, d);
  s->free = d;
}

int makeSET(SET *d, void *value) {
  SET *s = newSET();
  s->value = value;
  s->index = sizeDA(d->store);
  s->parent = s;
  insertDAback(d->store, s);
  return s->index;
}

int findSET(SET *d, int index) {
  SET *s = getDA(d->store, index);
  s = getRep(s);
  return s->index;
}

SET *getRep(SET *s) {
  if (s != s->parent)
    s->parent = getRep(s->parent);
  return s->parent;
}

int unionSET(SET *d,int index1,int index2) {
  int repIndex =  link(getDA(d->store, findSET(d, index1)), getDA(d->store, findSET(d, index2)));
  d->rep->index = repIndex;
  return repIndex;
}

int link(SET *s1, SET *s2) {
  if (s1->rank > s2->rank) {
    s2->parent = s1;
    return s1->index;
  }
  if (s2->rank > s1->rank) {
    s1->parent = s2;
    return s2->index;
  }
  if (s1->index < s2->index) {
    s2->parent = s1;
    s1->rank += 1;
    return s1->index;
  }
  if (s1->index > s2->index) {
    s1->parent = s2;
    s2->rank += 1;
    return s2->index;
  }
  return s1->index;
}

void displaySET(SET *d, FILE *fp) {
  for (int i = 0; i < sizeDA(d->store); i++) {
    SET *s = getDA(d->store, i);
    fprintf(fp, "%d: ", i);
    if (s == s->parent) {
      d->display(s->value, fp);
      fprintf(fp, "(%d)\n", s->rank);
    }
    else {
      while (s != s->parent) {
        d->display(s->value, fp);
        s = s->parent;
        fprintf(fp, " ");
      }
      d->display(s->value, fp);
      fprintf(fp, "\n");
    }
  }
}

int debugSET(SET *d,int level) {
  debugDA(d->store, level);
  return 0;
}

void freeSET(SET *d) {
  for (int i = 0; i < sizeDA(d->store); i++) {
    SET *s = getDA(d->store, i);
    d->free(s->value);
    setDAfree(s->store, 0);
    freeDA(s->store);
    free(s);
  }
  setDAfree(d->store, 0);
  freeDA(d->store);
  free(d);
}
