O=3

CC=gcc
CFLAGS=-c -Wall --std=c11 -O$(O) -g -pg

all: accretion gaussian_cloud star_planet

accretion: accretion.o vector.o body.o disjoint.o mesh.o bintree.o
	$(CC) accretion.o vector.o disjoint.o body.o mesh.o bintree.o -o accretion -lm

accretion.o: accretion.c vector.h body.h disjoint.h
	$(CC) $(CFLAGS) accretion.c

disjoint.o: disjoint.c
	$(CC) $(CFLAGS) disjoint.c

mesh.o: mesh.c vector.h body.h bintree.h
	$(CC) $(CFLAGS) mesh.c

bintree.o: bintree.c
	$(CC) $(CFLAGS) bintree.c

body.o: body.c vector.h
	$(CC) $(CFLAGS) body.c

vector.o: vector.c
	$(CC) $(CFLAGS) vector.c

# TODO: automate initial state generator compilation
# initial state generators
gaussian_cloud: gaussian_cloud.o vector.o body.o
	$(CC) gaussian_cloud.o vector.o body.o -o gaussian_cloud -lm

gaussian_cloud.o: init/gaussian_cloud.c vector.h body.h
	$(CC) $(CFLAGS) init/gaussian_cloud.c

star_planet: star_planet.o vector.o body.o
	$(CC) star_planet.o vector.o body.o -o star_planet -lm

star_planet.o: init/star_planet.c vector.h body.h
	$(CC) $(CFLAGS) init/star_planet.c

# tests
bintree_test: bintree_test.o bintree.o
	$(CC) bintree_test.o bintree.o -o bintree_test -lm

bintree_test.o: test/bintree_test.c bintree.h
	$(CC) $(CFLAGS) test/bintree_test.c

# TODO: automate binary removal
clean:
	rm *.o accretion gaussian_cloud star_planet
