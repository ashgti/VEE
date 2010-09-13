#include <iostream>
#include <fstream>
#include "private_config.h"

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

// validate_input() {
//     
// }

bool 
Arduino::add_input_file(char *name) {
    ifstream ifile;
    
    ifile.open(name, ifstream::in);
    
    while (ifile.good()) {
        char line[1024];
        ifile.getline(line, 1024);
        cout << line << endl;
    }
    
    cout << "Stuff" << endl;
    
    ifile.close();
    
    return true;
}

