#/bin/bash

set -e

BUILD_DIR="build"

./scripts/BuildUnix.sh

rm -rf $BUILD_DIR/cmaketmp

cmake -B build/cmaketmp -S . -DCMAKE_TOOLCHAIN_FILE=build/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build/cmaketmp
