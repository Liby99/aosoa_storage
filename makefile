CURR_DIR = $(shell pwd)

BUILD_DIR := build

BUILD_TYPE := Release

LOWER_BUILD_TYPE = `echo $(BUILD_TYPE) | tr A-Z a-z`

ENABLE_OPENMP := Off

ENABLE_CUDA := Off

# NVIDIA GTX 2080
CUDA_ARCH := TURING75

CUDA_COMPILER := $(shell which nvcc_wrapper)

ifeq ($(ENABLE_CUDA), On)
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
									 -D CMAKE_CXX_COMPILER=$(CUDA_COMPILER) \
									 -D STORAGE_ENABLE_CUDA=On \
									 -D STORAGE_ENABLE_OPENMP=Off \
									 -D Kokkos_ARCH_$(CUDA_ARCH)=On \
									 -D Kokkos_ENABLE_CUDA_LAMBDA=On \
									 $(CURR_DIR)
else
CMAKE_CMD := cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) \
									 -D STORAGE_ENABLE_OPENMP=$(ENABLE_OPENMP) \
									 $(CURR_DIR)
endif

export BUILD_TYPE

entry: all

serial:
	@ make BUILD_DIR=build/$(LOWER_BUILD_TYPE)/serial

configure-serial:
	@ make configure BUILD_DIR=build/$(LOWER_BUILD_TYPE)/serial

openmp:
	@ make ENABLE_OPENMP=On BUILD_DIR=build/$(LOWER_BUILD_TYPE)/openmp

configure-openmp:
	@ make configure ENABLE_OPENMP=On BUILD_DIR=build/$(LOWER_BUILD_TYPE)/openmp

cuda:
	@ make ENABLE_CUDA=On BUILD_DIR=build/$(LOWER_BUILD_TYPE)/cuda

configure-cuda:
	@ make configure ENABLE_CUDA=On BUILD_DIR=build/$(LOWER_BUILD_TYPE)/cuda

all: $(BUILD_DIR)/Makefile
	@ cd $(BUILD_DIR); make

$(BUILD_DIR)/Makefile: ./CMakeLists.txt | $(BUILD_DIR)
	@ make configure

configure: $(BUILD_DIR)
	@ cd $(BUILD_DIR); $(CMAKE_CMD)
.PHONY: configure

$(BUILD_DIR):
	@ mkdir -p $(BUILD_DIR)

test: all
	@ cd $(BUILD_DIR); make test

clean:
	@ rm -rf $(BUILD_DIR)
.PHONY: clean

format:
	@ find include tests bench -iname "*.*pp" -exec clang-format -i {} \;
.PHONY: format