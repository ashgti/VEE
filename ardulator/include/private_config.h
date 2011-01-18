#ifndef PRIVATE_CONFIG_H
#define PRIVATE_CONFIG_H

#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include "arduino_api.h"

using namespace std;

void wait(double seconds);

#define TICKS_PER_SECOND 10000
#define TICKS_PER_MICRO_SECONDS 100
#define MS2T(x) x * TICKS_PER_MICRO_SECONDS

void setupComponents();

enum Distribution {
    UNI,
    EXP
};

template <class T>
class RandNum {
  public:
    unsigned short _Xi[3];
    Distribution _dis_type;
    T _range_start, _range_end, _lambda;
    
    RandNum(Distribution dis_type, T lambda, unsigned short seed = 100);
    T next();
};

typedef struct ardu_clock_t {
    uint32_t _seconds;
    uint32_t _ticks;
} ardu_clock_t;

class Arduino {
  private:
    ifstream    _log;
    bool        _flag;
    int         _max_pins;
    uint64_t    _ticks;
    uint64_t    _total_ticks;
    uint64_t    _scenario_length;
    string      _registered_identifers;
    ardu_clock_t 
                _timer;

    map<int, void (*)(void)>
                _interupts;
    map<string, int>
                _mapping;
    map<int, int>
                _pins;
    map<string, double>
                _signals;

    void   updatePinState();
    string timestamp();
  public:
    map<string, uint8_t>
                _signal_map;

    Arduino();
    ~Arduino();
    void configurePin(uint8_t id, uint8_t mode);
    void addPin(string signal_id, uint8_t pin_id);
    bool addInputFile(char *name);
    void runScenario(double duration);
    bool addEventHandler(string id, Evt* (*)(bool, string));
    void scanHeaderChunk(string id, string line);
    void registerSignal(string id, string line);
    void setPin(uint8_t pin_id, uint8_t val);
    int  getPin(uint8_t pin_id);
    void dispatchSignal(const char *name);
    void addTicks(uint64_t);
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
