An n-body gravitational simulation with simple accretion dynamics
=================================================================

Exactly what it says on the tin.

`accretion` is the actual integrator. Build with `make all` and feed an initial state into its stdin.
Use `-v 2` to see full state dumps at every step.

The folder `init` contains initial state generators. States are currently stored in a simple binary format to preserve precision.
```[number of bodies: 4 bytes]
[body: 56 bytes, comprised of:
  [mass: double, 8 bytes]
  [position: 3 doubles, 24 bytes]
  [velocity: 3 doubles, 24 bytes]
]
[body]
[body]
...```

Currently uses leapfrog method to integrate accelerations.

To include:
-----------
  - binary format simulation output flag
  - simulation accepting file input
  - gaussian cloud statistical property verification
  - two-body integrator test
  - more efficient than n^2 collision detection
  - tree-based gravity approximation
  - a less stupid makefile
  - visualization, someday in the future...?
