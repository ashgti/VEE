#include <cstdlib>
#include "arduino.h"


/* Runs the current simulation. Will return the number of simulated 
 * seconds the simulation ran for.
 */
extern "C" int run() {
    return 3;
}

extern "C" int initalize_simulator() {
    return 45;
}

extern "C" int create_signal() {
    return 42;
}

