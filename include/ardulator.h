// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_H_
#define INCLUDE_ARDULATOR_H_
#include <vector>
#include <map>
#include <exception>
#include <utility>
#include <string>

#include "ardulator/bit_value.h"
#include "ardulator/containers.h"
#include "ardulator/pin_config.h"
#include "avr/config.h"

class HardwareSerial;

namespace ardulator {

using ardulator::containers::Clock;
using std::string;

const unsigned int LOOP_CONST = 240;
const unsigned int FIELD_WIDTH = 5;

void print_clock(FILE *file, string n, Clock t);

class Ardulator {
 private:
  FILE         *log_;
  int           max_pins_;
  double        runtime_;
  uint64_t      ticks_;
  uint64_t      total_ticks_;
  std::string   registered_identifers_;
  Clock         scenario_length_;
  bool          inside_interrupt_handler_;
  bool          prepared_;

  void        prepareScenario();
  void        updatePinState();
  void        updatePinMaps();
  void        finalizePinState();
  std::string timestamp();

 public:
  Clock timer_;

  typedef std::map<int, std::pair<int, void (*)(void)> > InterruptMap;
  typedef InterruptMap::iterator InterruptIterator;
  InterruptMap  interrupts_;

  typedef std::map<int, PinConfig*> PinConfigs;
  typedef PinConfigs::iterator PinConfigIterator;
  PinConfigs    pin_config_;

  typedef std::map<std::string, PinConfig*> UnusedPinConfigs;
  typedef UnusedPinConfigs::iterator UnusedPinConfigIterator;
  UnusedPinConfigs unused_pin_config_;

  typedef std::map<std::string, int> SingalNameMap;
  typedef SingalNameMap::iterator SingalNameMapIterator;
  SingalNameMap signal_names_;

  FILE         *debug_;
  std::string*  buffers_[256]; // For hardware serial ports

  bool interrupts_enabled_; // Can interrupts occure?

  Ardulator();
  ~Ardulator();
  void   configurePin(uint8_t id, uint8_t mode);
  double runScenario(double length);
  void   registerInterrupt(uint8_t pin_id,
                           void (*)(void),
                           uint8_t mode);
  void   dropInterrupt(uint8_t pin_id);
  void   addPin(std::string signal_name, uint8_t pin_id);
  void   addSerial(std::string signal_name, const HardwareSerial &serial);
  void   setPin(uint8_t pin_id, uint8_t val);
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
