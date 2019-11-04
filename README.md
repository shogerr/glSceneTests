GL00
====

## Building

### Windows

This project is most easily built in Windows using
(Vcpkg)[https://github.com/microsoft/vcpkg].

#### Vcpkg Static Build Example

**Vcpkg Setup**
```
vcpkg install assimp:x64-windows-static \
              sdl2:x64-windows-static \
              glm:x64-windows-static
```

**Generate Build Configuration**

cmake -A x64 -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows-static
-DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scipts\buildsystems\vcpkg.cmake"
```

**Build Project**
```
cmake --build build --config release
```

**Install**
Install locally using CMake with `--prefix`.
```
cmake --install build --prefix .
```
