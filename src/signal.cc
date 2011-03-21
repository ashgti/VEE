#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include "ardulator.h"
#include "arduino.h"

using namespace std;

Signal::Signal() {
    _caught_flag = false;
    _history.missed_evts = 0;
    _history.total_evts = 0;
    _history.caught_evts = 0;
    _history.avg_response_time = 0;
}

Signal::~Signal() {
    if (_log.is_open()) {
        _log << "\n\n";
        _log.flush();
        _log.close();
    }
}

DetSignal::DetSignal() : Signal() {
}

ExpSignal::ExpSignal() : Signal() {
}

UniSignal::UniSignal() : Signal() {
}

void 
Signal::initializeTimers() {
    string log_filename = "./logs/";
    log_filename.append(_name);
    log_filename.append(".log");
    _log.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        _log.open(log_filename.c_str(), ios::in | ios::out | ios::trunc);
    }
    catch (ifstream::failure e) {
        cout << "File error for " << log_filename << " " << _log.fail() << " < " << _log.bad() << " \n";
    }
    
    if (_signal_type == ST_DET) {
        _state = LOW;
    }
}

void
Signal::setState(ardu_clock_t &t) {
    if (_val_type == VT_SERIAL) {
        if (_state == HIGH) {
            if (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                    t._ticks > _next._ticks)) {
                updateState(t, LOW);
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
                updateState(t, LOW);
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
                updateState(t, LOW);
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
Signal::finalize(ardu_clock_t &t) {
    if (_val_type == VT_SERIAL) {
        if (_state == HIGH) {
            if (_caught_flag == false) {
                _history.missed_evts += 1;
            }
        }
    }
    else if (_val_type == VT_DIGITAL) {
        if (_state == HIGH) {
            if (_caught_flag == false) {
                _history.missed_evts += 1;
            }
        }
    }
    else if (_val_type == VT_ANALOG) {
        if (_state == HIGH) {
            if (_caught_flag == false) {
                _history.missed_evts += 1;
            }
        }
    }
}

void
Signal::updateState(ardu_clock_t &t, int new_state) {
    do {
        double next = calcNext(new_state);
        if (next == 0)
            continue;
        ardu_clock_t old = _next;
        int old_state = _state;
        int overflow = 0;
        _next._ticks += (next - floor(next)) * TICKS_PER_SECOND;
        if (_next._ticks > TICKS_PER_SECOND) {
            _next._ticks -= TICKS_PER_SECOND;
            overflow = 1;
        }
        _next._seconds += floor(next) + overflow;

        ardu->_debug << "Signal: " << _name << " switched to ";
        if (new_state == HIGH) {
            ardu->_debug << "HIGH";
        }
        else {
            ardu->_debug << "LOW";
        }
        ardu->_debug << " @ " << old._seconds << "." 
                     << setw(FIELD_WIDTH) << setfill('0') << old._ticks << "\n";
        _state = new_state;
        
        if (ardu->_interrupts == false) {
            continue;
        }
        
        map<int, std::pair<int, void (*)(void)> >::iterator map_iter = ardu->_interrupt_map.find(ardu->_mapping[_name]);
        
        // cout << "Name: " << _name << endl;
        // cout << "Map found: " << ardu->_mapping[_name] << endl;
        /* TODO: 
             if pin has an interrupt associated with it, check it and call it here.
         */
        if (_state == HIGH) {
            // LOW EVENT
            if (map_iter != ardu->_interrupt_map.end() && map_iter->second.first == LOW) {
                map_iter->second.second();
            }
        }
        if (old_state == LOW && _state == HIGH) {
            // FIRE RISING EVENT
            if (map_iter != ardu->_interrupt_map.end() && map_iter->second.first == RISING) {
                map_iter->second.second();
            }
            _caught_flag = false;
        }
        if (old_state != _state) {
            // FIRE CHANGED EVENT
            if (map_iter != ardu->_interrupt_map.end() && map_iter->second.first == CHANGE) {
                map_iter->second.second();
            }
        }
        if (old_state == HIGH && _state == LOW) {
            // FIRE FALLING EVENT
            if (map_iter != ardu->_interrupt_map.end() && map_iter->second.first == FALLING) {
                map_iter->second.second();
            }
            if (_caught_flag == false) {
                _history.missed_evts += 1;
            }
        }
    } while (t._seconds > _next._seconds || (t._seconds == _next._seconds &&
                t._ticks > _next._ticks));
}

double
Signal::calcNext(int new_state) {
    cout << "ERROR, bad function call\n";
    exit(-1);
}

double
DetSignal::calcNext(int new_state) {
    return _length * _ratio;
}

double
UniSignal::calcNext(int new_state) {
    if (new_state == HIGH) {
        double next = _num->next();
        next -= (_mu * 0.01) * _length;
        if (next < 0)
            return 0;
        else
            return next;
    }
    else {
        return (_mu * 0.01) * _length;
    }
}

double
ExpSignal::calcNext(int new_state) {
    if (new_state == HIGH) 
        return _num->next();
    else
        return (_mu * 0.01) * _length;
}

int
Signal::process() {
    if (_caught_flag == false && _state == HIGH) {
        _history.caught_evts += 1;
        _caught_flag = true;
    }
    else {
        ardu->_debug << "Processing event that has been handled. ";
    }
    
    ardu->_debug << "Processing: " << _name << "\n";
    
    return (_mu * 0.01) * _length * TICKS_PER_SECOND;
}

void
Signal::report(bool) {
    cerr << "ERROR, bad function call\n";
    exit(-1);
}

void
ExpSignal::report(bool used) {
    if (used == false) {
        cout << "Reporting for: " << _name << "\n";
        cout << "    -Not Used-\n";
        cout << "           Missed Events: " << _history.missed_evts << "\n";
        cout << "            Total Events: " << _history.total_evts << "\n"; 
        cout << "--------------------------\n\n";
        cout.flush();
        
        return;
    }
    
    cout << "Reporting for: " << _name << "\n";
    cout << "               Lambda: " << _length << "\n";
    cout << "                   Mu: " << _mu << "\n";
    cout << "                    --\n";
    cout << "        Missed Events: " << _history.missed_evts << "\n";
    cout << "         Total Events: " << _history.missed_evts + _history.caught_evts << "\n";
    
    cout << "--------------------------\n\n";
    cout.flush();
    
    _log << "Reporting for: " << _name << "\n";
    _log << "               Lambda: " << _length << "\n";
    _log << "                   Mu: " << _mu << "\n";
    _log << "                    --\n";
    _log << "        Missed Events: " << _history.missed_evts << "\n";
    _log << "         Total Events: " << _history.missed_evts + _history.caught_evts << "\n";
    
    _log << "--------------------------\n\n";
    _log.flush();
}

void
DetSignal::report(bool used) {
    if (used == false) {
        cout << "Reporting for: " << _name << "\n";
        cout << "    -Not Used-\n";
        cout << "           Missed Events: " << _history.missed_evts + _history.caught_evts + _history.total_evts << "\n";
        cout << "            Total Events: " << _history.missed_evts + _history.caught_evts + _history.total_evts << "\n"; 
        cout << "--------------------------\n\n";
        cout.flush();
        
        return;
    }
    
    cout << "Reporting for: " << _name << "\n";
    cout << "   Signal High Length: " << _length << "\n";
    cout << "                Ratio: " << _ratio << "\n";
    cout << "                   Mu: " << _mu << "\n";
    cout << "                    --\n";
    cout << "        Missed Events: " << _history.missed_evts << "\n";
    cout << "         Total Events: " << _history.missed_evts + _history.caught_evts + _history.total_evts << "\n";
    
    cout << "--------------------------\n\n";
    cout.flush();
    
    _log << "Reporting for: " << _name << "\n";
    _log << "   Signal High Length: " << _length << "\n";
    _log << "                Ratio: " << _ratio << "\n";
    _log << "                   Mu: " << _mu << "\n";
    _log << "                    --\n";
    _log << "        Missed Events: " << _history.missed_evts << "\n";
    _log << "         Total Events: " << _history.missed_evts + _history.caught_evts + _history.total_evts << "\n";

    _log << "--------------------------\n\n";
    _log.flush();
}

void
UniSignal::report(bool used) {
    if (used == false) {
        cout << "Reporting for: " << _name << "\n";
        cout << "    -Not Used-\n";
        cout << "           Missed Events: " << _history.missed_evts << "\n";
        cout << "            Total Events: " << _history.total_evts << "\n"; 
        cout << "--------------------------\n\n";
        cout.flush();
        
        return;
    }
    cout << "Reporting for: " << _name << "\n";
    cout << "               Lambda: " << _length << "\n";
    cout << "                   Mu: " << _mu << "\n";
    cout << "                   --\n";
    cout << "        Missed Events: " << ((_history.total_evts > _history.missed_evts) ? _history.missed_evts : _history.total_evts) << "\n";
    cout << "         Total Events: " << _history.total_evts << "\n";
    
    cout << "--------------------------\n\n";
    cout.flush();
    
    _log << "Reporting for: " << _name << "\n";
    _log << "               Lambda: " << _length << "\n";
    _log << "                   Mu: " << _mu << "\n";
    _log << "                   --\n";
    _log << "        Missed Events: " << ((_history.total_evts > _history.missed_evts) ? _history.missed_evts : _history.total_evts) << "\n";
    _log << "         Total Events: " << _history.total_evts << "\n";

    _log << "--------------------------\n\n";
    _log.flush();
}

string
Signal::parseConfiguration(string) {
    throw "std::string Pin::parseConfiguration(std::string) needs to be overriden by a proper subclass";
}

void
Signal::parseStart(stringstream &ss) {
    ValueType val_type;
    int initial_value = 0;
    string name, tmp_name, string_val;
    ss.seekg(4, ios::cur);
    ss >> tmp_name;
    if (tmp_name.length() > 1 && tmp_name.at(tmp_name.length() - 1) == ',') {
        name = tmp_name.substr(0, tmp_name.length() - 1);
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
DetSignal::parseConfiguration(string line) {
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
UniSignal::parseConfiguration(string line) {
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
ExpSignal::parseConfiguration(string line) {
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

