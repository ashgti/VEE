CFLAGS=-I. -I./include -Wall -g -Wextra
CXXFLAGS=$(CFLAGS)
LD_FLAGS=-lm
SRC_FILES = lab2.cc
OBJ_FILES = lab2.o
TEST_FILES := $(wildcard *.scn)

all: lab2

lab2: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -v -o $@ $< libardunio-emulation.a $(LD_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

*.scn: lab2
	@./lab2 $@

%.scn: lab2
	@./lab2 $@

test: $(TEST_FILES)

help:
	@echo ""
	@echo "Arduino Emulator Project"
	@echo "  Prof Dr. Biaz"
	@echo "  Embedded Systems"
	@echo ""