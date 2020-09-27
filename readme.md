
This code was melted together from [libigl-example-project](https://github.com/libigl/libigl-example-project) and [half-edge date structure by Yotam Gingold](https://github.com/yig/halfedge) and provided as a reference. 

**Remark**: This code is not optimized, but it gets the work done.

# Usage
Provide short description on how to run your code from command line.

## Prerequisites
For ubuntu:
```bash
sudo apt-get install git build-essential cmake libx11-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxi-dev libxmu-dev libblas-dev libxinerama-dev libxcursor-dev
```
Other OS: see [libigl tutorial](https://libigl.github.io/tutorial/) and [libigl github issues](https://github.com/libigl/libigl/issues)

Don't forget to clone libigl submodule:
```bash
git submodule update --init --recursive
```

## Build
```c
mkdir build
cd build
cmake ..
make
./example
```
