.PHONY: configure clean format

BUILD_DIR := build

BUILD_TYPE := Debug

ENABLE_OPENMP := On

ENABLE_CUDA := OFF

CUDA_COMPILER := $(HOME)/.local/bin/nvcc_wrapper

all: $(BUILD_DIR)/Makefile
	@ cd $(BUILD_DIR); make

$(BUILD_DIR)/Makefile: ./CMakeLists.txt | $(BUILD_DIR)
	@ make configure

configure: $(BUILD_DIR)
	@ cd $(BUILD_DIR); \
	cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
			-DSTORAGE_ENABLE_CUDA=$(ENABLE_CUDA) \
			-DSTORAGE_ENABLE_OPENMP=$(ENABLE_OPENMP) \
			-DCUDA_COMPILER=$(CUDA_COMPILER)
			..

$(BUILD_DIR):
	@ mkdir -p $(BUILD_DIR)

test: all
	@ cd $(BUILD_DIR); make test

clean:
	@ rm -rf $(BUILD_DIR)

format:
	@ clang-format -i include/**/*.hpp tests/**/*.cpp