Project 4
=========

The `src/scenes/` folder contains the solutions to the projects.
The `src/shaders/` folder contians the shaders for each project.

Project 6: `src/scenes/bezier_scene.hpp` and `src/scenes/bezier_scene.cpp`
  - `src/shaders/bezier.vs.glsl`
  - `src/shaders/bezier.fs.glsl`
  - `src/shaders/bezier-point.vs.glsl`
  - `src/shaders/bezier-point.fs.glsl`

Project 5: `src/scenes/shader_scene.hpp` and `src/scenes/shader_scene.cpp`
  The shaders for this project can also be found in the root of the submitted
  `.zip` submission as `shader_scene.vs.glsl` and `shader_scene.fs.glsl`.

Project 4: `src/scenes/lighting_scene.hpp` and `src/scenes/lighting_scene.cpp`

Project 3: `src/scenes/texture_scene.hpp` and `src/scenes/texture_scene.cpp`

Project 2: `src/scenes/helicopter_scene.hpp` and `src/scenes/helicopter_scene.cpp`

Project 1: `src/scenes/test_scene.hpp` and `src/scenes/test_scene.cpp`

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
