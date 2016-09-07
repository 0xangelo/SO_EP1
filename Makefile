CFLAGS= -Wall -std=c99 -ansi -pedantic -O2 -lreadline -lpthread
CC= gcc

ep1: ep1.c
	${CC} -o $@ $^ ${CFLAGS}

ep1sh: ep1sh.c
	${CC} -o $@ $^ ${CFLAGS}

clean: 
	rm -f *.o *.gch ep1sh

run: ep1sh
	./ep1sh

update: clean
	git add * 