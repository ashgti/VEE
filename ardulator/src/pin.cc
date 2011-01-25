#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include "private_config.h"

using namespace std;

Pin::Pin() {
    _history.missed_evts = 0;
    _history.total_evts = 0;
    _history.caught_evts = 0;
    _history.avg_response_time = 0;
}

DetPin::DetPin() : Pin() {
}

ExpPin::ExpPin() : Pin() {
}

UniPin::UniPin() : Pin() {
}

void 
Pin::initializeTimers() {
    if (_signal_type == ST_DET) {
        double next = _vals[0] * _vals[1];
        
        _next_u._seconds = (int)_vals[0];
        _next_u._ticks = (_vals[0] - floor(_vals[0])) * TICKS_PER_SECOND;
        
        _next_d._seconds = (int)next;
        _next_d._ticks = (next - floor(next)) * TICKS_PER_SECOND;
        
        if (_state == HIGH) {
            _history.total_evts += 1;
        }
    }
}

void
Pin::setState(ardu_clock_t t) {
    if (_signal_type == ST_DET) {
        if (_val_type == VT_SERIAL) {
            if (_state == HIGH) {
                if (t._seconds >= _next_d._seconds &&
                        t._ticks > _next_d._ticks) {
                    double next = _vals[0] * _vals[1];
                    int overflow = 0;
                    _next_d._ticks += (next - floor(next)) * TICKS_PER_SECOND;
                    if (_next_d._ticks > TICKS_PER_SECOND) {
                        _next_d._ticks -= TICKS_PER_SECOND;
                        overflow = 1;
                    }
                    _next_d._seconds += (int)next + overflow;
                    ardu->_debug << "Signal: " << _name << " switched to LOW @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
                    _history.missed_evts += 1;
                    _state = LOW;
                }
            }
            else {
                if (t._seconds >= _next_u._seconds && 
                        t._ticks > _next_u._ticks) {
                    double next = _vals[0] * _vals[1];
                    int overflow = 0;
                    _next_u._ticks += (next - floor(next)) * TICKS_PER_SECOND;
                    if (_next_u._ticks > TICKS_PER_SECOND) {
                        _next_u._ticks -= TICKS_PER_SECOND;
                        overflow = 1;
                    }
                    _next_u._seconds += (int)next + overflow;
                    ardu->_debug << "Signal: " << _name << " switched to HIGH @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
                    _history.total_evts += 1;
                    _state = HIGH;
                }
            }
        }
        else if (_val_type == VT_DIGITAL) {
            if (_state == HIGH) {
                if (t._seconds >= _next_d._seconds &&
                        t._ticks > _next_d._ticks) {
                    double next = _vals[0] * _vals[1];
                    int overflow = 0;
                    _next_d._ticks += (next - floor(next)) * TICKS_PER_SECOND;
                    if (_next_d._ticks > TICKS_PER_SECOND) {
                        _next_d._ticks -= TICKS_PER_SECOND;
                        overflow = 1;
                    }
                    _next_d._seconds += (int)next + overflow;
                    ardu->_debug << "Signal: " << _name << " switched to LOW @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
                    _history.missed_evts += 1;
                    _state = LOW;
                }
            }
            else {
                if (t._seconds >= _next_u._seconds && 
                        t._ticks > _next_u._ticks) {
                    double next = _vals[0] * _vals[1];
                    int overflow = 0;
                    _next_u._ticks += (next - floor(next)) * TICKS_PER_SECOND;
                    if (_next_u._ticks > TICKS_PER_SECOND) {
                        _next_u._ticks -= TICKS_PER_SECOND;
                        overflow = 1;
                    }
                    _next_u._seconds += (int)next + overflow;
                    ardu->_debug << "Signal: " << _name << " switched to HIGH @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
                    _history.total_evts += 1;
                    _state = HIGH;
                }
            }
        }
    }
    
    _last_t = t;
}

int
Pin::process() {
    assert(_configured == true);
    _state = LOW;
    _history.caught_evts += 1;
    
    return _processing_time * TICKS_PER_SECOND;
}

void
Pin::report() {
    cout << "Reporting for " << _name << "\n";
    cout << "   Total Events: " << _history.total_evts << "\n";
    cout << "  Missed Events: " << _history.missed_evts << "\n";
    
    cout << "--------------------------\n";
}

string
Pin::parseConfiguration(string) {
    throw "std::string Pin::parseConfiguration(std::string) needs to be overriden by a proper subclass";
}

string
DetPin::parseConfiguration(string line) {
    stringstream ss(line);
    ValueType val_type;
    int initial_value, length, ratio, processing_time;
    string name, string_val;
    ss.seekg(4, ios::cur);
    ss >> name;
    if (name.at(name.length() - 1) == ',') {
        name = name.substr(0, name.length() - 1);
    }
    ss.seekg(1, ios::cur);
    if (ss.peek() == '"') {
        val_type = VT_SERIAL;
        ss >> string_val; 
    }
    else {
        val_type = VT_DIGITAL;
        ss >> initial_value;
    }
    ss.seekg(1, ios::cur);
    ss >> length;
    ss.seekg(1, ios::cur);
    ss >> ratio;
    ss.seekg(1, ios::cur);
    ss >> processing_time;
    
    _name = name;
    _state = -1;
    _flags = 0;
    _val_type = val_type;
    _signal_type = ST_DET;
    if (val_type == VT_SERIAL) {
        _string_val = string_val;
    }
    if (val_type == VT_DIGITAL) {
        _digital_val = initial_value;
        _state = initial_value;
    }
    _vals[0] = length;
    _vals[1] = ratio;
    _processing_time = processing_time;
    _configured = true;
    initializeTimers();
    
    return _name;
}

string
UniPin::parseConfiguration(string line) {
    return "";
}

string
ExpPin::parseConfiguration(string line) {
    return "";
}

