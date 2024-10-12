#/bin/bash

set -e

BUILD_DIR="build"

./scripts/BuildUnix.sh

rm -rf $BUILD_DIR/cmaketmp

cmake -B build/cmaketmp -S . -DCMAKE_TOOLCHAIN_FILE=build/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build/cmaketmp

mv $BUILD_DIR/cmaketmp/src/network/server/rtype_server .

mv $BUILD_DIR/cmaketmp/src/network/client/rtype_client .
