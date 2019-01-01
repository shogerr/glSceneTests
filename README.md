GL00
======

# Required

 - OpenGL
 - SDL2 - `SDL2_ROOT`
 - GLEW
 - Assimp - `assimp_ROOT`

# Building

Use cmake to create your build solution. 

Currently supports "Visual Studio 15 2017".

```
$ mkdir build
$ cd build
$ cmake -G "Visual Studio 15 2017" ..
```

Open the solution file and compile. Must have GLEW, SDL2 and OpenGL libraries
available.
