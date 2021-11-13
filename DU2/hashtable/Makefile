CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm
FILES=hashtable.c test.c test_util.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -g

clean:
	rm -f test
