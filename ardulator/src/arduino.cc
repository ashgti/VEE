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
    ardu->_debug << n << " " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
}

Arduino::Arduino() {
    _log.exceptions ( ifstream::failbit | ifstream::badbit );
    _debug.exceptions ( ifstream::failbit | ifstream::badbit );
    
    _log.open("dispatch.log", fstream::out | fstream::trunc);
    _debug.open("debug.log",  fstream::out | fstream::trunc);
    cout << "Setting up the Arduino\n";
    
    _ticks = 0 ;
    _total_ticks = 0;
    _scenario_length._seconds = 0;
    _scenario_length._ticks = 0;
    _registered_identifers = "";
}

Arduino::~Arduino() {
    if (_log.is_open()) {
        _log << "\n\n";
        _log.close();
    }
    if (_debug.is_open()) {
        _debug << "\n\n";
        _debug.close();
    }
}

void
Arduino::configurePin(uint8_t pin_id, uint8_t mode) {
    map<int, Pin*>::iterator it;
    it = _pins.find(pin_id);
    
    if (it != _pins.end()) {
        _pins[pin_id]->_mode = mode;
        _debug << "Configuring Pin " << (int)pin_id << " to mode: " << (int)mode <<  "\n";
    }
    else {
        _debug << "Error, pin configured that is not registed\n";
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
                _debug << "Scanning signals\n";
                registerSignal(word, line);
            }
            else {
                _debug << "Scanning Header\n";
                scanHeaderChunk(word, line);
            }
        }
    }
    
    ifile.close();
    
    _debug << "done parsing file\n";
    return true;
}

void
Arduino::runScenario() {
    _timer._seconds = 0;
    _timer._ticks   = 0;
    setup();
    _debug << "Attempting to run the Scenario\n\n";
    
    print_clock("Scenario Length: ", _scenario_length);
    print_clock("Runtime Timer:", _timer);
    while (!((_timer._seconds > _scenario_length._seconds) || (_timer._seconds == _scenario_length._seconds && _timer._ticks >= _scenario_length._ticks))) {
        loop();
        addTicks(1);
        updatePinState();
    }
    print_clock("Scenario Length: ", _scenario_length);
    print_clock("Runtime Timer:", _timer);
    
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
    if (_pins.find(pin_id) == _pins.end())
        return 0;
    else
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
    _log << "Dispatching signal " << signal_id << " for " << _mapping[signal_id] << " a " << ticks << "\n";
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
        
        cout << "Configuration parsed runtime of " << run_time << "\n";
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
    Pin *p = NULL;
    int pin_id = -1;
    string name = "";
    if (identifer == "det") {
        p = new DetPin();
    }
    else if (identifer == "uni") {
        p = new UniPin();
    }
    else if (identifer == "exp") {
        p = new ExpPin();
    }
    else {
        cout << "Bad configuration line \"" << line << "\"\n";
        exit(-1);
    }
    name = p->parseConfiguration(line);
    pin_id = _mapping[name];
    if (p != NULL && pin_id != -1 && name != "") {
        _pins[pin_id] = p;
    }
}

void
Arduino::addPin(string signal_id, uint8_t pin_id) {
    _mapping[signal_id] = pin_id;
}

void
Arduino::addSerial(string signal_id, HardwareSerial *serial) {
    _mapping[signal_id] = serial->pin();
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

