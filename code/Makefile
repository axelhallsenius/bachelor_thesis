all:
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTS=ON \
		-DBUILD_BENCHMARKS=OFF &&\
	cmake --build build --config Debug --target vessel_tracker && \
	LD_PRELOAD=$$(gcc -print-file-name=libasan.so) \
	./build/Debug/vessel_tracker

test:
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTS=ON \
		-DBUILD_BENCHMARKS=OFF &&\
	cmake --build build --config Debug --target unit_tests && \
	LD_PRELOAD=$$(gcc -print-file-name=libasan.so) \
	./build/Debug/unit_tests 
 
bench:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
	  -DBUILD_TESTS=OFF -DBUILD_BENCHMARKS=ON && \
	cmake --build build --config Release --target benchmark_suite && \
	./build/Release/benchmark_suite

clean:
	rm -rf ./build/*

.PHONY: all run clean test debug bench release_bench
