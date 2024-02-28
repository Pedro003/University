CC = gcc
CFLAGS = -Wall -g

pessoas: Ex3.o person.o

person.o: person.c person.h

Ex3.o: Ex3.c person.h

clean:
	rm -f *.o pessoas file_pessoas
