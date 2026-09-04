all: lib main

lib: src/cmath.c 
	gcc -fPIC -Wall src/cmath.c -shared -llua5.4 -ldl -lm -o bin/libcmath.so

main: src/main.c bin/libcmath.so
	gcc -Wall src/main.c -Lbin -lcmath -llua5.4 -ldl -lm -o bin/app

clean:
	rm -f bin/libcmath.so bin/app
