#ifndef __VERTEX_INCLUDED__
#define __VERTEX_INCLUDED__

#include <stdio.h>


typedef struct vertex VERTEX;

extern VERTEX *newVERTEX(void *value);
extern VERTEX *getRep(VERTEX *v);
extern int link(VERTEX *r1, VERTEX *r2);
extern VERTEX *getValue(VERTEX *v);
extern int getIndex(VERTEX *v);
extern int getRank(VERTEX *v);
extern void setValue(VERTEX *v, void *value);
extern void setIndex(VERTEX *v, int index);
extern void setRank(VERTEX *v, int rank);
extern VERTEX *getParent(VERTEX *v);

#endif
