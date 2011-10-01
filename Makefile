all: build run_py_sample
.PHONY: all

help:
	@echo "all: build run_py_sample"
	@echo "app: Build an OS X .app for the GUI Components"
	@ehco "build: Builds the C++ src"
	@echo "clean: removes compile time code"
	@echo "wipe: dist-clean"
	@echo ""
.PHONY: help

%.ui: %.py
	pyside-uic -o $@ $<

src/vee/configuration_ui.py: resources/configuration_ui.ui
	pyside-uic -o $@ $<

src/vee/results_ui.py: resources/results_ui.ui
	pyside-uic -o $@ $<

gen: src/vee/configuration_ui.py src/vee/results_ui.py
.PHONY: gen

app: gen src/vee-gui.py
	rm -rf dist/mainwindow.app
	python setup.py py2app
.PHONY: app

build:
	@mkdir -p ./build
	@mkdir -p ./bin
	cd build && cmake ..
	cd build && make
.PHONY: build

run_py_sample: build
	@python src/vee-cmd.py
.PHONY: run_py_sample

clean_python:
	find . -name "*.pyo" -delete
	find . -name "*.pyc" -delete
.PHONY: clean_python

clean: clean_python
	rm -rf dist/mainwindow.app
	cd build && make clean
.PHONY: clean

wipe: clean
	rm -rf dist
	rm -rf build
.PHONY: wipe
