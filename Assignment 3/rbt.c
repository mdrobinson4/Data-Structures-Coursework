#include "gst.h"
#include "rbt.h"
#include "tnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

static void swapper(TNODE *a,TNODE *b);
static void displayRBTV(void *a, FILE *fp);
static int compareRBTV(void *a, void *b);
static void freeRED(void *value);
static TNODE *grandparent(TNODE *x);
static TNODE *uncle(TNODE *x);
static TNODE *sibling(TNODE *node);
static void rotateRight(RBT *t, TNODE *x);
static void rotateLeft(RBT *t, TNODE *x);
static void setColor(TNODE *x, char color);
static char getColor(TNODE *x);
static void insertionFixUp(RBT *t, TNODE *x);
static void deletionFixUp(RBT *t, TNODE *x);
static char checkLinear(RBT *t, TNODE *node);
static TNODE *nephew(TNODE *node);
static TNODE *niece(TNODE *node);

struct rbt {
  GST *gst;
  void (*display)(void *, FILE *);
  int (*comparator)(void *, void *);
  void (*free)(void *);
};

typedef struct RBTVALUE {
  void *value;
  char color;
  void (*display)(void *, FILE *);
  int (*comparator)(void *, void *);
  void (*free)(void *);
  RBT *self;
}RBTVALUE;

RBT *newRBT(int (*c)(void *,void *)) {
  RBT *red = malloc(sizeof(RBT));
  assert(red != 0);
  red->comparator = c;
  red->gst = newGST(compareRBTV);
  setGSTdisplay(red->gst, displayRBTV);
  setGSTfree(red->gst, freeRED);
  setGSTswapper(red->gst, swapper);
  return red;
}

RBTVALUE *newRBTV(void *value, void (*d)(void *,FILE *), int (*c)(void *, void *), void (*f)(void *), RBT *t) {
  RBTVALUE *redValue = malloc(sizeof(RBTVALUE));
  assert(redValue != 0);
  redValue->display = d;
  redValue->comparator = c;
  redValue->free = f;
  redValue->color = 'R';
  redValue->value = value;
  redValue->self = t;
  return redValue;
}

void setRBTdisplay(RBT *t,void (*d)(void *,FILE *)) {
  t->display = d;
  setGSTdisplay(t->gst, displayRBTV);
}

void setRBTswapper(RBT *t,void (*s)(TNODE *,TNODE *)) {
  setGSTswapper(t->gst, s);
}

void setRBTfree(RBT *t,void (*f)(void *)) {
  t->free = f;
  setGSTfree(t->gst, freeRED);
}

TNODE *getRBTroot(RBT *t) {
  return getGSTroot(t->gst);
}

void setRBTroot(RBT *t,TNODE *replacement) {
  setGSTroot(t->gst, replacement);
}

void setRBTsize(RBT *t, int s) {
  setGSTsize(t->gst, s);
}

TNODE *insertRBT(RBT *t,void *value) {
  RBTVALUE *rValue = newRBTV(value, t->display, t->comparator, t->free, t);
  TNODE *node = insertGST(t->gst, rValue);
  if (node != 0) {    // node not in tree
    insertionFixUp(t, node);
    return node;
  }
  // Node already in tree
  return NULL;
}

void *findRBT(RBT *t,void *key) {
  RBTVALUE *rValue = newRBTV(key, t->display, t->comparator, t->free, t);
  RBTVALUE *rValue1 = findGST(t->gst, rValue);
  free(rValue);
  if (rValue1 == 0) {
    return NULL;
  }
  return rValue1->value;
}

TNODE *locateRBT(RBT *t,void *key) {
  RBTVALUE *rValue = newRBTV(key, t->display, t->comparator, t->free, t);
  TNODE *node = locateGST(t->gst, rValue);
  free(rValue);
  if (node == 0) {
    return NULL;
  }
  return node;
}

int deleteRBT(RBT *t,void *key) {
  RBTVALUE *rValue = newRBTV(key, t->display, t->comparator, t->free, t);
  TNODE *node = locateGST(t->gst, rValue), *w = 0;
  // Value not in tree
  if (node == 0) {
    free(rValue);
    return 0;
  }
  if (freqRBT(t, key) == 1) {
    setGSTsize(t->gst, sizeGST(t->gst) - 1);
    node = swapToLeafGST(t->gst, node);
    deletionFixUp(t, node);
    w = unwrapGST(node);
    pruneLeafGST(t->gst, node);   // This will free the GST value
    setTNODEfree(node, 0);
    freeTNODE(node);              // This will free the TNODE (I do not setTNODEfree)
    free(w);                      // This will free the RBT value
    free(rValue);
    return 0;
  }
  deleteGST(t->gst, rValue);
  int freq = freqGST(t->gst, rValue);
  free(rValue);
  return freq;
}

TNODE *swapToLeafRBT(RBT *t,TNODE *node) {
  if (node == 0) {
    return NULL;
  }
  return swapToLeafGST(t->gst, node);
}

void pruneLeafRBT(RBT *t,TNODE *leaf) {
  if (leaf != 0) {
    return;
  }
  pruneLeafGST(t->gst, leaf);
  free(getTNODEvalue(leaf));
}

int sizeRBT(RBT *t) {
  return sizeGST(t->gst);
}

void statisticsRBT(RBT *t,FILE *fp) {
  statisticsGST(t->gst, fp);
}

int debugRBT(RBT *t,int level) {
  return debugGST(t->gst, level);
}

void freeRBT(RBT *t) {
  freeGST(t->gst);
  free(t);
}

void *unwrapRBT(TNODE *n) {
  RBTVALUE *rValue = getTNODEvalue(n);
  void *value = rValue->value;
  return value;
}

/*  Returns the frequency of the Green tree   */
int freqRBT(RBT *g,void *key) {
  RBTVALUE *rValue = newRBTV(key, g->display, g->comparator, g->free, g);
  RBTVALUE *find = findGST(g->gst, rValue);
  free(rValue);
  if (find == 0) {
    return 0;
  }
  return freqGST(g->gst, find);
}

/*  Returns the count of duplicate values in tree   */
int duplicatesRBT(RBT *g) {
  return duplicatesGST(g->gst);
}

void swapper(TNODE *a,TNODE *b) {
  void *va = getTNODEvalue(a);   //get the GST value
  void *vb = getTNODEvalue(b);   //get the GST value
  // swap the GST values
  setTNODEvalue(a,vb);
  setTNODEvalue(b,va);
  // the above swap swapped the colors,
  // but the colors should stay with the nodes,
  // so swap the colors back to the original nodes
  RBTVALUE *x = unwrapGST(a);
  RBTVALUE *y = unwrapGST(b);
  int color = x->color;
  x->color = y->color;
  y->color = color;
}

/*  Returns the color of the node (RED or BLACK)  */
char getColor(TNODE *x) {
  if (x == 0) {
    return 'B';
  }
  RBTVALUE *rbtv = unwrapGST(x);
  return rbtv->color;
}

/*  Sets the color of the node (RED or BLACK)   */
void setColor(TNODE *x, char color) {
  if (x == 0) {
    return;
  }
  RBTVALUE *rbtv = unwrapGST(x);
  rbtv->color = color;
}


/*    Fixes the RBT so the properties are upheld   */
void insertionFixUp(RBT *t, TNODE *x) {
  char rDirection = 'L';
  while (x != getGSTroot(t->gst) && getColor(getTNODEparent(x)) != 'B') {
    if (getColor(uncle(x)) == 'R') {
      setColor(getTNODEparent(x), 'B');           // color parent black
      setColor(uncle(x), 'B');                    // color uncle black
      setColor(grandparent(x), 'R');              // color grandparent red
      x = grandparent(x);
    }
    else {
      rDirection = checkLinear(t, x);
      if (rDirection == 'L') {
        rotateLeft(t, getTNODEparent(x));
        x = getTNODEleft(x);
      }
      else if (rDirection == 'R') {
        rotateRight(t, getTNODEparent(x));
        x = getTNODEright(x);
      }
      setColor(getTNODEparent(x), 'B');         // color parent black
      setColor(grandparent(x), 'R');            // color grandparent red

      if (x == getTNODEleft(getTNODEparent(x)))
        rotateRight(t, grandparent(x));
      else
        rotateLeft(t, grandparent(x));
      break;
    }
  }
  setColor(getGSTroot(t->gst), 'B'); // color root black
}

/*  Rotate X down to Y [LEFT]   */
void rotateLeft(RBT *t, TNODE *x) {
  TNODE *y = getTNODEright(x);                            //  y = x->right
  if (y == 0)
    return;
  setTNODEright(x, getTNODEleft(y));                      //  x->right = y->left
  if (getTNODEleft(y) != 0) {                             //  if y->left != 0
    setTNODEparent(getTNODEleft(y), x);                   //  y->left->parent = x
  }
  setTNODEparent(y, getTNODEparent(x));                   //  y->parent = x->parent
  if (getTNODEparent(x) == x) {                           //  if x->parent == x
    setGSTroot(t->gst, y);                                //  t->root = y
    setTNODEparent(y, y);
  }
  else if (x == getTNODEleft(getTNODEparent(x))) {        // if x == x->parent->leftChild
    setTNODEleft(getTNODEparent(x), y);                   // x->parent->leftChild = y;
  }
  else {
    setTNODEright(getTNODEparent(x), y);                  // x->parent->rightChild = y;
  }
  setTNODEleft(y, x);                                     //  y->leftChild = x;
  setTNODEparent(x, y);                                   //  x->parent = y;
  return;
}

/*  Rotate X down to Y [RIGHT]   */
void rotateRight(RBT *t, TNODE *x) {
  TNODE *y = getTNODEleft(x);                             //  y = x->left
  if (y == 0)
    return;
  setTNODEleft(x, getTNODEright(y));                      //  x->left = y->right
  if (getTNODEright(y) != 0) {                            //  if y->right != 0
    setTNODEparent(getTNODEright(y), x);                  //  y->right->parent = x
  }
  setTNODEparent(y, getTNODEparent(x));                   //  y->parent = x->parent
  if (getTNODEparent(x) == x) {                           //  if x->parent == x
    setGSTroot(t->gst, y);                                //  t->root = y
    setTNODEparent(y, y);
  }
  else if (x == getTNODEright(getTNODEparent(x))) {        // if x == x->parent->rightChild
    setTNODEright(getTNODEparent(x), y);                   // x->parent->leftChild = y;
  }
  else {
    setTNODEleft(getTNODEparent(x), y);                    // x->parent->leftChild = y;
  }
  setTNODEright(y, x);                                    //  y->rightChild = x;
  setTNODEparent(x, y);                                   //  x->parent = y;
  return;
}

int compareRBTV(void *a, void *b) {
  RBTVALUE *rbtva = a, *rbtvb = b;
  return rbtva->self->comparator(rbtva->value, rbtvb->value);
}

void displayRBTV(void *a, FILE *fp) {
  RBTVALUE *rbtv = a;
  rbtv->self->display(rbtv->value, fp);
  if (rbtv->color == 'R') {
    fprintf(fp, "*");
  }
}

void displayRBT(RBT *t,FILE *fp) {
  displayGST(t->gst, fp);
}

// void *value is the RBTVALUE
void freeRED(void *value) {
  RBTVALUE *rbtv = value;
  rbtv->self->free(rbtv->value);
  free(rbtv);
}

char checkLinear(RBT *t, TNODE *node) {
  TNODE *p = getTNODEparent(node);
  TNODE *g = grandparent(node);
  if (node == 0 || node == getGSTroot(t->gst)) {
    return '1';
  }
  if (g == 0) {
    return '1';
  }
  if (getTNODEleft(g) == p && getTNODEright(p) == node) {
    return 'L';
  }
  else if (getTNODEright(g) == p && getTNODEleft(p) == node) {
    return 'R';
  }
  return '0';
}

TNODE *sibling(TNODE *node) {
  TNODE *p = getTNODEparent(node);
  if (p == 0)
    return 0;
  if (node == getTNODEleft(p))
    return getTNODEright(p);
  return getTNODEleft(p);
}

TNODE *uncle(TNODE *node) {
  TNODE *p = getTNODEparent(node);
  TNODE *g = grandparent(node);
  if (g == 0)
    return 0;
  return sibling(p);
}

TNODE *grandparent(TNODE *node) {
  TNODE *p = getTNODEparent(node);
  if (p == 0)
    return 0;
  return getTNODEparent(p);
}

TNODE *niece(TNODE *node) {
  TNODE *s = sibling(node);
  if (s == 0)
    return 0;
  if (node == getTNODEleft(getTNODEparent(node))) {
    return getTNODEleft(s);
  }
  return getTNODEright(s);
}

TNODE *nephew(TNODE *node) {
  TNODE *s = sibling(node);
  if (s == 0)
    return 0;
  if (node == getTNODEleft(getTNODEparent(node)))
    return getTNODEright(s);
  return getTNODEleft(s);
}

void rotate(RBT *t, TNODE *node, TNODE *parent) {
  if (node == getTNODEleft(getTNODEparent(node)))
    rotateRight(t, parent);
  else
    rotateLeft(t, parent);
}

/*
  Responds to a node deletion and fixes
  the tree so it RBT properties are maintained
*/
void deletionFixUp(RBT *t, TNODE *x) {
  while (x != getGSTroot(t->gst) && getColor(x) != 'R') {
    if (getColor(sibling(x)) == 'R') {
      setColor(getTNODEparent(x), 'R');           // color parent red
      setColor(sibling(x), 'B');                  // color sibling black
      rotate(t, sibling(x), getTNODEparent(x));   // rotate sibling to parent
      // should have black sibling now
    }
    else if (getColor(nephew(x)) == 'R') {                  //  nephew is red
      setColor(sibling(x), getColor(getTNODEparent(x)));    // color sibling the same as parent
      setColor(getTNODEparent(x), 'B');                     // color parent black
      setColor(nephew(x), 'B');                             // color nephew black
      rotate(t, sibling(x), getTNODEparent(x));             // rotate sibling to parent
      // subtree and tree is BH balanced
      break;
    }
    // nephew must be black
    else if (getColor(niece(x)) == 'R') {        // niece is red
      setColor(niece(x), 'B');                   // color niece black
      setColor(sibling(x), 'R');                 // color sibling red
      rotate(t, niece(x), sibling(x));           // rotate niece to sibling
      // should have red nephew now
    }
    // sibling, niece, and nephew must be black
    else {
      setColor(sibling(x), 'R');         // color sibling red
      x = getTNODEparent(x);             // x = parent
      // this subtree is BH balanced, but tree is not
    }
  }
  setColor(x, 'B');                   // color x black
}
