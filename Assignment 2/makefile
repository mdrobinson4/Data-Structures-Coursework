OOPTS = -Wall -Wextra -g -std=c99

all : trees

trees : cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o trees.o integer.o
		gcc $(OOPTS) cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o trees.o integer.o -o trees

tnodeC : cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o tnode-0-9.o
		gcc $(OOPTS) cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o tnode-0-9.o -o trees

bstC : cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o bst-0-9.o
		gcc $(OOPTS) cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o bst-0-9.o -o trees

gstC : cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o gst-0-9.o
		gcc $(OOPTS) cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o gst-0-9.o -o trees

rbtC : cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o rbt-0-9.o
		gcc $(OOPTS) cda.o queue.o bst.o tnode.o gst.o rbt.o scanner.o integer.o rbt-0-9.o -o trees

test:	trees
		./trees corpus commands

valgrind : trees
		valgrind ./trees corpus commands

tnode : tnodeC
		./trees
		#tnode > .test/tnode9
		#diff .test/tnode9 .test/tnode-0-9.expected

bst : bstC
		./trees
		#bst > .test/bst9
		#diff .test/bst9 .test/bst-0-9.expected

gst : gstC
		./trees
		#gst > .test/gst9
		#diff .test/gst9 .test/gst-0-9.expected

rbt : rbtC
		./trees
		#rbt > .test/rbt9
		#diff .test/rbt9 .test/rbt-0-9.expected

copy:
		cp ../objects/bst.o .
		cp ../objects/tnode.o .
		cp ../objects/gst.o .

scanner.o : scanner.c scanner.h
		gcc $(OOPTS) -c scanner.c

integer.o : integer.c integer.h
		gcc $(OOPTS) -c integer.c

trees.o : trees.c
	gcc $(OOPTS) -c trees.c

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

test0 : trees
		trees -r t-0-0.corpus t-0-0.commands

test1 : trees
		trees -r .test/t-0-1.corpus .test/t-0-1.commands > .test/1
		diff .test/1 .test/t-0-1-r.expected

test2 : trees
		trees -r .test/t-0-2.corpus .test/t-0-2.commands > .test/2
		diff .test/2 .test/t-0-2-r.expected

test3 : trees
		trees -r .test/t-0-3.corpus .test/t-0-3.commands > .test/3
		diff .test/3 .test/t-0-3-r.expected

test4 : trees
		trees -r .test/t-0-4.corpus .test/t-0-4.commands > .test/4
		diff .test/4 .test/t-0-4-r.expected

test5 : trees
		trees -r .test/t-0-5.corpus .test/t-0-5.commands > .test/5
		diff .test/5 .test/t-0-5-r.expected

test6 : trees
		trees -r .test/t-0-6.corpus .test/t-0-6.commands > .test/6
		diff .test/6 .test/t-0-6-r.expected

test7 : trees
		trees -r .test/t-0-7.corpus .test/t-0-7.commands > .test/7
		diff .test/7 .test/t-0-7-r.expected

test8 : trees
		trees -r .test/t-0-8.corpus .test/t-0-8.commands > .test/8
		diff .test/8 .test/t-0-8-r.expected

test9 : trees
		trees -r .test/t-0-9.corpus .test/t-0-9.commands > .test/9
		diff .test/9 .test/t-0-9-r.expected

test10 : trees
		trees -r .test/trg-0-10.corpus .test/trg-0-10.commands > .test/10
		diff .test/10 .test/t-0-10-r.expected

test11 : trees
		trees -g .test/t-0-0.corpus .test/t-0-0.commands > .test/0g
		diff .test/0g .test/t-0-0-g.expected

test12 : trees
		trees -g .test/t-0-1.corpus .test/t-0-1.commands > .test/1g
		diff .test/1g .test/t-0-1-g.expected

test13 : trees
		trees -g .test/t-0-2.corpus .test/t-0-2.commands > .test/2g
		diff .test/2g .test/t-0-2-g.expected

test14 : trees
		trees -g .test/t-0-3.corpus .test/t-0-3.commands > .test/3g
		diff .test/3g .test/t-0-3-g.expected

test15 : trees
		trees -g .test/t-0-4.corpus .test/t-0-4.commands > .test/4g
		diff .test/4g .test/t-0-4-g.expected

test16 : trees
		trees -g .test/t-0-5.corpus .test/t-0-5.commands > .test/5g
		diff .test/5g .test/t-0-5-g.expected

test17 : trees
		trees -g .test/t-0-6.corpus .test/t-0-6.commands > .test/6g
		diff .test/6g .test/t-0-6-g.expected

test18 : trees
		trees -g .test/t-0-7.corpus .test/t-0-7.commands > .test/7g
		diff .test/7g .test/t-0-7-g.expected

test19 : trees
		trees -g .test/t-0-8.corpus .test/t-0-8.commands > .test/8g
		diff .test/8g .test/t-0-8-g.expected

test20 : trees
		trees -g .test/t-0-9.corpus .test/t-0-9.commands > .test/9g
		diff .test/9g .test/t-0-9-g.expected

test21 : trees
		trees -g .test/trg-0-10.corpus .test/trg-0-10.commands > .test/10g
		diff .test/10g .test/t-0-10-g.expected

clean	:
		rm -f trees.o cda.o queue.o tnode.o bst.o gst.o rbt.o scanner.o rbt-0-10.o integer.o rbt-0-0.o ./trees bst-0-9.o gst-0-9.o rbt-0-9.o tnode-0-9.o ./rbt ./tnode ./bst ./gst
