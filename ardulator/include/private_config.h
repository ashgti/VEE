#ifndef PRIVATE_CONFIG_H
#define PRIVATE_CONFIG_H

#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include "arduino_api.h"

using namespace std;

void wait(double seconds);

#define LOOP_CONST      15
#define FIELD_WIDTH      5
#define TICKS_PER_SECOND 10000
#define TICKS_PER_MICRO_SECONDS 100
#define MS2T(x) x * TICKS_PER_MICRO_SECONDS

void setupComponents();

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

class Pin {
  public:
    RandNum *_num;
    string _const_string;
    SignalType _signal_type;
    ValueType _val_type;
    bool _configured;
    
    uint32_t _state; /* Current state, could be 0 to 1024, or higher
                      * depending on the accuracy of the A2D */
    uint8_t  _flags;
    uint8_t  _mode;  /* INPUT | OUTPUT mode */
    
    ardu_clock_t _next;
    ardu_clock_t _last_t;
    
    fstream _log;
    
    string _name;
    double _mu;
    double _length, _ratio;
    
    string _string_val;
    int _digital_val;
    
    bool _caught_flag;

    history_t _history;
    
    Pin();
    ~Pin();

    int process();
    
    void setState(ardu_clock_t &t);
    void finalize(ardu_clock_t &t);
    void initializeTimers();
    
    void parseStart(stringstream &ss);
    
    virtual string parseConfiguration(string);
    virtual void updateState(ardu_clock_t &t, int new_state);
    virtual void report();
};

class DetPin : public Pin {
  public:
    DetPin();
    string parseConfiguration(string);
    void updateState(ardu_clock_t &t, int new_state);
    void report();
};

class UniPin : public Pin {
  public:
    UniPin();
    string parseConfiguration(string);
    void updateState(ardu_clock_t &t, int new_state);
    void report();
};

class ExpPin : public Pin {
  public:
    ExpPin();
    string parseConfiguration(string);
    void updateState(ardu_clock_t &t, int new_state);
    void report();
};

class Arduino {
  private:
    fstream     _log;
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

    void   updatePinState();
    void   finalizePinState();
    string timestamp();
  public:
    map<int, Pin*>
                _pins;
    vector<Pin*>
                _unused_pins;
    map<string, intptr_t>
                _mapping;
    fstream     _debug;
    
    string* _buffers[256];

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
