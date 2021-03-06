Param(
    [Switch]$Clean
)

if ($Clean -eq $True) {
    git clean -xdf -e glad -e .vs -e *.swp -e CMakeSettings.json
    exit
}

cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG\scripts\buildsystems\vcpkg.cmake"
cmake --build build --config
cmake --install build --prefix . --config Debug
