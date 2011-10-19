// vim: sw=2:sts=2:expandtab

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <cstdio>
#include <iostream>

#include "ardulator.h"
#include "arduino.h"

using ::std::string;
using ::std::cout;
using ::std::cerr;
using ::std::endl;
using ::std::setw;
using ::std::setfill;
using ::std::ifstream;
using ::std::fstream;
using ::std::map;
using ::std::vector;
using ::std::istringstream;
using ::std::ostringstream;
using ::std::make_pair;

extern "C" void loop() __attribute__((weak));
extern "C" void setup() __attribute__((weak));

namespace ardulator {

class EmulatorFinished {
} ef;

ProcessingSignalException p;

static void print_clock(string n, ArdulatorClock t){
  ardu->debug_ << n << " " << t.seconds_ << "." << setw(FIELD_WIDTH)
               << setfill('0') << t.ticks_ << std::endl;
}

Ardulator::Ardulator() : runtime_(0.0), ticks_(0), total_ticks_(0),
                         registered_identifers_(""), scenario_length_(0, 0),
                         inside_interrupt_handler_(false), prepared_(false),
                         timer_(0, 0), interrupts_(false) {
  log_.exceptions ( ifstream::failbit | ifstream::badbit );
  debug_.exceptions ( ifstream::failbit | ifstream::badbit );

  struct stat buffer;
  int status = stat("./logs", &buffer);

  if (status != 0 && errno == ENOENT) {
    if (mkdir("./logs", 0777)) {
      perror("./logs");
    }
  }

  log_.open("./logs/dispatch.log", fstream::out | fstream::trunc);
  debug_.open("./logs/debug.log",  fstream::out | fstream::trunc);
}

Ardulator::~Ardulator() {
  if (log_.is_open()) {
    log_ << "\n\n";
    log_.flush();
    log_.close();
  }
  if (debug_.is_open()) {
    debug_ << "\n\n";
    debug_.flush();
    debug_.close();
  }

  // for (PinConfigIterator it = pin_config_.begin();
  //      it != pin_config_.end();
  //      it++) {
  //   delete it->second;
  // }
}

void Ardulator::configurePin(uint8_t pin_id, uint8_t mode) {
  PinConfigIterator it = pin_config_.find(pin_id);

  if (it != pin_config_.end()) {
    pin_config_[pin_id]->mode_ = mode;
    debug_ << "Configuring Pin " << (int)pin_id << " to mode: "
           << (int)mode <<  "\n";
  } else {
    debug_ << "Error, pin configured that is not registed\n";
  }
}

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

double Ardulator::runScenario(double length) {
  prepareScenario();

  scenario_length_.seconds_ = (int)length;
  scenario_length_.ticks_ = (length - floor(length)) * TICKS_PER_SECOND;

  cout << "Running for: " << length << endl;

  debug_ << "Attempting to run the Scenario\n\n";

  print_clock("Scenario Length: ", scenario_length_);
  print_clock("Runtime Timer:", timer_);

  try {
    updatePinState();
  }
  catch (EmulatorFinished &e) {
  }
  while (runtime_ < length) {
    try {
      loop();
      addTicks(LOOP_CONST);
      updatePinState();
    }
    catch (EmulatorFinished &e) {
      break;
    }
  }
  finalizePinState();
  print_clock("Scenario Length: ", scenario_length_);
  print_clock("Runtime Timer:", timer_);

  cout << "--------------------------\n";

  cout << "Runtime: " << runtime_ << endl;

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


void Ardulator::updatePinMaps() {
  for (PinConfigIterator it = pin_config_.begin();
       it != pin_config_.end();
       ++it) {
    if (it->first >= 0 || it->first < 8) {
      it->second->bit_mask_ = 1 << it->first;
      it->second->bit_container_ = PORTD.getStateRef();
    }
    else if (it->first >= 8 || it->first < 14) {
      it->second->bit_mask_ = 1 << (8 - it->first);
      it->second->bit_container_ = PORTB.getStateRef();
    }
    else if (it->first >= 16 || it->first < 22) {
      it->second->bit_mask_ = 1 << (16 - it->first);
      it->second->bit_container_ = PORTC.getStateRef();
    }
    else {
      throw "Error this pin is out of bounds.";
    }
  }
}

// TODO(ashgti): Fixup this function. Needs to dispatch interrupts.
inline
void
dispatching_interrupt(// ardu_clock_t &t, int new_state) {
  ) {
  do {
    double next = calcNext(new_state);
    if (next == 0)
      continue;
    ardu_clock_t old = _next;
    int old_state = _state;
    int overflow = 0;
    _next._ticks += (next - floor(next)) * TICKS_PER_SECOND;
    if (_next._ticks > TICKS_PER_SECOND) {
      _next._ticks -= TICKS_PER_SECOND;
      overflow = 1;
    }
    _next._seconds += floor(next) + overflow;

    ardu->_debug << "Signal: " << _name << " switched to ";
    if (new_state == HIGH) {
      ardu->_debug << "HIGH";
    }
    else {
      ardu->_debug << "LOW";
    }
    ardu->_debug << " @ " << old._seconds << "." 
                 << setw(FIELD_WIDTH) << setfill('0') << old._ticks << "\n";
    _state = new_state;
    
    if (_bit_container)
      *_bit_container = _bit_mask & 0xf;
    
    map<int, std::pair<int, void (*)(void)> >::iterator map_iter = ardu->_interrupt_map.find(ardu->_mapping[_name]);
    
    // cout << "Name: " << _name << endl;
    // cout << "Map found: " << ardu->_mapping[_name] << endl;
    if (_state == LOW) {
      // LOW EVENT
      if (ardu->_interrupts == true && map_iter != ardu->_interrupt_map.end() && map_iter->second.first == LOW) {
        map_iter->second.second();
      }
    }
    if (old_state == LOW && _state == HIGH) {
      // FIRE RISING EVENT
      if (ardu->_interrupts == true && map_iter != ardu->_interrupt_map.end() && map_iter->second.first == RISING) {
        map_iter->second.second();
      }
      _history.total_evts += 1;
      // _caught_flag = false;
    }
    if (old_state != _state) {
      // FIRE CHANGED EVENT
      if (ardu->_interrupts == true && map_iter != ardu->_interrupt_map.end() && map_iter->second.first == CHANGE) {
        map_iter->second.second();
      }
    }
    if (old_state == HIGH && _state == LOW) {
      // FIRE FALLING EVENT
      if (ardu->_interrupts == true && map_iter != ardu->_interrupt_map.end() && map_iter->second.first == FALLING) {
          map_iter->second.second();
      }
      // if (_caught_flag == false) {
          // _history.missed_evts += 1;
      // }
    }
  } while (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
              t._ticks > _next._ticks));
}




// TODO(ash_gti): Fix this function.
/**
 * Ardulator::updatePinState() 
 *    Updates the state of each pin in the internal pin registery.
 */
void Ardulator::updatePinState() {
  // Report Changes in State
  size_t c = 0;
  inside_interrupt_handler_ = true;

  PinConfigIterator it;
  for (it = pin_config_.begin(); it != pin_config_.end(); it++) {
    PinConfig *pin = it->second;
    c++;
    if (runtime_ > pin->signal_.current_->tick) {
      pin->signal_.current_ = pin->signal_.current_->next;
    }
  }

  // printf("Updating the pins: %zu\n", c);

  for (it = pin_config_.begin(); it != pin_config_.end(); it++) {
    c++;
    // cout << "Updating " << it->first << endl;
    it->second->setState(timer_);
  }
  
  vector<Signal*>::iterator vit;
  for (vit = _unused_signals.begin(); vit != _unused_signals.end(); vit++) {
      (*vit)->setState(_timer);
  }















  inside_interrupt_handler_ = false;

  if ((timer_.seconds_ > scenario_length_.seconds_) ||
        (timer_.seconds_ == scenario_length_.seconds_ &&
         timer_.ticks_ >= scenario_length_.ticks_)) {
    cout << "time: " << timer_.seconds_ << "." << timer_.ticks_ << endl;
    cout << "scenario: " << scenario_length_.seconds_ << "."
         << scenario_length_.ticks_ << endl;
    throw ef;
  }
}

void Ardulator::finalizePinState() {
  // Report Changes in State
  PinConfigIterator it;
  for (it = pin_config_.begin(); it != pin_config_.end(); it++) {
    it->second->setState(timer_);
  }
}

double Ardulator::now() {
  return static_cast<double>(timer_.seconds_)
      + (static_cast<double>(timer_.ticks_)
      /  static_cast<double>(TICKS_PER_FREQ));
}

/*
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
      addTicks(MS2T(10));
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
    debug_ << "Error, processingSignal " << signal_id << " while within an interrupt"
           << " handler is not allowed." << endl;
    exit(1);
  }
#endif /* end NOISY_WARNINGS */
  if (signal_names_.find(signal_id) == signal_names_.end()) {
    return;
  }
  if (pin_config_.find(signal_names_[signal_id]) == pin_config_.end()) {
    return;
  }

  double processing_time = pin_config_[signal_names_[signal_id]]->signal_.current_->duration;
  int wait_till = 0;

  wait_till = static_cast<int>(processing_time * TICKS_PER_SECOND);

  log_ << "Dispatching signal " << signal_id << " for " << signal_names_[signal_id]
       << " a " << wait_till << endl;

  while (wait_till) {
     wait_till--;
     addTicks(1);
     updatePinState();
  }
}

string Ardulator::timestamp() {
  ostringstream result(ostringstream::out);
  result << static_cast<int>(timer_.seconds_) << "s " << static_cast<int>(timer_.ticks_) << "t";
  return result.str();
}

void Ardulator::addPin(string signal_name, uint8_t pin_id) {
  printf("Adding pin %s\n", signal_name.c_str());
  UnusedPinConfigIterator it = unused_pin_config_.find(signal_name);

  if (it != unused_pin_config_.end()) {
    printf("Moving pointer\n");
    pin_config_[pin_id] = it->second;
    unused_pin_config_.erase(it);
  }

  signal_names_[signal_name] = pin_id;
}

void Ardulator::addSerial(string signal_name, HardwareSerial &serial) {
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
  for (vit = unused_pin_config_.begin(); vit != unused_pin_config_.end(); vit++) {
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
  interrupt_map_[pin_id] = make_pair(mode, fn);
}

/**
 * Removes an interrupt handler.
 * @param pin_id the interrupt to remove.
 */
void Ardulator::dropInterrupt(uint8_t pin_id) {
  interrupt_map_.erase(pin_id);
  cout << "Dropped int on " << pin_id << endl;
}

} // END namespace ardulator

