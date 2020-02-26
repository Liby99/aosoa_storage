all:
	mkdir -p build
	pushd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make

format:
	clang-format -i include/**/*.hpp tests/**/*.cpp