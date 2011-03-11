#include "ardulator.h"
#include <iostream>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif 

void
attachInterrupt(uint8_t pin_id, void (*fn)(void), int mode) {
    ardu->registerInterrupt(pin_id, fn, mode);
}

void
detachInterrupt(uint8_t pin_id) {
    ardu->dropInterrupt(pin_id);
}

#ifdef __cplusplus
} /* end excern "C" */
#endif