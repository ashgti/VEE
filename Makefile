all: build run_py_sample

build:
	@mkdir -p ./build
	@mkdir -p ./bin
	cd build && cmake ..
	cd build && make
.PHONY: build

run_py_sample: build
	@python driver.py

run_hs_driver:
	./driver

hs_driver:
	ghc --make driver.hs

clean:
	cd build && make clean

wipe:
	rm -rf build
