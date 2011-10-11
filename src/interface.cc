#include <cstdlib>
#include <iostream>
#include "arduino.h"
#include "ardulator.h"

using namespace std;

using ardulator::PinConfig;

/* Runs the current simulation. Will return the number of simulated 
 * seconds the simulation ran for.
 */
static int val = 0;
static double runtime = 0;
static bool initalized = false;

/**
 * Reset the simulation.
 */
extern void reset_simulator() {
  runtime = 0;
}

extern void step() {
  // TODO: implement step
}

extern double run(double length) {
  initalize_simulator();

  return ardu->runScenario(length);
}

/** 
 * initalize_simulator setups the basic simulator, needs to be called
 * at least once.
 */
extern void initalize_simulator() {
  if (!initalized) {
    ardulator::setupArduino();
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
extern "C" bool register_signal(int pin_id, SignalImp* first) {
  initalize_simulator();
  
  cout << "Registering signal...\n";
  cout << "Pin: " << pin_id << endl;
  PinConfig* pin = new PinConfig();
  pin->signal_.current_ = first;
  pin->signal_.head_ = first;
  ardu->pin_config_.insert(make_pair(pin_id, pin));
  
  SignalImp* p = first;
  int c = 0;
  while (p) {
    c++;
    p = p->next;
  }
  
  return true;
}

