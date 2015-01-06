An n-body gravitational simulation with simple accretion dynamics
=================================================================

Exactly what it says on the tin.

`accretion` is the actual integrator. Build with `make all` and feed an initial
state into its `stdin`. A succession of states will exit `stdout`. `stderr` 
carries debug messages; by default, the number of bodies and current step 
number. Use `-v 0` to turn this off, or `-v 2` to see full state dumps in human
readable form at every step (default is `-v 1`). An example run might look
like:
```
make all
./gaussian_cloud -n 2000 | ./accretion > out.bin
```

The folder `init` contains initial state generators. States are currently 
represented in a simple binary format to preserve precision:
```
[number of bodies: 4 bytes]
[body: 56 bytes, comprised of:
  [mass: double, 8 bytes]
  [position: 3 doubles, 24 bytes]
  [velocity: 3 doubles, 24 bytes]
]
[body]
[body]
...
```

Currently uses leapfrog method to integrate accelerations.

Sets of intersecting objects are considered to be undergoing a collision and 
get merged into a single object which has mass and momentum the sum of those of
the colliders. Objects are taken to be spherical, with a constant density 
(specified in a header).

To include:
-----------
  - density option, gravitational constant option, timestep... for better 
    simulation control
  - gaussian cloud statistical property verification
  - two-body integrator accuracy test
  - better integration method
  - more efficient than n^2 collision detection
  - tree-based gravity approximation
  - a less stupid makefile
  - a build folder to contain the mess
  - visualization, someday in the future...?
  - energy/momentum/other conservation checking
  - state parser and conversion to human-readable form
  - malformed input handling
  - binary tree tests
