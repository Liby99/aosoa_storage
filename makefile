.PHONY: configure clean format

BUILD_DIR := build

BUILD_TYPE := Debug

ENABLE_OPENMP := On

ENABLE_CUDA := OFF

CUDA_COMPILER := $(HOME)/.local/bin/nvcc_wrapper

ifeq ($(ENABLE_CUDA), On)
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) -D STORAGE_ENABLE_CUDA=On -D STORAGE_ENABLE_OPENMP=Off -D CMAKE_CXX_COMPILER=$(CUDA_COMPILER) ..
else
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) -D STORAGE_ENABLE_CUDA=On -D STORAGE_ENABLE_OPENMP=Off ..
endif

all: $(BUILD_DIR)/Makefile
	@ cd $(BUILD_DIR); make

$(BUILD_DIR)/Makefile: ./CMakeLists.txt | $(BUILD_DIR)
	@ make configure

configure: $(BUILD_DIR)
	@ cd $(BUILD_DIR); $(CMAKE_CMD)

$(BUILD_DIR):
	@ mkdir -p $(BUILD_DIR)

test: all
	@ cd $(BUILD_DIR); make test

clean:
	@ rm -rf $(BUILD_DIR)

format:
	@ clang-format -i include/**/*.hpp tests/**/*.cpp