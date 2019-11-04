cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG\scripts\buildsystems\vcpkg.cmake"
cmake --build build
cmake --install build --prefix .
