#include "ardulator.h"
#include "arduino.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <getopt.h>

namespace ardulator {

void setupArduino() {
    ardu = new ardulator::Ardulator();
}

} // end namespace ardulator

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
ardulator::BitValue DDRB;
ardulator::BitValue PORTB;

ardulator::BitValue DDRC;
ardulator::BitValue PORTC;

ardulator::BitValue DDRD;
ardulator::BitValue PORTD;

