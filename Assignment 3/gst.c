#include "bst.h"
#include "gst.h"
#include "tnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

static void swapper(TNODE *a,TNODE *b);
static int compareGREEN(void *a, void *b);
static void displayGREEN(void *a, FILE *fp);
static void freeGREEN(void *value);

struct gst {
  BST *bst;
  int size;
  int total;
  void (*free)(void *);
  void (*display)(void *, FILE *);
  int (*comparator)(void *, void *);
};

typedef struct gValue {
  void *value;
  int frequency;
  void (*display)(void *, FILE *);
  int (*comparator)(void *, void *);
  void (*free)(void *);
  GST *self;
}gValue;

GST *newGST(int (*c)(void *,void *)) {
  GST *t = malloc(sizeof(GST));
  assert(t != 0);
  t->free = 0;
  t->comparator = c;
  t->size = 0;
  t->total = 0;
  t->bst = newBST(compareGREEN);
  setBSTdisplay(t->bst, displayGREEN);
  setBSTfree(t->bst, freeGREEN);
  setBSTswapper(t->bst, swapper);
  return t;
}

static gValue *newGValue(void *value, void (*d)(void *, FILE *), int (*c)(void *, void *), void (*f)(void *), GST *t) {
  gValue *greenValue = malloc(sizeof(gValue));
  assert(value != 0);
  greenValue->display = d;
  greenValue->comparator = c;
  greenValue->free = f;
  greenValue->frequency = 1;
  greenValue->value = value;
  greenValue->self = t;
  return greenValue;
}

/*  Default swapper function  */
void swapper(TNODE *a,TNODE *b) {
  void *x = getTNODEvalue(a);
  void *y = getTNODEvalue(b);
  setTNODEvalue(a,y);
  setTNODEvalue(b,x);
}

void setGSTdisplay(GST *t,void (*d)(void *,FILE *)) {
  t->display = d;
  setBSTdisplay(t->bst, displayGREEN);
}

void setGSTswapper(GST *t,void (*s)(TNODE *,TNODE *)) {
  setBSTswapper(t->bst, s);
}

void setGSTfree(GST *t,void (*f)(void *)) {
  t->free = f;
  setBSTfree(t->bst, freeGREEN);
}

TNODE *getGSTroot(GST *t) {
  return getBSTroot(t->bst);
}

void setGSTroot(GST *t,TNODE *replacement) {
  setBSTroot(t->bst, replacement);

}
void setGSTsize(GST *t,int s) {
  int diff = sizeGST(t) - s;
  setBSTsize(t->bst, s);
  t->total = t->total - diff;
}

/*
  This method attempts to inserts a generic value in the tree. If the value to
  be inserted is already in the tree, the frequency count of the value in the
  tree is incremented, the value passed to the insert method is perhaps freed,
  and a null pointer is returned. If the generic value is not in the tree, it is
  inserted and the tree node that holds the value is returned. The passed-in
  generic value should be freed if it is a duplicate and a freeing function has
  been passed to the tree via setGSTfree.
*/
TNODE *insertGST(GST *t, void *value) {
  t->total += 1;
  gValue *gVal = newGValue(value, t->display, t->comparator, t->free, t);
  TNODE *node = locateBST(t->bst, gVal);
  if (node == 0) {  // value not in the tree
    node = insertBST(t->bst, gVal);
    //if (sizeGST(t) == 1) {
      //setTNODEparent(node, node);
      //setBSTroot(t->bst, node);
    //}
    return node;
  }
  // Value already in tree
  free(gVal);
  if (t->free != 0) {
    t->free(value);
  }
  gVal = getTNODEvalue(node);
  gVal->frequency += 1;
  return NULL;
}
/*
  returns the value stored with the given key. It returns null if the key is
  not in the tree.
*/
void *findGST(GST *t,void *key) {
  gValue *gVal = newGValue(key, t->display, t->comparator, t->free, t);
  gValue *value = findBST(t->bst, gVal);
  free(gVal);
  if (value == 0) {
    return NULL;
  }
  return value->value;
}
/*
  returns the tree node holding the searched-for key. If the key is not in the
  tree, the method should return null.
*/
TNODE *locateGST(GST *t,void *key) {
  gValue *value = newGValue(key, t->display, t->comparator, t->free, t);
  TNODE *node = locateBST(t->bst, value);
  free(value);
  if (node == NULL) {
    return NULL;
  }
  return node;
}

/*
  starts by finding the generic value stored in the tree that matches the given
  value. If the value is not in the tree, -1 is returned. Otherwise the resulting
  frequency is returned. If the frequency count of the stored value is greater
  than one, this method reduces the frequency. If the frequency count is one,
  however, the GST value is removed from the tree (i.e. zero is returned).
*/
int deleteGST(GST *t,void *key) {
  gValue *gVal = newGValue(key, t->display, t->comparator, t->free, t);
  TNODE *node = locateBST(t->bst, gVal);
  free(gVal);
  // Value not in tree
  if (node == 0) {
    return -1;
  }
  gValue *val = getTNODEvalue(node);  // get the greenValue
  t->total -= 1;
  if (val->frequency == 1) {  // Delete green value if there is only one in BST
    deleteBST(t->bst, val);
    free(val);
    return 0;
  }
  val->frequency -= 1;
  return val->frequency;
}

TNODE *swapToLeafGST(GST *t,TNODE *node) {
  TNODE *leaf = swapToLeafBST(t->bst, node);
  return leaf;
}

void pruneLeafGST(GST *t,TNODE *leaf) {
  free(getTNODEvalue(leaf));
  pruneLeafBST(t->bst, leaf);
}

/*
display the number of duplicates. Then the method
calls the BST statistics method.
*/
void statisticsGST(GST *t, FILE *fp) {
  fprintf(fp, "Duplicates: %d\n", duplicatesGST(t));
  statisticsBST(t->bst, fp);
}

int debugGST(GST *t,int level) {
  return debugBST(t->bst, level);
}

/*
when passed a node n, extracts the GST value. From that extracted value, it
returns the generic value that is wrapped by the GST value.
*/
void *unwrapGST(TNODE *n) { //?
  gValue *gVal = getTNODEvalue(n);  // Gets GST value
  return gVal->value; // Returns void * [INTEGER, REAL, etc]
}

/*
  returns the frequency of the searched-for key. If the key is not in the
  tree, the method should return zero.
*/
int freqGST(GST *g,void *key) {
  gValue *gVal = newGValue(key, g->display, g->comparator, g->free, g);
  gValue *find = findBST(g->bst, gVal);
  free(gVal);
  if (find == 0) {
    return 0;
  }
  return find->frequency;
}

/*
  This method returns the number of duplicate values currently in the tree.
  It should run in amortized constant time. This should be equal to the net number
  of GST insertions minus the number of nodes in the underlying BST.
*/
int duplicatesGST(GST *g) {
  return g->total - sizeBST(g->bst);
}

int sizeGST(GST *t) {
  return sizeBST(t->bst);
}

int compareGREEN(void *a, void *b) {
  gValue *val1 = a, *val2 = b;
  return val1->self->comparator(val1->value, val2->value);
}

/*
calls the tree using a level-order traversal, via the
decorated display method of the underlying data structure.
*/
void displayGST(GST *t,FILE *fp) {
  displayBST(t->bst, fp);
}

//  void * must be GGSTVALUE
void displayGREEN(void *a, FILE *fp) {
  gValue *val = a;
  val->self->display(val->value, fp);   //  CALLS displayRBVTvalue
  if (val->frequency > 1) {
    fprintf(fp, "<%d>", val->frequency);
  }
}

void freeGST(GST *t) {
  freeBST(t->bst);
  free(t);
}

// void *value is the gstValue
void freeGREEN(void *value) {
  gValue *val = value;
  val->self->free(val->value);
  free(val);
}
