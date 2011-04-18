#include "ardulator.h"
#include <iostream>
#include <string>

extern "C" void
delay(unsigned long length) {
    ardu->addTicks(MS2T(length)); 
}

extern "C" unsigned long 
millis(void) {
    return static_cast<unsigned long>(((ardu->_timer._seconds * TICKS_PER_SECOND) + ardu->_timer._ticks)/1000);
}

extern "C" unsigned long
micros(void) {
    return static_cast<unsigned long>((ardu->_timer._seconds * TICKS_PER_SECOND) + ardu->_timer._ticks);
}
