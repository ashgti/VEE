CC=clang
CXX=clang++
CPP=clang++
PERL=perl
CFLAGS=-I./include -Wall -g -Wextra
CXXFLAGS=$(CFLAGS)
LD_FLAGS=-lm
OBJ_FILES := src/pin.o src/hardware_serial.o src/pin_commands.o src/private_config.o src/timing.o src/main.o src/arduino.o src/input_types.o src/rand_num.o $(EXT_OBJ_FILES)
SRC_FILES := src/pin.cc src/hardware_serial.cc src/pin_commands.cc src/private_config.cc src/timing.cc src/main.cc src/arduino.cc src/input_types.cc src/rand_num.cc $(EXT_FILES)
LIB_FILE := libardunio-emulation.a

all: staticlib test

staticlib: $(LIB_FILE)

samples: samples/polling_sensor/polling_sensor

samples/polling_sensor/polling_sensor: $(LIB_FILE) samples/polling_sensor/student.o
	$(CXX) $(CXXFLAGS) -v -o $@ $(LIB_FILE) samples/polling_sensor/student.o $(LD_FLAGS)

$(LIB_FILE) : $(OBJ_FILES)
	$(AR) cvsrS $(LIB_FILE) $?
	ranlib $(LIB_FILE)

samples/polling_sensor/student.o: samples/polling_sensor/student.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(LIB_FILE) samples
	# $(PERL) t/harness.pl
	time samples/polling_sensor/polling_sensor t/samples/polling_sensor/simple.scn

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
