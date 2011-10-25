// Copyright John Harrison, 2011

#include <cstdlib>

#include "arduino.h"
#include "ardulator.h"

using namespace std;

using ardulator::PinConfig;
using std::string;

/* Runs the current simulation. Will return the number of simulated 
 * seconds the simulation ran for.
 */
static double runtime = 0;
static bool initalized = false;

extern void reset_simulator() {
  runtime = 0;
}

extern void step() {
  /// \todo Implement step. To step through an execution of the emulation.
}

extern double run(double length) {
  initalize_simulator();

  return ardu->runScenario(length);
}

extern void initalize_simulator() {
  if (!initalized) {
    ardulator::setupArduino();
    initalized = true;
  }
}

extern bool register_signal(SignalImp* first) {
  initalize_simulator();

  puts("Registering signal...\n");
  printf("Signal ID: %s\n", first->name);
  PinConfig* pin = new PinConfig();
  pin->signal_.current_ = first;
  pin->signal_.head_ = first;
  ardu->unused_pin_config_.insert(make_pair(std::string(first->name), pin));

  return true;
}

