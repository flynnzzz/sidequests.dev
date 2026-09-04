CC = gcc
CFLAGS = -Wall
LDLIBS = -llua5.4 -ldl -lm

all: bin/libcmath.so bin/app

bin/libcmath.so: src/cmath.c
	$(CC) -fPIC -shared $(CFLAGS) $< $(LDLIBS) -o $@

bin/app: src/main.c src/luautils.c bin/libcmath.so
	$(CC) $(CFLAGS) $(wordlist 1,2,$^) -Lbin -lcmath \
		-Wl,-rpath,'$$ORIGIN' \
		$(LDLIBS) -o $@
# pass the relative path to the linker

clean:
	rm -f bin/libcmath.so bin/app
