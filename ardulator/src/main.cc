#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <getopt.h>
#include "ardulator.h"
#include "arduino.h"

using namespace std;

/* Globalally Used Values */
Arduino *ardu = NULL;

/* Special Pins that represent various assembly values */
/* PORTD maps to Arduino digital pins 0 to 7
 *      DDRD - The Port D Data Direction Register - read/write
 *      PORTD - The Port D Data Register - read/write
 *      PIND - The Port D Input Pins Register - read only
 * PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) 
 * map to the crystal pins and are not usable
 *      DDRB - The Port B Data Direction Register - read/write
 *      PORTB - The Port B Data Register - read/write
 *      PINB - The Port B Input Pins Register - read only
 * PORTC maps to Arduino analog pins 0 to 5. Pins 6 & 7 are only accessible on the Arduino Mini
 *      DDRC - The Port C Data Direction Register - read/write
 *      PORTC - The Port C Data Register - read/write
 *      PINC - The Port C Input Pins Register - read only
 */
BitValue PINB(true);
BitValue DDRB;
BitValue PORTB;

BitValue PINC(true);
BitValue DDRC;
BitValue PORTC;

BitValue PIND(true);
BitValue DDRD;
BitValue PORTD;

void
setupArduino() {
    ardu = new Arduino();
}

void
printHelpMenu(string name) {
    cout << "Usage: " << name << " [options] [file ...]" << endl;
    cout << "More usage information\n";
}

bool 
testInputFile(char *name) {
    ifstream ifile(name);
    return ifile;
}

int main(int argc, char** argv) {
    setupArduino();
    pinConfiguration();
    /* Parse the program options */
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            string arg(argv[i]);
            if (arg == "-h" || arg == "--help") {
                printHelpMenu(argv[0]);
                exit(0);
            }
            if (testInputFile(argv[i])) {
                cout << "found input file " << argv[i] << "\n\n";
                ardu->addInputFile(argv[i]);
            }
        }
    }
    
    cout << "Done parsing the scenario configuration file\n\n";
    ardu->runScenario();
    
    cout << "Done running the scenario\n";
    
    ardu->report();
    
    return 0;
}
