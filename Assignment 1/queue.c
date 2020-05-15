/*
    Author: Mark Douglas Ray Robinson
    Date: September 14, 2018
    Title: Assignment 0
    Description: Uses the circular dynamic array module to implement the queue data structure
*/

#include "queue.h"
#include "cda.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

struct queue {
  CDA *cda;
  void (*display)(void *, FILE *);
  int debugLevel;
};

QUEUE *newQUEUE(void) {
  QUEUE *n = malloc(sizeof(QUEUE));
  assert(n != 0);
  n->cda = newCDA();
  n->display = 0;
  n->debugLevel = 0;
  return n;
}

void setQUEUEdisplay(QUEUE *items,void (*display)(void *,FILE *)) {
  items->display = display;
  setCDAdisplay(items->cda, display);
}

void setQUEUEfree(QUEUE *items,void (*free)(void *)) {
  setCDAfree(items->cda, free);
}

void enqueue(QUEUE *items,void *value) {
  insertCDAback(items->cda, value);
}

void *dequeue(QUEUE *items) {
  assert(sizeQUEUE(items) > 0);
  return removeCDAfront(items->cda);
}

void *peekQUEUE(QUEUE *items) {
  assert(sizeQUEUE(items) > 0);
  // return getCDA(items->cda, sizeCDA(items->cda)-1);
  return getCDA(items->cda, 0);
}

void displayQUEUE(QUEUE *items,FILE *fp) {
  if (items->debugLevel == 1 || items->debugLevel == 2) {
    debugCDA(items->cda, items->debugLevel - 1);
    displayCDA(items->cda, fp);
  }
  else {
    fprintf(fp, "<");
    for (int i = 0; i < sizeQUEUE(items); i++) {
      if (items->display)
        items->display(getCDA(items->cda, i), fp);
      else
        fprintf(fp, "@%p", getCDA(items->cda, i));
      if (i < sizeQUEUE(items) - 1)
        fprintf(fp, ",");
    }
    fprintf(fp, ">");
  }
  return;
}

int debugQUEUE(QUEUE *items, int newLevel) {
  int prevLevel = items->debugLevel;
  items->debugLevel = newLevel;
  return prevLevel;
}

void freeQUEUE(QUEUE *items) {
  freeCDA(items->cda);
  free(items);
}

int sizeQUEUE(QUEUE *items) {
  return sizeCDA(items->cda);
}
