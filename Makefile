CFLAGS= -Wall -std=c99 -ansi -pedantic -O2 -lreadline -lpthread
CC= gcc

ep1sh: ep1sh.c
	${CC} -o $@ $^ ${CFLAGS}

clean: 
	rm -f *.o *.gch ep1sh

run: ep1sh
	./ep1sh

update: clean
	git add * 
	
ep1: ep1.c ep1.h FCFS.c FCFS.h SRTN.c SRTN.h MULT.c MULT.h
	${CC} -o $@ $^ ${CFLAGS}
