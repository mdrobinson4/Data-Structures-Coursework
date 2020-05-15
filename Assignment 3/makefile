OOPTS = -Wall -Wextra -g -std=c99

all : kruskal

kruskal : da.o set.o integer.o real.o kruskal.o rbt.o tnode.o bst.o gst.o queue.o cda.o scanner.o  edge.o
		gcc $(OOPTS) da.o set.o integer.o real.o edge.o rbt.o tnode.o bst.o gst.o queue.o cda.o scanner.o kruskal.o -o kruskal

test1 : da.o set.o integer.o set-0-2.o real.o  edge.o
		gcc $(OOPTS) da.o set.o integer.o real.o edge.o set-0-2.o -o kruskal

test:	test1
		./kruskal

valgrind:	test1
		valgrind --leak-check=full ./kruskal

integer.o : integer.c integer.h
		gcc $(OOPTS) -c integer.c

real.o : real.c real.h
		gcc $(OOPTS) -c real.c

da.o: da.c da.h
		gcc $(OOPTS) -c da.c

set.o : set.c set.h edge.c edge.h
		gcc $(OOPTS) -c set.c

vertex.o : vertex.c vertex.h
		gcc $(OOPTS) -c vertex.c

edge.o : edge.c edge.h set.c set.h
		gcc $(OOPTS) -c edge.c

set-0-2.o : set-0-2.c
		gcc $(OOPTS) -c set-0-2.c

kruskal.o : kruskal.c
		gcc $(OOPTS) -c kruskal.c

cda.o : cda.c cda.h
		gcc $(OOPTS) -c cda.c

queue.o : queue.c queue.h
		gcc $(OOPTS) -c queue.c

tnode.o: tnode.c tnode.h
		gcc $(OOPTS) -c tnode.c

bst.o: bst.c bst.h queue.h
		gcc $(OOPTS) -c bst.c

gst.o: gst.c gst.h bst.h tnode.h
		gcc $(OOPTS) -c gst.c

rbt.o: rbt.c rbt.h gst.h tnode.h
		gcc $(OOPTS) -c rbt.c

scanner.o : scanner.h scanner.c
		gcc $(OOPTS) -c scanner.c

clean	:
		rm -f da.o set.o kruakal.o integer.o real.o set-0-2.o vertex.o edge.o kruskal.o set.o cda.o queue.o tnode.o bst.o gst.o rbt.o tnode.o bst.o gst.o queue.o cda.o scanner.o ./kruskal
