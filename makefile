CC = g++
CFLAGS = -O2 -g -Wall -Wextra -Werror --pedantic
OBJS = graphs.o

a.out : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o a.out

graphs.o : graphs.cpp graphs.h
	$(CC) $(CFLAGS) -c graphs.cpp

clean : 
	rm *.o
