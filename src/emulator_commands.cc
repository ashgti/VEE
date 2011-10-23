// Copyright John Harrison, 2011

#include <string>

#include "ardulator.h"
#include "arduino.h"

using ::std::string;

void processSignal(const char* signal_id) {
    ardu->dispatchSignal(signal_id);
}

void registerPin(const char* signal_id, uint8_t pin_id) {
    ardu->addPin(string(signal_id), pin_id);
}

void registerSerial(const char* signal_id, const HardwareSerial &serial) {
    ardu->addSerial(string(signal_id), serial);
}

