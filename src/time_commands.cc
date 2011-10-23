// Copyright John Harrison, 2011

#include <string>

#include "ardulator.h"

#ifdef __cplusplus
extern "C" {
#endif

 void
delay(unsigned long length) {
    ardu->addTicks(MS2T(length)); 
}

extern "C" unsigned long 
millis(void) {
    return static_cast<unsigned long>(((ardu->timer_.seconds_ * TICKS_PER_SECOND) + ardu->timer_.ticks_)/1000);
}

extern "C" unsigned long
micros(void) {
    return static_cast<unsigned long>((ardu->timer_.seconds_ * TICKS_PER_SECOND) + ardu->timer_.ticks_);
}

#ifdef __cplusplus
}  // end extern "C"
#endif
