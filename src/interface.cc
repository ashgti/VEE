#include <cstdlib>
#include <iostream>
#include "arduino.h"
#include "ardulator.h"

using namespace std;

/* Runs the current simulation. Will return the number of simulated 
 * seconds the simulation ran for.
 */
static int val = 0;
static double runtime = 0;

extern void reset_simulator() {
    runtime = 0;
}

extern void step() {
    // TODO: implement step
}

extern double run(double length) {
    runtime += length;
    return runtime;
}

extern void initalize_simulator() {
    setupArduino();
}

extern bool register_signal(int pin_id, int signal_count, int* signals) {
    cout << "Registering signal...\n";
    cout << "Pin: " << pin_id << endl;
    cout << "Signal Count: " << signal_count << endl;
    if (signal_count > 4) {
        cout << "Signals... " << signals[signal_count - 3] << endl;
    }
    return true;
}

