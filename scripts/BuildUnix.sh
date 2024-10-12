#/bin/bash

set -e

BUILD_DIR="build"

echo "Checking vcpkg Configuration..."; \
if [ ! -d "$BUILD_DIR/vcpkg" ]; then \
		echo "Cloning vcpkg directory..."; \
		git clone https://github.com/microsoft/vcpkg.git $BUILD_DIR/vcpkg > /dev/null 2>&1; \
		echo "Installing vcpkg binary..."; \
		cd $BUILD_DIR/vcpkg && ./bootstrap-vcpkg.sh > /dev/null 2>&1; \
		echo "Exporting vcpkg path..."; \
		export VCPKG_ROOT=$BUILD_DIR/vcpkg
		echo "Installing raylib..."; \
		./vcpkg install raylib > /dev/null 2>&1; \
		echo "Installing asio..."; \
		./vcpkg install asio > /dev/null 2>&1; \
		echo "Installing glfw3..."; \
		./vcpkg install glfw3 > /dev/null 2>&1; \
fi
