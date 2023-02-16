CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LINKERFLAG = -lm
project: main.o matrix.o vector.o cluster_point.o ioparser.o linked_list.o
%: %.o
	${CC} ${LINKERFLAG} $&lt; -o $@
%.o: %.c
	 $(CC) -c $&lt; 
#clean:
#	rm -f some_file