all: # Build with tests (default)
	mkdir -p build
	cd build
	cmake .. -DCMAKE_BUILD_TYPE=Release
	cmake --build . --config Debug

run: # Build without tests
	cmake -B build -DBUILD_TESTS=OFF
	cmake --build build

clean:
	(find ./build/ -mindepth 1 -maxdepth 1 | grep -v deps) | xargs rm -r 

deep-clean:
	rm -rf ./build/*
