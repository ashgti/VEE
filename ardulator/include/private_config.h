#ifndef PRIVATE_CONFIG_H
#define PRIVATE_CONFIG_H

#include "arduino_api.h"
#include <map>

using namespace std;

struct Pin {
    Pin(int id, bool digital, bool io, bool interupt) 
            : id(id), digital(digital), io(io), interupt(interupt) { 
        configured = false;
        val = 0;
    }
    int id;
    bool digital;
    bool io;
    bool interupt;
    bool configured;
    int val;
};

class PinLayout {
    int _digital_pins;
    int _analog_pins;
    map<int, Pin*> _mapping;

  public:
    void addPin(int id, bool digital, bool io, bool interupt);
    ~PinLayout();
};

class Arduino {
    bool flag;
    int max_pins;
    PinLayout pins;
  public:
    Arduino();
    void configurePin(int id, int mode);
    bool add_input_file(char *name);
};

extern Arduino ardu;

#endif /* PRIVATE_CONFIG_H */
