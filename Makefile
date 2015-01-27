all:
	gcc -Wall -pedantic -std=c99 src/a1.c -Iinclude -o bin/runMe -lncurses