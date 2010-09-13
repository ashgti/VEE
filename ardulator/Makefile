# AR=ar
# CC=cc
# CPP=g++
# CFLAGS=-Wall -I.
# CPPFLAGS=$(CFLAGS)
CC=clang
CXX=clang++
CPP=clang++
PERL=perl
CFLAGS=-I./include
CXXFLAGS=$(CFLAGS)
OBJ_FILES := src/hardware_serial.o src/pin_commands.o src/private_config.o src/timing.o src/ardulator.o
SRC_FILES := src/hardware_serial.cpp src/pin_commands.cpp src/private_config.cpp src/timing.cpp src/ardulator.cpp
LIB_FILE := libardunio-emulation.a
# RM=rm -f

all: staticlib

staticlib: $(LIB_FILE)

samples: samples/polling_sensor/polling_sensor

samples/polling_sensor/polling_sensor: $(LIB_FILE) samples/polling_sensor/student.o
	$(CXX) $(CXXFLAGS) -v -o $@ $(LIB_FILE) samples/polling_sensor/student.o

$(LIB_FILE) : $(OBJ_FILES)
	$(AR) cvsrS $(LIB_FILE) $?
	ranlib $(LIB_FILE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(LIB_FILE)
	$(PERL) t/harness.pl

clean: 
	$(RM) $(OBJ_FILES) $(LIB_FILE)

help:
	@echo ""
	@echo "Arduino Emulator Project"
	@echo ""
	@echo "Commands:"
	@echo "  test:          Tests the Arduino Emulator framework."
	@echo "  staticlib:    Builds the link files."
	@echo "  samples:       Builds student object files."
	@echo "  clean:         Removes all old build files."
	@echo "  help:          Prints this help message."
	@echo ""