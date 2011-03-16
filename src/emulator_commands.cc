#include "ardulator.h"
#include "arduino.h"
#include <iostream>
#include <string>

using namespace std;

void 
processSignal(const char* signal_id) {
    ardu->dispatchSignal(signal_id);
}

void
registerPin(const char* signal_id, uint8_t pin_id) {
    ardu->addPin(string(signal_id), pin_id);
}

void
registerSerial(const char* signal_id, HardwareSerial *serial) {
    ardu->addSerial(string(signal_id), serial);
}