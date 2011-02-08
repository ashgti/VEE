#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include "private_config.h"

using namespace std;

Pin::Pin() {
    _caught_flag = false;
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
        _state = LOW;
        if (_state == HIGH) {
            _history.total_evts += 1;
        }
    }
}

void
Pin::setState(ardu_clock_t &t) {
    if (_val_type == VT_SERIAL) {
        if (_state == HIGH) {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                if (_caught_flag == false)
                    _history.missed_evts += 1;
                updateState(t, LOW);
                _caught_flag = false;
            }
        }
        else {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                _history.total_evts += 1;
                updateState(t, HIGH);
            }
        }
    }
    else if (_val_type == VT_DIGITAL) {
        if (_state == HIGH) {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                if (_caught_flag == false) {
                    _history.missed_evts += 1;
                }
                updateState(t, LOW);
                _caught_flag = false;
            }
        }
        else {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                _history.total_evts += 1;
                updateState(t, HIGH);
            }
        }
    }
    else if (_val_type == VT_ANALOG) {
        if (_state == HIGH) {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                if (_caught_flag == false)
                    _history.missed_evts += 1;
                updateState(t, LOW);
                _caught_flag = false;
            }
        }
        else {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                _history.total_evts += 1;
                updateState(t, HIGH);
            }
        }
    }
    
    _last_t = t;
}

void
Pin::finalize(ardu_clock_t &t) {
    if (_val_type == VT_SERIAL) {
        if (_state == HIGH) {
            if (t._seconds >= _next._seconds &&
                    t._ticks > _next._ticks) {
                if (_caught_flag == false) {
                    _history.missed_evts += 1;
                }
                _caught_flag = false;
            }
        }
    }
    else if (_val_type == VT_DIGITAL) {
        if (_state == HIGH) {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                if (_caught_flag == false) {
                    _history.missed_evts += 1;
                }
                _caught_flag = false;
            }
        }
    }
    else if (_val_type == VT_ANALOG) {
        if (_state == HIGH) {
            if (t._seconds >= _next._seconds &&
                    t._ticks > _next._ticks) {
                if (_caught_flag == false)
                    _history.missed_evts += 1;
                _caught_flag = false;
            }
        }
    }
}

void
Pin::updateState(ardu_clock_t &t, int new_state) {
    cout << "ERROR, bad function call\n";
    exit(-1);
}

void
DetPin::updateState(ardu_clock_t &t, int new_state) {
    double next = _length * _ratio;

    int overflow = 0;
    _next._ticks += (next - floor(next)) * TICKS_PER_SECOND;
    if (_next._ticks > TICKS_PER_SECOND) {
        _next._ticks -= TICKS_PER_SECOND;
        overflow = 1;
    }
    _next._seconds += floor(next) + overflow;
    ardu->_debug << "Signal: " << _name << " switched to ";
    if (new_state == HIGH)
        ardu->_debug << "HIGH";
    else
        ardu->_debug << "LOW";
    ardu->_debug  << " @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
    ardu->_debug.flush();
    _state = new_state;
}

void
UniPin::updateState(ardu_clock_t &t, int new_state) {
    double next = 0;
    if (new_state == HIGH) {
        next = _num->next();
    } 
    else {
        next = 0.1 * _length; 
    }
    
    int overflow = 0;
    _next._ticks += (next - floor(next)) * TICKS_PER_SECOND;
    if (_next._ticks > TICKS_PER_SECOND) {
        _next._ticks -= TICKS_PER_SECOND;
        overflow = 1;
    }
    _next._seconds += floor(next) + overflow;
    
    ardu->_debug << "Signal: " << _name << " switched to ";
    if (new_state == HIGH)
        ardu->_debug << "HIGH";
    else
        ardu->_debug << "LOW";
    ardu->_debug  << " @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
    _state = new_state;
}

void
ExpPin::updateState(ardu_clock_t &t, int new_state) {
    double next = 0;
    if (new_state == HIGH) 
        next = _num->next();
    else
        next = (_mu * 0.01) * _length;
    
    int overflow = 0;
    _next._ticks += (next - floor(next)) * TICKS_PER_SECOND;
    if (_next._ticks > TICKS_PER_SECOND) {
        _next._ticks -= TICKS_PER_SECOND;
        overflow = 1;
    }
    
    _next._seconds += (int)next + overflow;
    ardu->_debug << "Signal: " << _name << " switched to LOW @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
    _state = new_state;
}

int
Pin::process() {
    if (_caught_flag == false && _state == HIGH) {
        _history.caught_evts += 1;
        _caught_flag = true;
    }
    else {
        cerr << "Processing event that has been handled. ";
    }
    
    ardu->_debug << "Processing: " << _name << "\n";
    
    return (_mu * 0.01) * _length * TICKS_PER_SECOND;
}

void
Pin::report() {
    cerr << "ERROR, bad function call\n";
    exit(-1);
}

void
ExpPin::report() {
    cout << "Reporting for: " << _name << "\n";
    cout << "               Lambda: " << _length << "\n";
    cout << "                   Mu: " << _mu << "\n";
    cout << "                   --\n";
    cout << "        Missed Events: " << _history.missed_evts << "\n";
    cout << "         Total Events: " << _history.total_evts << "\n";
    
    cout << "--------------------------\n\n";
}

void
DetPin::report() {
    cout << "Reporting for: " << _name << "\n";
    cout << "--- used for testing ---\n";
    cout << "   Signal High Length: " << _length << "\n";
    cout << "                Ratio: " << _ratio << "\n";
    cout << "                   Mu: " << _mu << "\n";
    cout << "                   --\n";
    cout << "        Missed Events: " << _history.missed_evts << "\n";
    cout << "         Total Events: " << _history.total_evts << "\n";
    
    cout << "--------------------------\n\n";
}

void
UniPin::report() {
    cout << "Reporting for: " << _name << "\n";
    cout << "               Lambda: " << _length << "\n";
    cout << "                   Mu: " << _mu << "\n";
    cout << "                  --\n";
    cout << "        Missed Events: " << _history.missed_evts << "\n";
    cout << "         Total Events: " << _history.total_evts << "\n";
    
    cout << "--------------------------\n\n";
}

string
Pin::parseConfiguration(string) {
    throw "std::string Pin::parseConfiguration(std::string) needs to be overriden by a proper subclass";
}

void
Pin::parseStart(stringstream &ss) {
    ValueType val_type;
    int initial_value = 0;
    string name, string_val;
    ss.seekg(4, ios::cur);
    ss >> name;
    if (name.at(name.length() - 1) == ',') {
        name = name.substr(0, name.length() - 1);
    }
    ss.seekg(1, ios::cur);
    if (ss.peek() == '"') {
        val_type = VT_SERIAL;
        ss.seekg(1, ios::cur);
        ss >> string_val;
        string_val = string_val.substr(0, string_val.length() - 2);
    }
    else {
        val_type = VT_DIGITAL;
        ss >> initial_value;
    }
    
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
}

string
DetPin::parseConfiguration(string line) {
    stringstream ss(line);

    parseStart(ss);
    if (ss.peek() == ',')
        ss.seekg(1, ios::cur);
    ss >> _length;
    if (ss.peek() == ',')
        ss.seekg(1, ios::cur);
    ss >> _ratio;
    if (ss.peek() == ',')
        ss.seekg(1, ios::cur);
    ss >> _mu;
    
    _configured = true;
    initializeTimers();
    
    return _name;
}

string
UniPin::parseConfiguration(string line) {
    stringstream ss(line);

    parseStart(ss);
    if (ss.peek() == ',')
        ss.seekg(1, ios::cur);
    ss >> _length;
    if (ss.peek() == ',')
        ss.seekg(1, ios::cur);
    ss >> _mu;
    
    _num = new RandNum(ST_UNI, _length);
    
    _configured = true;
    initializeTimers();
    
    return _name;
}

string
ExpPin::parseConfiguration(string line) {
    stringstream ss(line);

    parseStart(ss);
    if (ss.peek() == ',')
        ss.seekg(1, ios::cur);
    ss >> _length;
    if (ss.peek() == ',')
        ss.seekg(1, ios::cur);
    ss >> _mu;
    
    _num = new RandNum(ST_EXP, _length);
    
    _configured = true;
    initializeTimers();
    
    return _name;
}
