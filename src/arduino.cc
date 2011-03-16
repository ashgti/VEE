#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ardulator.h"
#include "arduino.h"

using namespace std;

ProcessingSignal p;

static void
addTime(ardu_clock_t &t, int ticks) {
    t._ticks += ticks;
    while (t._ticks > TICKS_PER_SECOND) {
        t._seconds += 1;
        t._ticks -= TICKS_PER_SECOND;
    }
}

static void
print_clock(string n, ardu_clock_t t){
    ardu->_debug << n << " " << t._seconds << "." << setw(FIELD_WIDTH) << setfill('0') << t._ticks << "\n";
    
    ardu->_debug.flush();
}

Arduino::Arduino() : _wait_till(0) {
    _log.exceptions ( ifstream::failbit | ifstream::badbit );
    _debug.exceptions ( ifstream::failbit | ifstream::badbit );
    
    struct stat buffer;
    int status = stat("./logs", &buffer);
    
    if (status != 0 && errno == ENOENT) {
        if (mkdir("./logs", 0777)) {
            perror("./logs");
        }
    }
    
    _log.open("./logs/dispatch.log", fstream::out | fstream::trunc);
    _debug.open("./logs/debug.log",  fstream::out | fstream::trunc);
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
        _log.flush();
        _log.close();
    }
    if (_debug.is_open()) {
        _debug << "\n\n";
        _debug.flush();
        _debug.close();
    }
}

void
Arduino::configurePin(uint8_t pin_id, uint8_t mode) {
    map<int, Signal*>::iterator it;
    it = _signals.find(pin_id);
    
    if (it != _signals.end()) {
        _signals[pin_id]->_mode = mode;
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
    updatePinState();
    while (!((_timer._seconds > _scenario_length._seconds) || (_timer._seconds == _scenario_length._seconds && _timer._ticks >= _scenario_length._ticks))) {
        try {
            if (!this->_wait_till)
                loop();
            else 
                this->_wait_till--;
        }
        catch (ProcessingSignal &e) {
            cout << "tee hee " << endl;
        }
        addTicks(LOOP_CONST);
        updatePinState();
    }
    finalizePinState();
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
    map<int, Signal*>::iterator it;
    int c = 0;
    for (it = _signals.begin(); it != _signals.end(); it++) {
        c++;
        // cout << "Updating " << it->first << endl;
        it->second->setState(_timer);
    }
    
    vector<Signal*>::iterator vit;
    for (vit = _unused_signals.begin(); vit != _unused_signals.end(); vit++) {
        (*vit)->setState(_timer);
    }
}

void
Arduino::finalizePinState() {
    // Report Changes in State
    map<int, Signal*>::iterator it;
    for (it = _signals.begin(); it != _signals.end(); it++) {
        it->second->finalize(_timer);
    }
    
    vector<Signal*>::iterator vit;
    for (vit = _unused_signals.begin(); vit != _unused_signals.end(); vit++) {
        (*vit)->finalize(_timer);
    }   
}

/*
 * Analog Reads take 100 ms 
 * Digital Reads 58 cycles 
 */
int
Arduino::getPin(uint8_t pin_id) {
    if (_signals.find(pin_id) == _signals.end()) {
        return LOW;
    }
    else {
        if (_signals[pin_id]->_val_type == VT_SERIAL) {
            return LOW;
        }
        else if (_signals[pin_id]->_val_type == VT_DIGITAL) {
            addTicks(58);
            return _signals[pin_id]->_state;
        }
        else if (_signals[pin_id]->_val_type == VT_ANALOG) {
            addTicks(MS2T(100));
            return _signals[pin_id]->_state;
        }
        else 
            return LOW;
    }
}

void
Arduino::setPin(uint8_t pin_id, uint8_t val) {
    if (_signals[pin_id]->_state != val) {
        _signals[pin_id]->_state = val;
    }
}

void
Arduino::dispatchSignal(const char *signal_id) {
    if (_mapping.find(signal_id) == _mapping.end()) {
        return;
    }
    if (_signals.find(_mapping[signal_id]) == _signals.end()) {
        return;
    }
    int ticks = _signals[_mapping[signal_id]]->process();
    
    _wait_till = ticks;

    _log << "Dispatching signal " << signal_id << " for " << _mapping[signal_id] << " a " << ticks << "\n";
    _log.flush();
    
    throw p;
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
    } 
    else if (identifier == "identifiers:") {
        _registered_identifers = line.substr(identifier.length(), line.length() - identifier.length());
    }
    else if (identifier == "#") {
        // this is a comment, do nothing with this line
    }
    else {
        cout << "Unrecognized header configuration name \"" << identifier << "\"\n";
    }
}

void
Arduino::registerSignal(string identifer, string line) {    
    Signal *p = NULL;
    int pin_id = -1;
    string name = "";
    if (identifer == "det") {
        p = new DetSignal();
    }
    else if (identifer == "uni") {
        p = new UniSignal();
    }
    else if (identifer == "exp") {
        p = new ExpSignal();
    }
    else {
        cout << "Bad configuration line \"" << line << "\"\n";
        exit(-1);
    }
    name = p->parseConfiguration(line);
    if (_mapping.find(name) == _mapping.end()) {
        _unused_signals.push_back(p);
    }
    else {
        pin_id = _mapping[name];
        if (p != NULL && pin_id != -1 && name != "") {
            _signals[pin_id] = p;
        }
    }
}

void
Arduino::addPin(string signal_name, uint8_t pin_id) {
    _mapping[signal_name] = pin_id;
}

void
Arduino::addSerial(string signal_name, HardwareSerial *serial) {
    _mapping[signal_name] = serial->pin();
}

/* Reporting facilities */
void
Arduino::report() {
    map<int, Signal*>::iterator it;
    
    for (it = _signals.begin(); it != _signals.end(); it++) {
        it->second->report(true);
    }
    
    vector<Signal*>::iterator vit;
    for (vit = _unused_signals.begin(); vit != _unused_signals.end(); vit++) {
        (*vit)->report(false);
    }
}


/* Interrupts */
void
Arduino::registerInterrupt(uint8_t pin_id, void (*fn)(void), uint8_t mode) {
    _interrupt_map[pin_id] = make_pair(mode, fn);
}

void
Arduino::dropInterrupt(uint8_t pin_id) {
    _interrupt_map.erase(pin_id);
    cout << "Dropped int on " << pin_id << endl;
}
