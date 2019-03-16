GL00
======

## Required

 - OpenGL
 - SDL2 - `SDL2_ROOT`
 - Assimp - `assimp_ROOT`

### Assimp

When building the 'Debug' release for Assimp, configure the cmake project using
`CMAKE_BUILD_TYPE`. This will suffix the assimp libraries build with a 'd'.

```
cmake -G "Visual Studio 15 2017 Win64" .. -DCMAKE_BUILD_TYPE=Debug
```

### Windows

Cmake 3.12 or greated is required to build this project. The following entries
can be provided to cmake as either environment variables or through the command
line to provide the installation paths for the libraries reqruired.

 - SDL2 - `SDL2_ROOT`
 - Assimp - `assimp_ROOT`

## Building

Use cmake to create your build solution. 

Currently supports "Visual Studio 15 2017".

```
$ mkdir build
$ cd build
$ cmake -G "Visual Studio 15 2017" ..
```

Open the solution file and compile.
