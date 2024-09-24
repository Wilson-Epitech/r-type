##
## R_TYPE PROJECT, 2024
## r-type
## File description:
## Makefile
##

# ---------------------- CMAKE ------------------------------------------ #

BUILD_DIR = build

BUILD_OBJ = $(BUILD_DIR)/cmaketmp

BINARY_NAME = r-type

VCPKG = -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake

CMAKE = cmake

# ----------------------------------------------------------------------- #


# ---------------------- DOCUMENTATION ---------------------------------- #

DOXYGEN = doxygen
DOC_DIR = ./documentation/
DOXYFILE = $(DOC_DIR)Doxyfile

# ----------------------------------------------------------------------- #

all: check-vcpkg
	@echo "CMake"
	@cd $(BUILD_DIR) && $(CMAKE) -B cmaketmp -S . $(VCPKG)
	@cd $(BUILD_OBJ) && $(CMAKE) --build .
	@cd $(BUILD_OBJ) && mv $(BINARY_NAME) ../..

check-vcpkg:
	@if [ ! -d "$(BUILD_DIR)/vcpkg" ]; then \
		echo "Cloning vcpkg directory..."; \
		git clone https://github.com/microsoft/vcpkg.git $(BUILD_DIR)/vcpkg > /dev/null 2>&1; \
		echo "Installing vcpkg binary..."; \
		cd $(BUILD_DIR)/vcpkg && ./bootstrap-vcpkg.sh > /dev/null 2>&1; \
		echo "Installing raylib..."; \
		./vcpkg install raylib > /dev/null 2>&1; \
	fi

re: all

clean:
	@echo "Clean"
	@cd $(DOC_DIR) && rm -rf docs/html
	@rm -rf $(BUILD_OBJ)
	@rm -f $(BINARY_NAME)

cleanvcpkg:
	@cd $(BUILD_DIR) && rm -rf vcpkg

doc:
	@$(DOXYGEN) $(DOXYFILE) > /dev/null
	@printf "[\033[0;36mdoc built\033[0m]\n"

fclean: clean cleanvcpkg
