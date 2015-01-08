#!/usr/bin/env bash

# set up test: generate initial state
make star_planet
./star_planet > orbit_test_in.bin

# run simulator
./accretion -h 0.01 -t 100 < orbit_test_in.bin > orbit_test_out.bin

# examine results
make orbit_test
./orbit_test < orbit_test_out.bin > orbit_test_out.csv
