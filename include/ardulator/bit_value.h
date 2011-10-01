#ifndef ARDULATOR_BIT_VALUE_H
#define ARDULATOR_BIT_VALUE_H
#include <stdint.h>
#include <cstdlib>

namespace ardulator {

class BitValue {
    bool    _ro;
    volatile uint8_t _state;
  public:
    BitValue();
    BitValue(uint8_t ref, bool readonly = false);
    void updateState(int);
    int getState() const;
    
    volatile uint8_t* getStateRef();
    
    /* Operators */
    /* Basic Assignment */
    BitValue& operator=(const BitValue &b);
    BitValue& operator=(const int &b);

    /* Compound-Assignment */
    BitValue& operator^=(const BitValue &b);
    BitValue& operator|=(const BitValue &b);
    BitValue& operator&=(const BitValue &b);
    BitValue& operator^=(const int &b);
    BitValue& operator|=(const int &b);
    BitValue& operator&=(const int &b);
    
    /* Bitwise Operators */
    BitValue operator~() const;
    BitValue operator^(const int &b) const;
    BitValue operator|(const int &b) const;
    BitValue operator&(const int &b) const;
    BitValue operator^(const BitValue &b) const;
    BitValue operator|(const BitValue &b) const;
    BitValue operator&(const BitValue &b) const;
    
    /* Conversions */
    operator int();
};

} // end namespace ardulator

#endif /* ARDULATOR_BIT_VALUE_H */
