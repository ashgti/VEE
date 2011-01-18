#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include "private_config.h"

using namespace std;

void
addTime(ardu_clock_t &t, int ticks) {
    t._ticks += ticks;
    while (t._ticks > TICKS_PER_SECOND) {
        t._seconds += 1;
        t._ticks -= TICKS_PER_SECOND;
    }
}

void
print_clock(string n, ardu_clock_t t){
    cerr << n << " " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
}

Pin::Pin() {
    _history.missed_evts = 0;
    _history.total_evts = 0;
    _history.caught_evts = 0;
}

void 
Pin::calcNext() {
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
                cerr << "Signal: " << _name << " switched to LOW @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
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
                cerr << "Signal: " << _name << " switched to HIGH @ " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
                _history.total_evts += 1;
                _state = HIGH;
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

Arduino::Arduino() {
    _log.open("time.log", ifstream::in);
    cout << "Setting up the Arduino\n";
    
    _ticks = 0 ;
    _total_ticks = 0;
    _scenario_length._seconds = 0;
    _scenario_length._ticks = 0;
    _registered_identifers = "";
}

Arduino::~Arduino() {
    _log.close();
}

void
Arduino::configurePin(uint8_t pin_id, uint8_t mode) {
    map<int, Pin*>::iterator it;
    it = _pins.find(pin_id);
    
    if (it != _pins.end()) {
        _pins[pin_id]->_mode = mode;
        cerr << "Configuring Pin " << (int)pin_id << " to mode: " << (int)mode <<  "\n";
    }
    else {
        cerr << "Error, pin configured that is not registed\n";
    }
}

bool 
Arduino::addInputFile(char *name) {
    ifstream ifile;
    bool header_parsed = false;
    
    ifile.open(name, ifstream::in);
    
    while (ifile.good()) {
        string line;
        getline(ifile, line);
        if (line.compare("---") == 0) {
            header_parsed = true;
            continue;
        }
        size_t found = line.find(" ");
        
        if (found != string::npos) {
            string word = line.substr(0, found);
            if (header_parsed) {
                cerr << "Scanning signals\n";
                registerSignal(word, line);
            }
            else {
                cerr << "Scanning Header\n";
                scanHeaderChunk(word, line);
            }
        }
    }
    
    ifile.close();
    
    cerr << "done parsing file\n";
    return true;
}

void
Arduino::runScenario() {
    _timer._seconds = 0;
    _timer._ticks   = 0;
    setup();
    cerr << "Attempting to run the Scenario\n\n";
    
    print_clock("sl", _scenario_length);
    print_clock("tr", _timer);
    while (!((_timer._seconds > _scenario_length._seconds) || (_timer._seconds == _scenario_length._seconds && _timer._ticks >= _scenario_length._ticks))) {
        loop();
        addTicks(1);
        updatePinState();
    }
    print_clock("sl", _scenario_length);
    print_clock("tr", _timer);
    
    cout << "--------------------------\n";
}

void
Arduino::addTicks(uint64_t length) {
    _timer._ticks += length;
    if (_timer._ticks > TICKS_PER_SECOND) {
        _timer._seconds++;
        _timer._ticks -= TICKS_PER_SECOND;
    }
}

void
Arduino::updatePinState() {
    // Report Changes in State
    map<int, Pin*>::iterator it;
    for (it = _pins.begin(); it != _pins.end(); it++) {
        it->second->setState(_timer);
    }
}

int
Arduino::getPin(uint8_t pin_id) {
    return _pins[pin_id]->_state;
}

void
Arduino::setPin(uint8_t pin_id, uint8_t val) {
    if (_pins[pin_id]->_state != val) {
        _pins[pin_id]->_state = val;
        _pins[pin_id]->_flags &= 0x1;
        cout << timestamp() << "\nUpdating value for: " << static_cast<int>(pin_id) << " " << static_cast<int>(val) << "\n";
    }
}

void
Arduino::dispatchSignal(const char *signal_id) {
    int ticks = _pins[_mapping[signal_id]]->process();
    
    _timer._ticks += ticks;
    while (_timer._ticks > TICKS_PER_SECOND) {
        _timer._seconds += 1;
        _timer._ticks -= TICKS_PER_SECOND;
    }
    
    updatePinState();
    cout << "Dispatching signal " << signal_id << " for " << _mapping[signal_id] << " a " << ticks << "\n";
}

string
Arduino::timestamp() {
    ostringstream result(ostringstream::out);
    result << static_cast<int>(_timer._seconds) << "s " << static_cast<int>(_timer._ticks) << "t";
    return result.str();
}

void
Arduino::scanHeaderChunk(string identifier, string line) {
    if (identifier == "length:") {
        double run_time;
        istringstream is;

        is.str(line.substr(identifier.length(), line.length() - identifier.length()));
        is >> run_time;
        
        cout << "Got a runtime length of " << run_time << "\n\n";
        _scenario_length._seconds = (int)run_time;
        _scenario_length._ticks = (run_time - floor(run_time)) * TICKS_PER_SECOND;
    } else if (identifier == "identifiers:") {
        _registered_identifers = line.substr(identifier.length(), line.length() - identifier.length());
    }
    else {
        cout << "Unrecognized header configuration name \"" << identifier << "\"\n";
    }
}

void
Arduino::registerSignal(string identifer, string line) {
    cout << identifer << " and " << line << "\n";
    
    Pin *p = new Pin;
    
    if (identifer == "det") {
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
            cout << "Got a quoted string\n";
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
        if (val_type == VT_DIGITAL) {
            cout << "got D: " << name << ", " << initial_value << " and " << length << " r:" << ratio << "\n";
        }
        else {
            cout << "got S: " << name << ", " << string_val << " and " << length << " r:" << ratio << "\n";
        }
        
        if (_mapping.find(name) != _mapping.end()) {
            int pin_id = _mapping[name];
            p->_name = name;
            p->_state = -1;
            p->_flags = 0;
            p->_val_type = val_type;
            p->_signal_type = ST_DET;
            if (val_type == VT_SERIAL)
                p->_string_val = string_val;
            if (val_type == VT_DIGITAL) {
                p->_digital_val = initial_value;
                p->_state = initial_value;
            }
            p->_vals[0] = length;
            p->_vals[1] = ratio;
            p->_processing_time = processing_time;
            p->_configured = true;
        
            map<string, intptr_t>::iterator mit;
            for (mit = _mapping.begin(); mit != _mapping.end(); mit++) {
                cout << "got : " << mit->first << " b: " << mit->second << "\n";
            }
            
            p->calcNext();
        
            cout << "Pin_id: " << pin_id << "\n";
            _pins[pin_id] = p;
        }
    }
    else if (identifer == "uni") {
        /*
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
            cout << "Got a quoted string\n";
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
        if (val_type == VT_DIGITAL) {
            cout << "got: " << name << ", " << initial_value << " and " << length << " r:" << ratio << "\n";
        }
        else {
            cout << "got: " << name << ", " << string_val << " and " << length << " r:" << ratio << "\n";
        }
        
        int pin_id = _mapping[name];
        p->_state = -1;
        p->_flags = 0;
        p->_val_type = val_type;
        if (val_type == VT_SERIAL)
            p->_string_val = string_val;
        if (val_type == VT_DIGITAL) 
            p->_digital_val = initial_value;
        p->_vals[0] = length;
        p->_vals[1] = ratio;
        p->_processing_time = processing_time;
        p->_configured = true;
        _pins[pin_id] = p;
        */
    }
    else {
        cout << "Bad configuration line \"" << line << "\"\n";
        exit(-1);
    }
}

void
Arduino::addPin(string signal_id, uint8_t pin_id) {
    _mapping[signal_id] = pin_id;
}

void
Arduino::addSerial(string signal_id, HardwareSerial *serial) {
    _mapping[signal_id] = (intptr_t)serial;
}



/* Logging facilities */
void
Arduino::log(int level, string msg) {
    cout << timestamp() << " : " << "level [" << level << "] msg: " << msg << "\n";
}

void
Arduino::debug(string msg) {
    log(5, msg);
}

void
Arduino::info(string msg) {
    log(4, msg);
}

void
Arduino::warn(string msg) {
    log(3, msg);
}

void
Arduino::error(string msg) {
    log(2, msg);
}

void
Arduino::critical(string msg) {
    log(1, msg);
}

void
Arduino::report() {
    map<int, Pin*>::iterator it;
    
    for (it = _pins.begin(); it != _pins.end(); it++) {
        it->second->report();
    }
}

