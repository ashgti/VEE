#include "arduino_api.h"
#include "private_config.h"
#include <iostream>
#include <string>

using namespace std;

enum component_types { COMPONENT_PIN, COMPONENT_SERIAL };

bool registered_model = false;

void
checkRegistered() throw(ArduException) {
    if (registered_model == false) {
        throw ArduException("regsiter arduino model with setModel.");
    }
}

void
setModel(ArduinoModel arduino_model) {
    if (registered_model == false) {
        ardu = new Arduino(arduino_model);
        registered_model = true;
    }
    else {
        throw ArduException("setModel called twice, should only be called once.");
    }
}

void
registerPin(const char* signal_id, uint8_t pin_id) {
    string foo(signal_id);
    cout << "registering pin..." << foo << "\n";
    ardu->_signal_map[foo] = pin_id;
}

void
pinMode(uint8_t pin_id, uint8_t mode) {
    ardu->configurePin(pin_id, mode);
}

void
digitalWrite(uint8_t pin_id, uint8_t value) {
    ardu->setPin(pin_id, value);
}


