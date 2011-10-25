VEE_TARGET=student.cc
PYSIDE_UIC=/usr/local/bin/pyside-uic
CC=clang
CXX=clang++

DOXYGEN = doxygen

all: build
.PHONY: all

docs: docs/doxygen.cfg
	$(DOXYGEN) docs/doxygen.cfg
ifeq ($(UNAME),Darwin)
	@open docs/doxygen/html/index.html
endif
.PHONY: docs

docs/doxygen.cfg: docs/Doxyfile.cfg.in
	cat $< | sed \
	  -e 's/@abs_top_srcdir@/./g' \
	  -e 's/@DOT@/dot/g' \
	  -e 's/@PACKAGE_VERSION@/HEAD/' \
	  -e 's/@abs_top_builddir@/./g' > $@

help:
	@echo "all: build run_py_sample"
	@echo "app: Build an OS X .app for the GUI Components"
	@ehco "build: Builds the C++ src"
	@echo "docs: Builds the documentation for the C/C++ and Python Code."
	@echo "clean: removes compile time code"
	@echo "wipe: dist-clean"
	@echo "zip: Create a zip of the current work, for distribution."
	@echo ""
.PHONY: help

%.ui: %.py
	$(PYSIDE_UIC) -o $@ $<

src/vee/configuration_ui.py: resources/configuration_ui.ui
	$(PYSIDE_UIC) -o $@ $<

zip:
	@echo "Creating a zip out of the current directory."
	@tar -czvf vee-contents.tar.gz `find CMakeLists.txt Makefile include src -type f`
.PHONY: zip

gen: src/vee/configuration_ui.py
.PHONY: gen

app: gen src/vee-gui.py
	rm -rf dist/mainwindow.app
	python setup.py py2app
.PHONY: app

build:
	@echo $(CURDIR)
	@mkdir -p ./build
	@mkdir -p ./bin
	cd build && cmake .. -DVEE_SRC_FILE=$(VEE_TARGET)
	cd build && make
.PHONY: build

sample: build
	@nice python src/vee-cmd.py test/test.cfg
.PHONY: sample

gui: gen
	python src/vee-gui.py
.PHONY: gui

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
