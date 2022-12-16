PROG = gtkex
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config  --libs gtk+-3.0`

${PROG} : ${PROG.c}
	${CC} -Wall -Wextra -g ${CFLAGS} -o ${PROG} ${PROG}.c ${LIBS} -export-dynamic

clean : 	
	rm ${PROG}
