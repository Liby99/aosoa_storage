.PHONY: configure clean format

BUILD_DIR := build

BUILD_TYPE := Debug

all: $(BUILD_DIR)/Makefile
	pushd $(BUILD_DIR); make

$(BUILD_DIR)/Makefile: ./cmakelists.txt | $(BUILD_DIR)
	make configure

configure: $(BUILD_DIR)
	pushd $(BUILD_DIR); cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: all
	pushd $(BUILD_DIR); make test

clean:
	rm -rf $(BUILD_DIR)

format:
	clang-format -i include/**/*.hpp tests/**/*.cpp