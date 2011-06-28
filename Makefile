all: build run_sample

build:
	@mkdir -p ./build
	@mkdir -p ./bin
	cd build && cmake ..
	cd build && make
.PHONY: build

run_sample: build
	@python stuff.py


clean:
	cd build && make clean

wipe:
	rm -rf build
