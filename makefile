.PHONY: configure clean format

BUILD_DIR := build

BUILD_TYPE := Debug

ENABLE_OPENMP := On

ENABLE_CUDA := OFF

# NVIDIA GTX 2080
CUDA_ARCH := TURING75

CUDA_COMPILER := $(HOME)/.local/bin/nvcc_wrapper

ifeq ($(ENABLE_CUDA), On)
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
									 -D CMAKE_CXX_COMPILER=$(CUDA_COMPILER) \
									 -D STORAGE_ENABLE_CUDA=On \
									 -D STORAGE_ENABLE_OPENMP=Off \
									 -D Kokkos_ARCH_$(CUDA_ARCH)=On \
									 ..
else
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
									 -D STORAGE_ENABLE_CUDA=Off \
									 -D STORAGE_ENABLE_OPENMP=On \
									 ..
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
	@ find include tests bench -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -i {} \;