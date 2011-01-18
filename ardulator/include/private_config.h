#ifndef PRIVATE_CONFIG_H
#define PRIVATE_CONFIG_H

#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include "arduino_api.h"

using namespace std;

void wait(double seconds);

#define FIELD_WIDTH      5
#define TICKS_PER_SECOND 10000
#define TICKS_PER_MICRO_SECONDS 100
#define MS2T(x) x * TICKS_PER_MICRO_SECONDS

void setupComponents();

enum SignalType { ST_UNI, ST_DET, ST_EXP };
enum ValueType { VT_DIGITAL, VT_SERIAL, VT_RAND_STRING };

template <class T>
class RandNum {
  public:
    unsigned short _Xi[3];
    SignalType _dis_type;
    T _range_start, _range_end, _lambda;

    RandNum(SignalType dis_type, T lambda, unsigned short seed = 100);
    T next();
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

class Pin {
  public:
    RandNum<int> *_num;
    string _const_string;
    SignalType _signal_type;
    ValueType _val_type;
    double _vals[3];
    
    int _mode;
    
    bool _configured;
    
    int _state;
    int _flags;
    
    ardu_clock_t _next_u;
    ardu_clock_t _next_d;
    ardu_clock_t _last_t;
    
    string _name;
    
    int _processing_time;
    string _string_val;
    int _digital_val;

    history_t _history;
    
    Pin();
    Pin(SignalType st, int lambda) {
        _num = new RandNum<int>(st, lambda);
    }

    int process();
    void report();
    
    void setState(ardu_clock_t t);
    void calcNext();
};

class Arduino {
  private:
    ifstream    _log;
    bool        _flag;
    int         _max_pins;
    uint64_t    _ticks;
    uint64_t    _total_ticks;
    string      _registered_identifers;
    ardu_clock_t 
                _timer;
    ardu_clock_t
                _scenario_length;

    map<int, void (*)(void)>
                _interupts;
    map<string, intptr_t>
                _mapping;

    void   updatePinState();
    string timestamp();
  public:
    map<int, Pin*>
                _pins;

    Arduino();
    ~Arduino();
    void configurePin(uint8_t id, uint8_t mode);
    void addPin(string signal_id, uint8_t pin_id);
    void addSerial(string signal_id, HardwareSerial *serial);
    bool addInputFile(char *name);
    void runScenario();
    // bool addEventHandler(string id, Evt* (*)(bool, string));
    void scanHeaderChunk(string id, string line);
    void registerSignal(string id, string line);
    void setPin(uint8_t pin_id, uint8_t val);
    int  getPin(uint8_t pin_id);
    void dispatchSignal(const char *name);
    void addTicks(uint64_t);
    void report();
    void log(int level, string msg);
    void debug(string msg = "");
    void warn(string msg = "");
    void error(string msg = "");
    void info(string msg = "");
    void critical(string msg = "");
};

extern Arduino *ardu;

class ArduException : public exception {
  public:
    ArduException(string msg) : msg(msg) { };
    virtual ~ArduException() throw() { };
  private:
    string msg;
    virtual const char* what() const throw() {
        return msg.c_str();
    }
};


#endif /* PRIVATE_CONFIG_H */
