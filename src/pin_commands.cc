#include "ardulator.h"
#include "arduino.h"
#include <iostream>
#include <string>

using namespace std;

void
pinMode(uint8_t pin_id, uint8_t mode) {
    ardu->configurePin(pin_id, mode);
}

int 
digitalRead(uint8_t pin_id) {
    return ardu->getPin(pin_id) ? HIGH : LOW;
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
