# Build script for Windows
$ErrorActionPreference = "Stop"

# Create build directory
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build"
}

# Configure and build
Push-Location "build"
cmake -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ..
cmake --build . --config Release
Pop-Location