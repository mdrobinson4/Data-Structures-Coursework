#include "bst.h"
#include "tnode.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

//static void setRootHelper(BST *t, TNODE *replacement);
static TNODE *swapHelp(BST *t, TNODE *node, TNODE *swap);
static void swapper(TNODE *a,TNODE *b);
static char checkPos(TNODE *n);
static int checkLeaf(TNODE *n);
static int min(int a, int b);
static TNODE *successor(TNODE *node);
static TNODE *predecessor(TNODE *node);

static void levelOrder(BST *t, FILE *fp);
static void preorder(BST *t, TNODE *node, FILE *fp);
static void inorder(BST *t, TNODE *node, FILE *fp);
static void postorder(BST *t, TNODE *node, FILE *fp);

static void displayLevel(BST *t, TNODE *curr, FILE *fp);

static void freeTree(BST *t, TNODE *node);

struct bst {
  TNODE *root;
  QUEUE *queue;
  int size;
  int maxDepth;
  int minDepth;
  int debugLevel;
  int (*comparator)(void *, void *);
  void (*swapper)(TNODE *, TNODE *);
  void (*free)(void *);
  void (*display)(void *, FILE *);
};

              /*  LOCAL FUNCTIONS  */

/*  Traverse the binary search tree one level at a time  */
void levelOrder(BST *t, FILE *fp) {
  if (getBSTroot(t) == 0) {
    fprintf(fp, "%d:\n", 0);
    return;
  }
  TNODE *curr = getBSTroot(t);
  int level = 0, nodeCount = 0, flag = 0;
  enqueue(t->queue, curr);
  while (sizeQUEUE(t->queue) > 0) {
    nodeCount = sizeQUEUE(t->queue);
    flag = nodeCount;
    while (nodeCount > 0) {
      curr = dequeue(t->queue);
      if (flag == nodeCount) {
        fprintf(fp, "%d: ", level);
      }
      displayLevel(t, curr, fp);
      if (getTNODEleft(curr) != 0)
        enqueue(t->queue, getTNODEleft(curr));
      if (getTNODEright(curr) != 0)
        enqueue(t->queue, getTNODEright(curr));
      nodeCount -= 1;
      if (nodeCount > 0) {
        fprintf(fp, " ");
      }
    }
    fprintf(fp, "\n");
    level += 1;
  }
}

/* Given a binary tree, print its nodes in inorder*/
void inorder(BST *t, TNODE *node, FILE *fp) {
  if (node == 0) {
    return;
  }
  fprintf(fp, "[");
  inorder(t, getTNODEleft(node), fp);
  if (getTNODEleft(node) != 0)
    fprintf(fp, " ");
  t->display(getTNODEvalue(node), fp);
  if (getTNODEright(node) != 0)
    fprintf(fp, " ");
  inorder(t, getTNODEright(node), fp);
  fprintf(fp, "]");
}

void postorder(BST *t, TNODE *node, FILE *fp) {
  if (node == 0)
    return;
  fprintf(fp, "[");
  postorder(t, getTNODEleft(node), fp);
  postorder(t, getTNODEright(node), fp);
  t->display(getTNODEvalue(node), fp);
  fprintf(fp, "]");
  if (node != getBSTroot(t))
    fprintf(fp, " ");
}

void preorder(BST *t, TNODE *node, FILE *fp) {
  if (node == 0)
    return;
  fprintf(fp, "[");
  t->display(getTNODEvalue(node), fp);
  if (getTNODEleft(node) != 0)
    fprintf(fp, " ");
  preorder(t, getTNODEleft(node), fp);
  if (getTNODEright(node) != 0)
    fprintf(fp, " ");
  preorder(t, getTNODEright(node), fp);
  fprintf(fp, "]");
}

/*  Display each each level in the binary search tree  */
void displayLevel(BST *t, TNODE *curr, FILE *fp) {
  if (checkLeaf(curr) == 1) {
    fprintf(fp, "=");
  }
  t->display(getTNODEvalue(curr), fp);
  fprintf(fp, "(");
  if (getTNODEparent(curr) == curr)
    t->display(getTNODEvalue(curr), fp);
  else
    t->display(getTNODEvalue(getTNODEparent(curr)), fp);
  fprintf(fp, ")");
  fprintf(fp, "%c", checkPos(curr));
}

/*  Create new tree and set comparator function  */
BST *newBST(int (*c)(void *, void *)) {
  BST *tree = malloc(sizeof(BST));
  assert(tree != 0);
  tree->queue = newQUEUE();
  tree->comparator = c;
  tree->swapper = swapper;
  tree->display = 0;
  tree->free = 0;
  tree->root = 0;
  tree->size = 0;
  tree->debugLevel = 0;
  return tree;
}

/*  Set display function  */
void setBSTdisplay(BST *t, void (*d)(void *,FILE *)) {
  t->display = d;
}

void setBSTswapper(BST *t, void (*s)(TNODE *,TNODE *)) {
  t->swapper = s;
}

void setBSTfree(BST *t, void (*f)(void *)) {
  t->free = f;
}

TNODE *getBSTroot(BST *t) {
  return t->root;
}

/* This method updates the root pointer of a BST object. It should run in constant time.   */
void setBSTroot(BST *t, TNODE *replacement) {
  t->root = replacement;
  if (replacement != 0) {
    setTNODEparent(replacement, replacement);
  }
}

void setBSTsize(BST *t, int s) {
  t->size = s;
}

/*  inserts a value into the search tree, returning the inserted TNODE.  */
TNODE *insertBST(BST *t, void *value) {
  t->size += 1;
  TNODE *newNode = newTNODE(value, 0, 0, 0);
  setTNODEdisplay(newNode, t->display);
  TNODE *parent = 0, *curr = t->root;

  // Empty tree
  if (t->root == 0) {
    setBSTroot(t, newNode);
    setTNODEparent(newNode, newNode);
    return newNode;
  }
  while (curr != 0) {
    parent = curr;
    if (t->comparator(value, getTNODEvalue(curr)) < 0) {
      curr = getTNODEleft(curr);
    }
    else if (t->comparator(value, getTNODEvalue(curr)) > 0) {
      curr = getTNODEright(curr);
    }
    else {
      return 0;
    }
  }
  if (t->comparator(value, getTNODEvalue(parent)) < 0) {
    setTNODEleft(parent, newNode);
  }
  else {
    setTNODEright(parent, newNode);
  }
  setTNODEparent(newNode, parent);
  return newNode;
}

/*
  returns the tree node holding the searched-for key.
  If the key is not in the tree, the method should return null.
*/
TNODE *locateBST(BST *t, void *key) {
  TNODE *curr = getBSTroot(t);
  while (curr != 0) {
    int diff = t->comparator(key, getTNODEvalue(curr));
    // Value found !
    if (diff == 0) {
      return curr;
    }
    if (diff < 0) {
      curr = getTNODEleft(curr);
    }
    else if (diff > 0) {
      curr = getTNODEright(curr);
    }
  }
  return NULL;
}

/*
  returns the value with the searched-for key. If the
  key is not in the tree, the method should return null.
*/
void *findBST(BST *t,void *key) {
  TNODE *curr = getBSTroot(t);
  while (curr != 0) {
    int diff = t->comparator(key, getTNODEvalue(curr));
    if (diff == 0) {
      return getTNODEvalue(curr);
    }
    if (diff < 0) {
      curr = getTNODEleft(curr);
    }
    else if (diff > 0) {
      curr = getTNODEright(curr);
    }
  }
  return NULL;
}

int checkLeaf(TNODE *n) {
  if (getTNODEright(n) == 0 && getTNODEleft(n) == 0) {
    return 1;
  }
  return 0;
}

/*
  The method returns -1 if the if the given value is not in
  the tree, zero otherwise. If present, the tree node that holds
  the given value is removed from the tree. and the size of the tree
  is decremented. HINT: swapping the value to a leaf and pruning that
  leaf is an easy way to implement delete
*/
int deleteBST(BST *t, void *key) {
  TNODE *node = locateBST(t, key);
  if (node == NULL) {
    return -1;
  }
  if (checkLeaf(node) == 0) { // Not a leaf
    node = swapToLeafBST(t, node);
  }
  pruneLeafBST(t, node);
  free(node);
  t->size -= 1;
  return 0;
}

/*  Swap nodes and return swapped node  */
TNODE *swapHelp(BST *t, TNODE *node, TNODE *swap) {
  if (t->swapper != 0) {
    t->swapper(node, swap);
  }
  return swap;
}

/*
Takes a node and recursively swaps its value with its predecessor's (preferred)
or its successor's until a leaf node holds the original value. It calls the BST’s
swapper function to actually accomplish the swap, sending the two nodes whose values
need to be swapped. The method returns the leaf node holding the swapped value.
*/
TNODE *swapToLeafBST(BST *t, TNODE *node) {
  TNODE *swap = 0;
  if (getTNODEleft(node) != 0) {
  swap = swapHelp(t, node, predecessor(node));
  }
  else if (getTNODEright(node) != 0) {
    swap = swapHelp(t, node, successor(node));
  }
  else {
    return node;
  }
  return swapToLeafBST(t, swap);
}

/*
detaches the given node from the tree. It does
not free the node nor decrement the size, however.
*/
void pruneLeafBST(BST *t, TNODE *leaf) {
  TNODE *parent = getTNODEparent(leaf);
  if (leaf == getBSTroot(t)) {
    setBSTroot(t, 0);
    return;
  }
  if (leaf == getTNODEright(parent)) {
    //setTNODEparent(leaf, 0);
    setTNODEright(parent, 0);
  }
  else if (leaf == getTNODEleft(parent)) {
    //setTNODEparent(leaf, 0);
    setTNODEleft(parent, 0);
  }
}

/* Returns node's predecessor */
TNODE *predecessor(TNODE *node) {
   TNODE *curr = getTNODEleft(node);
   if (curr == 0)
    return 0;

  while (getTNODEright(curr) != 0)
    curr = getTNODEright(curr);
  return curr;
}

/* Returns node's successor */
TNODE *successor(TNODE *node) {
   TNODE *curr = getTNODEright(node);
   if (curr == 0)
    return 0;
  while (getTNODEleft(curr) != 0)
    curr = getTNODEleft(curr);
  return curr;
}

int sizeBST(BST *t) {
  return t->size;
}

int minDepth(TNODE *node) {
  if (node == 0)
    return -1;
  else {
    int left = minDepth(getTNODEleft(node));
    int right = minDepth(getTNODEright(node));
    return min(left, right) + 1;
  }
}

/*  Returns the minimum of two numbers  */
int min(int a, int b) {
  if (a < b)
    return a;
  else
    return b;
}

int maxDepth(TNODE *node) {
  if (node == 0)
    return -1;
  else {
    int left = maxDepth(getTNODEleft(node));
    int right = maxDepth(getTNODEright(node));

    if (left > right)
      return (left + 1);
    else
      return (right + 1);
  }
}

void statisticsBST(BST *t, FILE *fp) {
  int max = maxDepth(getBSTroot(t));
  int min = minDepth(getBSTroot(t));

  fprintf(fp, "Nodes: %d\n", sizeBST(t));
  fprintf(fp, "Minimum depth: %d\n", min);
  fprintf(fp, "Maximum depth: %d\n", max);
}

char checkPos(TNODE *n) {
  TNODE *parent = getTNODEparent(n);
  // ROOT
  if (parent != n) {
    if (getTNODEleft(parent) == n)  // LEFT CHILD
      return 'L';
    else // RIGHT CHILD
      return 'R';
  }
  return 'X'; // ROOT
}

/*
  prints a level-order traversal of the tree, each level on its own line.
  Each line should start with the level number and have no preceding
  whitespace and no trailing whitespace (other than a newline). A single
  space should separate entries on a line. A line entry is generated by
  calling the cached display function. The root value is tagged with an X,
  a left child with an L, a right child with an R, and a leaf with an equals sign.
  Unlike the other tags, the equal sign should precede the node value. All entries
  are tagged with their parents' value. This method should run in linear time
*/
void displayBST(BST *t, FILE *fp) {
  TNODE *root = getBSTroot(t);
  if (t->debugLevel >=1 && t->debugLevel <= 3) {
    if (sizeBST(t) == 0) {
      fprintf(fp, "[]");
      return;
    }
    if (t->debugLevel == 1)
      inorder(t, root, fp);
    else if (t->debugLevel == 2)
      preorder(t, root, fp);
    else
      postorder(t, root, fp);
  }
  else {
    if (sizeBST(t) == 0) {
      fprintf(fp, "0:\n");
      return;
    }
    levelOrder(t, fp);
  }
}

int debugBST(BST *t, int level) {
  int old = t->debugLevel;
  t->debugLevel = level;
  return old;
}

/*
walks through the tree, freeing the nodes that make
up the tree (and their values, if appropriate). Then,
the tree object itself is freed.
*/
void freeBST(BST *t) {
  freeTree(t, t->root);
  freeQUEUE(t->queue);
  free(t);
}

void freeTree(BST *t, TNODE *node) {
  if (node == 0)
    return;
  freeTree(t, getTNODEleft(node));
  freeTree(t, getTNODEright(node));
  t->free(getTNODEvalue(node));
  free(node);
}

void swapper(TNODE *a,TNODE *b)
    {
    void *x = getTNODEvalue(a);
    void *y = getTNODEvalue(b);
    setTNODEvalue(a,y);
    setTNODEvalue(b,x);
    }
