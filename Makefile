CC = g++
CFLAGS = -O2 -fopenmp -Wall -Wextra -Werror -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion --pedantic
OBJS = graphs.o

a.out : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o a.out

graphs.o : graphs.cpp graphs.h
	$(CC) $(CFLAGS) -c graphs.cpp

clean : 
	rm *.o
