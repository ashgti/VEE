#include "ardulator.h"
#include <iostream>
#include <string>

void
delay(unsigned long length) {
    ardu->addTicks(MS2T(length)); 
}

unsigned long 
millis(void) {
    
}

unsigned long
micros(void) {
    
}
