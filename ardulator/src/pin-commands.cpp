#include "problem.h"
#include "private_config.h"
#include <iostream>

using namespace std;

void
pinMode(uint8_t pin_id, uint8_t mode) {
    ardu.configurePin(pin_id, mode);
}

void
digitalWrite(uint8_t pin_id, uint8_t value) {
    
}


