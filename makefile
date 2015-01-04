O=3

CC=gcc
CFLAGS=-c -Wall --std=c11 -O$(O)

all: accretion

accretion: accretion.o vector.o body.o disjoint.o
	$(CC) accretion.o vector.o disjoint.o body.o -o accretion -lm

accretion.o: accretion.c vector.h body.h disjoint.h
	$(CC) $(CFLAGS) accretion.c

vector.o: vector.c
	$(CC) $(CFLAGS) vector.c

body.o: body.c vector.h
	$(CC) $(CFLAGS) body.c

disjoint.o: disjoint.c
	$(CC) $(CFLAGS) disjoint.c

cloud: gaussian_cloud.o vector.o body.o
	$(CC) gaussian_cloud.o vector.o body.o -o gaussian_cloud -lm

gaussian_cloud.o: gaussian_cloud.c vector.h body.h
	$(CC) $(CFLAGS) gaussian_cloud.c

clean:
	rm *.o accretion
