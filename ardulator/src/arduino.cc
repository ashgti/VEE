#include <iostream>
#include <fstream>
#include <sstream>
#include "private_config.h"

using namespace std;

Arduino::Arduino() {
    _log.open("time.log", ifstream::in);
    cout << "Setting up the Arduino\n";
    
    _ticks = 0 ;
    _total_ticks = 0;
    _scenario_length = 0;
    _registered_identifers = "";
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
                cout << "Scanning signals\n";
                registerSignal(word, line);
            }
            else {
                cout << "Scanning Header\n";
                scanHeaderChunk(word, line);
            }
        }
    }
    
    ifile.close();
    
    cout << "done parsing file\n";
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
    
    cout << "Update Pin State";
    exit(-1);
}

int
Arduino::getPin(uint8_t pin_id) {
    return _pins[pin_id];
}

void
Arduino::setPin(uint8_t pin_id, uint8_t val) {
    if (_pins[pin_id] != val) {
        _pins[pin_id] = val;
        cout << timestamp() << "\nUpdating value for: " << static_cast<int>(pin_id) << " " << static_cast<int>(val) << "\n";
    }
}

void
Arduino::dispatchSignal(const char *signal_id) {
    cout << "Dispatching signal " << signal_id << " for " << _signals[signal_id] << "\n";
    _signals[signal_id];
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
        
        _registered_identifers = (int) run_time * TICKS_PER_SECOND;
    } else if (identifier == "identifiers:") {
        _registered_identifers = line.substr(identifier.length(), line.length() - identifier.length());
    }
    else {
        cout << "Unrecognized header configuration name \"" << identifier << "\"\n";
    }
}

void
Arduino::registerSignal(string identifer, string line) {
    cout << "HI! " << identifer << " and " << line << "\n";
    
    if (identifer == "det") {
        stringstream ss(line);
        int initial_value, length, ratio, constant_processing_time;
        string name, processing_time;
        ss.seekg(4, ios::cur);
        ss >> name;
        if (name.at(name.length() - 1) == ',') {
            name = name.substr(0, name.length() - 1);
        }
        ss.seekg(1, ios::cur);
        if (ss.peek() == '"') {
            cout << "Got a quoted string\n";
        }
        ss >> initial_value;
        ss.seekg(1, ios::cur);
        ss >> length;
        ss.seekg(1, ios::cur);
        ss >> ratio;
        ss.seekg(1, ios::cur);
        ss >> processing_time;
        cout << "got: " << name << " , " << initial_value << " and " << length << "\n";
        if (processing_time.find("n") != string::npos) {
            cout << "has a N based length\n";
        }
        else {
            stringstream pt(processing_time);
            pt >> constant_processing_time; 
            cout << constant_processing_time << "\n";
        }
        
        // _mapping_
    }
    else {
        cout << "Bad configuration line \"" << line << "\"\n";
        exit(-1);
    }
}

void
Arduino::addPin(string signal_id, uint8_t pin_id) {
    _signal_map[signal_id] = pin_id;
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
    // PinEvt *result = new PinEvt;
    // cout << "Got PinEvt Evt Generator\n";
    cout << "Stuff";
    exit(-1);
    // return result;
}

Evt*
SerialEvt::evtHandler(bool in, string line) {
    SerialEvt *result = new SerialEvt;
    cout << "Got SerialEvt Evt Generator\n";
    return result;
}
