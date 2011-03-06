CC=clang
CXX=clang++
PERL=perl
CFLAGS=-I./include -Wall -g -Wextra
CXXFLAGS=$(CFLAGS)
LD_FLAGS=-lm
OBJ_FILES := src/pin.o src/hardware_serial.o src/pin_commands.o src/timing.o src/main.o src/arduino.o src/rand_num.o src/signal.o src/interrupt_commands.o $(EXT_OBJ_FILES)
SRC_FILES := src/pin.cc src/hardware_serial.cc src/pin_commands.cc src/timing.cc src/main.cc src/arduino.cc src/rand_num.cc src/signal.cc src/interrupt_commands.cc $(EXT_FILES)
LIB_FILE := libardunio-emulation.a

.PHONY: all
all: staticlib test

staticlib: $(LIB_FILE)

sample: $(LIB_FILE) student.o
	$(CXX) $(CXXFLAGS) -v -o $@ student.o $(LIB_FILE) $(LD_FLAGS)

$(LIB_FILE) : $(OBJ_FILES)
	$(AR) cvsrS $(LIB_FILE) $?
	ranlib $(LIB_FILE)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(LIB_FILE) sample
	time ./sample t/simple.scn

t1: $(LIB_FILE) sample
	time ./sample t/one.scn

t2: $(LIB_FILE) sample
	time ./sample t/uni.scn

%.scn: $(LIB_FILE) sample
	@./sample $@

t/*.scn: $(LIB_FILE) sample
	@./sample $@

.PHONY: hw1
hw1: $(LIB_FILE)
	mkdir -p ./homeworks/hw1
	cp ./*.a ./homeworks/hw1
	cp templates/student.cc ./homeworks/hw1/lab1.cc
	cp templates/Makefile ./homeworks/hw1
	cp templates/t1.scn ./homeworks/hw1
	cp templates/t2.scn ./homeworks/hw1
	cp templates/t3.scn ./homeworks/hw1
	cp templates/t4.scn ./homeworks/hw1
	cp templates/t5.scn ./homeworks/hw1
	cp templates/t6.scn ./homeworks/hw1
	cp templates/t7.scn ./homeworks/hw1
	cp templates/t8.scn ./homeworks/hw1
	cp templates/t9.scn ./homeworks/hw1
	cp templates/t10.scn ./homeworks/hw1
	cp templates/t11.scn ./homeworks/hw1
	cp templates/t12.scn ./homeworks/hw1
	cp include/arduino_api.h ./homeworks/hw1
	tar -C ./homeworks -czf hw1.tar.gz ./hw1

.PHONY: clean
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
