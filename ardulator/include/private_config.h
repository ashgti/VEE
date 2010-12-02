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

class Component {
  public:
    vector<uint8_t> _pins;
    vector<string>  _signals;
};

class Pin : public Component {
  public:
    Pin(uint16_t id, bool digital, bool io, bool interupt) 
            : _id(id), _digital(digital), _io(io), _interupt(interupt) { 
        _configured = false;
        _val = 0;
    }
    uint16_t _id;
    bool     _digital;
    bool     _io;
    bool     _interupt;
    bool     _configured;
    uint16_t _val;
    uint16_t _flags;
};

class Serial : public Component {
  public:
};

class Evt {
    
};

class PinEvt : public Evt {
  public:
    static string id();
    static Evt* evtHandler(bool in, string line);
};

class DetEvt : public Evt {
  public:
    static Evt* evtHandler(bool in, string line);
    static string id();
};

class UniEvt : public Evt {
  public:
    static string id();
    static Evt* evtHandler(bool in, string line);
};

class ExpEvt : public Evt {
  public:
    static string id();
    static Evt* evtHandler(bool in, string line);
};

class SerialEvt : public Evt {
  public:
    static string id();
    static Evt* evtHandler(bool in, string line);
};


class PinLayout {
  private:
  public:
    uint32_t _digital_pins;
    uint32_t _analog_pins;
    map<uint8_t, Pin*> _mapping;
    
    void addPin(int id, bool digital, bool io, bool interupt);    
    ~PinLayout();
};


class PinEvent {
    double   _start;
    double   _end;
    bool     _pinState;
    uint32_t _pin_id;
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
    map<double, vector<PinEvent> >
                _event_data;
    map<string, Evt* (*)(bool, string)>
                _evt_responders;
    PinLayout   _pins;
    
    ardu_clock_t 
                _timer;
    
    void   updatePinState();
    string timestamp();
  public:
    map<string, uint8_t>
                _signal_map;

    Arduino(ArduinoModel arduino_model);
    ~Arduino();
    void configurePin(uint8_t id, uint8_t mode);
    bool addInputFile(char *name);
    void runScenario(double duration);
    bool addEventHandler(string id, Evt* (*)(bool, string));
    void registerInputEvent(string id, string line);
    void registerOutputEvent(string id, string line);
    void setPin(uint8_t pin_id, uint8_t val);
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
