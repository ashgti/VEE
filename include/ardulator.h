#ifndef ARDULATOR_CONFIG_H
#define ARDULATOR_CONFIG_H
#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include <setjmp.h>
#include "ardulator/bit_value.h"

#define LOOP_CONST      15
#define FIELD_WIDTH      5
#define TICKS_PER_SECOND 10000
#define TICKS_PER_MICRO_SECONDS 100
#define MS2T(x) x * TICKS_PER_MICRO_SECONDS

class HardwareSerial;

enum SignalType { ST_UNI, ST_DET, ST_EXP };
enum ValueType { VT_DIGITAL = 1, VT_SERIAL = 2, VT_ANALOG = 3, VT_RAND_STRING = 4 };

class RandNum {
  public:
    unsigned short _Xi[3];
    SignalType _dis_type;
    double _range_start, _range_end, _lambda;

    RandNum(SignalType dis_type, double lambda, unsigned short seed = -1);
    double next();
};

typedef struct ardu_clock_t {
    uint32_t _seconds;
    uint32_t _ticks;
} ardu_clock_t;

typedef struct history_t {
    int missed_evts;
    int total_evts;
    int caught_evts;
    double avg_response_time;
} history_t;

class Signal;

class Ardulator {
  private:
    std::fstream  _log;
    bool          _flag;
    int           _max_pins;
    uint64_t      _ticks;
    uint64_t      _total_ticks;
    std::string   _registered_identifers;
    ardu_clock_t  _timer;
    ardu_clock_t  _scenario_length;

    void   updatePinState();
    void   updatePinMaps();
    void   finalizePinState();
    std::string timestamp();
  public:
      
    std::map<int, std::vector<std::string> >  
                _interrupt_connection;
    std::map<int, std::pair<int, void (*)(void)> >
                _interrupt_map;
    std::map<int, Signal*>
                _signals;
    std::vector<Signal*>
                _unused_signals;
    std::map<std::string, intptr_t>
                _mapping;
    std::fstream
                _debug;
    std::string*
                _buffers[256];
                
    bool _interrupts;

    Ardulator();
    ~Ardulator();
    void configurePin(uint8_t id, uint8_t mode);
    void addPin(std::string signal_id, uint8_t pin_id);
    void addSerial(std::string signal_id, HardwareSerial &serial);
    bool addInputFile(char *name);
    void runScenario();
    // bool addEventHandler(std::string id, Evt* (*)(bool, string));
    void scanHeaderChunk(std::string id, std::string line);
    void processConfiguration(std::string id, std::string line);
    void registerInterrupt(uint8_t pin_id, void (*)(void), uint8_t mode);
    void dropInterrupt(uint8_t pin_id);
    void setPin(uint8_t pin_id, uint8_t val);
    int  getPin(uint8_t pin_id);
    void dispatchSignal(const char *name);
    void addTicks(uint64_t);
    void report();
    void log(int level, std::string msg);
    void debug(std::string msg = "");
    void warn(std::string msg = "");
    void error(std::string msg = "");
    void info(std::string msg = "");
    void critical(std::string msg = "");
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

class ProcessingSignal : public std::exception {
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

#include "ardulator/signal.h"

#endif /* ARDULATOR_CONFIG_H */
