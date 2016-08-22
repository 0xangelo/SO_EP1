CFLAGS= -Wall -std=c99 -ansi -pedantic -O2
CC= gcc

ep1sh: ep1sh.c
	${CC} -o $@ $^ ${CFLAGS}

clean: 
	rm -f *.o *.gch 

run: ep1sh
	./ep1sh
