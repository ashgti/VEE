#include <iostream>
#include <fstream>
#include <sstream>
#include "private_config.h"

using namespace std;

Arduino::Arduino(ArduinoModel arduino_model) {
    _log.open("time.log", ifstream::in);
    cout << "Setting up the Arduino\n";
    
    _ticks = 0 ;
    _total_ticks = 0;
    
    switch (arduino_model) {
        case ArduinoUno: 
            break;
        case ArduinoMega:
            for (int i = 1; i <= 54; i++) {
                _pins.addPin(i, true, true, true);
            }
            break;
        case ArduinoNano:
            break;
        default:
            cout << "Bad configuration to Arduino, check setModel\n\n";
            exit(1);
    }
}

Arduino::~Arduino() {
    _log.close();
}

void
Arduino::configurePin(uint8_t pin_id, uint8_t mode) {
    cout << "Configuring Pin " << pin_id << " to mode: " << mode << "\n";
}

bool 
Arduino::addInputFile(char *name) {
    ifstream ifile;
    
    ifile.open(name, ifstream::in);
    
    bool in_vs_out = false;
    while (ifile.good()) {
        string line;
        getline(ifile, line);
        if (line.compare("---") == 0) {
            in_vs_out = true;
            continue;
        }
        size_t found = line.find(" ");

        if (found != string::npos) {
            string word = line.substr(0, found);
            if (in_vs_out) 
                registerOutputEvent(word, line);
            else 
                registerInputEvent(word, line);
        }
    }
    
    ifile.close();
    
    return true;
}

void
Arduino::runScenario(double duration) {
    _timer._seconds = 0;
    _timer._ticks   = 0;
    setup();
    int count = 0;
    while (_timer._seconds < duration) {
        loop();
        
        count++;
        
        if (count > 10) break;
        
        addTicks(1);
        
        updatePinState();
    }
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
    map<uint8_t, Pin*>::iterator pin_iter;
    
    for (pin_iter = _pins._mapping.begin();
         pin_iter != _pins._mapping.end();
         pin_iter++) {
         if (pin_iter->second->_flags & 0x1) {
            cout << "\n   Pin changed: " << pin_iter->first << " to " << pin_iter->second->_val << "\n";
            pin_iter->second->_flags = 0;
        }
    }
    
}

void
Arduino::setPin(uint8_t pin_id, uint8_t val) {
    if (_pins._mapping[pin_id]->_val != val) {
        _pins._mapping[pin_id]->_val = val;
        _pins._mapping[pin_id]->_flags = 0x1;
        cout << timestamp() << "\nUpdating value for: " << static_cast<int>(pin_id) << " " << static_cast<int>(val) << "\n";
    }
}

string
Arduino::timestamp() {
    ostringstream result(ostringstream::out);
    result << static_cast<int>(_timer._seconds) << "s " << static_cast<int>(_timer._ticks) << "t";
    return result.str();
}

void
Arduino::registerInputEvent(string identifer, string line) {
    map<string, Evt* (*)(bool, string)>::iterator it;
    if ((it = _evt_responders.find(identifer)) != _evt_responders.end()) {
        cout << "Got something \n";
        it->second(true, line);
    }
    else {
        throw ArduException("Bad input line.");
    }
}

void
Arduino::registerOutputEvent(string identifer, string line) {
    map<string, Evt* (*)(bool, string)>::iterator it;
    if ((it = _evt_responders.find(identifer)) != _evt_responders.end()) {
        cout << "Got something \n";
        it->second(false, line);
    }
    else {
        throw ArduException("Bad output line.");
    }
}

bool 
Arduino::addEventHandler(string id, Evt* (*comp)(bool, string)) {
    _evt_responders.insert(pair<string, Evt* (*)(bool, string)>(id, comp));
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

string
DetEvt::id() {
    return "Det";
}

string
UniEvt::id() {
    return "Uni";
}

string
ExpEvt::id() {
    return "Exp";
}

string
PinEvt::id() {
    return "Pin";
}

string
SerialEvt::id() {
    return "Serial";
}

Evt*
DetEvt::evtHandler(bool in, string line) {
    DetEvt *result = new DetEvt;
    cout << "Got DetEvt Evt Generator\n";
    return result;
}

Evt*
ExpEvt::evtHandler(bool in, string line) {
    ExpEvt *result = new ExpEvt;
    cout << "Got ExpEvt Evt Generator\n";
    return result;
}

Evt*
UniEvt::evtHandler(bool in, string line) {
    UniEvt *result = new UniEvt;
    cout << "Got UniEvt Evt Generator\n";
    return result;
}

Evt*
PinEvt::evtHandler(bool in, string line) {
    PinEvt *result = new PinEvt;
    cout << "Got PinEvt Evt Generator\n";
    return result;
}

Evt*
SerialEvt::evtHandler(bool in, string line) {
    SerialEvt *result = new SerialEvt;
    cout << "Got SerialEvt Evt Generator\n";
    return result;
}
