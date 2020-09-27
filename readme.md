# Homework 1 for IFT 6113: Subdivision surfaces
The code implements a subdivision surface for triangular meshes.

**Remark**: Taken from a template repository from https://github.com/ivanpuhachov/ift6113_2020/tree/master/hw1_cpp

# Usage
Provide short description on how to run your code from command line.

## Prerequisites
For Windows, the following c++ packages must be install with `vcpkg` (https://github.com/Microsoft/vcpkg):
* eigen3
* glfw
* glad
* opengl

Manual installation of `libigl` is required. As elaborated by the template repository https://github.com/ivanpuhachov/ift6113_2020/tree/master/hw1_cpp.

## Build
Inside a Visual Studio Developer prompt or powershell,
```batch
mkdir build
cd build
cmake .. -G"NMake Makefiles"
nmake
./example
```