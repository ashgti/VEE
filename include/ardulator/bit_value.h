#ifndef ARDULATOR_BIT_VALUE_H
#define ARDULATOR_BIT_VALUE_H
#include <stdint.h>

class BitValue {
    bool    _ro;
    uint8_t _state;
  public:
    BitValue(bool readonly = false);
    BitValue(int state, bool readonly = false);
    void updateState(int);
    int getState();
    
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

#endif /* ARDULATOR_BIT_VALUE_H */
