all: lib main

lib: src/main.c 
	gcc -fPIC -Wall src/cmath.c -shared -llua5.4 -ldl -lm -o bin/libcmath.so

main: src/main.c
	gcc -Wall src/main.c -L./bin -lcmath -llua5.4 -ldl -lm -o bin/app
