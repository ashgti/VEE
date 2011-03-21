#include "ardulator/bit_value.h"
#include <arduino.h>

/* TODO: Add clock ticks */
BitValue::BitValue() : _ro(false), _state(0) { }
BitValue::BitValue(uint8_t ref, bool readonly) : _ro(readonly), _state(ref) {
}
    
void 
BitValue::updateState(int s) {
    _state = s;
}
    
int
BitValue::getState() const {
    return _state;
}

volatile uint8_t* 
BitValue::getStateRef() {
    return &_state;
}

BitValue& 
BitValue::operator=(const int &b) {
    _state = b;    
    return *this;
}
        
BitValue
BitValue::operator^(const int &b) const {
    return BitValue(_state ^ b);
}

BitValue
BitValue::operator|(const int &b) const {
    return BitValue(_state | b);
}

BitValue
BitValue::operator&(const int &b) const {
    return BitValue(_state & b);
}

BitValue& 
BitValue::operator^=(const int &b) {
    if (!_ro)
        _state ^= b;
    return *this;
}

BitValue& 
BitValue::operator|=(const int &b) {
    if (!_ro)
        _state |= b;
    return *this;
}

BitValue& 
BitValue::operator&=(const int &b) {
    if (!_ro)
        _state &= b;
    return *this;
}

BitValue& 
BitValue::operator^=(const BitValue &b) {
    if (!_ro)
        _state ^= const_cast<BitValue&>(b).getState();
    return *this;
}

BitValue& 
BitValue::operator|=(const BitValue &b) {
    if (!_ro)
        _state |= const_cast<BitValue&>(b).getState();
    return *this;
}

BitValue& 
BitValue::operator&=(const BitValue &b) {
    if (!_ro)
        _state &= const_cast<BitValue&>(b).getState();
    return *this;
}

BitValue& 
BitValue::operator=(const BitValue &b) {
    if (!_ro)
        _state = const_cast<BitValue&>(b).getState();    
    return *this;
}

BitValue
BitValue::operator ~() const {
    return BitValue(~_state);
}
        
BitValue
BitValue::operator^(const BitValue &b) const {
    return BitValue(_state ^ const_cast<BitValue&>(b).getState());
}

BitValue
BitValue::operator|(const BitValue &b) const {
    return BitValue(_state | const_cast<BitValue&>(b).getState());
}

BitValue
BitValue::operator&(const BitValue &b) const {
    return BitValue(_state & const_cast<BitValue&>(b).getState());
}

BitValue::operator int() {
    return _state;
}
