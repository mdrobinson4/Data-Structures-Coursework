#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "real.h"
#include "rbt.h"
#include "tnode.h"
#include "da.h"
#include "scanner.h"
#include "set.h"
#include "edge.h"
#include "queue.h"

static void createSET(SET *d, RBT *t, TNODE *node);
static void kruskal(SET *d, RBT *t);
static int compare(void *v1, void *v2);

char *input;

int main(int argc, char **argv) {
  if (argc > 1) {
    if (!(argv[1][2]) && argv[1][0] == '-' && argv[1][1] == 'v') {
      printf("Mark Douglas Ray Robinson\n");
      exit(0);
    }
    else
      input = argv[1];
  }
  char *c;
  int v1 = 0, v2 = 0, w = 0, smallest = 0;
  TNODE *node = 0;
  RBT *t = newRBT(compare);
  //setRBTdisplay(display);
  //setRBTfree(free);
  SET *s = newSET();
  EDGE *root = 0;

  printf("Input: %s\n", input);
  FILE *fp = fopen(input, "r");

  while(!feof(fp)) {
    c = readToken(fp);
    if (c == 0)
      break;
    v1 = atoi(c);    // read first vertex
    c = readToken(fp);
    if (c == 0)
      break;
    v2 = atoi(c);    // read second vertex
    c = readToken(fp);
    if (c == 0)
      break;
    if (c[0] != ';')    // read weight
      w = atoi(c);
    else                // weight not provided
      w = 1;
    EDGE *e = newEDGE(v1, v2, w);
    node = insertRBT(t, e);
    if (node == 0) {
      deleteRBT(t, e);
      insertRBT(t, e);
    }
    int index = makeSET(s, e);
    setEDGEindex(e, index);
  }
  kruskal(s, t);
}

int compare(void *e1, void *e2) {
  return compareEDGE((EDGE *)e1, (EDGE *)e2);
}

void kruskal(SET *d, RBT *t) {
  if (getRBTroot(t) == 0)
    return;
  QUEUE *q = newQUEUE();
  TNODE *curr = getRBTroot(t);
  int nodeCount = 0, count = 0;

  createSET(d, t, getRBTroot(t));
  enqueue(q, curr);
  while (sizeQUEUE(q) > 0) {
    nodeCount = sizeQUEUE(q);
    while (nodeCount > 0) {
      curr = dequeue(q);
      if (count < sizeRBT(t) - 1) {
        if ( findSET(d, getEDGEindex(getTNODEvalue(curr))) != findSET(d, getEDGEindex(getTNODEvalue(peekQUEUE(q)))) ) {
          unionSET(d, getEDGEindex(getTNODEvalue(curr)), getEDGEindex(getTNODEvalue(peekQUEUE(q))));
        }
      }
      if (getTNODEleft(curr) != 0)
        enqueue(q, getTNODEleft(curr));
      if (getTNODEright(curr) != 0)
        enqueue(q, getTNODEright(curr));
      nodeCount -= 1;
      count += 1;
    }
  }
}

void createSET(SET *d, RBT *t, TNODE *node) {
  if (node == 0)
    return;
  createSET(d, t, getTNODEleft(node));
  createSET(d, t, getTNODEright(node));
  makeSET(d, getTNODEvalue(node));
}


void display(RBT *t, FILE *fp, int smallest) {
  if (getRBTroot(t) == 0) {
    fprintf(fp, "%d:\n", 0);
    return;
  }
  QUEUE *q = newQUEUE();
  RBT *r = newRBT(compare);
  insertRBT(r, root);

  TNODE *curr = getRBTroot(r);
  int level = 0, nodeCount = 0, flag = 0;
  enqueue(q, curr);
  while (sizeQUEUE(q) > 0) {
    nodeCount = sizeQUEUE(q);
    flag = nodeCount;
    while (nodeCount > 0) {
      curr = dequeue(q);
      if (flag == nodeCount) {
        fprintf(fp, "%d: ", level);
      }
      fprintf(fp, "%d(%d)%d", getTNODEvalue(curr), getTNODEvalue(curr), getWeight());
      //displayLevel(t, curr, fp);
      if (getTNODEleft(curr) != 0)
        enqueue(q, getTNODEleft(curr));
      if (getTNODEright(curr) != 0)
        enqueue(q, getTNODEright(curr));
      nodeCount -= 1;
      if (nodeCount > 0) {
        fprintf(fp, " ");
      }
    }
    fprintf(fp, "\n");
    level += 1;
  }
}
