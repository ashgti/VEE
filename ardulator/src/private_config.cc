#include <iostream>
#include <fstream>
#include <map>
// #include <vector>
#include <time.h>
#include "private_config.h"

using namespace std;

void
setupComponents() {
    // ardu->addEventHandler(SerialEvt::id(), &SerialEvt::evtHandler);
    // ardu->addEventHandler(PinEvt::id(), &PinEvt::evtHandler);
    // ardu->addEventHandler(DetEvt::id(), &DetEvt::evtHandler);
    // ardu->addEventHandler(UniEvt::id(), &UniEvt::evtHandler);
    // ardu->addEventHandler(ExpEvt::id(), &ExpEvt::evtHandler);
}


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
