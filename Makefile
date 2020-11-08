all: build

main.o: main.cpp AVLtree.hpp
	g++ -c -o main.o main.cpp

build: main.o
	g++ -o tree main.o

clean:
	rm -f main.o tree

.PHONY=	build clean all

