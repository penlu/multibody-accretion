O=3

CC=gcc
CFLAGS=-c -Wall --std=c11 -O$(O)

all: accretion gaussian_cloud star_planet

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

# TODO: automate initial state generator compilation
gaussian_cloud: gaussian_cloud.o vector.o body.o
	$(CC) gaussian_cloud.o vector.o body.o -o gaussian_cloud -lm

gaussian_cloud.o: init/gaussian_cloud.c vector.h body.h
	$(CC) $(CFLAGS) init/gaussian_cloud.c

star_planet: star_planet.o vector.o body.o
	$(CC) star_planet.o vector.o body.o -o star_planet -lm

star_planet.o: init/star_planet.c vector.h body.h
	$(CC) $(CFLAGS) init/star_planet.c

# TODO: automate binary removal
clean:
	rm -r *.o accretion gaussian_cloud star_planet
