#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "set.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(3);
    printf("REAL test of SET, simple\n");
    int i,size=10,count=8;
    SET *p = newSET();
    setSETdisplay(p,displayREAL);
    setSETfree(p,freeREAL);
    int *spots = malloc(sizeof(int) * size);
    REAL **elements = malloc(sizeof(REAL *) * size);
    assert(spots != 0 && elements != 0);
    for (i = 0; i < size; ++i)
        {
        spots[i] = -1;
        elements[i] = newREAL(i * 2.0);
        }
    for (i = 0; i < size; ++i)
        {
        int j = random() % size;
        REAL *temp = elements[i];
        elements[i] = elements[j];
        elements[j] = temp;
        }
    for (i = 0; i < size; ++i)
        {
        spots[i] = makeSET(p,elements[i]);
        }
    displaySET(p,stdout);
    for (i = 0; i < count; ++i)
        {
        int a = random() % size;
        int b = random() % size;
        printf("unioning %d and %d\n",a,b);
        unionSET(p,a,b);
        }
    displaySET(p,stdout);
    freeSET(p);
    free(spots);
    free(elements);
    return 0;
    }
