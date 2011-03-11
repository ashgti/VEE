CFLAGS=-I. -I./include -Wall -g -Wextra
CXXFLAGS=$(CFLAGS)
LD_FLAGS=-lm
SRC_FILES = lab1.cc
OBJ_FILES = lab1.o
TEST_FILES := $(wildcard *.scn)

all: lab1

lab1: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -v -o $@ $< libardunio-emulation.a $(LD_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

*.scn: lab1
	@./lab1 $@

%.scn: lab1
	@./lab1 $@

test: $(TEST_FILES)

help:
	@echo ""
	@echo "Arduino Emulator Project"
	@echo "  Prof Dr. Biaz"
	@echo "  Embedded Systems"
	@echo ""