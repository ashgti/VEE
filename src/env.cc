// Copyright John Harrison, 2011

#include <getopt.h>

#include <cstdio>
#include <string>
#include <algorithm>

#include "ardulator.h"
#include "arduino.h"


namespace ardulator {

void setupArduino() {
    ardu = new ardulator::Ardulator();
}

}  // end namespace ardulator

/* Globalally Used Values */
ardulator::Ardulator *ardu = NULL;

/* Special Pins that represent various assembly values */
/* PORTD maps to Arduino digital pins 0 to 7
 *      DDRD - The Port D Data Direction Register - read/write
 *      PORTD - The Port D Data Register - read/write
 * PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) 
 * map to the crystal pins and are not usable
 *      DDRB - The Port B Data Direction Register - read/write
 *      PORTB - The Port B Data Register - read/write
 * PORTC maps to Arduino analog pins 0 to 5. Pins 6 & 7 are only accessible on the Arduino Mini
 *      DDRC - The Port C Data Direction Register - read/write
 *      PORTC - The Port C Data Register - read/write
 */
ardulator::containers::BitValue DDRB;
ardulator::containers::BitValue PORTB;

ardulator::containers::BitValue DDRC;
ardulator::containers::BitValue PORTC;

ardulator::containers::BitValue DDRD;
ardulator::containers::BitValue PORTD;

