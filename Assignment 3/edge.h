#ifndef __EDGE_INCLUDED__
#define __EDGE_INCLUDED__

#include <stdio.h>

typedef struct edge EDGE;

extern EDGE *newEDGE(int v1, int v2, int weight);
extern int compareVERTEX(EDGE *e1, EDGE *e2);
extern int sumVERTEX(EDGE *e);
extern int minVERTEX(EDGE *e);
extern int getV1(EDGE *e);
extern int getV2(EDGE *e);
extern int getWeight(EDGE *e);
extern int compareEDGE(EDGE *e1, EDGE *e2);
extern void setEDGEindex(EDGE *e, int index);

extern int getEDGEindex(EDGE *e);

#endif
