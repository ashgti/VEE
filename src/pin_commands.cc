#include "ardulator.h"
#include "arduino.h"
#include <iostream>
#include <string>

using namespace std;

void
registerPin(const char* signal_id, uint8_t pin_id) {
    ardu->addPin(string(signal_id), pin_id);
}

void
registerSerial(const char* signal_id, HardwareSerial *serial) {
    ardu->addSerial(string(signal_id), serial);
}

void
pinMode(uint8_t pin_id, uint8_t mode) {
    ardu->configurePin(pin_id, mode);
}

int 
digitalRead(uint8_t pin_id) {
    return ardu->getPin(pin_id) ? HIGH : LOW;
}

void 
processSignal(const char* signal_id) {
    ardu->dispatchSignal(signal_id);
}

void
digitalWrite(uint8_t pin_id, uint8_t value) {
    ardu->setPin(pin_id, value);
}

int
analogRead(uint8_t pin_id) {
    return ardu->getPin(pin_id);
}

void
analogWrite(uint8_t pin_id, uint8_t value) {
    ardu->setPin(pin_id, value);
}    
