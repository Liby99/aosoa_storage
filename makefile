CURR_DIR = $(shell pwd)

BUILD_DIR := build

BUILD_TYPE := Debug

ENABLE_OPENMP := On

ENABLE_CUDA := OFF

# NVIDIA GTX 2080
CUDA_ARCH := TURING75

CUDA_COMPILER := $(shell which nvcc_wrapper)

# CMake command
ifeq ($(ENABLE_CUDA), On)
# If cuda enabled, then add all cuda API
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
									 -D CMAKE_CXX_COMPILER=$(CUDA_COMPILER) \
									 -D STORAGE_ENABLE_CUDA=On \
									 -D STORAGE_ENABLE_OPENMP=Off \
									 -D Kokkos_ARCH_$(CUDA_ARCH)=On \
									 $(CURR_DIR)
else
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) $(CURR_DIR)
endif

entry: all

all: $(BUILD_DIR)/Makefile
	@ cd $(BUILD_DIR); make

$(BUILD_DIR)/Makefile: ./CMakeLists.txt | $(BUILD_DIR)
	@ make configure

# `make configure`
# Run cmake on the build directory
configure: $(BUILD_DIR)
	@ cd $(BUILD_DIR); $(CMAKE_CMD)
.PHONY: configure

# Create the build directory
$(BUILD_DIR):
	@ mkdir -p $(BUILD_DIR)

# `make test`
# Run all the test in the build directory
test: all
	@ cd $(BUILD_DIR); make test

# `make clean`
# Clean the build directory specified by you
clean:
	@ rm -rf $(BUILD_DIR)
.PHONY: clean

# `make format`
# Format all the files
format:
	@ find include tests bench -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -i {} \;
.PHONY: format