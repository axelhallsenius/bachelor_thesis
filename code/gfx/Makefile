build: vessel_tracker.cpp
	cmake -S . -B build
	cmake --build build
clean:
	(find ./build/ -mindepth 1 -maxdepth 1 | grep -v vendored) | xargs rm -r 
run: build build/vessel_tracker
	./build/vessel_tracker

.PHONY: all build clean run test
