#ifndef ARDULATOR_CONFIG_H
#define ARDULATOR_CONFIG_H
#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include <setjmp.h>
#include "ardulator/bit_value.h"
#include "avr/config.h"

const unsigned int LOOP_CONST = 240;
const unsigned int FIELD_WIDTH = 5;

class HardwareSerial;

enum ValueType {
    VT_DIGITAL = 1,
    VT_SERIAL = 2,
    VT_ANALOG = 3,
    VT_RAND_STRING = 4
};

struct ArdulatorClock {
    uint32_t seconds_;
    uint32_t ticks_;
};

struct History {
    int missed_evts_;
    int total_evts_;
    int caught_evts_;
    double avg_response_time_;
};

struct PinConfig {
    int id_[10];
    bool mode_;
    bool interrupt_;
    bool pull_up_;
};

class Signal {
    PinConfig* mapped_to_;
    double *signal_changes_;
    size_t  signal_changes_length_;
};

class Ardulator {
  private:
    std::fstream   log_;
    bool           flag_;
    int            max_pins_;
    uint64_t       ticks_;
    uint64_t       total_ticks_;
    std::string    registered_identifers_;
    ArdulatorClock scenario_length_;
    bool           inside_interrupt_handler_;

    void   updatePinState();
    void   updatePinMaps();
    void   finalizePinState();
    std::string timestamp();
  public:
    ArdulatorClock timer_;
    std::map<int, std::vector<std::string> >
                interrupt_connection_;
    std::map<int, std::pair<int, void (*)(void)> >
                interrupt_map_;
    // std::map<int, std::vector<double> >
    //             signals_;
    typedef std::map<int, PinConfig*> PinConfigs;
    typedef PinConfigs::iterator PinConfigsIter;
    PinConfigs pin_config_;

    // std::map<std::string, intptr_t>
    //             _mapping;
    std::fstream
                debug_;
    std::string*
                buffers_[256];

    bool interrupts_;

    Ardulator();
    ~Ardulator();
    void   configurePin(uint8_t id, uint8_t mode);
    // void   addPin(std::string signal_id, uint8_t pin_id);
    // void   addSerial(std::string signal_id, HardwareSerial &serial);
    bool   addInputFile(char *name);
    void   runScenario();
    void   registerSignalReference(Signal s);
    // void   scanHeaderChunk(std::string id, std::string line);
    // void   processConfiguration(std::string id, std::string line);
    void   registerInterrupt(uint8_t pin_id,
                             void (*)(void),
                             uint8_t mode);
    void   dropInterrupt(uint8_t pin_id);
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
    // bool addEventHandler(std::string id, Evt* (*)(bool, string));
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

/* Declare Variables used by the emulation program */
/* The instance of the arduino emulation */
extern Ardulator *ardu;

class ProcessingSignalException : public std::exception {
};

/* Digital Ports for accessing pin state */
extern BitValue PINB;
extern BitValue DDRB;
extern BitValue PORTB;

extern BitValue PINC;
extern BitValue DDRC;
extern BitValue PORTC;

extern BitValue PIND;
extern BitValue DDRD;
extern BitValue PORTD;

void setupArduino();

#endif /* ARDULATOR_CONFIG_H */

