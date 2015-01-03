O=3

CC=gcc
CFLAGS=-c -Wall --std=c11 -O$(O)

all: accretion

accretion: accretion.o vector.o disjoint.o
	$(CC) accretion.o vector.o disjoint.o -o accretion -lm

accretion.o: accretion.c
	$(CC) $(CFLAGS) accretion.c

vector.o: vector.c
	$(CC) $(CFLAGS) vector.c

disjoint.o: disjoint.c
	$(CC) $(CFLAGS) disjoint.c

clean:
	rm *.o accretion
