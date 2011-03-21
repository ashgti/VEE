#ifndef ARDULATOR_SIGNAL_H
#define ARDULATOR_SIGNAL_H
#include "ardulator.h"

class Signal {
  public:
    RandNum *_num;
    std::string _const_string;
    SignalType _signal_type;
    ValueType _val_type;
    bool _configured;
    
    uint8_t _bit_mask;
    volatile uint8_t *_bit_container;
    
    uint32_t _state; /* Current state, could be 0 to 1024, or higher
                      * depending on the accuracy of the A2D */
    uint8_t  _flags;
    uint8_t  _mode;  /* INPUT | OUTPUT mode */
    
    ardu_clock_t _next;
    ardu_clock_t _last_t;
    
    std::fstream _log;
    
    std::string _name;
    double _mu;
    double _length, _ratio;
    
    std::string _string_val;
    int _digital_val;
    int _analog_value;
    
    bool _caught_flag;

    history_t _history;
    
    Signal();
    ~Signal();

    int process();
    
    void setState(ardu_clock_t &t);
    void finalize(ardu_clock_t &t);
    void initializeTimers();
    void updateState(ardu_clock_t &t, int new_state);

    void parseStart(std::stringstream &ss);
    
    virtual std::string parseConfiguration(std::string);
    virtual double calcNext(int new_state);
    virtual void report(bool);
};

#include "ardulator/signal/detsignal.h"
#include "ardulator/signal/unisignal.h"
#include "ardulator/signal/expsignal.h"

#endif
