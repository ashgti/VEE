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
  ardu->debug_ << n << " " << t.seconds_ << "." << setw(FIELD_WIDTH) << setfill('0') << t.ticks_ << "\n";
    
  ardu->debug_.flush();
}

Ardulator::Ardulator() : interrupts_(false) {
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
  
  ticks_ = 0 ;
  total_ticks_ = 0;
  scenario_length_.seconds_ = 0;
  scenario_length_.ticks_ = 0;
  registered_identifers_ = "";
  inside_interrupt_handler_ = false;
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

  for (PinConfigsIter it = pin_config_.begin();
       it != pin_config_.end();
       it++) {
    delete it->second;
  }
}

void Ardulator::configurePin(uint8_t pin_id, uint8_t mode) {
//     map<int, vector<double> >::iterator it;
//     it = signals_.find(pin_id);
// 
//     if (it != signals_.end()) {
//         pin_config_[pin_id]->mode_ = mode;
//         debug_ << "Configuring Pin " << (int)pin_id << " to mode: " 
//                << (int)mode <<  "\n";
//     } else {
//         debug_ << "Error, pin configured that is not registed\n";
//     }
}

double Ardulator::runScenario(double length) {
  cout << "Running for: " << length << endl;
  runtime_ = 0.0;
  timer_.seconds_ = 0;
  timer_.ticks_   = 0;
  setup();
  debug_ << "Attempting to run the Scenario\n\n";
  
  print_clock("Scenario Length: ", scenario_length_);
  print_clock("Runtime Timer:", timer_);
  
  updatePinMaps();
  
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
 * updatePinMaps is used to correct the pin/signals after the arduino
 * runs the pinConfiguration function.
 *
 * TODO(ash_gti): May not be needed anymore, this as been abstracted out to
 * the python code, but not sure.
 */
void Ardulator::updatePinMaps() {
    /*
    for (map<int, vector<double> >::iterator it = _signals.begin();
            it != _signals.end();
            ++it) {
        if (it->first >= 0 || it->first < 8) {
            it->second->_bit_mask = 1 << it->first;
            it->second->_bit_container = PORTD.getStateRef();
        } else if (it->first >= 8 || it->first < 14) {
            it->second->_bit_mask = 1 << (8 - it->first);
            it->second->_bit_container = PORTB.getStateRef();
        } else if (it->first >= 16 || it->first < 22) {
            it->second->_bit_mask = 1 << (16 - it->first);
            it->second->_bit_container = PORTC.getStateRef();
        } else {
            throw "Error this pin is out of bounds!!";
        }
    }
    */
}

/**
 * addTicks is used to increment the interanl clock
 */
void Ardulator::addTicks(uint64_t length) {
  timer_.ticks_ += length;
  if (timer_.ticks_ > TICKS_PER_SECOND) {
    timer_.seconds_++;
    timer_.ticks_ -= TICKS_PER_SECOND;
  }
  
  runtime_ = timer_.seconds_ + (timer_.ticks_ / TICKS_PER_SECOND);
}

// TODO(ash_gti): Fix this function.
void Ardulator::updatePinState() {
  // Report Changes in State
  map<int, vector<double> >::iterator it;
  // int c = 0;
  inside_interrupt_handler_ = true;
  // for (it = _signals.begin(); it != _signals.end(); it++) {
  //     c++;
  //     // cout << "Updating " << it->first << endl;
  //     it->second->setState(timer_);
  // }
    
  inside_interrupt_handler_ = false;
    
  if ((timer_.seconds_ > scenario_length_.seconds_) ||
        (timer_.seconds_ == scenario_length_.seconds_ &&
         timer_.ticks_ >= scenario_length_.ticks_)) {
    cout << "time: " << timer_.seconds_ << "." << timer_.ticks_ << endl;
    cout << "scne: " << scenario_length_.seconds_ << "." 
         << scenario_length_.ticks_ << endl;
    throw ef;
  }
}

void Ardulator::finalizePinState() {
  // Report Changes in State
  map<int, vector<double> >::iterator it;
  // for (it = _signals.begin(); it != _signals.end(); it++) {
  //     it->second->finalize(timer_);
  // }
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
  // if (_signals.find(pin_id) == _signals.end()) {
    return LOW;
  // } else {
  //         if (_signals[pin_id]->_val_type == VT_SERIAL) {
  //             return LOW;
  //         } else if (_signals[pin_id]->_val_type == VT_DIGITAL) {
  //             addTicks(58);
  //             return _signals[pin_id]->_state;
  //         } else if (_signals[pin_id]->_val_type == VT_ANALOG) {
  //             addTicks(MS2T(10));
  //             return _signals[pin_id]->_state;
  //         } else {
  //             return LOW;
  //         }
  //     }
}

void Ardulator::setPin(uint8_t pin_id, uint8_t val) {
  // if (_signals[pin_id]->_state != val) {
  //     _signals[pin_id]->_state = val;
  // }
}

void Ardulator::dispatchSignal(const char *signal_id) {
#ifdef NOISY_WARNINGS
  if (_inside_interrupt_handler) {
    debug_ << "Error, processingSignal " << signal_id << " while within an interrupt"
           << " handler is not allowed." << endl;
    // exit(12);
  }
#endif /* end NOISY_WARNINGS */
    
    //if (mapping_.find(signal_id) == mapping_.end()) {
    //    return;
    //}
    // if (_signals.find(_mapping[signal_id]) == _signals.end()) {
    //     return;
    // }
    //int wait_till = signals_[_mapping[signal_id]]->process();
    //_log << "Dispatching signal " << signal_id << " for " << _mapping[signal_id] << " a " << wait_till << endl;
    
    //while (wait_till) {
    //    wait_till--;
    //    addTicks(1);
    //    updatePinState();
    //}
}

string Ardulator::timestamp() {
    ostringstream result(ostringstream::out);
    result << static_cast<int>(timer_.seconds_) << "s " << static_cast<int>(timer_.ticks_) << "t";
    return result.str();
}

//void Ardulator::addPin(string signal_name, uint8_t pin_id) {
//    _mapping[signal_name] = pin_id;
//}

//void Ardulator::addSerial(string signal_name, HardwareSerial &serial) {
//    _mapping[signal_name] = serial.pin();
//}

/* Reporting facilities */
void Ardulator::report() {
    //map<int, Signal*>::iterator it;
    //for (it = signals_.begin(); it != signals_.end(); it++) {
    //    it->second->report(true);
    //}
    
    //vector<Signal*>::iterator vit;
    //for (vit = unused_signals_.begin(); vit != unused_signals_.end(); vit++) {
    //    (*vit)->report(false);
    //}
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

