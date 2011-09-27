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
static bool initalized = false;

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

/** 
 * initalize_simulator setups the basic simulator, needs to be called
 * at least once.
 */
extern void initalize_simulator() {
    if (!initalized) {
        setupArduino();
        initalized = true;
    }
}

/**
 * Registers the pins to the current simulator.
 *
 * @param pin_id the pin's id for the current signals
 * @param signal_count the number of signal changes
 * @param signals the time at which the signal changes, all signals start
 *        at 0 and go high afterwards. 
 */
extern bool register_signal(int pin_id, int signal_count, double* signals) {
    cout << "Registering signal...\n";
    cout << "Pin: " << pin_id << endl;
    cout << "Signal Count: " << signal_count << endl;
    if (signal_count > 4) {
        cout << "Signals... " << signals[signal_count - 3] << endl;
    }
    return true;
}

