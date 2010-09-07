CC=gcc
CPP=g++
CFLAGS=-Wall -I.
CPPFLAGS=$(CFLAGS)
LD=ld

OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp *.c))

all: $(OBJFILES)
	$(CPP) -o project $(OBJFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $<

test: all
	./project -h
	./project --help
	./project -r
	./project --realtime

help:
	@echo ""
	@echo "Arduino Emulator Project"
	@echo ""
	@echo "Commands:"
	@echo "  test:          Tests the Arduino Emulator framework."
	@echo "  student:       Builds student object files."
	@echo "  help:          Prints this help message."
	@echo ""