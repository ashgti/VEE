#ifndef ARDULATOR_CONFIG_H
#define ARDULATOR_CONFIG_H
#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include <setjmp.h>
#include "ardulator/bit_value.h"
#include "avr/config.h"

class HardwareSerial;

#ifdef __cplusplus
extern "C"{
#endif

union ValueImp {
  char* str;
  uint8_t digital;
  uint16_t analog;
};

struct SignalImp;
struct SignalImp {
  double tick;
  double duration;
  union ValueImp value;
  uint32_t type;
  const char *name;
  struct SignalImp* next;
};

#ifdef __cplusplus
}
#endif

namespace ardulator {

const unsigned int LOOP_CONST = 240;
const unsigned int FIELD_WIDTH = 5;

enum ValueType {
    VT_DIGITAL = 1 << 0,
    VT_SERIAL = 1 << 1,
    VT_ANALOG = 1 << 2,
    VT_RAND_STRING = 4
};

struct ArdulatorClock {
  ArdulatorClock(uint32_t seconds, uint32_t ticks) :
      seconds_(seconds), ticks_(ticks) { };
  uint32_t seconds_;
  uint32_t ticks_;
};

struct History {
  int missed_evts_;
  int total_evts_;
  int caught_evts_;
  double avg_response_time_;
};

struct SignalContainer {
  SignalImp* head_;
  SignalImp* current_;
};

struct PinConfig {
  /* Constructor */
  PinConfig();
  
  int id_[15];
  bool mode_;
  bool interrupt_;
  bool pull_up_;
  uint8_t state_;
  uint8_t bit_mask_;
  volatile uint8_t *bit_container_;

  History pin_history_;

  SignalContainer signal_;

  void setState(ArdulatorClock &t);
  void initializeTimers();
  void updateState(ArdulatorClock &t, int new_state);
};

class Ardulator {
  private:
    std::fstream   log_;
    int            max_pins_;
    double         runtime_;
    uint64_t       ticks_;
    uint64_t       total_ticks_;
    std::string    registered_identifers_;
    ArdulatorClock scenario_length_;
    bool           inside_interrupt_handler_;
    bool           prepared_;

    void   prepareScenario();
    void   updatePinState();
    void   updatePinMaps();
    void   finalizePinState();
    std::string timestamp();
  public:
    ArdulatorClock timer_;

    typedef std::map<int, std::pair<int, void (*)(void)> > InterruptMap;
    typedef InterruptMap::iterator InterruptIterator;
    InterruptMap  interrupt_map_;

    typedef std::map<int, PinConfig*> PinConfigs;
    typedef PinConfigs::iterator PinConfigIterator;
    PinConfigs    pin_config_;

    typedef std::map<std::string, PinConfig*> UnusedPinConfigs;
    typedef UnusedPinConfigs::iterator UnusedPinConfigIterator;
    UnusedPinConfigs unused_pin_config_;

    typedef std::map<std::string, int> SingalNameMap;
    typedef SingalNameMap::iterator SingalNameMapIterator;
    SingalNameMap signal_names_;

    std::fstream  debug_;
    std::string*  buffers_[256];

    bool interrupts_;

    Ardulator();
    ~Ardulator();
    void   configurePin(uint8_t id, uint8_t mode);
    bool   addInputFile(char *name);
    double runScenario(double length);
    void   registerInterrupt(uint8_t pin_id,
                             void (*)(void),
                             uint8_t mode);
    void   dropInterrupt(uint8_t pin_id);
    void   addPin(std::string signal_name, uint8_t pin_id);
    void   addSerial(std::string signal_name, HardwareSerial &serial);
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
    ArduException(std::string msg) : msg(msg) { };
    virtual ~ArduException() throw() { };
  private:
    std::string msg;
    virtual const char* what() const throw() {
        return msg.c_str();
    }
};

class ProcessingSignalException : public std::exception {
};

void setupArduino();

} // end namespace ardulator

/* Declare Variables used by the emulation program */
/* The instance of the arduino emulation */
extern ardulator::Ardulator *ardu;

/* Digital Ports for accessing pin state */
extern ardulator::BitValue PINB;
extern ardulator::BitValue DDRB;
extern ardulator::BitValue PORTB;

extern ardulator::BitValue PINC;
extern ardulator::BitValue DDRC;
extern ardulator::BitValue PORTC;

extern ardulator::BitValue PIND;
extern ardulator::BitValue DDRD;
extern ardulator::BitValue PORTD;

#endif /* ARDULATOR_CONFIG_H */
