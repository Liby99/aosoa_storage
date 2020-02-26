BUILD_DIR := build

all: $(BUILD_DIR)/CMakeCache.txt
	pushd $(BUILD_DIR); make

$(BUILD_DIR)/Makefile: ./cmakelists.txt | $(BUILD_DIR)
	pushd $(BUILD_DIR); cmake -DCMAKE_BUILD_TYPE=Release ..

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

format:
	clang-format -i include/**/*.hpp tests/**/*.cpp