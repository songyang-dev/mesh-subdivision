# Homework 1 for IFT 6113: Subdivision surfaces
The code implements a subdivision surface for triangular meshes.

**Remark**: Taken from a template repository from https://github.com/ivanpuhachov/ift6113_2020/tree/master/hw1_cpp

# Usage
The executable is called `subdivide.exe` on Windows, which `nmake` places in the build directory. Calling the executable without options gives this instruction message:
```bash
$ ./subdivide.exe
Usage: ./subdivide.exe file (--loop | --butterfly | --sqrt3) [-n iterations] [-o output]
If no iterations are given, default is 0.
Default output is "output.obj" the current directory.
```
An example command is
```bash
$ ./subdivide.exe ../input/cube.obj --butterfly -o output/cube.obj -n 3
```
The `-o` argument must be pointing to an existing directory, or else no output is generated.

## Limitations
The `sqrt3` subdivision scheme is not implemented.

# Installation
## Prerequisites
For Windows, the following c++ packages must be install with `vcpkg` (https://github.com/Microsoft/vcpkg):
* eigen3
* glfw
* glad
* opengl

Manual installation of `libigl` is required, as elaborated by the [template repository](https://github.com/ivanpuhachov/ift6113_2020/tree/master/hw1_cpp).

*Note*: There is no additional dependency in this project compared to the template repository. So the project should be able to compile on any system that satisfies the template repo.

## Build
### Windows
Inside a Visual Studio Developer prompt or powershell,
```bash
mkdir build
cd build
cmake .. -G"NMake Makefiles"
nmake
./example
```
### Other OS
Follow the steps on the template repository.