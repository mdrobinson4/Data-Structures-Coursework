#include "tnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

struct tnode {
  void *value;
  TNODE *leftChild;
  TNODE *rightChild;
  TNODE *parent;
  int debugLevel;
  void (*display)(void *value, FILE *fp);
  void (*free)(void *value);
};

/*  Create new node element   */
TNODE *newTNODE(void *v, TNODE *l, TNODE *r, TNODE *p) {
  TNODE *node = malloc(sizeof(TNODE));
  assert(node != 0);
  node->value = v;
  node->leftChild = l;
  node->rightChild = r;
  node->parent = p;
  node->display = 0;
  node->free = 0;
  node->debugLevel = 0;
  return node;
}

/*  Set display function  */
void setTNODEdisplay(TNODE *n, void (*d)(void *value, FILE *fp)) {
  n->display = d;
}

/*  Set free function   */
void setTNODEfree(TNODE *n, void (*f)(void *value)) {
  n->free = f;
}

/*  Return node value   */
void *getTNODEvalue(TNODE *n) {
  if (n == 0) {
    return 0;
  }
  return n->value;
}

/*  Set node value  */
void setTNODEvalue(TNODE *n, void *replacement) {
  n->value = replacement;
}

/*  Get left child value  */
TNODE *getTNODEleft(TNODE *n) {
  if (n == 0) {
    return 0;
  }
  return n->leftChild;
}

/*  Set left child value  */
void setTNODEleft(TNODE *n, TNODE *replacement) {
  n->leftChild = replacement;
}

/*  Get right child value   */
TNODE *getTNODEright(TNODE *n) {
  if (n == 0) {
    return 0;
  }
  return n->rightChild;
}

/*  Set right node value  */
void setTNODEright(TNODE *n, TNODE *replacement) {
  n->rightChild = replacement;
}

/*  Get parent value  */
TNODE *getTNODEparent(TNODE *n) {
  if (n == 0) {
    return 0;
  }
  return n->parent;
}

/* Set parent value   */
void setTNODEparent(TNODE *n, TNODE *replacement) {
  n->parent = replacement;
}

/*  Display node  */
void displayTNODE(TNODE *n, FILE *fp) {
  if (n->display == 0)
    fprintf(fp, "&%p", n->value);
  else {
    n->display(n->value, fp);
  }
  if (n->debugLevel > 0)
    fprintf(fp, "&%p", n);
}

/*  Change debug level  */
int debugTNODE(TNODE *n, int level) {
  int oldLevel = n->debugLevel;
  n->debugLevel = level;
  return oldLevel;
}

/*  Free node   */
void freeTNODE(TNODE *n) {
  if (n->free != 0) {
    n->free(n->value);
    free(n);
  }
  else {
    free(n);
  }
}
