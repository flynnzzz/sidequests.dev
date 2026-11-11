CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCDIR = src
BINDIR = bin

.PHONY: clean
clean:
	rm -rf $(BINDIR)/main $(BINDIR)/test

$(BINDIR)/main: $(SRCDIR)/main.c $(SRCDIR)/arraylist.c $(SRCDIR)/arraylist.h
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/main $(SRCDIR)/main.c $(SRCDIR)/arraylist.c

$(BINDIR)/test: test/test.c $(SRCDIR)/arraylist.c $(SRCDIR)/arraylist.h
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $(BINDIR)/test $(SRCDIR)/arraylist.c test/test.c

# Clean, compile and run main
run-main: clean $(BINDIR)/main
	./$(BINDIR)/main

# Clean, compile and run test
run-test: clean $(BINDIR)/test
	./$(BINDIR)/test
