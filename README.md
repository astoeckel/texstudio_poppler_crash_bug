Test Case for TeXstudio Bug #1409
=================================

See 
* https://github.com/texstudio-org/texstudio/issues/1409
* https://github.com/texstudio-org/texstudio/pull/1463
* https://github.com/texstudio-org/texstudio/pull/1498
for more information.

This code uses the `meson` build system. You'll need to install `meson` via `pip`,
as well as `ninja`.

To build, run the followingq
```sh
# Clone the repository
git clone git@github.com:astoeckel/texstudio_poppler_crash_bug

# Create and change into the build directory
cd texstudio_poppler_crash_bug
mkdir build
cd build

# Configure and build
meson ..
ninja

# Download some test data
wget http://compneuro.uwaterloo.ca/files/publications/stoeckel.2020c.pdf

# Run the test
./texstudio_poppler_crash_bug stoeckel.2020c.pdf
```

You should get an output along the following lines
```
Iteration 0/1000000 (0%)"/proc/253266/root"
Iteration 236100/1000000 (23%)texstudio_poppler_crash_bug: strtod_l.c:1497: ____strtod_l_internal: Assertion `numsize == 1 && n < d' failed.
Aborted (core dumped)
```
