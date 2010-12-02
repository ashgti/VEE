
OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp *.c))

all: $(OBJFILES) arduino-emulation.a
	$(CPP) -o project $(OBJFILES) arduino-emulation.a

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
	@echo "For Dr. Biaz"
	@echo ""