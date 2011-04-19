CFLAGS=-I. -I./include -Wall -g -Wextra
CXXFLAGS=$(CFLAGS)
LD_FLAGS=-lm
SRC_FILES = lab3.cc
OBJ_FILES = lab3.o
TEST_FILES := $(wildcard *.scn)

all: lab3

lab3: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -v -o $@ $< libardunio-emulation.a $(LD_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

*.scn: lab3
	@./lab3 $@

%.scn: lab3
	@./lab3 $@

test: $(TEST_FILES)

help:
	@echo ""
	@echo "Arduino Emulator Project"
	@echo "  Prof Dr. Biaz"
	@echo "  Embedded Systems"
	@echo ""