##
## R_TYPE PROJECT, 2024
## r-type
## File description:
## Makefile
##

# ---------------------- CMAKE ---------------------------------- #

BUILD_DIR = build

BUILD_OBJ = $(BUILD_DIR)/cmaketmp

BINARY_NAME = r-type_server

VCPKG = -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake

CMAKE = cmake

# ----------------------------------------------------------------------- #


# ---------------------- DOCUMENTATION ---------------------------------- #

DOXYGEN = doxygen
DOC_DIR = ./documentation/
DOXYFILE = $(DOC_DIR)Doxyfile

# ----------------------------------------------------------------------- #

all:
	@echo "CMake"
	@cd $(BUILD_DIR) && $(CMAKE) -B cmaketmp -S . $(VCPKG)
	@cd $(BUILD_OBJ) && $(CMAKE) --build .
	@cd $(BUILD_OBJ) && mv $(BINARY_NAME) ../..

re: all

clean:
	@echo "Clean"
	@cd $(DOC_DIR) && rm -rf docs/html
	@rm -rf $(BUILD_OBJ)
	@rm -f r-type_server

doc:
	@$(DOXYGEN) $(DOXYFILE) > /dev/null
	@printf "[\033[0;36mdoc built\033[0m]\n"

fclean: clean re
