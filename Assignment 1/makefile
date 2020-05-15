OOPTS = -Wall -Wextra -g -std=c99

all : amaze

test : test1

valgrind : valgrind1

amaze : da.o cda.o stack.o queue.o amaze.o maze.o cell.o
		gcc $(OOPTS) amaze.o maze.o cell.o da.o cda.o stack.o queue.o -o amaze

amaze.o : amaze.c
		gcc $(OOPTS) -c amaze.c

da.o : da.c da.h
	gcc $(OOPTS) -c da.c

cda.o : cda.c da.h
		gcc $(OOPTS) -c cda.c

queue.o : queue.c queue.h
		gcc $(OOPTS) -c queue.c

stack.o : stack.c stack.h
		gcc $(OOPTS) -c stack.c

cell.o: cell.c cell.h
		gcc $(OOPTS) -c cell.c

maze.o: maze.c maze.h cell.h stack.h queue.h
		gcc $(OOPTS) -c maze.c

test1 : amaze
	./amaze -c 20 20 unsolved-maze -s maze solved-maze -d solved-maze

valgrind1 : amaze
		valgrind ./amaze -c 20 20 unsolved-maze -s maze solved-maze -d solved-maze

clean	:
		rm -f da.o cda.o stack.o queue.o integer.o cell.o maze.o amaze.o ./da ./cda ./queue ./stack ./amaze
