// Copyright John Harrison, 2011

#include <getopt.h>

#include <cstdio>
#include <string>
#include <algorithm>

#include "ardulator.h"
#include "arduino.h"


namespace ardulator {

/** Creates a global instance of the Arduino. **/
void setupArduino() {
    ardu = new ardulator::Ardulator();
}

}  // end namespace ardulator

/** Globalally Used Values **/
ardulator::Ardulator *ardu = NULL;

/** The Port B Data Direction Register - controlls direction of IO for digital pins 8 to 13 **/
ardulator::containers::BitValue DDRB;
/** The Port B Data Register - value registers for IO digital pins 8 to 13 **/
ardulator::containers::BitValue PORTB;

/** The Port C Data Direction Register - controlls direction of IO for analog pins 0 to 5 **/
ardulator::containers::BitValue DDRC;
/** The Port C Data Register - value registers for IO analog pins 0 to 5 **/
ardulator::containers::BitValue PORTC;

/** The Port D Data Direction Register - controlls direction of IO for digital pins 0 to 7 **/
ardulator::containers::BitValue DDRD;
/** The Port D Data Register - value registers for IO digital pins 0 to 7 **/
ardulator::containers::BitValue PORTD;

