CFLAGS= -Wall -std=c99 -ansi -pedantic -O2 -lreadline
CC= gcc

ep1sh: ep1sh.c
	${CC} -o $@ $^ ${CFLAGS}

clean: 
	rm -f *.o *.gch ep1sh

run: ep1sh
	./ep1sh

update: clean
	git add * 