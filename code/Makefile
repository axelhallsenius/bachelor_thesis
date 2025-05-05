.PHONY: all vessel_tracker

all:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && \
	cmake --build build --target vessel_tracker && \
	LD_PRELOAD=$$(gcc -print-file-name=libasan.so) \
	./build/Debug/vessel_tracker


vessel_tracker:
	cmake --build build --target vessel_tracker

clean:
	(find ./build/ -mindepth 1 -maxdepth 1 | grep -v deps) | xargs rm -r 

deep-clean:
	rm -rf ./build/*

.PHONY: all run clean deep-clean
