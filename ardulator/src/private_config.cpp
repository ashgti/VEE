#include "private_config.h"
#include <iostream>

using namespace std;

int pin_max = 10;
Arduino ardu;

PinLayout::~PinLayout() {
    for (size_t i; i < _mapping.size(); i++) {
        delete _mapping[i];
    }
}

void
PinLayout::addPin(int id, bool digital, bool io, bool interupt) {
    if (_mapping[id]) {
        
    }
    else {
        Pin *new_pin = new Pin(id, digital, io, interupt);
        _mapping[id] = new_pin;
    }
}


Arduino::Arduino() {
    cout << "Setting up the Arduino" << endl;
    
    for (int i; i < 10; i++) {
        pins.addPin(i, true, true, true);
    }
}

void
Arduino::configurePin(int pin_id, int mode) {
    cout << "Configuring Pin " << pin_id << " to mode: " << mode << endl;
}
