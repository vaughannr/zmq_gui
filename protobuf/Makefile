
CC=clang++
CFLAGS= -c -Wall -std=c++17
LFLAGS=-lzmq -ljsoncpp

all: bufTest

bufTest: bufTest.o
	$(CC) bufTest.o -o bufTest  $(LFLAGS)
bufTest.o: bufTest.cpp
	$(CC) $(CFLAGS) bufTest.cpp
clean:
	rm -rf *.o
	rm bufTest
	# rm a.out
