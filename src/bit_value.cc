// Copyright John Harrison, 2011

/**
 * 
 */

#include "ardulator/bit_value.h"
#include "arduino.h"

namespace ardulator {
namespace containers {

/* TODO: Add clock ticks */
BitValue::BitValue() : ro_(false), state_(0) { }
BitValue::BitValue(uint8_t ref, bool readonly) : ro_(readonly), state_(ref) {
}

void
BitValue::updateState(uint8_t s) {
    state_ = s;
}

uint8_t
BitValue::state() const {
    return state_;
}

volatile uint8_t* BitValue::getStateRef() {
    return &state_;
}

BitValue&
BitValue::operator=(const int &b) {
    state_ = b;
    return *this;
}

BitValue
BitValue::operator^(const int &b) const {
    return BitValue(state_ ^ b);
}

BitValue
BitValue::operator|(const int &b) const {
    return BitValue(state_ | b);
}

BitValue
BitValue::operator&(const int &b) const {
    return BitValue(state_ & b);
}

BitValue&
BitValue::operator^=(const int &b) {
    if (!ro_)
        state_ ^= b;
    return *this;
}

BitValue&
BitValue::operator|=(const int &b) {
    if (!ro_)
        state_ |= b;
    return *this;
}

BitValue&
BitValue::operator&=(const int &b) {
    if (!ro_)
        state_ &= b;
    return *this;
}

BitValue&
BitValue::operator^=(const BitValue &b) {
    if (!ro_)
        state_ ^= const_cast<BitValue&>(b).state();
    return *this;
}

BitValue&
BitValue::operator|=(const BitValue &b) {
    if (!ro_)
        state_ |= const_cast<BitValue&>(b).state();
    return *this;
}

BitValue&
BitValue::operator&=(const BitValue &b) {
    if (!ro_)
        state_ &= const_cast<BitValue&>(b).state();
    return *this;
}

BitValue&
BitValue::operator=(const BitValue &b) {
    if (!ro_)
        state_ = const_cast<BitValue&>(b).state();
    return *this;
}

BitValue
BitValue::operator ~() const {
    return BitValue(~state_);
}

BitValue
BitValue::operator^(const BitValue &b) const {
    return BitValue(state_ ^ const_cast<BitValue&>(b).state());
}

BitValue
BitValue::operator|(const BitValue &b) const {
    return BitValue(state_ | const_cast<BitValue&>(b).state());
}

BitValue
BitValue::operator&(const BitValue &b) const {
    return BitValue(state_ & const_cast<BitValue&>(b).state());
}

BitValue::operator int() {
    return state_;
}

}  // end namespace containers
}  // end namespace ardulator

