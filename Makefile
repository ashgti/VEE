all: build run_py_sample

help:
	@echo "Hi"

app: docs/basic_ui.ui docs/results_ui.ui mainwindow.py
	rm -rf dist/mainwindow.app
	pyside-uic docs/basic_ui.ui -o basic_ui.py
	pyside-uic docs/results_ui.ui -o results_ui.py
	python setup.py py2app
.PHONY: app

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
