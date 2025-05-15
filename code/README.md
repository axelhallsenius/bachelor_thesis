# Code
For more in depth documentation, see the report

# Build Instructions

## Linux and other Unix based OS
A prerequisite for building is ``cmake`` This is a build tool used to compile the code into an executable. In order to acquire the project, use ``git``.

### Downloading:

```
git clone --recurse-submodules https://github.com/axelhallsenius/bachelor_thesis.git
```

### Building:

For convenience, a ``Makefile`` is provided, so the user may simply ``cd`` the ``code`` there run
```
make
```
which will run the needed ``cmake`` commands for you.

In order to run the projects tests, simply run
```
make test
```

There is also a ``clean`` and provided in the ``Makefile``, should the user wish to rebuild or remove the build directory. For this run
```
make clean
```

To run benchmarks:
```
make bench
```
These process should work for most UNIX based OS, but it has only been tested on Linux.

# Directory and File Structure
```
.
├── assets
│   ├── fonts
│   │   ├── DroidSans.ttf
│   │   └── LICENSE.txt
│   └── maps
│       ├── Mercator_Projection.svg
│       ├── Mercator_Projection.svg.license
│       ├── World_map_nations.svg
│       ├── World_map_nations.svg.license
│       ├── World-map.svg
│       └── World-map.svg.license
├── CMakeLists.txt
├── deps
│   ├── ...
├── imgui.ini
├── Makefile
├── README.md
├── src
│   ├── c
│   │   ├── draw.c
│   │   ├── draw.h
│   │   ├── ellipsoid.h
│   │   ├── map.c
│   │   ├── map.h
│   │   ├── vessel.c
│   │   ├── vessel.h
│   │   ├── vincenty.c
│   │   └── vincenty.h
│   ├── CMakeLists.txt
│   └── cpp
│       └── vessel_tracker.cpp
├── tests
│   ├── bench
│   │   ├── bench_all.cpp
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt
│   ├── oldtests.c
│   ├── oldtests.h
│   └── unit
│       ├── CMakeLists.txt
│       ├── test_all.cpp
│       ├── test_map.cpp
│       ├── test_vessel.cpp
│       └── test_vincenty.cpp
├── TODO.md
└── vessel_tracker
```
