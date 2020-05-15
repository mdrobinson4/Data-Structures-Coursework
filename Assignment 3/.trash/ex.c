//
// testSET.c
// Created by William Jiang on 11/7/19
//
// This is a test of the set class declared in set.h and
// defined in set.c.
//
// Output should match example given in set specification:
// http://beastie.cs.ua.edu/cs201/assign-disjoint.html
//

#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "set.h"

int main(void) {
    SET* s = newSET();
    setSETdisplay(s,displayINTEGER);
    setSETfree(s,freeINTEGER);
    int four  = makeSET(s,newINTEGER(4));
    int eight = makeSET(s,newINTEGER(8));
    int three = makeSET(s,newINTEGER(3));
    int one   = makeSET(s,newINTEGER(1));
    int seven = makeSET(s,newINTEGER(7));
    int nine  = makeSET(s,newINTEGER(9));

    unionSET(s,four,three);
    unionSET(s,seven,one);
    unionSET(s,eight,one);
    unionSET(s,three,one);

    displaySET(s,stdout);
    freeSET(s);

    return 0;
}
