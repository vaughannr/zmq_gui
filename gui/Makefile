CC=clang++
CFLAGS= -c -Wall -std=c++17
LFLAGS=-lzmq -ljsoncpp

all: sub

sub: sub.o
	$(CC) sub.o -o sub $(LFLAGS)
sub.o: sub.cpp
	$(CC) $(CFLAGS) sub.cpp
clean:
	rm -rf *.o
	# rm a.out
