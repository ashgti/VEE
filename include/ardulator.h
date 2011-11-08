// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_H_
#define INCLUDE_ARDULATOR_H_

#include <pthread.h>

#include <vector>
#include <map>
#include <exception>
#include <utility>
#include <string>

#include "ardulator/containers/bit_value.h"
#include "ardulator/containers/clock.h"
#include "ardulator/containers/history.h"
#include "ardulator/containers/signals.h"
#include "ardulator/pin_config.h"
#include "avr/config.h"

class HardwareSerial;

namespace ardulator {

using ::ardulator::containers::Clock;
using ::std::string;
using ::std::map;

const unsigned int LOOP_CONST = 240; //!< A setting for the amount a single
                                     //!< loop of the user code costs in terms
                                     //!< of clock ticks.

/**
 * Main instance of the emulator. Controls the state of the pins connected.
 * Calls the control code.
 * \see setup
 * \see loop
 * \see pinConfiguraiton
 */
class Ardulator {
 private:
  FILE           *log_;  //!< Log file for all events that happen in the
                         //!< scenario
  double          runtime_; //!< The total runtime as a double to make
                            //!< comparisions easier
  std::string     registered_identifers_;
  Clock           scenario_length_; //!< The total amount of time the scenario should run
  bool            inside_interrupt_handler_; //!< A guard to prevent interrupts
                                             //!< from happening inside an
                                             //!< interrupt handler.
  bool            prepared_; //!< A gaurd for initalizing the scenario

  /** Initialize the scenario. This is only run once **/
  void            prepareScenario();

  /** Updates pins, checks if we need to fire interrupt. **/
  void            updatePinState();
  /** Constructs a mapping of pins to signal names, useful reference. **/
  void            updatePinMaps();
  /** Finalize the state of the pins after the scenario is finished. **/
  void            finalizePinState();
  std::string     timestamp();

 public:
  Clock timer_; //!< Current time of the simulation

  typedef std::map<int, std::pair<int, void (*)(void)> > InterruptMap;
  typedef InterruptMap::iterator InterruptIterator;
  InterruptMap  interrupts_; //!< A list of interrupts, matching the pin and
                             //!< type to a function callback.

  typedef std::map<int, PinConfig*> PinConfigs;
  typedef PinConfigs::iterator PinConfigIterator;
  PinConfigs    pin_config_; //!< A map of pins to their respective PinConfigs

  typedef std::map<std::string, PinConfig*> UnusedPinConfigs;
  typedef UnusedPinConfigs::iterator UnusedPinConfigIterator;
  UnusedPinConfigs unused_pin_config_; //!< A list of unused pins.

  typedef std::map<std::string, int> SingalNameMap;
  typedef SingalNameMap::iterator SingalNameMapIterator;
  SingalNameMap signal_names_; //!< A map to connect a signal name to a given
                               //!< pin.

  FILE         *debug_;  //!< Debug log file
  std::string*  buffers_[256]; //!< Hardware serial ports buffers

  bool interrupts_enabled_; //!< Controlls the ability to dispatch interrupts

  Ardulator();
  ~Ardulator();
  
  void   configurePin(uint8_t id, uint8_t mode);

  /**
   * \callgraph
   * Run the scenario for a given length.
   * \returns the exact amount of time the scenario ran for
   */
  double runScenario(double length);
  void   registerInterrupt(uint8_t pin_id,
                           void (*)(void),
                           uint8_t mode);
  void   dropInterrupt(uint8_t pin_id);
  void   addPin(std::string signal_name, uint8_t pin_id);
  void   addSerial(std::string signal_name, const HardwareSerial &serial);
  void   setPin(uint8_t pin_id, uint8_t val);

  /**
   * Get the value of a given pin and increment the clock based off the kind of
   * access.
   * Analog Reads take 100 s
   * Digital Reads 58 cycles
  **/
  int    getPin(uint8_t pin_id);
  void   dispatchSignal(const char *name);
  void   addTicks(uint64_t);
  double now();
  void   report();
  void   log(int level, std::string msg);
  void   debug(std::string msg = "");
  void   warn(std::string msg = "");
  void   error(std::string msg = "");
  void   info(std::string msg = "");
  void   critical(std::string msg = "");
};

class ArduException : public std::exception {
  public:
    explicit ArduException(std::string msg) : msg(msg) { }
    virtual ~ArduException() throw() { }
  private:
    std::string msg;
    virtual const char* what() const throw() {
        return msg.c_str();
    }
};

class ProcessingSignalException : public std::exception {
};

void setupArduino();


}  // end namespace ardulator

/* Declare Variables used by the emulation program */
/* The instance of the arduino emulation */
extern ardulator::Ardulator *ardu;

/* Digital Ports for accessing pin state */
extern ardulator::containers::BitValue PINB;
extern ardulator::containers::BitValue DDRB;
extern ardulator::containers::BitValue PORTB;

extern ardulator::containers::BitValue PINC;
extern ardulator::containers::BitValue DDRC;
extern ardulator::containers::BitValue PORTC;

extern ardulator::containers::BitValue PIND;
extern ardulator::containers::BitValue DDRD;
extern ardulator::containers::BitValue PORTD;

#endif  // INCLUDE_ARDULATOR_H_
