GL00
====

## Building

The project is intended to be cross platform.
Only instructions for building on Windows are given.

### Windows

This project is most easily built in Windows using
[Vcpkg](https://github.com/microsoft/vcpkg).

#### Vcpkg Build Example

The following instuctions use vcpkg to fulfill dependencies, and build a
dynamically linked executable.

##### Vcpkg Setup
```
vcpkg install assimp:x64-windows \
              sdl2:x64-windows \
              glm:x64-windows
```

##### Generate Build Configuration
```
cmake -A x64 -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scipts\buildsystems\vcpkg.cmake"
```

##### Build Project
```
cmake --build build --config release
```

##### Install
```
cmake --install build --prefix .
```

#### Vcpkg Static Build Example

Generating the build configuration in the following manner creates a test
application that is statically linked.

```
vcpkg install assimp:x64-windows-static \
              sdl2:x64-windows-static \
              glm:x64-windows-static

cmake -A x64 -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scipts\buildsystems\vcpkg.cmake"
```

