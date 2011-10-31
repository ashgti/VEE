// Copyright John Harrison, 2011

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cmath>
#include <cassert>
#include <iomanip>
#include <cstdio>

#include "ardulator.h"
#include "arduino.h"

using ::std::string;
using ::std::map;
using ::std::vector;
using ::std::fprintf;
using ::std::make_pair;

extern "C" void loop() __attribute__((weak));
extern "C" void setup() __attribute__((weak));

namespace ardulator {

using ::ardulator::containers::Clock;

class EmulatorFinished {
};

static EmulatorFinished ef;

static ProcessingSignalException p;

Ardulator::Ardulator() : runtime_(0.0), ticks_(0), total_ticks_(0),
                         registered_identifers_(""), scenario_length_(0, 0),
                         inside_interrupt_handler_(false), prepared_(false),
                         timer_(0, 0), interrupts_enabled_(false) {
  struct stat buffer;
  int status = stat("./logs", &buffer);

  if (status != 0 && errno == ENOENT) {
    if (mkdir("./logs", 0777)) {
      perror("./logs");
    }
  }

  log_ = fopen("./logs/dispatch.log", "w+");
  debug_ = fopen("./logs/debug.log",  "w+");
}

Ardulator::~Ardulator() {
  if (log_ != NULL) {
    fprintf(log_, "\n\n");
    fclose(log_);
  }
  if (debug_ != NULL) {
    fprintf(debug_, "\n\n");
    fclose(debug_);
  }
}

void Ardulator::configurePin(uint8_t pin_id, uint8_t mode) {
  PinConfigIterator it = pin_config_.find(pin_id);

  if (it != pin_config_.end()) {
    pin_config_[pin_id]->mode_ = mode;
    fprintf(debug_, "Configuring Pin %d to mode: %d\n", pin_id, mode);
  } else {
    fprintf(debug_, "Error, pin configured that is not registed\n");
  }
}

/**
 * 
 */
void Ardulator::prepareScenario() {
  if (prepared_) {
    return;
  } else {
    // Update the secnario length.
    timer_.seconds_ = 0;
    timer_.ticks_   = 0;
    // Run pinConfiguration from the Students to setup the pins the students
    // are going to need access to.
    pinConfiguration();
    setup();
    updatePinMaps();

    prepared_ = true;
  }
}

/**
 * Run the scenario for a given amount of time.
 */
double Ardulator::runScenario(double length) {
  prepareScenario();

  scenario_length_.seconds_ = static_cast<int>(length);
  scenario_length_.ticks_ = (length - floor(length)) * TICKS_PER_SECOND;

  printf("Running for: %f\n", length);

  fprintf(debug_, "Attempting to run the Scenario\n\n");

  fprintf(debug_, "Scenario Length: %s\n", scenario_length_.str().c_str());
  fprintf(debug_, "Runtime Timer: %s\n", timer_.str().c_str());

  try {
    updatePinState();
  }
  catch(const EmulatorFinished &e) {
  }
  while (runtime_ < length) {
    try {
      loop();
      addTicks(LOOP_CONST);
      updatePinState();
    }
    catch(const EmulatorFinished &e) {
      break;
    }
  }
  finalizePinState();
  fprintf(debug_, "Scenario Length: %s\n", scenario_length_.str().c_str());
  fprintf(debug_, "Runtime Timer: %s\n", timer_.str().c_str());

  printf("--------------------------\n");
  printf("Runtime: %f\n", runtime_);

  return runtime_;
}

/**
 * addTicks is used to increment the interanl clock and increment the total
 * runtime of the emulation.
 */
void Ardulator::addTicks(uint64_t length) {
  timer_.ticks_ += length;
  if (timer_.ticks_ > TICKS_PER_SECOND) {
    timer_.seconds_++;
    timer_.ticks_ -= TICKS_PER_SECOND;
  }

  runtime_ = timer_.seconds_ + (timer_.ticks_ / TICKS_PER_SECOND);
}

/**
 * Update the pins so they are connected to the various memory registers.
 */
void Ardulator::updatePinMaps() {
  for (PinConfigIterator it = pin_config_.begin();
       it != pin_config_.end();
       ++it) {
    if (it->first >= 0 || it->first < 8) {
      it->second->bit_mask_ = 1 << it->first;
      it->second->bit_container_ = PORTD.getStateRef();
    } else if (it->first >= 8 || it->first < 14) {
      it->second->bit_mask_ = 1 << (8 - it->first);
      it->second->bit_container_ = PORTB.getStateRef();
    } else if (it->first >= 16 || it->first < 22) {
      it->second->bit_mask_ = 1 << (16 - it->first);
      it->second->bit_container_ = PORTC.getStateRef();
    } else {
      throw "Error this pin is out of bounds.";
    }
  }
}

/**
 * Updates the state of each pin in the internal pin registery.
**/
void Ardulator::updatePinState() {
  // Report Changes in State
  inside_interrupt_handler_ = true;

  PinConfigIterator it;
  for (it = pin_config_.begin();
       it != pin_config_.end();
       it++) {
    it->second->setState(timer_);
  }

  UnusedPinConfigIterator vit;
  for (vit = unused_pin_config_.begin();
       vit != unused_pin_config_.end();
       vit++) {
    vit->second->setState(timer_);
  }

  inside_interrupt_handler_ = false;

  if ((timer_.seconds_ > scenario_length_.seconds_) ||
        (timer_.seconds_ == scenario_length_.seconds_ &&
         timer_.ticks_ >= scenario_length_.ticks_)) {
    fprintf(stdout, "Scenario Length: %s\n", scenario_length_.str().c_str());
    fprintf(stdout, "Runtime Timer: %s\n", timer_.str().c_str());
    throw ef;
  }
}

/**
 * For each pin, finalize the state of the pin.
 */
void Ardulator::finalizePinState() {
  // Report Changes in State
  PinConfigIterator it;
  for (it = pin_config_.begin(); it != pin_config_.end(); it++) {
    it->second->setState(timer_);
  }
}

/**
 * Returns current time as a double instead of the internal time structure
 * format.
 */
double Ardulator::now() {
  return timer_.now();
}

/**
 * Get the value of a given pin and increment the clock based off the kind of
 * access.
 * Analog Reads take 100 ms
 * Digital Reads 58 cycles
 */
int Ardulator::getPin(uint8_t pin_id) {
  if (pin_config_.find(pin_id) == pin_config_.end()) {
    return LOW;
  } else {
    if (pin_config_[pin_id]->signal_.current_->type == VT_SERIAL) {
      return LOW;
    } else if (pin_config_[pin_id]->signal_.current_->type == VT_DIGITAL) {
      addTicks(58);
      return pin_config_[pin_id]->signal_.current_->value.digital;
    } else if (pin_config_[pin_id]->signal_.current_->type == VT_ANALOG) {
      addTicks(MS2T(100));
      return pin_config_[pin_id]->signal_.current_->value.analog;
    } else {
      return LOW;
    }
  }
}

void Ardulator::setPin(uint8_t pin_id, uint8_t val) {
  if (pin_config_[pin_id]->state_ != val) {
    pin_config_[pin_id]->state_ = val;
  }
}

void Ardulator::dispatchSignal(const char *signal_id) {
#ifdef NOISY_WARNINGS
  if (_inside_interrupt_handler) {
    fprintf(debug_,
            "Error, processingSignal %d while within an interrupt handler is "
            "not allowed.\n",
            signal_id);
    exit(1);
  }
#endif /* end NOISY_WARNINGS */
  if (signal_names_.find(signal_id) == signal_names_.end()) {
    return;
  }
  if (pin_config_.find(signal_names_[signal_id]) == pin_config_.end()) {
    return;
  }

  size_t id = signal_names_[signal_id];
  double processing_time = pin_config_[id]->signal_.current_->duration;
  int wait_till = 0;

  wait_till = static_cast<int>(processing_time * TICKS_PER_SECOND);

  fprintf(log_, "Dispatching signal %s for %d a %d\n",
                signal_id,
                signal_names_[signal_id],
                wait_till);

  while (wait_till) {
     wait_till--;
     addTicks(1);
     updatePinState();
  }
}

string Ardulator::timestamp() {
  char result[100];
  snprintf(result, 100, "%ds %dt", timer_.seconds_, timer_.ticks_);
  return string(result);
}

void Ardulator::addPin(string signal_name, uint8_t pin_id) {
  printf("Adding pin %s %d\n", signal_name.c_str(), pin_id);
  UnusedPinConfigIterator it = unused_pin_config_.find(signal_name);

  if (it != unused_pin_config_.end()) {
    printf("Moving pointer\n");
    it->second->pid_ = pin_id;
    pin_config_[pin_id] = it->second;
    unused_pin_config_.erase(it);
  }

  signal_names_[signal_name] = pin_id;
}

void Ardulator::addSerial(string signal_name, const HardwareSerial &serial) {
  signal_names_[signal_name] = serial.pin();
}

/* Reporting facilities */
void Ardulator::report() {
  PinConfigIterator it;
  for (it = pin_config_.begin(); it != pin_config_.end(); it++) {
    // it->second->report(true);
  }

  // vector<Signal*>::iterator vit;
  UnusedPinConfigIterator vit;
  for (vit = unused_pin_config_.begin();
       vit != unused_pin_config_.end();
       vit++) {
    // (*vit)->report(false);
  }
}

/**
 * Registers an interrupt handler.
 * @param pin_id interrupt id.
 * @param fn     function pointer to the interrupt handler.
 * @param mode   interrupt mode, valid choices, LOW, CHANGE, RISING, FALLING
 */
void Ardulator::registerInterrupt(uint8_t pin_id,
                                  void (*fn)(void),
                                  uint8_t mode) {
  printf("Registering interrupt %d\n", pin_id);
  interrupts_[pin_id] = make_pair(mode, fn);
  pin_config_[pin_id]->interrupt_ = true;
}

/**
 * Removes an interrupt handler.
 * @param pin_id the interrupt to remove.
 */
void Ardulator::dropInterrupt(uint8_t pin_id) {
  interrupts_.erase(pin_id);
  pin_config_[pin_id]->interrupt_ = false;
  printf("Dropped int on %d\n", pin_id);
}

}  // END namespace ardulator

// vim: sw=2:sts=2:expandtab
